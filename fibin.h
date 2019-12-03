#include <iostream>
#include <type_traits>

template<uint64_t n>
struct Fib {};

struct True : public std::true_type {};

struct False : public std::false_type {};

template<typename T>
struct isFib : public std::false_type {};

template<uint64_t n>
struct isFib<Fib<n>> : public std::true_type {};

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

template<typename V, typename T>
struct Eval {};

template<typename V, uint64_t n>
struct Eval<V, Fib<n>> : public std::integral_constant<V, Eval<V, Fib<n - 1>>::value + Eval<V, Fib<n - 2>>::value> {};

template<typename V>
struct Eval<V, Fib<1>> : public std::integral_constant<V, 1> {};

template<typename V>
struct Eval<V, Fib<0>> : public std::integral_constant<V, 0> {};

template<typename V>
struct Eval<V, Lit<True>> {
    using result = True;
};

template<typename V>
struct Eval<V, Lit<False>> {
    using result = False;
};

template<typename V, uint64_t n>
struct Eval<V, Lit<Fib<n>>> {
    using result = Eval<V, Fib<n>>;
};

template<typename V, typename Then, typename Else>
struct Eval<V, If<True, Then, Else>> {
    using result = typename Eval<V, Then>::result;
};

template<typename V, typename Then, typename Else>
struct Eval<V, If<False, Then, Else>> {
    using result = typename Eval<V, Else>::result;
};

template<typename V, typename Condition, typename Then, typename Else>
struct Eval<V, If<Condition, Then, Else>> {
    using result = typename Eval<V, If<typename Eval<V, Condition>::result, Then, Else>>::result;
};

template<typename ValueType>
struct Fibin {
    template<typename Expr, typename V = ValueType>
    static constexpr typename std::enable_if<std::is_integral<V>::value, V>::type
    eval() {
        return Eval<V, Expr>::result::value;
    }

    template<typename Expr, typename V = ValueType>
    static constexpr typename std::enable_if<!(std::is_integral<V>::value)>::type
    eval() {
        std::cout << "Fibin doesn't support: PKc" << std::endl;
    }
};


