#include <iostream>

class AbstractLiteral {
public:
    virtual ~AbstractLiteral() = 0;
};

template <uint64_t n>
class Fib : private AbstractLiteral, public std::integral_constant<uint64_t, Fib<n - 1>::value + Fib<n - 2>::value> {};

template<>
class Fib<1> : private AbstractLiteral, public std::integral_constant<uint64_t, 1> {};

template<>
class Fib<0> : private AbstractLiteral, public std::integral_constant<uint64_t, 0> {};

class True : private AbstractLiteral, public std::true_type {};

class False : private AbstractLiteral, public std::false_type {};

template<typename T, typename = typename std::enable_if<std::is_base_of<AbstractLiteral, T>::value>::type>
class Lit {};

template<uint64_t n>
class Lit<Fib<n>> : public std::integral_constant<uint64_t, Fib<n>::value> {};

template<>
class Lit<True> : public std::true_type {};

template<>
class Lit<False> : public std::false_type {};

template<typename T, typename U>
class Eq : public std::bool_constant<(
        std::is_same<decltype(T::value), decltype(U::value)>::value &&
        T::value == U::value)> {};
