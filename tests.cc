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

BOOST_AUTO_TEST_CASE(moodle_not_compile) {

//    Fibin<int>::eval<
//        Let<
//            If<
//                Lit<True>,
//                Var("x"),
//                Lit<Fib<2>>
//            >,
//            Lit<Fib<3>>,
//            Ref<Var("x")>
//        >>();       // Shouldn't compile

//    Fibin<int>::eval<
//        Let<
//            If<
//                Lit<True>,
//                Var("x"),
//                Var("x")
//            >,
//            Lit<Fib<3>>,
//            Ref<Var("x")>>
//        >();      // Shouldn't compile

//    Fibin<std::uint32_t>::eval<
//        Let<
//            Var("f"),
//            Lambda<
//                Var("x"),
//                If<
//                    Eq<
//                        Ref<Var("x")>,
//                        Lit<Fib<5>>
//                    >,
//                    Lit<Fib<0>>,
//                    Invoke<
//                        Ref<Var("f")>,
//                        Lit<Fib<5>>
//                    >
//                >
//            >,
//            Invoke<
//                Ref<Var("f")>,
//                Lit<Fib<0>>
//                >
//        >>();     // Shouldn't compile

//    Fibin<std::uint32_t>::eval<
//        Let<
//            Var("f"),
//            Lambda<
//                Var("x"),
//                Ref<Var("x")>
//            >,
//            Ref<Var("f")>
//        >
//    >();         // Shouldn't compile

//    Fibin<std::uint32_t>::eval<
//        Let<
//            Var("t"),
//            Lit<True>,
//            Ref<Var("t")>
//        >
//    >();         // Shouldn't compile

    // Eq function with number
    Fibin<int>::eval<
        Eq<
            Lambda<
                Var("x"),
                Ref<Var("x")>
                >,
            Lit<Fib<5>>
        >
    >();

    // Eq function with boolean
    Fibin<int>::eval<
        Eq<
            Lambda<
                Var("x"),
                Ref<Var("x")>
            >,
            Lit<True>
        >
    >();

    // Eq function with function
    Fibin<int>::eval<
        Eq<
            Lambda<
                Var("x"),
                Fib<5>
            >,
            Lambda<
                Var("y"),
                Ref<Var("y")>
            >
        >
    >();

}

BOOST_AUTO_TEST_CASE(moodle_compiles) {

    // Should compile
    Fibin<std::uint32_t>::eval<
        Let<
            Var("t"),
            Lit<True>,
            If<
                Ref<Var("t")>,
                Lit<Fib<0>>,
                Lit<Fib<1>>
            >
        >
    >();


}

BOOST_AUTO_TEST_CASE(moodle_returns_value) {

    BOOST_STATIC_ASSERT(Fibin<uint64_t>::eval<Invoke<Let<Var("x"), Lit<Fib<0>>, Lambda<Var("x"), Ref<Var("x")>>>, Lit<Fib<1>>>>() == 1);

    BOOST_STATIC_ASSERT(Fibin<int>::eval<Let<
        Var("const"),
        Lit<Fib<1>>,
        Let<
            Var("f"),
            Lambda<
                Var("x"),
                Sum<
                    Ref<Var("const")>,
                    Ref<Var("x")>
                >
            >,
            Let<
                Var("const"),
                Lit<Fib<2>>,
                Invoke<
                    Ref<Var("f")>,
                    Lit<Fib<0>>
                >
            >
        >
        >>() == 1);

    BOOST_STATIC_ASSERT(Fibin<int>::eval<
        Let<
            Var("X"),
            Lit<Fib<0>>,
            Invoke <
                Lambda<
                    Var("X"),
                    Ref<Var("X")>
                    >,
                Lit<Fib<1>>
                >
            >
        >() == 1);

    BOOST_STATIC_ASSERT(Fibin<std::uint32_t>::eval<
        Let<
            Var("t"),
            Lit<True>,
            If<
                Ref<Var("t")>,
                Lit<Fib<0>>,
                Lit<Fib<1>>
            >
        >
    >() == 0);

    BOOST_STATIC_ASSERT(Fibin<std::uint32_t>::eval<
        Let<
            Var("tf"),
            Lambda<
                Var("x"),
                Lit<True>
            >,
            If<
                Invoke<Ref<Var("tf")>, Lit<False>>,
                Lit<Fib<0>>,
                Lit<Fib<1>>
            >
        >
    >() == 0);
}

//BOOST_AUTO_TEST_CASE(non_compile) {
//    Lit<char>;
//}

BOOST_AUTO_TEST_CASE(eval_void) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        // Prints out to std::cout: "Fibin doesn't support: PKc"
        Fibin<const char*>::eval<Lit<Fib<0>>>();
    }

    BOOST_CHECK(output.is_equal("Fibin doesn't support: PKc\n"));
}

