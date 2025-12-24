

# File output\_recorder.hpp



[**FileList**](files.md) **>** [**backends**](dir_e0e3bad64fbfd08934d555b945409197.md) **>** [**cxx**](dir_2a0640ff8f8d193383b3226ce9e70e40.md) **>** [**include**](dir_33cabc3ab2bb40d6ea24a24cae2f30b8.md) **>** [**zmbt**](dir_2115e3e51895e4107b806d6d2319263e.md) **>** [**model**](dir_b97e8e9bc83032fe6d4e26779db64c76.md) **>** [**output\_recorder.hpp**](output__recorder_8hpp.md)

[Go to the source code of this file](output__recorder_8hpp_source.md)

[More...](#detailed-description)

* `#include <atomic>`
* `#include <bitset>`
* `#include <chrono>`
* `#include <deque>`
* `#include <functional>`
* `#include <memory>`
* `#include <string>`
* `#include <thread>`
* `#include <typeindex>`
* `#include <typeinfo>`
* `#include <boost/json.hpp>`
* `#include <boost/unordered/concurrent_flat_map.hpp>`
* `#include <boost/core/demangle.hpp>`
* `#include "zmbt/application/log.hpp"`
* `#include "zmbt/core.hpp"`
* `#include "zmbt/reflect.hpp"`
* `#include "channel_kind.hpp"`
* `#include "global_flags.hpp"`
* `#include "global_stats.hpp"`
* `#include "error_or_return.hpp"`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**zmbt**](namespacezmbt.md) <br>_ZMBT root namespace._  |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**OutputRecorder**](classzmbt_1_1OutputRecorder.md) <br> |
| struct | [**Frame**](structzmbt_1_1OutputRecorder_1_1Frame.md) &lt;class T&gt;<br> |
| struct | [**FrameBuffs**](structzmbt_1_1OutputRecorder_1_1Registry_1_1FrameBuffs.md) <br> |
| struct | [**output\_recorder\_error**](structzmbt_1_1output__recorder__error.md) <br> |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**BOOST\_UNORDERED\_DISABLE\_REENTRANCY\_CHECK**](output__recorder_8hpp.md#define-boost_unordered_disable_reentrancy_check)  <br> |

## Detailed Description




**Copyright:**

(c) Copyright 2024-2025 Zenseact AB 




**
**

SPDX-License-Identifier: Apache-2.0 





    
## Macro Definition Documentation





### define BOOST\_UNORDERED\_DISABLE\_REENTRANCY\_CHECK 

```C++
#define BOOST_UNORDERED_DISABLE_REENTRANCY_CHECK 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/model/output_recorder.hpp`

