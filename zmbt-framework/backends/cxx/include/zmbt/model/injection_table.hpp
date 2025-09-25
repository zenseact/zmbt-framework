/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_MODEL_INJECTION_TABLE_HPP_
#define ZMBT_MODEL_INJECTION_TABLE_HPP_

#include <memory>
#include <vector>

#include "zmbt/core.hpp"
#include "zmbt/expr.hpp"
#include "generator.hpp"
#include "channel_kind.hpp"


namespace zmbt {


class InjectionTable
{
  public:
    using Shared = std::shared_ptr<InjectionTable>;

    struct Record
    {
        boost::json::string jptr;
        Generator::Shared generator;
        lang::Expression transform;
        boost::optional<boost::json::value> maybe_constant;

        Record(
            boost::json::string_view jp,
            Generator::Shared gen,
            lang::Expression const& tf
        );
    };

  private:
    interface_id ifc_id_;
    object_id obj_id_;
    reflect::Prototypes prototypes_;

    std::vector<Record> args_;
    std::vector<Record> ret_;
    std::vector<Record> exception_;

    boost::optional<boost::json::value> maybe_constant_args_;
    boost::optional<boost::json::value> maybe_constant_ret_;
    boost::optional<boost::json::value> maybe_constant_exception_;


    std::vector<Record>& get_records(ChannelKind const& ck) &;
    boost::optional<boost::json::value>& get_maybe_const(ChannelKind const& ck) &;

  public:

    InjectionTable(interface_id const& ifc_id, object_id const& obj_id);
    void add_record(ChannelKind const& ck, Record&& rec);
    Record& last_record(ChannelKind const& ck) &;

    /// Evaluate input for given category and return error object if any, null otherwise
    boost::json::value yield(ChannelKind const& ck, boost::json::value& result_value);

    static Shared Make(interface_id const& ifc_id, object_id const& obj_id);
};

} // namespace

#endif
