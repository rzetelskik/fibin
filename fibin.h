#include <iostream>
#include <type_traits>

template<uint64_t n>
struct Fib {};

template<typename T>
struct isFib : public std::false_type {};

template<uint64_t n>
struct isFib<Fib<n>> : public std::true_type {};

struct True {};

struct False {};

template<typename T>
struct isBoolean : public std::false_type {};

template<>
struct isBoolean<True> : public std::true_type {};

template<>
struct isBoolean<False> : public std::true_type {};

template<typename T, typename = typename std::enable_if<(isBoolean<T>::value || isFib<T>::value)>::type>
struct Lit {};

template<uint64_t n>
struct Lit<Fib<n>> {};

template<>
struct Lit<True> {};

template<>
struct Lit<False> {};

template<typename T>
struct isLit : std::false_type {};

template<>
struct isLit<Lit<True>> : std::true_type {};

template<>
struct isLit<Lit<False>> : std::true_type {};

template<uint64_t n>
struct isLit<Lit<Fib<n>>> : std::true_type {};

template<typename Condition, typename Then, typename Else>
struct If {};

template<typename Left, typename Right>
struct Eq {};

using ValueType = uint64_t;

template<typename T>
struct Eval {};

template<>
struct Eval<True> : public std::true_type {};

template<>
struct Eval<False> : public std::false_type {};

template<uint64_t n>
struct Eval<Fib<n>> : public std::integral_constant<ValueType, Eval<Fib<n - 1>>::value + Eval<Fib<n - 2>>::value> {};

template<>
struct Eval<Fib<1>> : public std::integral_constant<ValueType, 1> {};

template<>
struct Eval<Fib<0>> : public std::integral_constant<ValueType, 0> {};

template<>
struct Eval<Lit<True>> {
    using result = Eval<True>;
};

template<>
struct Eval<Lit<False>> {
    using result = Eval<False>;
};

template<ValueType n>
struct Eval<Lit<Fib<n>>> {
    using result = Eval<Fib<n>>;
};

template<typename Then, typename Else>
struct Eval<If<True, Then, Else>> {
    using result = typename Eval<Then>::result;
};

template<typename Then, typename Else>
struct Eval<If<False, Then, Else>> {
    using result = typename Eval<Else>::result;
};

template<typename Condition, typename Then, typename Else>
struct Eval<If<Condition, Then, Else>> {
    using result = typename Eval<If<Eval<Condition>, Then, Else>>::result;
};

template<typename ValueType>
struct Fibin {
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


