

# Struct zmbt::Config::PersistentConfig



[**ClassList**](annotated.md) **>** [**PersistentConfig**](structzmbt_1_1Config_1_1PersistentConfig.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  FailureHandler | [**failure\_handler**](#variable-failure_handler)   = `{&[**zmbt::default\_test\_failure**](namespacezmbt.md#function-default_test_failure)}`<br> |
|  std::atomic&lt; std::uint64\_t &gt; | [**rng\_counter**](#variable-rng_counter)   = `{0}`<br> |
|  std::atomic&lt; std::uint64\_t &gt; | [**rng\_epoch**](#variable-rng_epoch)   = `{0}`<br> |
|  std::uint64\_t | [**rng\_seed**](#variable-rng_seed)   = `{0x6a09e667f3bcc909ULL}`<br> |












































## Public Attributes Documentation




### variable failure\_handler 

```C++
FailureHandler zmbt::Config::PersistentConfig::failure_handler;
```




<hr>



### variable rng\_counter 

```C++
std::atomic<std::uint64_t> zmbt::Config::PersistentConfig::rng_counter;
```




<hr>



### variable rng\_epoch 

```C++
std::atomic<std::uint64_t> zmbt::Config::PersistentConfig::rng_epoch;
```




<hr>



### variable rng\_seed 

```C++
std::uint64_t zmbt::Config::PersistentConfig::rng_seed;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `zmbt-framework/zmbt-framework/backends/cxx/include/zmbt/application/config.hpp`

