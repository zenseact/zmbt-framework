

# Class zmbt::lang::EncodingView



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**lang**](namespacezmbt_1_1lang.md) **>** [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md)




















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**ExprRow**](structzmbt_1_1lang_1_1EncodingView_1_1ExprRow.md) <br> |
| class | [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**K**](#typedef-k)  <br> |
| typedef boost::json::value | [**V**](#typedef-v)  <br> |
| typedef [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) | [**const\_iterator**](#typedef-const_iterator)  <br> |
| typedef std::reverse\_iterator&lt; [**const\_iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) &gt; | [**const\_reverse\_iterator**](#typedef-const_reverse_iterator)  <br> |
| typedef [**Iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) | [**iterator**](#typedef-iterator)  <br> |
| typedef std::reverse\_iterator&lt; [**iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) &gt; | [**reverse\_iterator**](#typedef-reverse_iterator)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**EncodingView**](#function-encodingview-14) ([**Encoding**](structzmbt_1_1lang_1_1Encoding.md) && root) = delete<br> |
|   | [**EncodingView**](#function-encodingview-24) () = default<br> |
|   | [**EncodingView**](#function-encodingview-34) ([**K**](namespacezmbt_1_1lang.md#enum-keyword) const \* k, std::size\_t const \* d, V const \* v, std::size\_t sz, std::size\_t index\_offset) <br> |
|   | [**EncodingView**](#function-encodingview-44) ([**Encoding**](structzmbt_1_1lang_1_1Encoding.md) const & root) <br> |
|  std::size\_t | [**arity**](#function-arity) () const<br>_Number of children._  |
|  [**ExprRow**](structzmbt_1_1lang_1_1EncodingView_1_1ExprRow.md) | [**at**](#function-at) (std::size\_t i) const<br> |
|  [**ExprRow**](structzmbt_1_1lang_1_1EncodingView_1_1ExprRow.md) | [**back**](#function-back) () const<br> |
|  [**const\_iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) | [**begin**](#function-begin) () const<br> |
|  [**const\_iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) | [**cbegin**](#function-cbegin) () const<br> |
|  [**const\_iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) | [**cend**](#function-cend) () const<br> |
|  [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) | [**child**](#function-child) (int ord) noexcept const<br> |
|  std::size\_t | [**child\_idx**](#function-child_idx) (int ord) noexcept const<br> |
|  std::vector&lt; [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) &gt; | [**children**](#function-children) () const<br>_Children subviews._  |
|  const\_reverse\_iterator | [**crbegin**](#function-crbegin) () const<br> |
|  const\_reverse\_iterator | [**crend**](#function-crend) () const<br> |
|  std::size\_t | [**depth\_offset**](#function-depth_offset) () const<br> |
|  bool | [**empty**](#function-empty) () const<br> |
|  [**const\_iterator**](classzmbt_1_1lang_1_1EncodingView_1_1Iterator.md) | [**end**](#function-end) () const<br> |
|  [**Encoding**](structzmbt_1_1lang_1_1Encoding.md) | [**freeze**](#function-freeze) () const<br>_Hardcopy referenced encoding subview._  |
|  [**ExprRow**](structzmbt_1_1lang_1_1EncodingView_1_1ExprRow.md) | [**front**](#function-front) () const<br> |
|  [**Keyword**](namespacezmbt_1_1lang.md#enum-keyword) | [**head**](#function-head) () noexcept const<br>_Root node keyword._  |
|  V const \* | [**if\_data**](#function-if_data) () const<br> |
|  std::size\_t const \* | [**if\_depth**](#function-if_depth) () const<br> |
|  [**K**](namespacezmbt_1_1lang.md#enum-keyword) const \* | [**if\_keywords**](#function-if_keywords) () const<br> |
|  std::size\_t | [**index\_offset**](#function-index_offset) () const<br> |
|  bool | [**is\_boolean**](#function-is_boolean) () const<br> |
|  bool | [**is\_const**](#function-is_const) () const<br> |
|  bool | [**operator!=**](#function-operator) ([**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) const & o) const<br> |
|  bool | [**operator==**](#function-operator_1) ([**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) const & o) const<br> |
|  [**ExprRow**](structzmbt_1_1lang_1_1EncodingView_1_1ExprRow.md) | [**operator[]**](#function-operator_2) (std::size\_t i) const<br> |
|  const\_reverse\_iterator | [**rbegin**](#function-rbegin) () const<br> |
|  const\_reverse\_iterator | [**rend**](#function-rend) () const<br> |
|  std::size\_t | [**size**](#function-size) () const<br> |
|  [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) | [**slice**](#function-slice) (std::size\_t start, std::size\_t count) noexcept const<br>_Create a subview [start, start+count)_  |
|  [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) | [**subtree**](#function-subtree) (std::size\_t const node) noexcept const<br>_Get a subview at specified node._  |
|  boost::json::value | [**to\_json**](#function-to_json) () const<br> |
|  [**EncodingView**](classzmbt_1_1lang_1_1EncodingView.md) | [**traverse\_subtrees**](#function-traverse_subtrees) (std::size\_t const node, std::size\_t & next) noexcept const<br> |




























## Public Types Documentation




### typedef K 

```C++
using zmbt::lang::EncodingView::K =  Keyword;
```




<hr>



### typedef V 

```C++
using zmbt::lang::EncodingView::V =  boost::json::value;
```




<hr>



### typedef const\_iterator 

```C++
using zmbt::lang::EncodingView::const_iterator =  Iterator;
```




<hr>



### typedef const\_reverse\_iterator 

```C++
using zmbt::lang::EncodingView::const_reverse_iterator =  std::reverse_iterator<const_iterator>;
```




<hr>



### typedef iterator 

```C++
using zmbt::lang::EncodingView::iterator =  Iterator;
```




<hr>



### typedef reverse\_iterator 

```C++
using zmbt::lang::EncodingView::reverse_iterator =  std::reverse_iterator< iterator>;
```




<hr>
## Public Functions Documentation




### function EncodingView [1/4]

```C++
zmbt::lang::EncodingView::EncodingView (
    Encoding && root
) = delete
```




<hr>



### function EncodingView [2/4]

```C++
zmbt::lang::EncodingView::EncodingView () = default
```




<hr>



### function EncodingView [3/4]

```C++
zmbt::lang::EncodingView::EncodingView (
    K const * k,
    std::size_t const * d,
    V const * v,
    std::size_t sz,
    std::size_t index_offset
) 
```




<hr>



### function EncodingView [4/4]

```C++
zmbt::lang::EncodingView::EncodingView (
    Encoding const & root
) 
```




<hr>



### function arity 

_Number of children._ 
```C++
std::size_t zmbt::lang::EncodingView::arity () const
```




<hr>



### function at 

```C++
ExprRow zmbt::lang::EncodingView::at (
    std::size_t i
) const
```



Get node at index i Throws std::range\_error if out of bounds 


        

<hr>



### function back 

```C++
ExprRow zmbt::lang::EncodingView::back () const
```




<hr>



### function begin 

```C++
inline const_iterator zmbt::lang::EncodingView::begin () const
```




<hr>



### function cbegin 

```C++
inline const_iterator zmbt::lang::EncodingView::cbegin () const
```




<hr>



### function cend 

```C++
inline const_iterator zmbt::lang::EncodingView::cend () const
```




<hr>



### function child 

```C++
EncodingView zmbt::lang::EncodingView::child (
    int ord
) noexcept const
```



Get subview on child # ord If not found, returns empty view 


        

<hr>



### function child\_idx 

```C++
std::size_t zmbt::lang::EncodingView::child_idx (
    int ord
) noexcept const
```



Get child # ord node index. Negative ord resolves as reverse. If not found, returns size() 


        

<hr>



### function children 

_Children subviews._ 
```C++
std::vector< EncodingView > zmbt::lang::EncodingView::children () const
```




<hr>



### function crbegin 

```C++
inline const_reverse_iterator zmbt::lang::EncodingView::crbegin () const
```




<hr>



### function crend 

```C++
inline const_reverse_iterator zmbt::lang::EncodingView::crend () const
```




<hr>



### function depth\_offset 

```C++
std::size_t zmbt::lang::EncodingView::depth_offset () const
```




<hr>



### function empty 

```C++
bool zmbt::lang::EncodingView::empty () const
```




<hr>



### function end 

```C++
inline const_iterator zmbt::lang::EncodingView::end () const
```




<hr>



### function freeze 

_Hardcopy referenced encoding subview._ 
```C++
Encoding zmbt::lang::EncodingView::freeze () const
```




<hr>



### function front 

```C++
ExprRow zmbt::lang::EncodingView::front () const
```




<hr>



### function head 

_Root node keyword._ 
```C++
Keyword zmbt::lang::EncodingView::head () noexcept const
```




<hr>



### function if\_data 

```C++
inline V const * zmbt::lang::EncodingView::if_data () const
```




<hr>



### function if\_depth 

```C++
inline std::size_t const * zmbt::lang::EncodingView::if_depth () const
```




<hr>



### function if\_keywords 

```C++
inline K const * zmbt::lang::EncodingView::if_keywords () const
```




<hr>



### function index\_offset 

```C++
std::size_t zmbt::lang::EncodingView::index_offset () const
```




<hr>



### function is\_boolean 

```C++
bool zmbt::lang::EncodingView::is_boolean () const
```




<hr>



### function is\_const 

```C++
bool zmbt::lang::EncodingView::is_const () const
```




<hr>



### function operator!= 

```C++
bool zmbt::lang::EncodingView::operator!= (
    EncodingView const & o
) const
```




<hr>



### function operator== 

```C++
bool zmbt::lang::EncodingView::operator== (
    EncodingView const & o
) const
```




<hr>



### function operator[] 

```C++
inline ExprRow zmbt::lang::EncodingView::operator[] (
    std::size_t i
) const
```




<hr>



### function rbegin 

```C++
inline const_reverse_iterator zmbt::lang::EncodingView::rbegin () const
```




<hr>



### function rend 

```C++
inline const_reverse_iterator zmbt::lang::EncodingView::rend () const
```




<hr>



### function size 

```C++
std::size_t zmbt::lang::EncodingView::size () const
```




<hr>



### function slice 

_Create a subview [start, start+count)_ 
```C++
EncodingView zmbt::lang::EncodingView::slice (
    std::size_t start,
    std::size_t count
) noexcept const
```




<hr>



### function subtree 

_Get a subview at specified node._ 
```C++
EncodingView zmbt::lang::EncodingView::subtree (
    std::size_t const node
) noexcept const
```




<hr>



### function to\_json 

```C++
boost::json::value zmbt::lang::EncodingView::to_json () const
```




<hr>



### function traverse\_subtrees 

```C++
EncodingView zmbt::lang::EncodingView::traverse_subtrees (
    std::size_t const node,
    std::size_t & next
) noexcept const
```



Traverse subtrees from starting position. `next` arg points to the next subtree index at the same depth, or to size() if none. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/expr/encoding.hpp`

