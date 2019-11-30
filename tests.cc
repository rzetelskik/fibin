#include "fibin.h"

#define BOOST_TEST_MODULE tests
#include <iostream>
#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

struct cout_redirect {
    cout_redirect(std::streambuf * new_buffer) : old( std::cout.rdbuf(new_buffer)) {}

    ~cout_redirect() {
        std::cout.rdbuf(old);
    }

private:
    std::streambuf * old;
};

BOOST_AUTO_TEST_CASE(fib) {
    BOOST_STATIC_ASSERT(Fib<0>::value == 0);
    BOOST_STATIC_ASSERT(Fib<1>::value == 1);
    BOOST_STATIC_ASSERT(Fib<21>::value == 10946);
}

BOOST_AUTO_TEST_CASE(booleans) {
    BOOST_STATIC_ASSERT(True::value);
    BOOST_STATIC_ASSERT(!False::value);
}

BOOST_AUTO_TEST_CASE(lit) {
    BOOST_STATIC_ASSERT(Lit<True>::value);
    BOOST_STATIC_ASSERT(!Lit<False>::value);
}

//BOOST_AUTO_TEST_CASE(provided) {
//    // Testing: lambda(x) {x + (Fib(1) + Fib(10)) + Fib(2)}(Fib(3))
//    // Fib(0) = 0, Fib(1) = 1, Fib(2) = 1, Fib(3) = 2, Fib(10) = 55
//    BOOST_STATIC_ASSERT(59 == Fibin<uint64_t>::eval<Invoke<Lambda<Var("x"),
//                  Sum<Ref<Var("x")>, Inc10<Lit<Fib<1>>>, Lit<Fib<2>>>>, Lit<Fib<3>>>>());
//
//    // Testing: if False then Fib(0) else Fib(1)
//    BOOST_STATIC_ASSERT(1 == Fibin<uint8_t>::eval<If<Lit<False>, Lit<Fib<0>>, Lit<Fib<1>>>>());
//
//    // Testing: let z = Fib(0) in {z + Fib(1)}
//    BOOST_STATIC_ASSERT(1 == Fibin<int16_t>::eval<Let<Var("z"), Lit<Fib<0>>, Inc1<Ref<Var("Z")>>>>());
//
//    boost::test_tools::output_test_stream output;
//    {
//        cout_redirect guard(output.rdbuf());
//        // Prints out to std::cout: "Fibin doesn't support: PKc"
//        Fibin<const char*>::eval<Lit<Fib<0>>>();
//    }
//
//    BOOST_CHECK(output.is_equal("Fibin doesn't support: PKc"));
//}