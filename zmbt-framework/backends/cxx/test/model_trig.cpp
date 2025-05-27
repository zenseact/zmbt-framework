/**
 * @file
 * @copyright (c) Copyright 2024 Zenseact AB
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include <array>
#include <deque>
#include <list>
#include <tuple>
#include <vector>


#include <boost/test/unit_test.hpp>

#include "zmbt/model.hpp"


using namespace zmbt;


using boost::json::array;


#define TYPENAME(V) zmbt::type(V)

namespace {

struct O {
    double x;
    O() = default;
    O(double v) : x{v} {}
    int operator()(int y) const& {
        return y;
    }
    void set_x(double newx) {
        x = newx;
    }
};


int return_int(int) { return 42; }

void increment_args(int& x, int& y) { ++x; ++y; }

}


using zmbt::Trigger;

struct TestMappingTrigger {
    Environment env {};
};

struct TestActuator {
    Environment env {};
};

BOOST_FIXTURE_TEST_CASE(GetIfcPointer, TestMappingTrigger)
{
    BOOST_CHECK_EQUAL(get_ifc_handle(&return_int), get_ifc_handle(return_int));
    BOOST_CHECK_EQUAL(get_ifc_pointer(&return_int), get_ifc_pointer(return_int));
    BOOST_CHECK_EQUAL(interface_id(&return_int), interface_id(return_int));
    // << interface_id(&return_int).type() << " != " << interface_id(return_int).type();
}

BOOST_FIXTURE_TEST_CASE(InterfaceId, TestMappingTrigger)
{
    BOOST_CHECK_EQUAL(interface_id(&return_int), interface_id(return_int));

    auto trig_ref = Trigger(nullptr, return_int ).ifc_id();
    auto trig_ptr = Trigger(nullptr, &return_int).ifc_id();

    auto id_ref = interface_id(return_int);
    auto id_ptr = interface_id(&return_int);


    BOOST_CHECK_EQUAL(trig_ptr, trig_ref); // << trig_ptr.type() << " != " << trig_ref.type();
    BOOST_CHECK_EQUAL(trig_ptr, id_ref  ); // << trig_ptr.type() << " != " << id_ref  .type();
    BOOST_CHECK_EQUAL(trig_ptr, id_ptr  ); // << trig_ptr.type() << " != " << id_ptr  .type();
    BOOST_CHECK_EQUAL(trig_ref, id_ref  ); // << trig_ref.type() << " != " << id_ref  .type();
    BOOST_CHECK_EQUAL(id_ptr  , id_ref  ); // << id_ptr  .type() << " != " << id_ref  .type();

    auto lambda = [](){};

    BOOST_CHECK_EQUAL(Trigger(nullptr, lambda).ifc_id(), interface_id(lambda));
    BOOST_CHECK_EQUAL(Trigger(nullptr, &lambda).ifc_id(), interface_id(&lambda));
    BOOST_CHECK_EQUAL(Trigger(nullptr, &lambda).ifc_id(), interface_id(lambda));
}


BOOST_FIXTURE_TEST_CASE(TriggerReturn, TestMappingTrigger)
{
    auto trigger = Trigger(nullptr, return_int);
    auto ifc_rec = InterfaceRecord(return_int);
    boost::json::object capture = trigger(0).as_object();
    BOOST_CHECK_EQUAL(capture.at("return"), 42);
}

BOOST_FIXTURE_TEST_CASE(TriggerInOutArgs, TestMappingTrigger)
{
    auto ifc_rec = InterfaceRecord(increment_args);
    auto trigger = Trigger(nullptr, increment_args);
    boost::json::object capture = trigger({1,2}).as_object();
    BOOST_CHECK_EQUAL(capture.at("args"), (array{2, 3}));
}



BOOST_FIXTURE_TEST_CASE(LambdaRef, TestMappingTrigger)
{
    int side_effect = 0;
    auto lambda {[&](){ ++side_effect; return 42; }};
    auto ifc_rec = InterfaceRecord(lambda);
    auto trigger = Trigger(nullptr, lambda);
    boost::json::object capture = trigger().as_object();
    BOOST_CHECK_EQUAL(side_effect, 1);
    BOOST_CHECK_EQUAL(capture.at("return"), 42);
}


BOOST_FIXTURE_TEST_CASE(StdFunctionRef, TestMappingTrigger)
{
    std::function<decltype(increment_args)> std_functor = increment_args;
    auto ifc_rec = InterfaceRecord(std_functor);
    auto trigger = Trigger(nullptr, std_functor);
    boost::json::object capture = trigger({1,2}).as_object();
    BOOST_CHECK_EQUAL(capture.at("args"), (array{2, 3}));
}


BOOST_FIXTURE_TEST_CASE(ObjFunctorRef, TestMappingTrigger)
{
    O obj_functor {};
    auto ifc_rec = InterfaceRecord(obj_functor);
    auto trigger = Trigger(nullptr, obj_functor);
    boost::json::object capture = trigger(42).as_object();
    BOOST_CHECK_EQUAL(capture.at("return"), 42);
}

BOOST_FIXTURE_TEST_CASE(AddressEquivalence, TestMappingTrigger)
{
    O obj {11};
    BOOST_CHECK_EQUAL(Trigger(&obj, &O::set_x).obj_id(), InterfaceRecord(&O::set_x, &obj).refobj());
    BOOST_CHECK_EQUAL(Trigger(obj, &O::set_x).obj_id() , InterfaceRecord(&O::set_x, obj ).refobj());
    BOOST_CHECK_EQUAL(Trigger(obj, &O::set_x).obj_id() , Trigger(&obj, &O::set_x).obj_id());

    BOOST_CHECK_EQUAL(Trigger(nullptr, return_int).obj_id(), object_id(nullptr));
}


BOOST_FIXTURE_TEST_CASE(PointerObjectMemberPointer, TestMappingTrigger)
{
    O obj {11};
    auto ifc_rec = InterfaceRecord(&O::set_x, obj);
    auto trigger = Trigger(&obj, &O::set_x);
    trigger(13);
    BOOST_CHECK_EQUAL(obj.x, 13);
}


BOOST_FIXTURE_TEST_CASE(ExecSharedActuator, TestActuator)
{
    auto obj = std::make_shared<O>(11.5);
    auto actuator = Trigger(obj, &O::set_x);
    InterfaceRecord(&O::set_x, actuator.obj_id());
    actuator(13);
    BOOST_CHECK_EQUAL(obj->x, 13);
}


struct Base
{
    int test_field;
    int another_field;

    Base() : test_field{0}, another_field{0} {}
    Base(int x) : test_field{x}, another_field{x} {}

    virtual ~Base() = default;
    Base(Base const&) = default;
    Base(Base &) = default;
    Base& operator=(Base const&) = default;
    Base& operator=(Base &) = default;

    virtual std::string test_method() const
    {
        return { "Base" };
    }
};

struct Final final : public virtual Base
{
    using Base::Base;
    using Base::operator=;
    virtual ~Final() = default;
    virtual std::string test_method() const final
    {
        return { "Final" };
    }
};

BOOST_FIXTURE_TEST_CASE(TriggerObjCtor, TestMappingTrigger, *boost::unit_test::disabled())
{
    auto const trig_test_method = TriggerObj(&Base::test_method);
    auto const trig_test_field = TriggerObj(&Base::test_field);
    using free_fn_ptr = decltype(&return_int);
    using free_fn_ref = decltype(return_int);

    // FIXME: this trait shall be false
    BOOST_CHECK((!is_constructible<TriggerObj, free_fn_ptr>::value));
    BOOST_CHECK((!is_constructible<TriggerObj, free_fn_ref>::value));
}

BOOST_FIXTURE_TEST_CASE(PolymorphicSmartPtr, TestMappingTrigger)
{
    std::shared_ptr<void> final = std::make_shared<Final>();
    std::shared_ptr<Base> base_ptr = std::static_pointer_cast<Base>(final);

    {
        auto result = Trigger(final, &Base::test_method).execute().as_object().at("return");
        BOOST_CHECK_EQUAL(result, "Final");
    }


    {
        auto result = Trigger(base_ptr, &Base::test_method).execute().as_object().at("return");
        BOOST_CHECK_EQUAL(result, "Final");
    }

    {
        auto ptr_getter = [base_ptr]{ return base_ptr; };
        auto result = Trigger(ptr_getter(), &Base::test_method).execute().as_object().at("return");
        BOOST_CHECK_EQUAL(result, "Final");
    }

    {
        auto ptr_getter = [base_ptr]{ return base_ptr; };
        auto result = Trigger(TriggerObj(ptr_getter()), TriggerIfc(&Base::test_method)).execute().as_object().at("return");
        BOOST_CHECK_EQUAL(result, "Final");
    }
}

BOOST_FIXTURE_TEST_CASE(PolymorphicUnsafeRef, TestMappingTrigger)
{
    Final final;
    Base& base_ref = final;

    {
        auto result = Trigger(final, &Base::test_method).execute().as_object().at("return");
        BOOST_CHECK_EQUAL(result, "Final");
    }


    {
        auto result = Trigger(base_ref, &Base::test_method).execute().as_object().at("return");
        BOOST_CHECK_EQUAL(result, "Final");
    }
}


BOOST_FIXTURE_TEST_CASE(ReturnRef, TestMappingTrigger)
{
    int x = 42;
    auto const getx = [&x] () -> int& { return x; };
    BOOST_CHECK_EQUAL(Trigger(nullptr, getx).execute().as_object().at("return"), 42);
}
