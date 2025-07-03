

# File logging.hpp



[**FileList**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**logging.hpp**](logging_8hpp.md)

[Go to the source code of this file](logging_8hpp_source.md)

[More...](#detailed-description)

* `#include <boost/log/core.hpp>`
* `#include <boost/log/trivial.hpp>`
* `#include <boost/log/expressions.hpp>`
* `#include <boost/log/sources/global_logger_storage.hpp>`
* `#include <boost/log/attributes.hpp>`
* `#include <boost/log/common.hpp>`
* `#include <boost/log/exceptions.hpp>`
* `#include <boost/log/sinks.hpp>`
* `#include <boost/log/sources/logger.hpp>`
* `#include <boost/current_function.hpp>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**zmbt**](namespacezmbt.md) <br>_ZMBT root namespace._  |
| namespace | [**logging**](namespacezmbt_1_1logging.md) <br>_Logging (work in progress)_  |



















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ZMBT\_LOG**](logging_8hpp.md#define-zmbt_log) (sev) `ZMBT\_LOG\_WITH\_SRCLOC(ZMBT\_LOG\_INSTANCE, sev)`<br> |
| define  | [**ZMBT\_LOG\_FORMAT**](logging_8hpp.md#define-zmbt_log_format)  `ZMBT\_LOG\_FORMAT\_BRIEF`<br> |
| define  | [**ZMBT\_LOG\_FORMAT\_BRIEF**](logging_8hpp.md#define-zmbt_log_format_brief)  `"%LineID% &lt;%Severity%&gt;: %Message%"`<br> |
| define  | [**ZMBT\_LOG\_FORMAT\_FULL**](logging_8hpp.md#define-zmbt_log_format_full)  `"%LineID% %File%#Line% (%Func%) &lt;%Severity%&gt;\t%Message%"`<br> |
| define  | [**ZMBT\_LOG\_INSTANCE**](logging_8hpp.md#define-zmbt_log_instance)  `::boost::log::trivial::logger::get()`<br> |
| define  | [**ZMBT\_LOG\_INSTANCE\_INTERNAL**](logging_8hpp.md#define-zmbt_log_instance_internal)  `::boost::log::trivial::logger::get()`<br> |
| define  | [**ZMBT\_LOG\_INTERNAL**](logging_8hpp.md#define-zmbt_log_internal) (sev) `ZMBT\_LOG\_WITH\_SRCLOC(ZMBT\_LOG\_INSTANCE\_INTERNAL, sev)`<br> |
| define  | [**ZMBT\_LOG\_JSON**](logging_8hpp.md#define-zmbt_log_json) (sev) `ZMBT\_LOG\_WITH\_JSON\_TAG(ZMBT\_LOG\_INSTANCE\_INTERNAL, sev)`<br> |
| define  | [**ZMBT\_LOG\_WITH\_JSON\_TAG**](logging_8hpp.md#define-zmbt_log_with_json_tag) (logger, sev) `/* multi line expression */`<br> |
| define  | [**ZMBT\_LOG\_WITH\_SRCLOC**](logging_8hpp.md#define-zmbt_log_with_srcloc) (logger, sev) `/* multi line expression */`<br> |

## Detailed Description




**Copyright:**

(c) Copyright 2024 Zenseact AB 




**
**

SPDX-License-Identifier: Apache-2.0 





    
## Macro Definition Documentation





### define ZMBT\_LOG 

```C++
#define ZMBT_LOG (
    sev
) `ZMBT_LOG_WITH_SRCLOC(ZMBT_LOG_INSTANCE, sev)`
```




<hr>



### define ZMBT\_LOG\_FORMAT 

```C++
#define ZMBT_LOG_FORMAT `ZMBT_LOG_FORMAT_BRIEF`
```




<hr>



### define ZMBT\_LOG\_FORMAT\_BRIEF 

```C++
#define ZMBT_LOG_FORMAT_BRIEF `"%LineID% <%Severity%>: %Message%"`
```




<hr>



### define ZMBT\_LOG\_FORMAT\_FULL 

```C++
#define ZMBT_LOG_FORMAT_FULL `"%LineID% %File%#Line% (%Func%) <%Severity%>\t%Message%"`
```




<hr>



### define ZMBT\_LOG\_INSTANCE 

```C++
#define ZMBT_LOG_INSTANCE `::boost::log::trivial::logger::get()`
```




<hr>



### define ZMBT\_LOG\_INSTANCE\_INTERNAL 

```C++
#define ZMBT_LOG_INSTANCE_INTERNAL `::boost::log::trivial::logger::get()`
```




<hr>



### define ZMBT\_LOG\_INTERNAL 

```C++
#define ZMBT_LOG_INTERNAL (
    sev
) `ZMBT_LOG_WITH_SRCLOC(ZMBT_LOG_INSTANCE_INTERNAL, sev)`
```




<hr>



### define ZMBT\_LOG\_JSON 

```C++
#define ZMBT_LOG_JSON (
    sev
) `ZMBT_LOG_WITH_JSON_TAG(ZMBT_LOG_INSTANCE_INTERNAL, sev)`
```




<hr>



### define ZMBT\_LOG\_WITH\_JSON\_TAG 

```C++
#define ZMBT_LOG_WITH_JSON_TAG (
    logger,
    sev
) `/* multi line expression */`
```




<hr>



### define ZMBT\_LOG\_WITH\_SRCLOC 

```C++
#define ZMBT_LOG_WITH_SRCLOC (
    logger,
    sev
) `/* multi line expression */`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/logging.hpp`

