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


struct InjectionTable
{
    using Shared = std::shared_ptr<InjectionTable>;

    struct Record
    {
        boost::json::string jptr;
        Generator::Shared generator;
        lang::Expression transform;
    };

    std::vector<Record> args;
    std::vector<Record> ret;
    std::vector<Record> exception;

    std::vector<Record>& at(ChannelKind const& ck) &
    {
        static std::vector<Record> dummy {};
        switch(ck)
        {
            case ChannelKind::Args:
                return args;
            case ChannelKind::Return:
                return ret;
            case ChannelKind::Exception:
                return exception;
            default:
            throw_exception(environment_error(""));
                return dummy;
        }
    }


    static Shared Make() {
        auto t = std::make_shared<InjectionTable>();
        return t;
    }
};

} // namespace

#endif
