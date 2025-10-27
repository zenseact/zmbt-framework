

# Class zmbt::lang::EncodingView::Iterator



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) **>** [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::ptrdiff\_t | [**difference\_type**](#typedef-difference_type)  <br> |
| typedef std::random\_access\_iterator\_tag | [**iterator\_category**](#typedef-iterator_category)  <br> |
| typedef void | [**pointer**](#typedef-pointer)  <br> |
| typedef [**ExprRow**](structzmbt_1_1lang_1_1EncodingView_1_1ExprRow.md) | [**reference**](#typedef-reference)  <br> |
| typedef [**ExprRow**](structzmbt_1_1lang_1_1EncodingView_1_1ExprRow.md) | [**value\_type**](#typedef-value_type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Iterator**](#function-iterator) ([**K**](namespacezmbt_1_1lang.md#enum-keyword) const \* k, std::size\_t const \* d, V const \* v, std::size\_t index, std::size\_t offset) <br> |
|  bool | [**operator!=**](#function-operator) ([**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) const & other) const<br> |
|  [**reference**](structzmbt_1_1lang_1_1EncodingView_1_1ExprRow.md) | [**operator\***](#function-operator_1) () const<br> |
|  [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) | [**operator+**](#function-operator_2) (difference\_type n) const<br> |
|  [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) & | [**operator++**](#function-operator_3) () <br> |
|  [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) | [**operator++**](#function-operator_4) (int) <br> |
|  [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) & | [**operator+=**](#function-operator_5) (difference\_type n) <br> |
|  [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) | [**operator-**](#function-operator-) (difference\_type n) const<br> |
|  difference\_type | [**operator-**](#function-operator-_1) ([**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) const & other) const<br> |
|  [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) & | [**operator--**](#function-operator-) () <br> |
|  [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) | [**operator--**](#function-operator-_1) (int) <br> |
|  [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) & | [**operator-=**](#function-operator-_2) (difference\_type n) <br> |
|  bool | [**operator&lt;**](#function-operator_6) ([**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) const & other) const<br> |
|  bool | [**operator&lt;=**](#function-operator_7) ([**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) const & other) const<br> |
|  bool | [**operator==**](#function-operator_8) ([**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) const & other) const<br> |
|  bool | [**operator&gt;**](#function-operator_9) ([**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) const & other) const<br> |
|  bool | [**operator&gt;=**](#function-operator_10) ([**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) const & other) const<br> |
|  [**reference**](structzmbt_1_1lang_1_1EncodingView_1_1ExprRow.md) | [**operator[]**](#function-operator_11) (difference\_type n) const<br> |




























## Public Types Documentation




### typedef difference\_type 

```C++
using zmbt::lang::EncodingView::Iterator::difference_type =  std::ptrdiff_t;
```




<hr>



### typedef iterator\_category 

```C++
using zmbt::lang::EncodingView::Iterator::iterator_category =  std::random_access_iterator_tag;
```




<hr>



### typedef pointer 

```C++
using zmbt::lang::EncodingView::Iterator::pointer =  void;
```




<hr>



### typedef reference 

```C++
using zmbt::lang::EncodingView::Iterator::reference =  ExprRow;
```




<hr>



### typedef value\_type 

```C++
using zmbt::lang::EncodingView::Iterator::value_type =  ExprRow;
```




<hr>
## Public Functions Documentation




### function Iterator 

```C++
inline zmbt::lang::EncodingView::Iterator::Iterator (
    K const * k,
    std::size_t const * d,
    V const * v,
    std::size_t index,
    std::size_t offset
) 
```




<hr>



### function operator!= 

```C++
inline bool zmbt::lang::EncodingView::Iterator::operator!= (
    Iterator const & other
) const
```




<hr>



### function operator\* 

```C++
inline reference zmbt::lang::EncodingView::Iterator::operator* () const
```




<hr>



### function operator+ 

```C++
inline Iterator zmbt::lang::EncodingView::Iterator::operator+ (
    difference_type n
) const
```




<hr>



### function operator++ 

```C++
inline Iterator & zmbt::lang::EncodingView::Iterator::operator++ () 
```




<hr>



### function operator++ 

```C++
inline Iterator zmbt::lang::EncodingView::Iterator::operator++ (
    int
) 
```




<hr>



### function operator+= 

```C++
inline Iterator & zmbt::lang::EncodingView::Iterator::operator+= (
    difference_type n
) 
```




<hr>



### function operator- 

```C++
inline Iterator zmbt::lang::EncodingView::Iterator::operator- (
    difference_type n
) const
```




<hr>



### function operator- 

```C++
inline difference_type zmbt::lang::EncodingView::Iterator::operator- (
    Iterator const & other
) const
```




<hr>



### function operator-- 

```C++
inline Iterator & zmbt::lang::EncodingView::Iterator::operator-- () 
```




<hr>



### function operator-- 

```C++
inline Iterator zmbt::lang::EncodingView::Iterator::operator-- (
    int
) 
```




<hr>



### function operator-= 

```C++
inline Iterator & zmbt::lang::EncodingView::Iterator::operator-= (
    difference_type n
) 
```




<hr>



### function operator&lt; 

```C++
inline bool zmbt::lang::EncodingView::Iterator::operator< (
    Iterator const & other
) const
```




<hr>



### function operator&lt;= 

```C++
inline bool zmbt::lang::EncodingView::Iterator::operator<= (
    Iterator const & other
) const
```




<hr>



### function operator== 

```C++
inline bool zmbt::lang::EncodingView::Iterator::operator== (
    Iterator const & other
) const
```




<hr>



### function operator&gt; 

```C++
inline bool zmbt::lang::EncodingView::Iterator::operator> (
    Iterator const & other
) const
```




<hr>



### function operator&gt;= 

```C++
inline bool zmbt::lang::EncodingView::Iterator::operator>= (
    Iterator const & other
) const
```




<hr>



### function operator[] 

```C++
inline reference zmbt::lang::EncodingView::Iterator::operator[] (
    difference_type n
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/encoding.hpp`

