

# File log.hpp



[**FileList**](files.md) **>** [**application**](dir_0cc19fbf7340471280b165ed90304d9a.md) **>** [**log.hpp**](log_8hpp.md)

[Go to the source code of this file](log_8hpp_source.md)

[More...](#detailed-description)

* `#include <chrono>`
* `#include <iomanip>`
* `#include <string>`
* `#include <boost/describe.hpp>`
* `#include <boost/json.hpp>`
* `#include "zmbt/core/preprocessor.hpp"`
* `#include "zmbt/core/entity_id.hpp"`













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
| define  | [**ZMBT\_DEBUG**](log_8hpp.md#define-zmbt_debug)  `[**::zmbt::StubLogger**](structzmbt_1_1StubLogger.md)()`<br> |
| define  | [**ZMBT\_LOG**](log_8hpp.md#define-zmbt_log) (lvl) `[**::zmbt::Logger**](classzmbt_1_1Logger.md)().WithSrcLoc(ZMBT\_CUR\_LOC).WithLevel(::zmbt::Logger::lvl)`<br> |
| define  | [**ZMBT\_LOG\_CERR**](log_8hpp.md#define-zmbt_log_cerr) (lvl) `ZMBT\_LOG(lvl).WithOutput(::zmbt::Logger::STDERR)`<br> |
| define  | [**ZMBT\_LOG\_JSON**](log_8hpp.md#define-zmbt_log_json) (lvl) `ZMBT\_LOG(lvl).WithOutput(::zmbt::Logger::JSON)`<br> |

## Detailed Description




**Copyright:**

(c) Copyright 2024-2025 Zenseact AB 




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
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/application/log.hpp`

