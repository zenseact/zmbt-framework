/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/model/environment.hpp"
#include "zmbt/model/injection_table.hpp"


namespace zmbt {


InjectionTable::Record::Record(
    boost::json::string_view jp,
    Generator::Shared gen,
    lang::Expression const& tf
)
    : jptr{jp}
    , generator{gen}
    , transform(tf)
    , maybe_constant{}
{
    if (!generator->is_noop() && generator->expression().is_const())
    {
        if (!transform.is_noop())
        {
            auto res = transform.eval_e(generator->expression(), {});
            maybe_constant = res.to_json();
            // if(res.is_error())
            // {
                // TODO
            // }
        }
        else
        {
            maybe_constant = generator->expression().eval();
        }
    }
}


std::vector<InjectionTable::Record>& InjectionTable::get_records(ChannelKind const& ck) &
{
    static std::vector<Record> dummy {};
    switch(ck)
    {
        case ChannelKind::Args:
            return args_;
        case ChannelKind::Return:
            return ret_;
        case ChannelKind::Exception:
            return exception_;
        default:
        throw_exception(environment_error(""));
            return dummy;
    }
}

boost::optional<boost::json::value>& InjectionTable::get_maybe_const(ChannelKind const& ck) &
{
    static boost::optional<boost::json::value> dummy {};
    switch(ck)
    {
        case ChannelKind::Args:
            return maybe_constant_args_;
        case ChannelKind::Return:
            return maybe_constant_ret_;
        case ChannelKind::Exception:
            return maybe_constant_exception_;
        default:
        throw_exception(environment_error(""));
            return dummy;
    }
}

InjectionTable::InjectionTable(interface_id const& ifc_id, object_id const& obj_id)
    : ifc_id_{ifc_id}
    , obj_id_{obj_id}
    , prototypes_{Environment().GetPrototypes(ifc_id_)}
    , maybe_constant_args_{prototypes_.args()}
    , maybe_constant_ret_{prototypes_.ret()}
    , maybe_constant_exception_{nullptr}
{
}

void InjectionTable::add_record(ChannelKind const& ck, Record&& rec)
{
    auto& maybe_const = get_maybe_const(ck);
    if (maybe_const.has_value() && rec.maybe_constant.has_value())
    {
        auto v = maybe_const.value();
        v.set_at_pointer(rec.jptr, rec.maybe_constant.value());
        maybe_const = v;
    }
    else
    {
        maybe_const.reset();
    }
    get_records(ck).push_back(std::move(rec));
}

InjectionTable::Record& InjectionTable::last_record(ChannelKind const& ck) &
{
    return get_records(ck).back();
}

/// Evaluate input for given category and return error object if any, null otherwise
boost::json::value InjectionTable::yield(ChannelKind const& ck, boost::json::value& maybe_error)
{
    boost::json::value result_value;

    if (ChannelKind::Return == ck)
    {
        result_value = prototypes_.ret();
    }
    else if (ChannelKind::Args == ck)
    {
        result_value = prototypes_.args();
    }
    else if (ChannelKind::Exception == ck)
    {
        result_value = nullptr;
    }
    else
    {
        maybe_error = boost::json::object {
            {"type", "model implementation assertion"},
            {"what", format("invalid injection category: %s", json_from(ck))},
        };
        return result_value;
    }

    if (get_maybe_const(ck).has_value())
    {
        return get_maybe_const(ck).value();
    }

    auto& records = get_records(ck);

    for (auto& record: records)
    {
        boost::json::string_view const record_pointer = record.jptr;

        if (record.generator->is_noop())
        {
            continue;
        }
        else if (record.maybe_constant.has_value())
        {
            result_value.set_at_pointer(record_pointer, record.maybe_constant.value());
            continue;
        }

        auto& generator = *record.generator;
        auto const& tf = record.transform;

        boost::json::value raw_v;
        // TODO: optimize recursive expr
        // TODO: optimize expr <-> json flow (v_as_expr for error check is expensive here)
        auto const iteration = generator(raw_v);
        auto const v = tf.is_noop() ? raw_v : tf.eval(raw_v); // transform can handle generator errors
        lang::Expression const v_as_expr(v);

        if (!v_as_expr.is_error())
        {
            result_value.set_at_pointer(record_pointer, v);
        }
        else if (ck == ChannelKind::Exception)
        {
            result_value = v_as_expr.to_json();
            break;
        }
        else // unexpected error
        {
            static_cast<void>(iteration);
            // reevaluate to collect log

            auto generator_ctx = lang::EvalContext::make();
            generator.expression().eval(iteration, generator_ctx);

            auto transform_ctx = lang::EvalContext::make();
            if (not tf.is_noop())
            {
                tf.eval(raw_v, transform_ctx);
            }

            ZMBT_LOG_JSON(FATAL) << boost::json::object{
                {"ZMBT_INJECTION", boost::json::object{
                    {"interface", {ifc_id_, obj_id_}},
                    {"group", json_from(ck)},
                    {"pointer", record_pointer},
                    {"Inject log", *generator_ctx.log.stack},
                    {"Take log", *transform_ctx.log.stack},
                }}
            };

            auto const failed_inject = raw_v == v;
            auto const error_context = failed_inject ? "Inject" : "Take";

            auto const origin = format("ZMBT_INJECTION(%s, %s%s, %s)",
                ifc_id_, json_from(ck), record_pointer, error_context);

            ZMBT_LOG_CERR(FATAL).WithSrcLoc(origin) << "\n"
                << (error_context ? generator_ctx.log.str(2) : transform_ctx.log.str(2));
            maybe_error = v_as_expr.to_json();
        }
    }
    return result_value;
}

InjectionTable::Shared InjectionTable::Make(interface_id const& ifc_id, object_id const& obj_id) {
    auto t = std::make_shared<InjectionTable>(ifc_id, obj_id);
    return t;
}

} // namespace
