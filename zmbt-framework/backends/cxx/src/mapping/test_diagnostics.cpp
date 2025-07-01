/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "zmbt/mapping/test_diagnostics.hpp"

#include <zmbt/core/format_string.hpp>

namespace zmbt {
namespace mapping {


/// report test setup or execution error
TestDiagnostics& TestDiagnostics::Error(boost::json::string_view origin, boost::json::string_view msg)
{
    this->result = Result::Error;
    this->message = format("%s: %s", origin, msg);
    return *this;
}

/// report test expectation failure
TestDiagnostics& TestDiagnostics::Fail(lang::Expression expected, boost::json::value observed)
{
    this->result = Result::Fail;
    this->expected = expected.serialize();
    this->observed = observed;
    this->message = "expectation match failed";
    return *this;
}

boost::json::value TestDiagnostics::to_json() const
{
    boost::json::string verdict = "";

    switch (this->result)
    {
    case Result::Fail:
        verdict = "FAIL";
        break;
    case Result::Error:
        verdict = "ERROR";
        break;
    case Result::Success:
    default:
        verdict = "OK";
        break;
    }

    boost::json::value cond = tabular_condition_failure_
        ? boost::json::value{this->tr, this->tc}
        : boost::json::value{{"pipe", this->pipe_id}};

    return boost::json::object {
        {"model"         , this->model_name  },
        {"condition"     , cond              },
        {"channel"       , this->channel_id  },
        {"verdict"       , verdict           },
        {"message"       , this->message     },
        {"expected"      , this->expected    },
        {"observed"      , this->observed    },
        // {"test vector"   , this->vector      }, // TODO: refine with respect to inline conditions
        {"description"   , this->description },
        {"comment"       , this->comment     },
        {"eval_stack"    , this->eval_stack  },
    };
}


} // namespace mapping
} // namespace zmbt
