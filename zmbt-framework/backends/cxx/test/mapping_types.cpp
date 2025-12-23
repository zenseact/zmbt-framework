/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <deque>

#include <boost/test/unit_test.hpp>

#include "zmbt/mapping.hpp"
#include "zmbt/model.hpp"


using namespace zmbt;
using namespace zmbt::mapping;
using namespace zmbt::expr;



#define TEST_PAIR_Int     {13, 42 }
#define TEST_PAIR_Double  {.5, .2 }
#define TEST_PAIR_Enum    {TestEnum::A, TestEnum::B}
#define TEST_PAIR_Struct  {TestStruct{13.,42.}, TestStruct{1.,2.} }
#define TEST_PAIR_String  {"lol", "kek" }

#define TEST_PAIR(Type) BOOST_PP_CAT(TEST_PAIR_, Type)
#define TEST_KV_PAIR_D(T) {{1, test_value_for<T>()}, {2, test_value_for<T>()}}
#define TEST_KV_PAIR_S(T) {{"a", test_value_for<T>()}, {"B", test_value_for<T>()}}

#define DEF_TEST_VAL(T, ...) template<> inline T test_value_for<T>() { return __VA_ARGS__; }


namespace {

BOOST_DEFINE_ENUM_CLASS(TestEnum, A, B)

struct TestStruct { double x, y; };
BOOST_DESCRIBE_STRUCT(TestStruct, (), (x, y))


inline bool operator<(const TestStruct& lhs, const TestStruct& rhs)
{
    return (lhs.x < rhs.x) or (lhs.y < rhs.y);
}

}

template<> struct std::hash<TestStruct>
{
    std::size_t operator()(TestStruct const& p) const noexcept
    {
        auto h = (p.x / 5) * (p.y / 7);
        return *reinterpret_cast<std::size_t*>(&h);
    }
};


namespace {

template<class T>
T test_value_for();


using TestInt = int;
using TestDouble = double;
using TestString = std::string;




DEF_TEST_VAL(TestInt    , 42          )
DEF_TEST_VAL(TestDouble , 1.0F / 23   )
DEF_TEST_VAL(TestEnum   , TestEnum::A )
DEF_TEST_VAL(TestStruct , {13., 42.}    )
DEF_TEST_VAL(TestString , "lol"       )

using VectorInt    = std::vector<TestInt>   ;  DEF_TEST_VAL(VectorInt   , TEST_PAIR(Int)   )
using VectorDouble = std::vector<TestDouble>;  DEF_TEST_VAL(VectorDouble, TEST_PAIR(Double))
using VectorEnum   = std::vector<TestEnum>  ;  DEF_TEST_VAL(VectorEnum  , TEST_PAIR(Enum)  )
using VectorStruct = std::vector<TestStruct>;  DEF_TEST_VAL(VectorStruct, TEST_PAIR(Struct))
using VectorString = std::vector<TestString>;  DEF_TEST_VAL(VectorString, TEST_PAIR(String))

using ListInt    = std::list<TestInt>   ;  DEF_TEST_VAL(ListInt   ,  TEST_PAIR(Int)   )
using ListDouble = std::list<TestDouble>;  DEF_TEST_VAL(ListDouble,  TEST_PAIR(Double))
using ListEnum   = std::list<TestEnum>  ;  DEF_TEST_VAL(ListEnum  ,  TEST_PAIR(Enum)  )
using ListStruct = std::list<TestStruct>;  DEF_TEST_VAL(ListStruct,  TEST_PAIR(Struct))
using ListString = std::list<TestString>;  DEF_TEST_VAL(ListString,  TEST_PAIR(String))

using DequeInt    = std::deque<TestInt>   ;  DEF_TEST_VAL(DequeInt   ,  TEST_PAIR(Int)   )
using DequeDouble = std::deque<TestDouble>;  DEF_TEST_VAL(DequeDouble,  TEST_PAIR(Double))
using DequeEnum   = std::deque<TestEnum>  ;  DEF_TEST_VAL(DequeEnum  ,  TEST_PAIR(Enum)  )
using DequeStruct = std::deque<TestStruct>;  DEF_TEST_VAL(DequeStruct,  TEST_PAIR(Struct))
using DequeString = std::deque<TestString>;  DEF_TEST_VAL(DequeString,  TEST_PAIR(String))

using ArrayInt    = std::array<TestInt   , 2>;  DEF_TEST_VAL(ArrayInt   ,  TEST_PAIR(Int)   )
using ArrayDouble = std::array<TestDouble, 2>;  DEF_TEST_VAL(ArrayDouble,  TEST_PAIR(Double))
using ArrayEnum   = std::array<TestEnum  , 2>;  DEF_TEST_VAL(ArrayEnum  ,  TEST_PAIR(Enum)  )
using ArrayStruct = std::array<TestStruct, 2>;  DEF_TEST_VAL(ArrayStruct,  TEST_PAIR(Struct))
using ArrayString = std::array<TestString, 2>;  DEF_TEST_VAL(ArrayString,  TEST_PAIR(String))

using SetInt    = std::set<TestInt>   ;  DEF_TEST_VAL(SetInt   ,  TEST_PAIR(Int)   )
using SetDouble = std::set<TestDouble>;  DEF_TEST_VAL(SetDouble,  TEST_PAIR(Double))
using SetEnum   = std::set<TestEnum>  ;  DEF_TEST_VAL(SetEnum  ,  TEST_PAIR(Enum)  )
using SetStruct = std::set<TestStruct>;  DEF_TEST_VAL(SetStruct,  TEST_PAIR(Struct))
using SetString = std::set<TestString>;  DEF_TEST_VAL(SetString,  TEST_PAIR(String))


using MapInt2SetInt    = std::map<TestInt, SetInt>   ;  DEF_TEST_VAL(MapInt2SetInt   , TEST_KV_PAIR_D(SetInt))
using MapInt2SetDouble = std::map<TestInt, SetDouble>;  DEF_TEST_VAL(MapInt2SetDouble, TEST_KV_PAIR_D(SetDouble))
using MapInt2SetEnum   = std::map<TestInt, SetEnum>  ;  DEF_TEST_VAL(MapInt2SetEnum  , TEST_KV_PAIR_D(SetEnum))
using MapInt2SetStruct = std::map<TestInt, SetStruct>;  DEF_TEST_VAL(MapInt2SetStruct, TEST_KV_PAIR_D(SetStruct))
using MapInt2SetString = std::map<TestInt, SetString>;  DEF_TEST_VAL(MapInt2SetString, TEST_KV_PAIR_D(SetString))

using UmapStr2VectorInt    = std::unordered_map<TestString, VectorInt>   ;  DEF_TEST_VAL(UmapStr2VectorInt   , TEST_KV_PAIR_S(VectorInt))
using UmapStr2VectorDouble = std::unordered_map<TestString, VectorDouble>;  DEF_TEST_VAL(UmapStr2VectorDouble, TEST_KV_PAIR_S(VectorDouble))
using UmapStr2VectorEnum   = std::unordered_map<TestString, VectorEnum>  ;  DEF_TEST_VAL(UmapStr2VectorEnum  , TEST_KV_PAIR_S(VectorEnum))
using UmapStr2VectorStruct = std::unordered_map<TestString, VectorStruct>;  DEF_TEST_VAL(UmapStr2VectorStruct, TEST_KV_PAIR_S(VectorStruct))
using UmapStr2VectorString = std::unordered_map<TestString, VectorString>;  DEF_TEST_VAL(UmapStr2VectorString, TEST_KV_PAIR_S(VectorString))



using TestTypes = mp_list<
    TestInt,
    TestDouble,
    TestEnum,
    TestStruct,
    TestString,

