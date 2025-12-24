/**
 * @file
 * @copyright (c) Copyright 2022-2023 Volvo Car Corporation
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <boost/test/unit_test.hpp>

#include "zmbt/core.hpp"

using namespace zmbt;
using namespace boost::mp11;


#define TYPENAME(V) zmbt::type_name(V)
#define RECORD_TYPE_PARAM BOOST_TEST_MESSAGE(zmbt::type_name<T>());


template<class T1, class T2, class T3, class T4, class T5, class T6>
struct TestHandle
{
    using Signature    = T1;
    using ExpectHandle = T2;
    using Pointer      = T3;
    using Host         = T4;
    using Return       = T5;
    using Args         = T6;
};


struct O {
    int operator()(float) const& { return 0;}
    double x;
};


auto lambda(){ return [](int){}; }

using lambda_t = decltype(lambda());
using lambda_ref_t = add_lvalue_reference_t<lambda_t>;
using lambda_ptr_t = add_pointer_t<lambda_t>;


using InterfaceTests = mp_list<
    //          T                               is_ifc_handle<T>   ifc_pointer_t<T>           ifc_host_t<T>  ifc_return_t<T>   ifc_args_t<T>
    TestHandle< int                           , mp_false         , nullptr_t                 , nullptr_t    , void           , void                >,
    TestHandle< std::function<void()>**       , mp_false         , nullptr_t                 , nullptr_t    , void           , void                >,
    TestHandle< std::function<void()>         , mp_false         , nullptr_t                 , nullptr_t    , void           , void                >,

    TestHandle< void (O::**)(int)             , mp_false         , nullptr_t                 , nullptr_t    , void           , void                >,
    TestHandle< void (O::**&)(int)            , mp_false         , nullptr_t                 , nullptr_t    , void           , void                >,
    TestHandle< float O::**                   , mp_false         , nullptr_t                 , nullptr_t    , void           , void                >,
    TestHandle< float O::**&                  , mp_false         , nullptr_t                 , nullptr_t    , void           , void                >,

    // void() -> void(*)() because of declval
    TestHandle< void()                        , mp_false         , void(*)()                 , nullptr_t    , void           , std::tuple<>        >,

    TestHandle< void(*)(int)                  , mp_true          , void(*)(int)              , nullptr_t    , void           , std::tuple<int>     >,
    TestHandle< void(&)(int&)                 , mp_true          , void(*)(int&)             , nullptr_t    , void           , std::tuple<int&>    >,
    TestHandle< void(O::*)(int)               , mp_true          , void(O::*)(int)           , O&           , void           , std::tuple<int>     >,
    TestHandle< void(O::*const&)(int)         , mp_true          , void(O::*)(int)           , O&           , void           , std::tuple<int>     >,
    TestHandle< void(O::*      )(int)volatile , mp_true          , void(O::*)(int) volatile  , O volatile&  , void           , std::tuple<int>     >,
    TestHandle< void(O::*const )(int)         , mp_true          , void(O::*)(int)           , O&           , void           , std::tuple<int>     >,
    TestHandle< void(O::*const&)(int)const    , mp_true          , void(O::*)(int) const     , O const&     , void           , std::tuple<int>     >,
    TestHandle< void(O::*const&)(int)&        , mp_true          , void(O::*)(int) &         , O&           , void           , std::tuple<int>     >,
    TestHandle< void(O::*     &)(int)&&       , mp_true          , void(O::*)(int) &&        , O&&          , void           , std::tuple<int>     >,

    TestHandle< float O::*                    , mp_true          , float O::*                , O const&     , float&         , std::tuple<>        >,
    TestHandle< float O::*&                   , mp_true          , float O::*                , O const&     , float&         , std::tuple<>        >,
    TestHandle< O*                            , mp_true          , O*                        , nullptr_t    , int            , std::tuple<float>   >,
    TestHandle< O&                            , mp_true          , O*                        , nullptr_t    , int            , std::tuple<float>   >,
    TestHandle< O const*                      , mp_true          , O const*                  , nullptr_t    , int            , std::tuple<float>   >,
    TestHandle< O const&                      , mp_true          , O const*                  , nullptr_t    , int            , std::tuple<float>   >,
    TestHandle< std::function<int()> *        , mp_true          , std::function<int()> *    , nullptr_t    , int            , std::tuple<>        >,
    TestHandle< std::function<int()>       &  , mp_true          , std::function<int()> *    , nullptr_t    , int            , std::tuple<>        >,
    TestHandle< std::function<int()>  const&  , mp_true          , std::function<int()>const*, nullptr_t    , int            , std::tuple<>        >,
    TestHandle< const std::function<int()>*&  , mp_true          , std::function<int()>const*, nullptr_t    , int            , std::tuple<>        >,

    TestHandle< lambda_ref_t                  , mp_true          , lambda_ptr_t              , nullptr_t    , void           , std::tuple<int>     >
>;




BOOST_AUTO_TEST_CASE_TEMPLATE(test_is_ifc_handle, T, InterfaceTests)
{
    using Signature      = typename T::Signature;
    using ExpectHandle = typename T::ExpectHandle;

    using IsHandle = is_ifc_handle<Signature>;
    BOOST_CHECK_MESSAGE(ExpectHandle::value == IsHandle::value, "Signature: "    <<  zmbt::type_name<Signature>());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_ifc_pointer_t, T, InterfaceTests)
{
    using Signature       = typename T::Signature;
    using ExpectedPointer = typename T::Pointer;
    using ObservedPointer = zmbt::ifc_pointer_t<Signature>;

    BOOST_CHECK_MESSAGE((std::is_same<ExpectedPointer, ObservedPointer>::value),
           "Signature       : "    <<  zmbt::type_name<Signature   >() << "\n"
        << "ExpectedPointer : " <<  zmbt::type_name<ExpectedPointer>() << "\n"
        << "ObservedPointer : " <<  zmbt::type_name<ObservedPointer>() << "\n"
    );
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_ifc_host, T, InterfaceTests)
{
    using Signature  = typename T::Signature;
    using Pointer    = typename T::Pointer;
    using ExpectedHost = typename T::Host;
    using SkipTest   = mp_same<Pointer, nullptr_t>;
    using ObservedHost = typename mp_eval_if<SkipTest, mp_identity<nullptr_t>, ifc_host, Pointer>::type;

    BOOST_CHECK_MESSAGE((std::is_same<ExpectedHost, ObservedHost>::value),
           "Signature    : " <<  zmbt::type_name<Signature   >() << "\n"
        << "ExpectedHost : " <<  zmbt::type_name<ExpectedHost>() << "\n"
        << "ObservedHost : " <<  zmbt::type_name<ObservedHost>() << "\n"
    );
}

// TYPED_TEST(ParametrizedInterfaceTraits, ReturnType)
BOOST_AUTO_TEST_CASE_TEMPLATE(test_ifc_return, T, InterfaceTests)
{
    using Signature      = typename T::Signature;
    using Pointer        = typename T::Pointer;
    using ExpectedReturn = typename T::Return;
    using SkipTest       = mp_same<Pointer, nullptr_t>;

    using ObservedReturn = typename mp_eval_if<SkipTest, mp_identity<void>, ifc_return, Pointer>::type;
    BOOST_CHECK_MESSAGE((std::is_same<ExpectedReturn, ObservedReturn>::value),
           "Signature      : " <<  zmbt::type_name<Signature     >() << "\n"
        << "ExpectedReturn : " <<  zmbt::type_name<ExpectedReturn>() << "\n"
        << "ObservedReturn : " <<  zmbt::type_name<ObservedReturn>() << "\n"
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_ifc_args, T, InterfaceTests)
{
    using Signature      = typename T::Signature;
    using Pointer        = typename T::Pointer;
    using ExpectedArgs   = typename T::Args;
    using SkipTest       = mp_same<Pointer, nullptr_t>;

    using ObservedArgs = typename mp_eval_if<SkipTest, mp_identity<void>, ifc_args, Pointer>::type;
    BOOST_CHECK_MESSAGE((std::is_same<ExpectedArgs, ObservedArgs>::value),
           "Signature    : " <<  zmbt::type_name<Signature   >() << "\n"
        << "ExpectedArgs : " <<  zmbt::type_name<ExpectedArgs>() << "\n"
        << "ObservedArgs : " <<  zmbt::type_name<ObservedArgs>() << "\n"
    );
}
