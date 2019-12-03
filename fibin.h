#include <iostream>
#include <type_traits>

template<uint64_t n>
class Fib {};

template<typename T>
class isFib {
public:
    static constexpr bool value = false;
};

template<uint64_t n>
class isFib<Fib<n>> {
public:
    static constexpr bool value = true;
};

class True {};

class False {};

template<typename T>
class isBoolean {
public:
    static constexpr bool value = false;
};

template<>
class isBoolean<True> {
public:
    static constexpr bool value = true;
};

template<>
class isBoolean<False> {
public:
    static constexpr bool value = true;
};

template<typename T, typename = typename std::enable_if<(isBoolean<T>::value || isFib<T>::value)>::type>
class Lit {};

template<uint64_t n>
class Lit<Fib<n>> {};

template<>
class Lit<True> {};

template<>
class Lit<False> {};

template<typename T>
class isLit : std::false_type {};

template<>
class isLit<Lit<True>> : std::true_type {};

template<>
class isLit<Lit<False>> : std::true_type {};

template<uint64_t n>
class isLit<Lit<Fib<n>>> : std::true_type {};

template<typename Condition, typename Then, typename Else>
class If {};

template<typename Left, typename Right>
class Eq {};

using ValueType = uint64_t;

template<typename T>
class Eval {};

template<>
class Eval<True> : public std::true_type {};

template<>
class Eval<False> : public std::false_type {};

template<uint64_t n>
class Eval<Fib<n>> : public std::integral_constant<ValueType, Eval<Fib<n - 1>>::value + Eval<Fib<n - 2>>::value> {};

template<>
class Eval<Fib<1>> : public std::integral_constant<ValueType, 1> {};

template<>
class Eval<Fib<0>> : public std::integral_constant<ValueType, 0> {};

template<>
class Eval<Lit<True>> {
public:
    using result = Eval<True>;
};

template<>
class Eval<Lit<False>> {
public:
    using result = Eval<False>;
};

template<ValueType n>
class Eval<Lit<Fib<n>>> {
public:
    using result = Eval<Fib<n>>;
};

template<typename Then, typename Else>
class Eval<If<True, Then, Else>> {
public:
    using result = typename Eval<Then>::result;
};

template<typename Then, typename Else>
class Eval<If<False, Then, Else>> {
public:
    using result = typename Eval<Else>::result;
};

template<typename Condition, typename Then, typename Else>
class Eval<If<Condition, Then, Else>> {
public:
    using result = typename Eval<If<Eval<Condition>, Then, Else>>::result;
};

template<typename ValueType>
class Fibin {
public:
    template<typename Expr, typename V = ValueType>
    static constexpr typename std::enable_if<std::is_integral<V>::value, V>::type
    eval() {
        return Eval<Expr>::result::value;
    }

    template<typename Expr, typename V = ValueType>
    static constexpr typename std::enable_if<!(std::is_integral<V>::value)>::type
    eval() {
        std::cout << "Fibin doesn't support: PKc" << std::endl;
    }
};