    VectorInt,
    VectorDouble,
    VectorEnum,
    VectorStruct,
    VectorString,

    ListInt,
    ListDouble,
    ListEnum,
    ListStruct,
    ListString,

    DequeInt,
    DequeDouble,
    DequeEnum,
    DequeStruct,
    DequeString,

    ArrayInt,
    ArrayDouble,
    ArrayEnum,
    ArrayStruct,
    ArrayString,

    SetInt,
    SetDouble,
    SetEnum,
    SetStruct,
    SetString,

    MapInt2SetInt,
    MapInt2SetDouble,
    MapInt2SetEnum,
    MapInt2SetStruct,
    MapInt2SetString,

    UmapStr2VectorInt,
    UmapStr2VectorDouble,
    UmapStr2VectorEnum,
    UmapStr2VectorStruct,
    UmapStr2VectorString
>;



namespace TestInterface
{
    template <class T>
    T identity(T arg)
    {
        return arg;
    }


    template <class T>
    T init()
    {
        return zmbt::reflect::signal_traits<T>::init();
    }

    template <class T>
    void initRef(T& second)
    {
        second = zmbt::reflect::signal_traits<T>::init();
    }

    template <class T>
    void ignore(T, T)
    {
    }

} // namespace TestInterface
} // namespace


BOOST_AUTO_TEST_CASE_TEMPLATE(Identity, T, TestTypes)
{

    auto init_value = zmbt::reflect::signal_traits<T>::init();
    auto test_value = test_value_for<T>();


    SignalMapping("identity<%s>", type_name<T>())
    .OnTrigger (&TestInterface::identity<T>)
        .At(&TestInterface::identity<T>).Inject()
        .At(&TestInterface::identity<T>).Expect()
    .Test
        ( Noop        , Noop         )
        ( _             , _              )
        ( init_value    , init_value     )
        ( test_value    , test_value     )
    ;
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Init, T, TestTypes)
{

    auto const init_value = zmbt::reflect::signal_traits<T>::init();

    SignalMapping("init<%s>", type_name<T>())
    .OnTrigger (&TestInterface::init<T>)
        .At(&TestInterface::init<T>).Inject()
        .At(&TestInterface::init<T>).Expect()
    .Test
        ( Noop  , Noop         )
        ( Noop  , init_value     )
    .PreRun([]{}).PostRun([]{})
    .Description("")
    ;
}


BOOST_AUTO_TEST_CASE_TEMPLATE(InitRef, T, TestTypes)
{

    auto init_value = zmbt::reflect::signal_traits<T>::init();
    auto test_value = test_value_for<T>();

    SignalMapping("initRef<%s>", type_name<T>())
    .OnTrigger (&TestInterface::initRef<T>)
        .At(&TestInterface::initRef<T>).Args(0).Inject()
        .At(&TestInterface::initRef<T>).Args(0).Expect()
    .Test
        ( Noop      , Noop      )
        ( Noop      , init_value  )
        ( init_value  , init_value  )
        ( test_value  , init_value  )
    ;
}


BOOST_AUTO_TEST_CASE_TEMPLATE(Ignore, T, TestTypes)
{

    auto init_value = zmbt::reflect::signal_traits<T>::init();
    auto test_value = test_value_for<T>();

    SignalMapping("ignore<%s>", type_name<T>())
    .OnTrigger (&TestInterface::ignore<T>)
        .At(&TestInterface::ignore<T>).Args(0)  .Inject()
        .At(&TestInterface::ignore<T>).Args(1)  .Inject()
        .At(&TestInterface::ignore<T>)          .Expect()
    .Test
        ( Noop       , Noop       , Noop )
        ( init_value , init_value , Noop )
        ( test_value , test_value , Noop )
    ;
}