BOOST_AUTO_TEST_CASE(eval_type) {
    BOOST_STATIC_ASSERT(1 == Fibin<int16_t>::eval<Lit<Fib<1>>>());
    BOOST_STATIC_ASSERT(6765 == Fibin<uint64_t>::eval<Lit<Fib<20>>>());
    BOOST_STATIC_ASSERT(0 == Fibin<uint64_t>::eval<If<Lit<True>, Lit<Fib<0>>, Lit<Fib<1>>>>());
    BOOST_STATIC_ASSERT(1 == Fibin<uint64_t>::eval<If<Lit<False>, Lit<Fib<0>>, Lit<Fib<1>>>>());
    BOOST_STATIC_ASSERT(121 == Fibin<uint8_t>::eval<Lit<Fib<14>>>());
    BOOST_STATIC_ASSERT(98 == Fibin<uint8_t>::eval<Lit<Fib<15>>>());
}

BOOST_AUTO_TEST_CASE(var) {
    BOOST_STATIC_ASSERT(49 == Var("1"));
    BOOST_STATIC_ASSERT(Var("1") == Var("1"));
    BOOST_STATIC_ASSERT(Var("abC") == Var("Abc"));
    BOOST_CHECK_THROW(Var("[]"), std::invalid_argument);
    BOOST_CHECK_THROW(Var("abcdefg"), std::range_error);
}

BOOST_AUTO_TEST_CASE(if_) {
    BOOST_STATIC_ASSERT(Fibin<uint8_t>::eval<If< Eq<Lit<Fib<1>>, Lit<Fib<2>>>, Lit<Fib<10>>, Lit<Fib<11>> >>() == 55);
}

BOOST_AUTO_TEST_CASE(sum) {
    BOOST_STATIC_ASSERT(2 == Fibin<int>::eval<Sum<Lit<Fib<1>>, Lit<Fib<1>>>>());
    BOOST_STATIC_ASSERT(4 == Fibin<int>::eval<Sum<Lit<Fib<3>>, Lit<Fib<3>>>>());
    BOOST_STATIC_ASSERT(5 == Fibin<int>::eval<Sum<Lit<Fib<5>>, Lit<Fib<0>>>>());
    BOOST_STATIC_ASSERT(3 == Fibin<int>::eval<Sum<Lit<Fib<1>>, Lit<Fib<1>>, Lit<Fib<1>>>>());
}

BOOST_AUTO_TEST_CASE(inc1) {
    BOOST_STATIC_ASSERT(1 == Fibin<int>::eval<Inc1<Lit<Fib<0>>>>());
    BOOST_STATIC_ASSERT(4 == Fibin<int>::eval<Inc1<Lit<Fib<4>>>>());
}

BOOST_AUTO_TEST_CASE(inc10) {
    BOOST_STATIC_ASSERT(55 == Fibin<int>::eval<Inc10<Lit<Fib<0>>>>());
    BOOST_STATIC_ASSERT(56 == Fibin<int>::eval<Inc10<Lit<Fib<2>>>>());
}


//TODO should this compile?`
//BOOST_AUTO_TEST_CASE(eq) {
//    BOOST_STATIC_ASSERT(Fibin<int>::eval<Eq<Lit<Fib<1>>, Lit<Fib<1>>>>() == 1);
//    BOOST_STATIC_ASSERT(Fibin<bool>::eval<Eq<Lit<Fib<1>>, Lit<Fib<0>>>>() == false);
//    BOOST_STATIC_ASSERT(Fibin<uint64_t>::eval<Eq<Lit<Fib<3>>, Sum<Lit<Fib<2>>, Lit<Fib<1>>>>>() == 1);
//}

BOOST_AUTO_TEST_CASE(let) {
    BOOST_STATIC_ASSERT(1 == Fibin<int16_t>::eval<Let<Var("z"), Lit<Fib<0>>, Inc1<Ref<Var("Z")>>>>());
}

BOOST_AUTO_TEST_CASE(provided) {
    // Testing: lambda(x) {x + (Fib(1) + Fib(10)) + Fib(2)}(Fib(3))
    // Fib(0) = 0, Fib(1) = 1, Fib(2) = 1, Fib(3) = 2, Fib(10) = 55
    BOOST_STATIC_ASSERT(59 == Fibin<uint64_t>::eval<Invoke<Lambda<Var("x"),
                  Sum<Ref<Var("x")>, Inc10<Lit<Fib<1>>>, Lit<Fib<2>>>>, Lit<Fib<3>>>>());

    // Testing: if False then Fib(0) else Fib(1)
    BOOST_STATIC_ASSERT(1 == Fibin<uint8_t>::eval<If<Lit<False>, Lit<Fib<0>>, Lit<Fib<1>>>>());

    // Testing: let z = Fib(0) in {z + Fib(1)}
    BOOST_STATIC_ASSERT(1 == Fibin<int16_t>::eval<Let<Var("z"), Lit<Fib<0>>, Inc1<Ref<Var("Z")>>>>());

    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        // Prints out to std::cout: "Fibin doesn't support: PKc"
        Fibin<const char*>::eval<Lit<Fib<0>>>();
    }

    BOOST_CHECK(output.is_equal("Fibin doesn't support: PKc\n"));
}
