#include <iostream>

class AbstractLiteral {
public:
    virtual ~AbstractLiteral() = 0;
};

template <int n>
class Fib : private AbstractLiteral, public std::integral_constant<int, Fib<n - 1>::value + Fib<n - 2>::value> {};

template<>
class Fib<1> : private AbstractLiteral, public std::integral_constant<int, 1> {};

template<>
class Fib<0> : private AbstractLiteral, public std::integral_constant<int, 0> {};

class True : private AbstractLiteral, public std::__bool_constant<true> {};

class False : private AbstractLiteral, public std::__bool_constant<false> {};

template<typename T, typename = typename std::enable_if<std::is_base_of<AbstractLiteral, T>::value>::type>
class Lit {};

template<int n>
class Lit<Fib<n>> : public Fib<n> {};

template<>
class Lit<True> : public True {};

template<>
class Lit<False> : public False {};

