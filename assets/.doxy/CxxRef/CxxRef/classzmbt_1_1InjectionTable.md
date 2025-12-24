

# Class zmbt::InjectionTable



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**InjectionTable**](classzmbt_1_1InjectionTable.md)




















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Record**](structzmbt_1_1InjectionTable_1_1Record.md) <br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**InjectionTable**](#function-injectiontable) ([**interface\_id**](classzmbt_1_1interface__id.md) const & ifc\_id, [**object\_id**](classzmbt_1_1object__id.md) const & obj\_id) <br> |
|  void | [**add\_record**](#function-add_record) (ChannelKind const & ck, [**Record**](structzmbt_1_1InjectionTable_1_1Record.md) && rec) <br> |
|  [**Record**](structzmbt_1_1InjectionTable_1_1Record.md) & | [**last\_record**](#function-last_record) (ChannelKind const & ck) <br> |
|  boost::json::value | [**yield**](#function-yield) (ChannelKind const & ck, boost::json::value & result\_value) <br>_Evaluate input for given category and return error object if any, null otherwise._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  std::shared\_ptr&lt; [**InjectionTable**](classzmbt_1_1InjectionTable.md) &gt; | [**Make**](#function-make) ([**interface\_id**](classzmbt_1_1interface__id.md) const & ifc\_id, [**object\_id**](classzmbt_1_1object__id.md) const & obj\_id) <br> |


























## Public Functions Documentation




### function InjectionTable 

```C++
zmbt::InjectionTable::InjectionTable (
    interface_id const & ifc_id,
    object_id const & obj_id
) 
```




<hr>



### function add\_record 

```C++
void zmbt::InjectionTable::add_record (
    ChannelKind const & ck,
    Record && rec
) 
```




<hr>



### function last\_record 

```C++
Record & zmbt::InjectionTable::last_record (
    ChannelKind const & ck
) 
```




<hr>



### function yield 

_Evaluate input for given category and return error object if any, null otherwise._ 
```C++
boost::json::value zmbt::InjectionTable::yield (
    ChannelKind const & ck,
    boost::json::value & result_value
) 
```




<hr>
## Public Static Functions Documentation




### function Make 

```C++
static std::shared_ptr< InjectionTable > zmbt::InjectionTable::Make (
    interface_id const & ifc_id,
    object_id const & obj_id
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/injection_table.hpp`

