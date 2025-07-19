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
#include "attributes.hpp"
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

    static bool is_preproc_token(boost::json::value const& value);

    Encoding() = default;

    explicit Encoding(boost::json::value const& value);

    std::size_t size() const;

    bool operator==(Encoding const& o) const;

    bool operator!=(Encoding const& o) const;


    void push_back(K const& k, std::size_t const d, V const& v, V const& b);

    void append_to_root(Encoding const& tail);

    /// Single-pass preprocessing, return true if no multipass tokens left
    bool preprocess();
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

    using       iterator = Iterator;
    using const_iterator = Iterator;
    using       reverse_iterator = std::reverse_iterator<      iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    const_iterator begin()  const { return {keywords_, depth_, data_, binding_, 0    , index_offset_}; }
    const_iterator end()    const { return {keywords_, depth_, data_, binding_, size_, index_offset_}; }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend()   const { return end(); }

    const_reverse_iterator rbegin()  const { return const_reverse_iterator{end()}; }
    const_reverse_iterator rend()    const { return const_reverse_iterator{begin()}; }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend()   const { return rend(); }

    EncodingView(Encoding&& root) = delete;

    EncodingView(K const* k, std::size_t const* d, V const* v, V const* b,
                 std::size_t sz, std::size_t index_offset);

    EncodingView(Encoding const& root);

    std::size_t size() const;
    bool empty() const;

    std::size_t index_offset() const;
    std::size_t depth_offset() const;


    ExprRow front() const;
    ExprRow back() const;

    /// Get node at index i
    /// Throws std::range_error if out of bounds
    ExprRow at(std::size_t i) const;


    ExprRow operator[](std::size_t i) const
    {
        return at(i);
    }

    bool operator==(EncodingView const& o) const;

    bool operator!=(EncodingView const& o) const;

    /// Hardcopy referenced encoding subview
    Encoding freeze() const;

    /// Create a subview [start, start+count)
    EncodingView slice(std::size_t start, std::size_t count) const noexcept;

    /// Get a subview at specified node
    EncodingView subtree(std::size_t const node) const noexcept;

    /// Traverse subtrees from starting position.
    /// `next` arg points to the next subtree index at the same depth,
    /// or to size() if none.
    EncodingView traverse_subtrees(std::size_t const node, std::size_t& next) const noexcept;

    /// Children subviews
    std::list<EncodingView> children() const;

    /// Number of children
    std::size_t arity() const;

    /// Root node keyword
    Keyword head() const noexcept;

    /// Get child # ord node index.
    /// Negative ord resolves as reverse.
    /// If not found, returns size()
    std::size_t child_idx(int ord) const noexcept;

    /// Get subview on child # ord
    /// If not found, returns empty view
    EncodingView child(int ord) const noexcept;

    bool is_const() const;
    bool is_boolean() const;

    K const*  if_keywords() const { return keywords_; }
    std::size_t const* if_depth() const { return depth_; }
    V const* if_data() const { return data_; }
    V const* if_binding() const { return binding_; }

private:

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
