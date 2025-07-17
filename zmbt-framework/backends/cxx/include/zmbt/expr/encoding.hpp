/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZMBT_EXPR_ENCODING_HPP_
#define ZMBT_EXPR_ENCODING_HPP_

#include <algorithm>
#include <list>
#include <ostream>

#include "zmbt/core.hpp"
#include "zmbt/reflect.hpp"
#include "keyword.hpp"
#include "keyword_grammar.hpp"


namespace zmbt {
namespace lang {

struct Encoding
{
    using K = Keyword;
    using V = boost::json::value;
    std::vector<K> keywords;
    std::vector<std::size_t> depth;
    std::vector<V> data;
    std::vector<V> bindings;

    static bool is_preproc_token(boost::json::value const& value)
    {
        auto const if_str = value.if_string();
        return if_str && if_str->starts_with("$[") && if_str->ends_with("]");
    }

    Encoding() = default;

    explicit Encoding(boost::json::value const& value)
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

    std::size_t size() const
    {
        return keywords.size();
    }

    bool operator==(Encoding const& o) const
    {
        return (keywords == o.keywords)
        && (depth == o.depth)
        && (bindings == o.bindings)
        && (data == o.data);
    }

    bool operator!=(Encoding const& o) const
    {
        return not operator==(o);
    }


    void push_back(K const& k, std::size_t const d, V const& v, V const& b)
    {
        keywords.push_back(k);
        depth   .push_back(d);
        data    .push_back(v);
        bindings.push_back(b);
    }

    void append_to_root(Encoding const& tail)
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
    bool preprocess()
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

};

BOOST_DESCRIBE_STRUCT(Encoding, (void), (keywords, depth, data, bindings))
ZMBT_INJECT_SERIALIZATION


class EncodingView {
public:
    using K = Keyword;
    using V = boost::json::value;

    struct ExprRow {
        K keyword;
        std::size_t depth;
        V const* data;
        V const* binding;
        std::size_t index;
    };

    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = ExprRow;
        using difference_type   = std::ptrdiff_t;
        using pointer           = void;
        using reference         = ExprRow;

        Iterator(K const* k, std::size_t const* d, V const* v, V const* b, std::size_t index, std::size_t offset)
            : k_(k)
            , d_(d)
            , v_(v)
            , b_(b)
            , i_(index)
            , index_offset_(offset)
        {
        }

        reference operator*() const {
            return {k_[i_], d_[i_] - d_[0], &v_[i_], &b_[i_], i_ + index_offset_};
        }

        Iterator& operator++() { ++i_; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        Iterator& operator--() { --i_; return *this; }
        Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

        Iterator& operator+=(difference_type n) { i_ += n; return *this; }
        Iterator& operator-=(difference_type n) { i_ -= n; return *this; }

        reference operator[](difference_type n) const { return *(*this + n); }

        Iterator operator+(difference_type n) const { return Iterator{k_, d_, v_, b_, i_ + n, index_offset_}; }
        Iterator operator-(difference_type n) const { return Iterator{k_, d_, v_, b_, i_ - n, index_offset_}; }

        difference_type operator-(Iterator const& other) const { return i_ - other.i_; }

        bool operator==(Iterator const& other) const { return i_ == other.i_; }
        bool operator!=(Iterator const& other) const { return !(*this == other); }
        bool operator<(Iterator const& other) const  { return i_ < other.i_; }
        bool operator<=(Iterator const& other) const { return i_ <= other.i_; }
        bool operator>(Iterator const& other) const  { return i_ > other.i_; }
        bool operator>=(Iterator const& other) const { return i_ >= other.i_; }

    private:
        K const* k_;
        std::size_t const* d_;
        V const* v_;
        V const* b_;
        std::size_t index_offset_;
        std::size_t i_;
    };

    using iterator = Iterator;
    using const_iterator = Iterator;

    EncodingView(K const* k, std::size_t const* d, V const* v, V const* b,
                 std::size_t sz, std::size_t index_offset)
        : keywords_(k)
        , depth_(d)
        , data_(v)
        , binding_(b)
        , size_(sz)
        , index_offset_(index_offset)
    {
    }

    EncodingView(Encoding&& root) = delete;

    EncodingView(Encoding const& root)
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


    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    std::size_t index_offset() const { return index_offset_; }
    std::size_t depth_offset() const { return depth_[0]; }


    ExprRow front() const { return (*this)[0]; }
    ExprRow back() const { return (*this)[size_ - 1]; }

    ExprRow operator[](std::size_t i) const {
        return ExprRow{keywords_[i], depth_[i] - depth_[0], &data_[i], &binding_[i], i + index_offset_};
    }

    bool operator==(EncodingView const& o) const
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

    bool operator!=(EncodingView const& o) const
    {
        return not operator==(o);
    }


    const_iterator begin() const { return {keywords_, depth_, data_, binding_, 0    , index_offset_}; }
    const_iterator end()   const { return {keywords_, depth_, data_, binding_, size_, index_offset_}; }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend()   const { return end(); }

    Encoding freeze() const {
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

    /// Create a subview [start, start+count)
    EncodingView slice(std::size_t start, std::size_t count) const {
        if (start + count > size_)
        {
            throw std::range_error("Out of bounds EncodingView::slice");
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

    EncodingView subtree(std::size_t const node) const
    {
        std::size_t ignored{};
        return subtree(node, ignored);
    }

    std::list<EncodingView> subtrees() const
    {
        std::list<EncodingView> st;
        if (size() < 2)
        {
            return st;
        }
        std::size_t next{1};
        while(next < size())
        {
            st.emplace_back(subtree(next, next));
        }
        return st;
    }

private:

    EncodingView subtree(std::size_t const node, std::size_t& next) const
    {
        auto it = cbegin() + node;
        if ((it >= cend()) || (it < cbegin()))
        {
            throw std::range_error("Out of bounds EncodingView::subtree");
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

    K const* keywords_;
    std::size_t const* depth_;
    V const* data_;
    V const* binding_;
    std::size_t size_;
    std::size_t index_offset_;
};


}  // namespace lang
}  // namespace zmbt

#endif
