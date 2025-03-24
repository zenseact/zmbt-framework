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
TestDiagnostics& TestDiagnostics::Error (boost::json::string_view origin, boost::json::string_view msg)
{
    this->kind = Result::Error;
    this->message = format("%s: %s", origin, msg);
    return *this;
}

/// report test expectation failure
TestDiagnostics& TestDiagnostics::Fail(Expression expected, boost::json::value observed, SignalOperatorHandler op)
{
    this->kind = Result::Fail;
    this->expected = expected.serialize();
    this->observed = observed;
    this->decorator = op.annotation();
    this->message = "expectation match failed";
    return *this;
}

boost::json::value TestDiagnostics::to_json() const
{
    boost::json::string verdict = "";

    switch (this->kind)
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

    return boost::json::object {
        {"model"         , this->model       },
        {"test"          , this->nof_vector  },
        {"trigger"       , this->trigger     },
        {"channel"       , this->nof_channel },
        {"verdict"       , verdict           },
        {"message"       , this->message     },
        {"expected"      , this->expected    },
        {"test vector"   , this->vector      },
        {"observed"      , this->observed    },
        {"decorator"     , this->decorator   },
        {"description"   , this->description },
        {"comment"       , this->comment     },
        {"channels"      , this->channels    },
        {"eval_stack"    , this->eval_stack  },
    };
}


} // namespace mapping
} // namespace zmbt
