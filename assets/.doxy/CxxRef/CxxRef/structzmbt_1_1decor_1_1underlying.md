

# Struct zmbt::decor::underlying

**template &lt;class E&gt;**



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**decor**](namespacezmbt_1_1decor.md) **>** [**underlying**](structzmbt_1_1decor_1_1underlying.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::underlying\_type\_t&lt; E &gt; | [**decorated\_type**](#typedef-decorated_type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**operator decorated\_type**](#function-operator-decorated_type) () const<br> |
|  auto | [**operator!**](#function-operator) () const<br> |
|  auto | [**operator!=**](#function-operator_1) (T const & other) const<br> |
|  auto | [**operator%**](#function-operator_2) (T const & other) const<br> |
|  auto | [**operator&**](#function-operator_3) (T const & other) const<br> |
|  auto | [**operator&&**](#function-operator_4) (T const & other) const<br> |
|  auto | [**operator\***](#function-operator_5) (T const & other) const<br> |
|  auto | [**operator+**](#function-operator_6) (T const & other) const<br> |
|  auto | [**operator-**](#function-operator-) () const<br> |
|  auto | [**operator-**](#function-operator-_1) (T const & other) const<br> |
|  auto | [**operator/**](#function-operator_7) (T const & other) const<br> |
|  auto | [**operator&lt;**](#function-operator_8) (T const & other) const<br> |
|  auto | [**operator&lt;&lt;**](#function-operator_9) (T const & other) const<br> |
|  auto | [**operator&lt;=**](#function-operator_10) (T const & other) const<br> |
|  [**underlying**](structzmbt_1_1decor_1_1underlying.md) & | [**operator=**](#function-operator_11) ([**underlying**](structzmbt_1_1decor_1_1underlying.md) const &) = default<br> |
|  [**underlying**](structzmbt_1_1decor_1_1underlying.md) & | [**operator=**](#function-operator_12) ([**underlying**](structzmbt_1_1decor_1_1underlying.md) &&) = default<br> |
|  [**underlying**](structzmbt_1_1decor_1_1underlying.md) & | [**operator=**](#function-operator_13) (decorated\_type const v) <br> |
|  [**underlying**](structzmbt_1_1decor_1_1underlying.md) & | [**operator=**](#function-operator_14) (E const v) <br> |
|  [**underlying**](structzmbt_1_1decor_1_1underlying.md) & | [**operator=**](#function-operator_15) (boost::json::string\_view v) <br> |
|  auto | [**operator==**](#function-operator_16) (T const & other) const<br> |
|  auto | [**operator&gt;**](#function-operator_17) (T const & other) const<br> |
|  auto | [**operator&gt;=**](#function-operator_18) (T const & other) const<br> |
|  auto | [**operator&gt;&gt;**](#function-operator_19) (T const & other) const<br> |
|  auto | [**operator^**](#function-operator_20) (T const & other) const<br> |
|  auto | [**operator\|**](#function-operator_21) (T const & other) const<br> |
|  auto | [**operator\|\|**](#function-operator_22) (T const & other) const<br> |
|  auto | [**operator~**](#function-operator_23) () const<br> |
|  std::string | [**stringify**](#function-stringify) () const<br> |
|   | [**underlying**](#function-underlying-16) () = default<br> |
|   | [**underlying**](#function-underlying-26) ([**underlying**](structzmbt_1_1decor_1_1underlying.md) const &) = default<br> |
|   | [**underlying**](#function-underlying-36) ([**underlying**](structzmbt_1_1decor_1_1underlying.md) &&) = default<br> |
|   | [**underlying**](#function-underlying-46) (decorated\_type const v) <br> |
|   | [**underlying**](#function-underlying-56) (E const v) <br> |
|   | [**underlying**](#function-underlying-66) (boost::json::string\_view v) <br> |
|  decorated\_type | [**value**](#function-value) () const<br> |
|   | [**~underlying**](#function-underlying) () = default<br> |




























## Public Types Documentation




### typedef decorated\_type 

```C++
using zmbt::decor::underlying< E >::decorated_type =  std::underlying_type_t<E>;
```




<hr>
## Public Functions Documentation




### function operator decorated\_type 

```C++
inline zmbt::decor::underlying::operator decorated_type () const
```




<hr>



### function operator! 

```C++
inline auto zmbt::decor::underlying::operator! () const
```




<hr>



### function operator!= 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator!= (
    T const & other
) const
```




<hr>



### function operator% 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator% (
    T const & other
) const
```




<hr>



### function operator& 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator& (
    T const & other
) const
```




<hr>



### function operator&& 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator&& (
    T const & other
) const
```




<hr>



### function operator\* 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator* (
    T const & other
) const
```




<hr>



### function operator+ 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator+ (
    T const & other
) const
```




<hr>



### function operator- 

```C++
inline auto zmbt::decor::underlying::operator- () const
```




<hr>



### function operator- 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator- (
    T const & other
) const
```




<hr>



### function operator/ 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator/ (
    T const & other
) const
```




<hr>



### function operator&lt; 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator< (
    T const & other
) const
```




<hr>



### function operator&lt;&lt; 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator<< (
    T const & other
) const
```




<hr>



### function operator&lt;= 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator<= (
    T const & other
) const
```




<hr>



### function operator= 

```C++
underlying & zmbt::decor::underlying::operator= (
    underlying const &
) = default
```




<hr>



### function operator= 

```C++
underlying & zmbt::decor::underlying::operator= (
    underlying &&
) = default
```




<hr>



### function operator= 

```C++
inline underlying & zmbt::decor::underlying::operator= (
    decorated_type const v
) 
```




<hr>



### function operator= 

```C++
inline underlying & zmbt::decor::underlying::operator= (
    E const v
) 
```




<hr>



### function operator= 

```C++
inline underlying & zmbt::decor::underlying::operator= (
    boost::json::string_view v
) 
```




<hr>



### function operator== 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator== (
    T const & other
) const
```




<hr>



### function operator&gt; 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator> (
    T const & other
) const
```




<hr>



### function operator&gt;= 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator>= (
    T const & other
) const
```




<hr>



### function operator&gt;&gt; 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator>> (
    T const & other
) const
```




<hr>



### function operator^ 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator^ (
    T const & other
) const
```




<hr>



### function operator\| 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator| (
    T const & other
) const
```




<hr>



### function operator\|\| 

```C++
template<class T>
inline auto zmbt::decor::underlying::operator|| (
    T const & other
) const
```




<hr>



### function operator~ 

```C++
inline auto zmbt::decor::underlying::operator~ () const
```




<hr>



### function stringify 

```C++
inline std::string zmbt::decor::underlying::stringify () const
```




<hr>



### function underlying [1/6]

```C++
zmbt::decor::underlying::underlying () = default
```




<hr>



### function underlying [2/6]

```C++
zmbt::decor::underlying::underlying (
    underlying const &
) = default
```




<hr>



### function underlying [3/6]

```C++
zmbt::decor::underlying::underlying (
    underlying &&
) = default
```




<hr>



### function underlying [4/6]

```C++
inline zmbt::decor::underlying::underlying (
    decorated_type const v
) 
```




<hr>



### function underlying [5/6]

```C++
inline zmbt::decor::underlying::underlying (
    E const v
) 
```




<hr>



### function underlying [6/6]

```C++
inline zmbt::decor::underlying::underlying (
    boost::json::string_view v
) 
```




<hr>



### function value 

```C++
inline decorated_type zmbt::decor::underlying::value () const
```




<hr>



### function ~underlying 

```C++
zmbt::decor::underlying::~underlying () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/decor/underlying.hpp`

