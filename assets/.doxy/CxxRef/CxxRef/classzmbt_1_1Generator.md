

# Class zmbt::Generator



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**Generator**](classzmbt_1_1Generator.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::shared\_ptr&lt; [**Generator**](classzmbt_1_1Generator.md) &gt; | [**Shared**](#typedef-shared)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Generator**](#function-generator-14) (boost::json::array const & serialized) <br> |
|   | [**Generator**](#function-generator-24) ([**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & expr) <br> |
|   | [**Generator**](#function-generator-34) ([**Generator**](classzmbt_1_1Generator.md) const &) = default<br> |
|   | [**Generator**](#function-generator-44) ([**Generator**](classzmbt_1_1Generator.md) &&) = default<br> |
|  [**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & | [**expression**](#function-expression) () const<br>_Get underlying expression._  |
|  bool | [**is\_noop**](#function-is_noop) () const<br> |
|  std::uint64\_t | [**operator()**](#function-operator) (boost::json::value & value) <br>_Generate value and return iteration._  |
|  [**Generator**](classzmbt_1_1Generator.md) & | [**operator=**](#function-operator_1) ([**Generator**](classzmbt_1_1Generator.md) const &) = default<br> |
|  [**Generator**](classzmbt_1_1Generator.md) & | [**operator=**](#function-operator_2) ([**Generator**](classzmbt_1_1Generator.md) &&) = default<br> |
|  void | [**reset**](#function-reset) () <br>_Reset atomic counter._  |
|   | [**~Generator**](#function-generator) () = default<br> |




























## Public Types Documentation




### typedef Shared 

```C++
using zmbt::Generator::Shared =  std::shared_ptr<Generator>;
```




<hr>
## Public Functions Documentation




### function Generator [1/4]

```C++
explicit zmbt::Generator::Generator (
    boost::json::array const & serialized
) 
```




<hr>



### function Generator [2/4]

```C++
explicit zmbt::Generator::Generator (
    lang::Expression const & expr
) 
```




<hr>



### function Generator [3/4]

```C++
zmbt::Generator::Generator (
    Generator const &
) = default
```




<hr>



### function Generator [4/4]

```C++
zmbt::Generator::Generator (
    Generator &&
) = default
```




<hr>



### function expression 

_Get underlying expression._ 
```C++
lang::Expression const & zmbt::Generator::expression () const
```




<hr>



### function is\_noop 

```C++
bool zmbt::Generator::is_noop () const
```




<hr>



### function operator() 

_Generate value and return iteration._ 
```C++
std::uint64_t zmbt::Generator::operator() (
    boost::json::value & value
) 
```




<hr>



### function operator= 

```C++
Generator & zmbt::Generator::operator= (
    Generator const &
) = default
```




<hr>



### function operator= 

```C++
Generator & zmbt::Generator::operator= (
    Generator &&
) = default
```




<hr>



### function reset 

_Reset atomic counter._ 
```C++
void zmbt::Generator::reset () 
```




<hr>



### function ~Generator 

```C++
zmbt::Generator::~Generator () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/generator.hpp`

