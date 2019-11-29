#include <iostream>

template <int n>
struct Fib : std::integral_constant<int, Fib<n- 1>::value + Fib<n - 2>::value> {};

template<>
struct Fib<1> : std::integral_constant<int, 1> {};

template<>
struct Fib<0> : std::integral_constant<int, 0> {};

