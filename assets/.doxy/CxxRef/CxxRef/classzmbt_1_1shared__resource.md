

# Class zmbt::shared\_resource

**template &lt;class T&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**shared\_resource**](classzmbt_1_1shared__resource.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|  T & | [**operator\***](#function-operator) () <br> |
|  T const & | [**operator\***](#function-operator_1) () const<br> |
|  std::shared\_ptr&lt; T &gt; | [**operator-&gt;**](#function-operator-) () <br> |
|  std::shared\_ptr&lt; T &gt; const | [**operator-&gt;**](#function-operator-_1) () const<br> |
|  [**shared\_resource**](classzmbt_1_1shared__resource.md) & | [**operator=**](#function-operator_2) ([**shared\_resource**](classzmbt_1_1shared__resource.md) const &) = default<br> |
|  [**shared\_resource**](classzmbt_1_1shared__resource.md) & | [**operator=**](#function-operator_3) ([**shared\_resource**](classzmbt_1_1shared__resource.md) &&) = default<br> |
|  std::shared\_ptr&lt; T &gt; | [**ptr**](#function-ptr) () const<br> |
|   | [**shared\_resource**](#function-shared_resource-18) (T && v) <br> |
|   | [**shared\_resource**](#function-shared_resource-28) (std::function&lt; T()&gt; f) <br> |
|   | [**shared\_resource**](#function-shared_resource-38) (std::function&lt; std::shared\_ptr&lt; T &gt;()&gt; f) <br> |
|   | [**shared\_resource**](#function-shared_resource-48) (std::shared\_ptr&lt; T &gt; ptr) <br> |
|   | [**shared\_resource**](#function-shared_resource-58) (detail::emplace\_shared\_tag, A &&... args) <br> |
|   | [**shared\_resource**](#function-shared_resource-68) () = default<br> |
|   | [**shared\_resource**](#function-shared_resource-78) ([**shared\_resource**](classzmbt_1_1shared__resource.md) const &) = default<br> |
|   | [**shared\_resource**](#function-shared_resource-88) ([**shared\_resource**](classzmbt_1_1shared__resource.md) &&) = default<br> |
|   | [**~shared\_resource**](#function-shared_resource) () = default<br> |




























## Public Functions Documentation




### function operator\* 

```C++
inline T & zmbt::shared_resource::operator* () 
```




<hr>



### function operator\* 

```C++
inline T const & zmbt::shared_resource::operator* () const
```




<hr>



### function operator-&gt; 

```C++
inline std::shared_ptr< T > zmbt::shared_resource::operator-> () 
```




<hr>



### function operator-&gt; 

```C++
inline std::shared_ptr< T > const zmbt::shared_resource::operator-> () const
```




<hr>



### function operator= 

```C++
shared_resource & zmbt::shared_resource::operator= (
    shared_resource const &
) = default
```




<hr>



### function operator= 

```C++
shared_resource & zmbt::shared_resource::operator= (
    shared_resource &&
) = default
```




<hr>



### function ptr 

```C++
inline std::shared_ptr< T > zmbt::shared_resource::ptr () const
```




<hr>



### function shared\_resource [1/8]

```C++
inline zmbt::shared_resource::shared_resource (
    T && v
) 
```




<hr>



### function shared\_resource [2/8]

```C++
inline zmbt::shared_resource::shared_resource (
    std::function< T()> f
) 
```




<hr>



### function shared\_resource [3/8]

```C++
inline zmbt::shared_resource::shared_resource (
    std::function< std::shared_ptr< T >()> f
) 
```




<hr>



### function shared\_resource [4/8]

```C++
inline zmbt::shared_resource::shared_resource (
    std::shared_ptr< T > ptr
) 
```




<hr>



### function shared\_resource [5/8]

```C++
template<class... A>
inline zmbt::shared_resource::shared_resource (
    detail::emplace_shared_tag,
    A &&... args
) 
```




<hr>



### function shared\_resource [6/8]

```C++
zmbt::shared_resource::shared_resource () = default
```




<hr>



### function shared\_resource [7/8]

```C++
zmbt::shared_resource::shared_resource (
    shared_resource const &
) = default
```




<hr>



### function shared\_resource [8/8]

```C++
zmbt::shared_resource::shared_resource (
    shared_resource &&
) = default
```




<hr>



### function ~shared\_resource 

```C++
zmbt::shared_resource::~shared_resource () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/core/shared_resource.hpp`

