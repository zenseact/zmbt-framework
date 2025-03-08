/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */


#include <boost/json.hpp>
#include <zmbt/core/exceptions.hpp>
#include <algorithm>
#include <iterator>
#include <vector>

#include "zmbt/core/json_iter.hpp"


void zmbt::JsonIterZipProdBase::init_iters(boost::json::array const& seqences)
{
    if (seqences.empty())
    {
        halt_ = true;
        return;
    }
    for (auto const& v: seqences)
    {
        if (!v.is_array())
        {
            throw base_error("not a sequence: `%s`", v);
        }
        ends_.push_back(v.as_array().cend());
        begins_.push_back(v.as_array().cbegin());
    }
    its_ = begins_;
}

boost::json::array zmbt::JsonIterZipProdBase::get()
{
    boost::json::array arr {};
    if (its_ == ends_)
    {
        halt_ = true;
    }
    if (halt_) { return arr; }
    for (auto const& it: its_)
    {
        arr.push_back(*it);
    }
    return arr;
}

void zmbt::JsonProdIter::step()
{
    auto iter   = its_.rbegin();
    auto begin  = begins_.crbegin();
    auto end    = ends_.crbegin();

    while (iter != its_.crend())
    {
        (*iter)++;

        if (*iter != *end) { break; }
        else if (std::next(iter) != its_.rend())
        {
            *iter = *begin;
        }
        else {
            halt_ = true;
            break;
        }
        iter++;
        begin++;
        end++;
    }
}

void zmbt::JsonZipIter::step()
{
    auto iter = its_.begin();
    auto end = ends_.cbegin();

    while(iter != its_.cend())
    {
        (*iter)++;
        if (*iter == *end)
        {
            halt_ = true;
            // if (its_ != ends_)
            // {
                
            // }
        }
        iter++;
        end++;
    }
}
