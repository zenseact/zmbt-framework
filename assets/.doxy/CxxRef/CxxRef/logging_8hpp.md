

# File logging.hpp



[**FileList**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**logging.hpp**](logging_8hpp.md)

[Go to the source code of this file](logging_8hpp_source.md)

[More...](#detailed-description)

* `#include <boost/describe.hpp>`
* `#include <boost/json.hpp>`
* `#include <zmbt/core/preprocessor.hpp>`
* `#include <chrono>`
* `#include <fstream>`
* `#include <iomanip>`
* `#include <iostream>`
* `#include <memory>`
* `#include <mutex>`
* `#include <sstream>`
* `#include <string>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**zmbt**](namespacezmbt.md) <br>_ZMBT root namespace._  |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**Logger**](classzmbt_1_1Logger.md) <br> |
| struct | [**StubLogger**](structzmbt_1_1StubLogger.md) <br> |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ZMBT\_DEBUG**](logging_8hpp.md#define-zmbt_debug)  `[**::zmbt::StubLogger**](structzmbt_1_1StubLogger.md)()`<br> |
| define  | [**ZMBT\_LOG**](logging_8hpp.md#define-zmbt_log) (lvl) `[**::zmbt::Logger**](classzmbt_1_1Logger.md)().WithSrcLoc(ZMBT\_CUR\_LOC).WithLevel(::zmbt::Logger::lvl)`<br> |
| define  | [**ZMBT\_LOG\_CERR**](logging_8hpp.md#define-zmbt_log_cerr) (lvl) `ZMBT\_LOG(lvl).WithOutput(::zmbt::Logger::STDERR)`<br> |
| define  | [**ZMBT\_LOG\_JSON**](logging_8hpp.md#define-zmbt_log_json) (lvl) `ZMBT\_LOG(lvl).WithOutput(::zmbt::Logger::JSON)`<br> |

## Detailed Description




**Copyright:**

(c) Copyright 2024 Zenseact AB 




**
**

SPDX-License-Identifier: Apache-2.0 





    
## Macro Definition Documentation





### define ZMBT\_DEBUG 

```C++
#define ZMBT_DEBUG `::zmbt::StubLogger ()`
```




<hr>



### define ZMBT\_LOG 

```C++
#define ZMBT_LOG (
    lvl
) `::zmbt::Logger ().WithSrcLoc(ZMBT_CUR_LOC).WithLevel(::zmbt::Logger::lvl)`
```




<hr>



### define ZMBT\_LOG\_CERR 

```C++
#define ZMBT_LOG_CERR (
    lvl
) `ZMBT_LOG(lvl).WithOutput(::zmbt::Logger::STDERR)`
```




<hr>



### define ZMBT\_LOG\_JSON 

```C++
#define ZMBT_LOG_JSON (
    lvl
) `ZMBT_LOG(lvl).WithOutput(::zmbt::Logger::JSON)`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/logging.hpp`

