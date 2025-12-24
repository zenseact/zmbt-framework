/**
 * @file
 * @copyright (c) Copyright 2024-2025 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <unordered_set>

#include <boost/test/unit_test.hpp>

#include "zmbt/core.hpp"


using namespace zmbt;



namespace {

struct O {
    double x;
    int operator()(int y) const& { return y;}
    void set_x(double newx) { x = newx; }
};

int return_int(int) { return 42; }

void increment_args(int& x, int& y) { ++x; ++y; }

}


BOOST_AUTO_TEST_CASE(GetIfcPointerGetIfcHandle)
{
    BOOST_CHECK_EQUAL(get_ifc_pointer(&return_int)    , get_ifc_pointer(return_int)    );
    BOOST_CHECK_EQUAL(get_ifc_pointer(&increment_args), get_ifc_pointer(increment_args));
}


BOOST_AUTO_TEST_CASE(GetIfcHandle)
{
    BOOST_CHECK_EQUAL(get_ifc_handle(&return_int)    , get_ifc_handle(return_int)    );
    BOOST_CHECK_EQUAL(get_ifc_handle(&increment_args), get_ifc_handle(increment_args));
}


BOOST_AUTO_TEST_CASE(PtrVsRefFn)
{
    BOOST_CHECK_EQUAL(interface_id(&return_int)    , interface_id(return_int)    );
    BOOST_CHECK_EQUAL(interface_id(&increment_args), interface_id(increment_args));

    BOOST_CHECK_EQUAL(interface_id(&return_int)     .annotation(), interface_id(return_int)     .annotation());
    BOOST_CHECK_EQUAL(interface_id(&increment_args) .annotation(), interface_id(increment_args) .annotation());
}


BOOST_AUTO_TEST_CASE(PtrVsRefObj)
{
    auto lambda1 = [](){};
    auto lambda2 = [&](){ return 1;};
    auto lambda3 = [&](){ return 2; };
    O functor1 {};
    O functor2 {};
    O functor3 {};

    BOOST_CHECK_EQUAL(interface_id(&lambda1) , interface_id(lambda1));
    BOOST_CHECK_EQUAL(interface_id(&lambda2) , interface_id(lambda2));
    BOOST_CHECK_EQUAL(interface_id(&lambda3) , interface_id(lambda3));
    BOOST_CHECK_NE(   interface_id(&lambda1) , interface_id(lambda2));
    BOOST_CHECK_NE(   interface_id(&lambda1) , interface_id(lambda3));

    BOOST_CHECK_EQUAL(interface_id(&functor1), interface_id(functor1));
    BOOST_CHECK_EQUAL(interface_id(&functor2), interface_id(functor2));
    BOOST_CHECK_EQUAL(interface_id(&functor3), interface_id(functor3));
    BOOST_CHECK_NE(   interface_id(&functor1), interface_id(functor2));
    BOOST_CHECK_NE(   interface_id(&functor1), interface_id(functor3));
}



BOOST_AUTO_TEST_CASE(Functor)
{
    O fn {};
    BOOST_CHECK_EQUAL(interface_id(fn), interface_id(&fn));
}



namespace {

    void f() {}
    void g() {}

    struct F {
        bool operator()(int x) const {
            return x;
        }

        void f() const {}
        void g() const {}
        void h() const& {}
        void h() & {}
        void h() && {}
    };

    struct Abstract {
        virtual void f() const  = 0;
        virtual void g() const  = 0;
        virtual void h() const& = 0;
        virtual void h() &      = 0;
        virtual void h() &&     = 0;
    };

    template<class T> using mem_const     =  void (T::*)() const;
    template<class T> using mem_const_ref =  void (T::*)() const&;
    template<class T> using mem_ref       =  void (T::*)() &;
    template<class T> using mem_tempref   =  void (T::*)() &&;
}


BOOST_AUTO_TEST_SUITE(ObjectIdTest)

BOOST_AUTO_TEST_CASE(EliminatePtrRefConst)
{
    std::shared_ptr<O> obj = std::make_shared<O>();
    O& mref = *obj.get();
    O* mptr = obj.get();
    O const& cref = *obj.get();
    O const* cptr = obj.get();

    BOOST_CHECK_EQUAL(object_id(obj), object_id(mref));
    BOOST_CHECK_EQUAL(object_id(obj), object_id(cref));
    BOOST_CHECK_EQUAL(object_id(obj), object_id(cptr));
    BOOST_CHECK_EQUAL(object_id(obj), object_id(mptr));
    BOOST_TEST_MESSAGE(obj);

}

BOOST_AUTO_TEST_CASE(StringId)
{
    BOOST_CHECK_EQUAL(object_id("string literal").key(), "string literal");
    BOOST_CHECK_EQUAL(object_id("string literal").annotation(), "zmbt::object_id::string_key");

    std::string const std_string {"std::string"};
    boost::json::string_view const string_view {"boost::json::string_view"};
    char const* const c_string {"c_string"};

    BOOST_CHECK_EQUAL(object_id(std_string).key(), std_string);
    BOOST_CHECK_EQUAL(object_id(std_string).annotation(), "zmbt::object_id::string_key");

    BOOST_CHECK_EQUAL(object_id(string_view).key(), string_view);
    BOOST_CHECK_EQUAL(object_id(string_view).annotation(), "zmbt::object_id::string_key");

    BOOST_CHECK_EQUAL(object_id(c_string).key(), c_string);
    BOOST_CHECK_EQUAL(object_id(c_string).annotation(), "zmbt::object_id::string_key");
    BOOST_TEST_MESSAGE(c_string);

}



BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(InterfaceIdTest)

BOOST_AUTO_TEST_CASE(Uniqueness)
{
    auto lambda = [](){ return 42; };
    auto const const_lambda = [](){ return 42; };
    auto functor = F();

    std::vector<interface_id> ids
    {
        interface_id(lambda),
        interface_id(const_lambda),
        interface_id(f),
        interface_id(g),
        interface_id(functor),

        interface_id(&F::f),
        interface_id(&F::g),
        interface_id(mem_const_ref<F>{&F::h}),
        interface_id(mem_ref<F>{&F::h}),
        interface_id(mem_tempref<F>{&F::h}),

        interface_id(&Abstract::f),
        interface_id(&Abstract::g),
        interface_id(mem_const_ref<Abstract>{&Abstract::h}),
        interface_id(mem_ref<Abstract>{&Abstract::h}),
        interface_id(mem_tempref<Abstract>{&Abstract::h})
    };

    std::unordered_set<interface_id, std::hash<interface_id>, std::equal_to<interface_id>> unique_ids{};

    for (auto const& id : ids)
    {
        unique_ids.insert(id);
        interface_id bijection {boost::json::value(id)};
        BOOST_CHECK_EQUAL(bijection, id);
        BOOST_TEST_MESSAGE(id);
    }
    BOOST_CHECK_EQUAL(ids.size(), unique_ids.size());

}


BOOST_AUTO_TEST_SUITE_END()
