

# Struct zmbt::InjectionTable::Record



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**InjectionTable**](classzmbt_1_1InjectionTable.md) **>** [**Record**](structzmbt_1_1InjectionTable_1_1Record.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  Generator::Shared | [**generator**](#variable-generator)  <br> |
|  boost::json::string | [**jptr**](#variable-jptr)  <br> |
|  boost::optional&lt; boost::json::value &gt; | [**maybe\_constant**](#variable-maybe_constant)  <br> |
|  [**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) | [**transform**](#variable-transform)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Record**](#function-record) (boost::json::string\_view jp, Generator::Shared gen, [**lang::Expression**](classzmbt_1_1lang_1_1Expression.md) const & tf) <br> |




























## Public Attributes Documentation




### variable generator 

```C++
Generator::Shared zmbt::InjectionTable::Record::generator;
```




<hr>



### variable jptr 

```C++
boost::json::string zmbt::InjectionTable::Record::jptr;
```




<hr>



### variable maybe\_constant 

```C++
boost::optional<boost::json::value> zmbt::InjectionTable::Record::maybe_constant;
```




<hr>



### variable transform 

```C++
lang::Expression zmbt::InjectionTable::Record::transform;
```




<hr>
## Public Functions Documentation




### function Record 

```C++
zmbt::InjectionTable::Record::Record (
    boost::json::string_view jp,
    Generator::Shared gen,
    lang::Expression const & tf
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/injection_table.hpp`

