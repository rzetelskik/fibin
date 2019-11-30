#include <iostream>

template <int n>
struct Fib : std::integral_constant<int, Fib<n- 1>::value + Fib<n - 2>::value> {};

template<>
struct Fib<1> : std::integral_constant<int, 1> {};

template<>
struct Fib<0> : std::integral_constant<int, 0> {};

struct True : std::bool_constant<true> {};

struct False : std::bool_constant<false> {};

template<typename T, typename = typename std::enable_if<(
        std::is_same<T, True>::value ||
        std::is_same<T, False>::value)>::type>
struct Lit {};

template<>
struct Lit<True> : True {};

template<>
struct Lit<False> : False {};

