/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */
#include <algorithm>
#include <list>
#include <ostream>

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


Encoding::Encoding(boost::json::value const& value)
{
    auto const if_obj = value.if_object();

    if (if_obj &&if_obj->contains("keywords")
        && if_obj->contains("depth")
        && if_obj->contains("data")
        && if_obj->contains("bindings")
    )
    {
        *this = boost::json::value_to<Encoding>(value);
        preprocess();
        // while (preprocess()) // TODO: test and enable multipass pp
        // {
        // }
    }
    else
    {
        auto k = Keyword::Literal;
        if (is_preproc_token(value))
        {
            k = Keyword::PreProc;
        }
        push_back(k, 0, value, nullptr);
    }
}

std::size_t Encoding::size() const
{
    return keywords.size();
}

bool Encoding::operator==(Encoding const& o) const
{
    return (keywords == o.keywords)
    && (depth == o.depth)
    && (bindings == o.bindings)
    && (data == o.data);
}

bool Encoding::operator!=(Encoding const& o) const
{
    return not operator==(o);
}


void Encoding::push_back(K const& k, std::size_t const d, V const& v, V const& b)
{
    keywords.push_back(k);
    depth   .push_back(d);
    data    .push_back(v);
    bindings.push_back(b);
}

void Encoding::append_to_root(Encoding const& tail)
{
    keywords.reserve(size() + tail.size());
    depth   .reserve(size() + tail.size());
    data    .reserve(size() + tail.size());
    bindings.reserve(size() + tail.size());

    auto d = depth.end();

    keywords.insert(keywords.end(), tail.keywords.cbegin(), tail.keywords.cend());
    depth   .insert(depth   .end(), tail.depth   .cbegin(), tail.depth   .cend());
    data    .insert(data    .end(), tail.data    .cbegin(), tail.data    .cend());
    bindings.insert(bindings.end(), tail.bindings.cbegin(), tail.bindings.cend());

    while(d < depth.cend())
    {
        ++(*d++);
    }
}


/// Single-pass preprocessing, return true if no multipass tokens left
bool Encoding::preprocess()
{
    bool complete{true};
    auto const keywords_end = keywords.cend();
    auto it_keywords = keywords.cbegin();

    if (std::find(it_keywords, keywords_end, Keyword::PreProc) == keywords_end)
    {
        return complete;
    }

    decltype(keywords)  new_keywords;
    decltype(depth)     new_depth;
    decltype(data)      new_data;
    decltype(bindings)  new_bindings;

    new_keywords.reserve(keywords.size());
    new_depth   .reserve(depth   .size());
    new_data    .reserve(data    .size());
    new_bindings.reserve(bindings.size());

    auto it_depth = depth.cbegin();
    auto it_data = data.cbegin();
    auto it_bindings = bindings.cbegin();

    while(it_keywords != keywords_end)
    {
        if (*it_keywords == Keyword::PreProc)
        {
            if (!is_preproc_token(*it_data)) // ensure single pass, no recursion
            {
                auto subenc = Encoding(*it_data);
                for (auto& d: subenc.depth)
                {
                    d += *it_depth;
                }

                new_keywords.reserve(subenc.keywords.size());
                new_depth   .reserve(subenc.depth   .size());
                new_data    .reserve(subenc.data    .size());
                new_bindings.reserve(subenc.bindings.size());
                new_keywords.insert(new_keywords.end(), std::make_move_iterator(subenc.keywords.begin()), std::make_move_iterator(subenc.keywords.end()));
                new_depth   .insert(new_depth   .end(), std::make_move_iterator(subenc.depth   .begin()), std::make_move_iterator(subenc.depth   .end()));
                new_data    .insert(new_data    .end(), std::make_move_iterator(subenc.data    .begin()), std::make_move_iterator(subenc.data    .end()));
                new_bindings.insert(new_bindings.end(), std::make_move_iterator(subenc.bindings.begin()), std::make_move_iterator(subenc.bindings.end()));
            }
            else
            {
                complete = false;
                new_keywords.push_back(*it_keywords);
                new_depth   .push_back(*it_depth);
                new_data    .push_back(*it_data);
                new_bindings.push_back(*it_bindings);
            }
        }
        else if (*it_keywords != Keyword::PreProc)
        {
            new_keywords.push_back(*it_keywords);
            new_depth   .push_back(*it_depth);
            new_data    .push_back(*it_data);
            new_bindings.push_back(*it_bindings);
        }

        ++it_keywords;
        ++it_depth;
        ++it_data;
        ++it_bindings;
    }

    keywords = new_keywords;
    depth    = new_depth;
    data     = new_data;
    bindings = new_bindings;

    return complete;
}



