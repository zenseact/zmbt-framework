

# Class zmbt::Logger



[**ClassList**](annotated.md) **>** [**zmbt**](namespacezmbt.md) **>** [**Logger**](classzmbt_1_1Logger.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Level**](#enum-level)  <br> |
| enum  | [**Output**](#enum-output)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Logger**](#function-logger) () <br> |
|  [**Logger**](classzmbt_1_1Logger.md) & | [**WithLevel**](#function-withlevel) (Level const level) <br> |
|  [**Logger**](classzmbt_1_1Logger.md) & | [**WithOutput**](#function-withoutput) (Output const output) <br> |
|  [**Logger**](classzmbt_1_1Logger.md) & | [**WithSrcLoc**](#function-withsrcloc) (boost::json::string\_view const src\_loc) <br> |
|  [**Logger**](classzmbt_1_1Logger.md) & | [**operator&lt;&lt;**](#function-operator) (boost::json::value const & value) <br> |
|  [**Logger**](classzmbt_1_1Logger.md) & | [**operator&lt;&lt;**](#function-operator_1) ([**entity\_id**](classzmbt_1_1entity__id.md) const & value) <br> |
|   | [**~Logger**](#function-logger) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  bool | [**is\_notrim\_enabled**](#function-is_notrim_enabled) () <br> |
|  bool | [**is\_pretty\_print\_enabled**](#function-is_pretty_print_enabled) () <br> |
|  Level | [**max\_level**](#function-max_level) () <br> |
|  void | [**open\_json**](#function-open_json) (const std::string & filename="") <br>_Attach logger to file sink ("" means stdout)_  |
|  void | [**set\_max\_level**](#function-set_max_level) (Level const max\_level) <br> |
|  void | [**set\_notrim**](#function-set_notrim) (bool const trim\_line) <br> |
|  void | [**set\_pretty\_print**](#function-set_pretty_print) (bool const trim\_line) <br> |


























## Public Types Documentation




### enum Level 

```C++
enum zmbt::Logger::Level {
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
    TRACE,
    DEVMODE
};
```




<hr>



### enum Output 

```C++
enum zmbt::Logger::Output {
    NIL = 0,
    STDERR = 1 << 0,
    JSON = 1 << 1
};
```




<hr>
## Public Functions Documentation




### function Logger 

```C++
zmbt::Logger::Logger () 
```




<hr>



### function WithLevel 

```C++
Logger & zmbt::Logger::WithLevel (
    Level const level
) 
```




<hr>



### function WithOutput 

```C++
Logger & zmbt::Logger::WithOutput (
    Output const output
) 
```




<hr>



### function WithSrcLoc 

```C++
Logger & zmbt::Logger::WithSrcLoc (
    boost::json::string_view const src_loc
) 
```




<hr>



### function operator&lt;&lt; 

```C++
Logger & zmbt::Logger::operator<< (
    boost::json::value const & value
) 
```




<hr>



### function operator&lt;&lt; 

```C++
Logger & zmbt::Logger::operator<< (
    entity_id const & value
) 
```




<hr>



### function ~Logger 

```C++
zmbt::Logger::~Logger () 
```




<hr>
## Public Static Functions Documentation




### function is\_notrim\_enabled 

```C++
static bool zmbt::Logger::is_notrim_enabled () 
```




<hr>



### function is\_pretty\_print\_enabled 

```C++
static bool zmbt::Logger::is_pretty_print_enabled () 
```




<hr>



### function max\_level 

```C++
static Level zmbt::Logger::max_level () 
```




<hr>



### function open\_json 

_Attach logger to file sink ("" means stdout)_ 
```C++
static void zmbt::Logger::open_json (
    const std::string & filename=""
) 
```




<hr>



### function set\_max\_level 

```C++
static void zmbt::Logger::set_max_level (
    Level const max_level
) 
```




<hr>



### function set\_notrim 

```C++
static void zmbt::Logger::set_notrim (
    bool const trim_line
) 
```




<hr>



### function set\_pretty\_print 

```C++
static void zmbt::Logger::set_pretty_print (
    bool const trim_line
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/application/log.hpp`

