

# File encoding.hpp

[**File List**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**expr**](dir_5ca6873c4d246ae1a35f5fe5ff3edd5d.md) **>** [**encoding.hpp**](encoding_8hpp.md)

[Go to the documentation of this file](encoding_8hpp.md)


```C++

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

    static bool is_preproc_token(boost::json::value const& value);
    static bool is_link_token(boost::json::value const& value);

    Encoding() = default;
    explicit Encoding(boost::json::value const& value);
    explicit Encoding(boost::json::value && value);

    std::size_t size() const;

    bool operator==(Encoding const& o) const;

    bool operator!=(Encoding const& o) const;


    void push_back(K const& k, std::size_t const d, V const& v);
    void push_back(K const& k, std::size_t const d, V && v);

    void append_to_root(Encoding const& tail);
    void append_to_root(Encoding&& tail);

    bool preprocess();
};




class EncodingView {
public:
    using K = Keyword;
    using V = boost::json::value;

    struct ExprRow {
        K keyword;
        std::size_t depth;
        V const* data;
        std::size_t index;
    };

    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = ExprRow;
        using difference_type   = std::ptrdiff_t;
        using pointer           = void;
        using reference         = ExprRow;

        Iterator(K const* k, std::size_t const* d, V const* v, std::size_t index, std::size_t offset)
            : k_(k)
            , d_(d)
            , v_(v)
            , i_(index)
            , index_offset_(offset)
        {
        }

        reference operator*() const {
            return {k_[i_], d_[i_] - d_[0], &v_[i_], i_ + index_offset_};
        }

        Iterator& operator++() { ++i_; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        Iterator& operator--() { --i_; return *this; }
        Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

        Iterator& operator+=(difference_type n) { i_ += n; return *this; }
        Iterator& operator-=(difference_type n) { i_ -= n; return *this; }

        reference operator[](difference_type n) const { return *(*this + n); }

        Iterator operator+(difference_type n) const { return Iterator{k_, d_, v_, i_ + n, index_offset_}; }
        Iterator operator-(difference_type n) const { return Iterator{k_, d_, v_, i_ - n, index_offset_}; }

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
        std::size_t index_offset_;
        std::size_t i_;
    };

    using       iterator = Iterator;
    using const_iterator = Iterator;
    using       reverse_iterator = std::reverse_iterator<      iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    const_iterator begin()  const { return {keywords_, depth_, data_, 0    , index_offset_}; }
    const_iterator end()    const { return {keywords_, depth_, data_, size_, index_offset_}; }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend()   const { return end(); }

    const_reverse_iterator rbegin()  const { return const_reverse_iterator{end()}; }
    const_reverse_iterator rend()    const { return const_reverse_iterator{begin()}; }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend()   const { return rend(); }

    EncodingView(Encoding&& root) = delete;

    EncodingView() = default;


    EncodingView(K const* k, std::size_t const* d, V const* v,
                 std::size_t sz, std::size_t index_offset);

    EncodingView(Encoding const& root);

    std::size_t size() const;
    bool empty() const;

    std::size_t index_offset() const;
    std::size_t depth_offset() const;


    ExprRow front() const;
    ExprRow back() const;

    ExprRow at(std::size_t i) const;


    ExprRow operator[](std::size_t i) const
    {
        return at(i);
    }

    bool operator==(EncodingView const& o) const;

    bool operator!=(EncodingView const& o) const;

    Encoding freeze() const;

    boost::json::value to_json() const;

    EncodingView slice(std::size_t start, std::size_t count) const noexcept;

    EncodingView subtree(std::size_t const node) const noexcept;

    EncodingView traverse_subtrees(std::size_t const node, std::size_t& next) const noexcept;

    std::vector<EncodingView> children() const;

    std::size_t arity() const;

    Keyword head() const noexcept;

    std::size_t child_idx(int ord) const noexcept;

    EncodingView child(int ord) const noexcept;

    bool is_const() const;
    bool is_boolean() const;

    K const*  if_keywords() const { return keywords_; }
    std::size_t const* if_depth() const { return depth_; }
    V const* if_data() const { return data_; }

private:

    K const* keywords_ = nullptr;
    std::size_t const* depth_ = nullptr;
    V const* data_ = nullptr;
    std::size_t size_ = 0U;
    std::size_t index_offset_ = 0U;
};


}  // namespace lang


template<>
struct reflect::custom_serialization<lang::Encoding>
{

    static boost::json::value
    json_from(lang::Encoding const& enc);

    static lang::Encoding
    dejsonize(boost::json::value const& v);
};

ZMBT_INJECT_SERIALIZATION

}  // namespace zmbt

#endif
```