EncodingView::EncodingView(K const* k, std::size_t const* d, V const* v, V const* b,
                std::size_t sz, std::size_t index_offset)
    : keywords_(k)
    , depth_(d)
    , data_(v)
    , binding_(b)
    , size_(sz)
    , index_offset_(index_offset)
{
}

EncodingView::EncodingView(Encoding const& root)
    : EncodingView(
        root.keywords.cbegin().base(),
        root.depth.cbegin().base(),
        root.data.cbegin().base(),
        root.bindings.cbegin().base(),
        root.keywords.size(),
        0
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
        throw std::range_error("Out of bounds EncodingView::at");
    }
    return ExprRow{keywords_[i], depth_[i] - depth_[0], &data_[i], &binding_[i], i + index_offset_};
}


bool EncodingView::operator==(EncodingView const& o) const
{
    if (size_ != o.size_)
    {
        return false;
    }

    if ((keywords_ == o.keywords_)
        && (depth_ == o.depth_)
        && (data_ == o.data_)
        && (binding_ == o.binding_)
    )
    {
        return true;
    }

    if (not std::equal(keywords_, keywords_ + size_, o.keywords_))
    {
        return false;
    }

    for (std::size_t i = 0; i < size_; ++i)
    {
        if ((depth_[i] - depth_[0]) != (o.depth_[i] - o.depth_[0]))
        {
            return false;
        }
    }

    return std::equal(binding_, binding_ + size_ , o.binding_)
        && std::equal(data_, data_ + size_, o.data_);
}

bool EncodingView::operator!=(EncodingView const& o) const
{
    return not operator==(o);
}


Encoding EncodingView::freeze() const {
    Encoding out;
    out.keywords.assign(keywords_, keywords_ + size_);
    out.depth.reserve(size_);
    for (std::size_t i = 0; i < size_; ++i)
    {
        out.depth.push_back(depth_[i] - depth_[0]);
    }
    out.data.assign(data_, data_ + size_);
    out.bindings.assign(binding_, binding_ + size_);
    return out;
}


EncodingView EncodingView::slice(std::size_t start, std::size_t count) const noexcept
{
    if ((count == 0) || (start + count > size_))
    {
        return EncodingView{
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            0,
            0
        };
    }
    return EncodingView{
        keywords_ + start,
        depth_ + start,
        data_ + start,
        binding_ + start,
        count,
        index_offset_ + start
    };
}

EncodingView EncodingView::subtree(std::size_t const node) const noexcept
{
    std::size_t ignored{};
    return traverse_subtrees(node, ignored);
}

std::list<EncodingView> EncodingView::children() const
{
    std::list<EncodingView> st;
    if (size() < 2)
    {
        return st;
    }
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
        for (std::size_t i = size_-1; i >=0; --i)
        {
            if ((depth_[i] - depth_[0]) == 1)
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
    else if (a & attr::is_fork && size() > 1)
    {
        std::size_t next{1};
        while(next < size())
        {
            if (not traverse_subtrees(next, next).is_const()) return false;
        }
        return true;
    }
    else if (a & attr::is_overload && size() > 1)
    {
        return subtree(1).is_const();
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
    // if (is(Keyword::Op) && has_params())
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
}  // namespace zmbt
