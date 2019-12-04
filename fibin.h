#include <iostream>
#include <type_traits>

namespace {
    constexpr void checkNullptr(const char* arg) {
        if (!arg)
            throw std::invalid_argument("Invalid argument provided.");
    }

    constexpr void checkIsAlnum(int c) {
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
            throw std::invalid_argument("Non-alphanumeric character provided.");
    }

    constexpr int toLower(int c) {
        if (c >= 'A' && c <= 'Z') {
            c |= 32;
        }
        return c;
    }

    constexpr void checkLength(int len, int maxLen, const char* arg) {
        if (!len || (len == maxLen && arg[maxLen] != '\0'))
            throw std::range_error("Invalid variable name size provided.");
    }
}

template<uint64_t n>
struct Fib {};

struct True {};

struct False {};

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

//template<uint64_t n>
//struct Lit<Fib<n>> {};

//template<>
//struct Lit<True> {};
//
//template<>
//struct Lit<False> {};
//
//template<typename T>
//struct isLit : std::false_type {};
//
//template<>
//struct isLit<Lit<True>> : std::true_type {};
//
//template<>
//struct isLit<Lit<False>> : std::true_type {};
//
//template<uint64_t n>
//struct isLit<Lit<Fib<n>>> : std::true_type {};

constexpr uint64_t Var(const char* arg) {
    checkNullptr(arg);

    constexpr int maxLen = 6;
    int len = 0, c = 0;

    uint64_t id = 0;
    while (len < maxLen && (c = arg[len]) != '\0') {
        checkIsAlnum(c);
        id = (id << 8) | toLower(c);
        ++len;
    }

    checkLength(len, maxLen, arg);
    return id;
};

template<typename Condition, typename Then, typename Else>
struct If {};

template<typename Left, typename Right>
struct Eq {};

template<typename ValueType>
struct Fibin {
private:
    template<typename Expr, typename Env>
    struct Eval {};

    template<typename T, typename Env>
    struct Eval<Lit<T>, Env> {
        using result = Lit<T>;
    };

    template<uint64_t n, typename Env>
    struct Eval<Fib<n>, Env> {
        static constexpr ValueType value = Eval<Fib<n - 1>, Env>::value + Eval<Fib<n - 2>, Env>::value;
    };

    template<typename Env>
    struct Eval<Fib<1>, Env> {
        static const ValueType value = 1;
    };

    template<typename Env>
    struct Eval<Fib<0>, Env> {
        static const ValueType value = 0;
    };

    template<uint64_t n, typename Env>
    struct Eval<Lit<Fib<n>>, Env> {
        using result = Eval<Fib<n>, Env>;
    };

    template<typename Then, typename Else, typename Env>
    struct Eval<If<Lit<True>, Then, Else>, Env> {
        using result = typename Eval<Then, Env>::result;
    };

    template<typename Then, typename Else, typename Env>
    struct Eval<If<Lit<False>, Then, Else>, Env> {
        using result = typename Eval<Else, Env>::result;
    };

    template<typename Condition, typename Then, typename Else, typename Env>
    struct Eval<If<Condition, Then, Else>, Env> {
        using result = typename Eval<If<typename Eval<Condition, Env>::result, Then, Else>, Env>::result;
    };

public:
    template<typename Expr, typename V = ValueType>
    static constexpr typename std::enable_if<std::is_integral<V>::value, V>::type
    eval() {
        return Eval<Expr, void>::result::value;
    }

    template<typename Expr, typename V = ValueType>
    static constexpr typename std::enable_if<!std::is_integral<V>::value>::type
    eval() {
        std::cout << "Fibin doesn't support: PKc" << std::endl;
    }
};


