/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <memory>
#include <mutex>
#include <ostream>
#include <boost/json.hpp>

#include "zmbt/expr/global_env.hpp"

#include "zmbt/application/log.hpp"
#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "zmbt/expr/operator.hpp"


namespace zmbt {
namespace lang {


class GlobalEnv::Impl
{
  public:
    static std::shared_ptr<Impl> instance()
    {
        static std::shared_ptr<Impl> inst {std::make_shared<Impl>()};
        return inst;
    }

    Impl()
    {
        Reset();
    }

    boost::json::value Store(boost::json::string_view const json_pointer, boost::json::value value) noexcept
    try
    {
        if (json_pointer.empty())
        {
            return detail::make_error_expr(
                "Global environment root is not writeable (empty JSON Pointer)",
                "EnvStore"
            );
        }
        else if(json_pointer.starts_with('/'))
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return state_.set_at_pointer(json_pointer, value);
        }
        else
        {
            std::lock_guard<std::mutex> lock(mutex_);
            state_.get_object().insert_or_assign(json_pointer, value);
            return value;
        }
    }
    catch(const std::exception& e)
    {
        return detail::make_error_expr(e.what(), "EnvStore");
    }

    boost::json::value Load(boost::json::string_view const json_pointer) const noexcept
    try
    {
        std::error_code ec;
        boost::json::value const* found;

        if (json_pointer.empty())
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return state_;
        }
        else if(json_pointer.starts_with('/'))
        {
            std::lock_guard<std::mutex> lock(mutex_);
            found = state_.find_pointer(json_pointer, ec);
        }
        else
        {
            std::lock_guard<std::mutex> lock(mutex_);
            found = state_.get_object().if_contains(json_pointer);
        }
        return found ? *found : nullptr;
    }
    catch(const std::exception& e)
    {
        return detail::make_error_expr(e.what(), "EnvLoad");
    }

    void Reset()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        state_.emplace_object();
    }

    void print(std::ostream& os) const
    {
        auto const pretty_print = Logger::is_pretty_print_enabled();

        std::lock_guard<std::mutex> lock(mutex_);
        if (pretty_print)
        {
            zmbt::pretty_print(os, state_);
        }
        else
        {
            os << state_;
        }
    }

private:
    mutable std::mutex mutex_{};
    boost::json::value state_{};
};


GlobalEnv::GlobalEnv() : impl_{Impl::instance()}
{
}

GlobalEnv::~GlobalEnv() {}

boost::json::value GlobalEnv::Store(boost::json::string_view const json_pointer, boost::json::value const& value) noexcept
{
    return impl_->Store(json_pointer, value);
}

boost::json::value GlobalEnv::Load(boost::json::string_view const json_pointer) const noexcept
{
    return impl_->Load(json_pointer);
}

void GlobalEnv::Reset() noexcept
{
    return impl_->Reset();
}

std::ostream& operator<< (std::ostream& os, GlobalEnv const& env)
{
    env.impl_->print(os);
    return os;
}

}  // namespace lang
}  // namespace zmbt
