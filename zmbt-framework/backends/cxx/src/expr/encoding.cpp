/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
#include <algorithm>
#include <iterator>
#include <list>
#include <ostream>
#include <utility>
#include <vector>

#include "zmbt/application/log.hpp"

#include "zmbt/expr/encoding.hpp"
#include "zmbt/expr/keyword.hpp"
#include "zmbt/expr/attributes.hpp"
#include "zmbt/expr/keyword_grammar.hpp"


namespace zmbt {
namespace lang {


bool Encoding::is_preproc_token(boost::json::value const& value)
{
    auto const if_str = value.if_string();
    return if_str && if_str->starts_with("$[") && if_str->ends_with("]");
}

bool Encoding::is_link_token(boost::json::value const& value)
{
    auto const if_str = value.if_string();
    if (!if_str || if_str->size() <= 1 || !if_str->starts_with("$")) return false;
    auto const second = if_str->at(1);
    auto const end = if_str->back();

    bool const is_backeted // TODO: regex
        =   (second == '[' && end == ']')
        ||  (second == '{' && end == '}')
        ||  (second == '(' && end == ')');
    return not is_backeted && (second != '$');
}

Encoding::Encoding(boost::json::value&& value)
{
    auto const if_obj = value.if_object();

    if (if_obj &&if_obj->contains("keywords")
        && if_obj->contains("depth")
        && if_obj->contains("data")
    )
    {
        keywords = boost::json::value_to<std::vector<K>>(if_obj->at("keywords"));
        depth = boost::json::value_to<std::vector<std::size_t>>(if_obj->at("depth"));
        auto& data_arr = if_obj->at("data").as_array();
        data.insert(data.end(), std::make_move_iterator(data_arr.begin()), std::make_move_iterator(data_arr.end()));
        preprocess();
    }
    else
    {
        auto k = Keyword::Literal;
        if (is_preproc_token(value))
        {
            k = Keyword::PreProc;
        }
        else if (is_link_token(value))
        {
            k = Keyword::Link;
        }
        push_back(k, 0, std::move(value));
    }
}

Encoding::Encoding(boost::json::value const& value)
    : Encoding(boost::json::value{value})
{
}


std::size_t Encoding::size() const
{
    return keywords.size();
}

bool Encoding::operator==(Encoding const& o) const
{
    return (keywords == o.keywords)
    && (depth == o.depth)
    && (data == o.data);
}

bool Encoding::operator!=(Encoding const& o) const
{
    return not operator==(o);
}


void Encoding::push_back(K const& k, std::size_t const d, V const& v)
{
    keywords.push_back(k);
    depth   .push_back(d);
    data    .push_back(v);
}

void Encoding::push_back(K const& k, std::size_t const d, V && v)
{
    keywords.push_back(k);
    depth   .push_back(d);
    data    .push_back(std::move(v));
}

void Encoding::append_to_root(Encoding const& tail)
{
    auto const offset = size();
    auto const tail_size = tail.size();
    if (tail_size == 0)
    {
        return;
    }

    keywords.reserve(offset + tail_size);
    depth.reserve(offset + tail_size);
    data.reserve(offset + tail_size);

    keywords.insert(keywords.end(), tail.keywords.begin(), tail.keywords.end());
    depth.insert(depth.end(), tail.depth.begin(), tail.depth.end());
    data.insert(data.end(), tail.data.begin(), tail.data.end());

    for (std::size_t i = offset; i < depth.size(); ++i)
    {
        ++depth[i];
    }
}

void Encoding::append_to_root(Encoding&& tail)
{
    auto const offset = size();
    auto const tail_size = tail.size();
    if (tail_size == 0)
    {
        return;
    }

    keywords.reserve(offset + tail_size);
    depth.reserve(offset + tail_size);
    data.reserve(offset + tail_size);

    keywords.insert(
        keywords.end(),
        std::make_move_iterator(tail.keywords.begin()),
        std::make_move_iterator(tail.keywords.end()));
    depth.insert(depth.end(), tail.depth.begin(), tail.depth.end());
    data.insert(
        data.end(),
        std::make_move_iterator(tail.data.begin()),
        std::make_move_iterator(tail.data.end()));

    for (std::size_t i = offset; i < depth.size(); ++i)
    {
        ++depth[i];
    }

    tail.keywords.clear();
    tail.depth.clear();
    tail.data.clear();
}


/// Single-pass preprocessing, return true if no multipass tokens left
bool Encoding::preprocess()
{
    bool complete{true};

    if (std::none_of(keywords.begin(), keywords.end(), [](Keyword k) { return k == Keyword::PreProc; }))
    {
        return true;
    }

    std::vector<K> new_keywords;
    std::vector<std::size_t> new_depth;
    std::vector<V> new_data;

    new_keywords.reserve(keywords.size());
    new_depth.reserve(depth.size());
    new_data.reserve(data.size());

    for (std::size_t i = 0; i < keywords.size(); ++i)
    {
        auto const kw = keywords[i];
        auto const d = depth[i];
        auto& payload = data[i];

        if (kw == Keyword::PreProc)
        {
            if (is_preproc_token(payload))
            {
                complete = false;
                new_keywords.push_back(kw);
                new_depth.push_back(d);
                new_data.push_back(payload);
                continue;
            }

            auto subenc = Encoding(payload);
            for (auto& sub_depth : subenc.depth)
            {
                sub_depth += d;
            }

            new_keywords.insert(
                new_keywords.end(),
                std::make_move_iterator(subenc.keywords.begin()),
                std::make_move_iterator(subenc.keywords.end()));
            new_depth.insert(
                new_depth.end(),
                std::make_move_iterator(subenc.depth.begin()),
                std::make_move_iterator(subenc.depth.end()));
            new_data.insert(
                new_data.end(),
                std::make_move_iterator(subenc.data.begin()),
                std::make_move_iterator(subenc.data.end()));
        }
        else
        {
            new_keywords.push_back(kw);
            new_depth.push_back(d);
            new_data.push_back(payload);
        }
    }

    keywords = std::move(new_keywords);
    depth = std::move(new_depth);
    data = std::move(new_data);

    return complete;
}


EncodingView::EncodingView(K const* k, std::size_t const* d, V const* v,
                std::size_t sz, std::size_t index_offset)
    : keywords_(sz ? k : nullptr)
    , depth_(sz ? d : nullptr)
    , data_(sz ? v : nullptr)
    , size_(sz)
    , index_offset_(sz ? index_offset : 0U)
{
}

EncodingView::EncodingView(Encoding const& root)
    : EncodingView(
        root.keywords.data(),
        root.depth.data(),
        root.data.data(),
        root.keywords.size(),
        0U
    )
{
}


std::size_t EncodingView::size() const { return size_; }
bool EncodingView::empty() const { return size_ == 0; }

std::size_t EncodingView::index_offset() const { return index_offset_; }
std::size_t EncodingView::depth_offset() const { return depth_ ? depth_[0] : 0; }


EncodingView::ExprRow EncodingView::front() const { return at(0); }
EncodingView::ExprRow EncodingView::back() const { return at(size_ - 1); }


EncodingView::ExprRow EncodingView::at(std::size_t i) const
{
    if (i >= size_)
    {
        throw_exception(std::range_error("Out of bounds EncodingView::at"));
    }
    auto const depth0 = depth_ ? depth_[0] : 0U;
    return ExprRow{keywords_[i], depth_[i] - depth0, &data_[i], i + index_offset_};
}


bool EncodingView::operator==(EncodingView const& o) const
{
    if (size_ != o.size_)
    {
        return false;
    }

    if (size_ == 0)
    {
        return true;
    }

    if ((keywords_ == o.keywords_)
        && (depth_ == o.depth_)
        && (data_ == o.data_)
    )
    {
        return true;
    }

    if (not std::equal(keywords_, keywords_ + size_, o.keywords_))
    {
        return false;
    }

    auto const lhs_base = depth_[0];
    auto const rhs_base = o.depth_[0];
    for (std::size_t i = 0; i < size_; ++i)
    {
        if ((depth_[i] - lhs_base) != (o.depth_[i] - rhs_base))
        {
            return false;
        }
    }

    return std::equal(data_, data_ + size_, o.data_);
}

bool EncodingView::operator!=(EncodingView const& o) const
{
    return not operator==(o);
}


Encoding EncodingView::freeze() const {
    Encoding out;
    if (size_ == 0)
    {
        return out;
    }

    out.keywords.assign(keywords_, keywords_ + size_);
    out.depth.reserve(size_);
    auto const base = depth_[0];
    for (std::size_t i = 0; i < size_; ++i)
    {
        out.depth.push_back(depth_[i] - base);
    }
    out.data.assign(data_, data_ + size_);
    return out;
}


EncodingView EncodingView::slice(std::size_t start, std::size_t count) const noexcept
{
    if ((count == 0) || (start + count > size_))
    {
        return EncodingView{};
    }
    return EncodingView{
        keywords_ + start,
        depth_ + start,
        data_ + start,
        count,
        index_offset_ + start
    };
}

boost::json::value EncodingView::to_json() const
{
    boost::json::array out_keywords;
    boost::json::array out_depth;
    boost::json::array out_data;

    out_keywords.reserve(size_);
    out_depth.reserve(size_);
    out_data.reserve(size_);

    auto const base = depth_ ? depth_[0] : 0U;
    for (std::size_t i = 0; i < size_; ++i)
    {
        out_keywords.push_back(static_cast<std::underlying_type_t<K>>(keywords_[i]));
        out_depth.push_back(depth_[i] - base);
        out_data.push_back(data_[i]);
    }

    return {
        {"keywords", std::move(out_keywords)},
        {"depth", std::move(out_depth)},
        {"data", std::move(out_data)},
    };
}

EncodingView EncodingView::subtree(std::size_t const node) const noexcept
{
    std::size_t ignored{};
    return traverse_subtrees(node, ignored);
}

std::size_t EncodingView::arity() const
{
    if (empty()) return 0;

    std::size_t const offset = depth_[0];
    std::size_t n {0};
    for (size_t i = 0; i < size(); i++)
    {
        if ((depth_[i] - offset) == 1)
        {
            ++n;
        }
    }
    return n;
}

std::vector<EncodingView> EncodingView::children() const
{
    std::vector<EncodingView> st;
    if (size() < 2)
    {
        return st;
    }
    st.reserve(size() - 1);
    std::size_t next{1};
    while(next < size())
    {
        st.emplace_back(traverse_subtrees(next, next));
    }
    return st;
}

Keyword EncodingView::head() const noexcept
{
    return keywords_ ? keywords_[0] : Keyword::Undefined;
}

EncodingView EncodingView::child(int ord) const noexcept
{
    return subtree(child_idx(ord));
}

std::size_t EncodingView::child_idx(int ord) const noexcept
{
    std::size_t idx{size_};

    if (ord < 0)
    {
        for (std::int64_t i = size_-1; i >=0; --i)
        {
            if ((depth_[static_cast<std::size_t>(i)] - depth_[0]) == 1)
            {
                idx = i;
                if (++ord >= 0) break;
            }
        }
    }
    else
    {
        for (std::size_t i = 0; i < size_; ++i)
        {
            if ((depth_[i] - depth_[0]) == 1)
            {
                idx = i;
                if (--ord < 0) break;
            }
        }
    }
    return idx;
}


bool EncodingView::is_const() const
{
    auto const a = attributes(head());
    if (a & attr::is_pipe && size() > 1)
    {
        {
            // TODO: make it safe (maybe move to Encoding init)
            auto const kwrds = if_keywords();
            for (std::size_t i = 0; i < size(); i++)
            {
                if (attributes(kwrds[i]) & attr::is_sideeffect)
                {
                    return false;
                }
            }
        }
        std::size_t next{1};
        auto st = traverse_subtrees(next, next);
        if (st.is_const() || st.head() == Keyword::Literal) return true;
        while(next < size())
        {
            st = traverse_subtrees(next, next);
            if (st.is_const() && (st.head() != Keyword::Literal))
            {
                return true;
            }
        }
        return false;
    }
    else if ((a & attr::is_fork) && (size() > 1))
    {
        std::size_t next{1};
        while(next < size())
        {
            if (not traverse_subtrees(next, next).is_const()) return false;
        }
        return true;
    }
    else if ((a & attr::is_overload) && (size() > 1))
    {
        return subtree(2).is_const();
    }
    else
    {
        return (a & attr::is_literal)
            || (a & attr::is_quote)
            || (a & attr::is_preproc)
            || (a & attr::is_noop)
            || (a & attr::is_error)
            || (a & attr::is_const);
    }
}

bool EncodingView::is_boolean() const
{
    auto const a = attributes(head());

    if(a & attr::is_literal && size() > 1)
    {
        return data_[1].is_bool();
    }
    else if (a & attr::is_predicate)
    {
        return true;
    }
    else if (a & attr::is_overload && size() > 1)
    {
        return child(1).child(-1).is_boolean();
    }
    if (a & attr::is_pipe && size() > 1)
    {
        return child(-1).is_boolean();
    }
    return false;
}



EncodingView EncodingView::traverse_subtrees(std::size_t const node, std::size_t& next) const noexcept
{
    auto it = cbegin() + node;
    if ((it >= cend()) || (it < cbegin()))
    {
        return slice(0, 0);
    }

    next = size_;
    std::size_t const node_depth = (*it).depth;
    std::size_t count {1};
    ++it;
    while (it < cend())
    {
        if ((*it).depth <= node_depth)
        {
            next = (*it).index - index_offset_;
            break;
        }
        ++count;
        ++it;
    }
    return slice(node, count);
}

}  // namespace lang


boost::json::value
reflect::custom_serialization<lang::Encoding>::json_from(lang::Encoding const& enc)
{
    return lang::EncodingView(enc).to_json();
}

lang::Encoding
reflect::custom_serialization<lang::Encoding>::dejsonize(boost::json::value const& v)
{
    return lang::Encoding(v);
}


}  // namespace zmbt
