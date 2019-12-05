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
}

BOOST_AUTO_TEST_CASE(var) {
    BOOST_STATIC_ASSERT(49 == Var("1"));
    BOOST_STATIC_ASSERT(Var("1") == Var("1"));
    BOOST_STATIC_ASSERT(Var("abC") == Var("Abc"));
    BOOST_CHECK_THROW(Var("[]"), std::invalid_argument);
    BOOST_CHECK_THROW(Var("abcdefg"), std::range_error);
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

BOOST_AUTO_TEST_CASE(eq) {
    BOOST_STATIC_ASSERT(Fibin<int>::eval<Eq<Lit<Fib<1>>, Lit<Fib<1>>>>() == 1);
    BOOST_STATIC_ASSERT(Fibin<bool>::eval<Eq<Lit<Fib<1>>, Lit<Fib<0>>>>() == false);
    BOOST_STATIC_ASSERT(Fibin<uint64_t>::eval<Eq<Lit<Fib<3>>, Sum<Lit<Fib<2>>, Lit<Fib<1>>>>>() == 1);
}

BOOST_AUTO_TEST_CASE(let) {
    BOOST_STATIC_ASSERT(1 == Fibin<int16_t>::eval<Let<Var("z"), Lit<Fib<0>>, Inc1<Ref<Var("Z")>>>>());
}

BOOST_AUTO_TEST_CASE(provided) {
//    // Testing: lambda(x) {x + (Fib(1) + Fib(10)) + Fib(2)}(Fib(3))
//    // Fib(0) = 0, Fib(1) = 1, Fib(2) = 1, Fib(3) = 2, Fib(10) = 55
//    BOOST_STATIC_ASSERT(59 == Fibin<uint64_t>::eval<Invoke<Lambda<Var("x"),
//                  Sum<Ref<Var("x")>, Inc10<Lit<Fib<1>>>, Lit<Fib<2>>>>, Lit<Fib<3>>>>());
//
    // Testing: if False then Fib(0) else Fib(1)
    BOOST_STATIC_ASSERT(1 == Fibin<uint8_t>::eval<If<Lit<False>, Lit<Fib<0>>, Lit<Fib<1>>>>());
//
    // Testing: let z = Fib(0) in {z + Fib(1)}
    BOOST_STATIC_ASSERT(1 == Fibin<int16_t>::eval<Let<Var("z"), Lit<Fib<0>>, Inc1<Ref<Var("Z")>>>>());

//
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        // Prints out to std::cout: "Fibin doesn't support: PKc"
        Fibin<const char*>::eval<Lit<Fib<0>>>();
    }

    BOOST_CHECK(output.is_equal("Fibin doesn't support: PKc\n"));
}

/**
template<Var skonwertowany do typu liczbowego, expression val - typename, body>
class Let

wszystkie sruktury maja byc pustymi szablonami do pattern matchingu

struct Eval<If<True,Then,Else>>
 using result = typename Eval<Then>::result;


 eval<>()
Eval<T>::result::value;
 oddzielic struktury od ewaluacji

 let dodaje zmienne do srodowiska
 mapy nie dodajemy!!!
 ma dzialac w trakcie kompilacji!
 stworzyc strukture rekurencyjna, ktora dodaje cos do srodowiska

 musi byc szablon find, ktora znajduje cos w srodowisku


 struct EmptyEnvironment

 <Name, Value, Environment>

 dodanie x:
 Environment<x, Lit<1>, Empty>
 dodanie y:
 Environment<y, Lit<2>, Environment>

 Eval<If(cond), Then, Else> = Eval<If<Eval<Cond>...
**/