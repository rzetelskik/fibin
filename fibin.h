#ifndef FIBIN_H
#define FIBIN_H

#include <iostream>
#include <type_traits>

constexpr bool isDigit(int c) {
    return (unsigned)c - '0' < 10;
}

constexpr bool isAlpha(int c) {
    return ((unsigned)c | 32) - 'a' < 26;
}

constexpr bool isAlnum(int c) {
    return isDigit(c) | isAlpha(c);
}

constexpr bool isUpper(int c) {
    return (unsigned)c - 'A' < 26;
}

constexpr int toLower(int c) {
    if (isUpper(c)) {
        c |= 32;
    }
    return c;
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

constexpr uint64_t Var(const char* arg) {
    if (!arg) throw std::invalid_argument("Invalid argument provided (nullptr).");

    constexpr int maxLen = 6;
    int len = 0, c = 0;

    uint64_t id = 0;
    while (len < maxLen && (c = arg[len]) != '\0') {
        if (!isAlnum(c)) throw std::invalid_argument("Non-alphanumeric character provided.");

        id = static_cast<uint64_t>(static_cast<uint64_t>(id << 8) | toLower(c));
        ++len;
    }

    if (!len || (len == maxLen && arg[maxLen] != '\0')) throw std::range_error("Variable of invalid name size provided.");
    return id;
};

template<typename Condition, typename Then, typename Else>
struct If {};

template<typename First, typename Second, typename ... Args>
struct Sum {};

template<typename T>
struct Inc1 {};

template<typename T>
struct Inc10 {};

template<typename Left, typename Right>
struct Eq {};

template<uint64_t varId>
struct Ref {};

template<uint64_t varId, typename Value, typename Expression>
struct Let {};

template<uint64_t varId, typename Body>
struct Lambda {};

template<typename Fun, typename Param>
struct Invoke {};

template<typename ValueType>
struct Fibin {
private:
    template<ValueType n>
    struct Integral {
        static constexpr ValueType value = n;
    };

    template<uint64_t varId, typename Body, typename Env>
    struct Func {};

    struct EmptyEnvironment {};

    template<uint64_t varId, typename Value, typename Tail>
    struct Environment {};

    template<uint64_t varId, typename Environment>
    struct findVarValue {};

    template<uint64_t thisId, uint64_t varId, typename Value, typename Tail>
    struct findVarValue<thisId, Environment<varId, Value, Tail>> {
        using result = typename findVarValue<thisId, Tail>::result;
    };

    template<uint64_t varId, typename Value, typename Tail>
    struct findVarValue<varId, Environment<varId, Value, Tail>> {
        using result = Value;
    };

    template<typename Expr, typename Env>
    struct Eval {};

    template<typename Env>
    struct Eval<Lit<True>, Env> {
        using result = True;
    };

    template<typename Env>
    struct Eval<Lit<False>, Env> {
        using result = False;
    };

    template<typename Env>
    struct Eval<Lit<Fib<1>>, Env> {
        using result = Integral<1>;
    };

    template<typename Env>
    struct Eval<Lit<Fib<0>>, Env> {
        using result = Integral<0>;
    };

    template<uint64_t n, typename Env>
    struct Eval<Lit<Fib<n>>, Env> {
    private:
        using first = typename Eval<Lit<Fib<n - 1>>, Env>::result;
        using second = typename Eval<Lit<Fib<n - 2>>, Env>::result;
    public:
        using result = Integral<static_cast<ValueType>(first::value + second::value)>;
    };

    template<typename Then, typename Else, typename Env>
    struct Eval<If<True, Then, Else>, Env> {
        using result = typename Eval<Then, Env>::result;
    };

    template<typename Then, typename Else, typename Env>
    struct Eval<If<False, Then, Else>, Env> {
        using result = typename Eval<Else, Env>::result;
    };

    template<typename Condition, typename Then, typename Else, typename Env>
    struct Eval<If<Condition, Then, Else>, Env> {
        using result = typename Eval<If<typename Eval<Condition, Env>::result, Then, Else>, Env>::result;
    };

    template<typename First, typename Second, typename ... Args, typename Env>
    struct Eval<Sum<First, Second, Args ...>, Env> {
    private:
        using firstRes = typename Eval<First, Env>::result;
        using restRes = typename Eval<Sum<Second, Args ...>, Env>::result;
    public:
        using result = Integral<static_cast<ValueType>(firstRes::value + restRes ::value)>;
    };

    template<typename First, typename Second, typename Env>
    struct Eval<Sum<First, Second>, Env> {
    private:
        using firstRes = typename Eval<First, Env>::result;
        using secondRes = typename Eval<Second, Env>::result;
    public:
        using result = Integral<static_cast<ValueType>(firstRes::value + secondRes::value)>;
    };

    template<typename T, typename Env>
    struct Eval<Inc1<T>, Env> {
        using result = typename Eval<Sum<T, Lit<Fib<1>>>, Env>::result;
    };

    template<typename T, typename Env>
    struct Eval<Inc10<T>, Env> {
        using result = typename Eval<Sum<T, Lit<Fib<10>>>, Env>::result;
    };

    template<typename Left, typename Right, typename Env>
    struct Eval<Eq<Left, Right>, Env> {
    private:
        using leftRes = typename Eval<Left, Env>::result;
        using rightRes = typename Eval<Right, Env>::result;
    public:
        using result = typename Eval<Eq<leftRes, rightRes>, Env>::result;
    };

    template<ValueType n, ValueType m, typename Env>
    struct Eval<Eq<Integral<n>, Integral<m>>, Env> {
        using result = typename Eval<Lit<False>, Env>::result;
    };

    template<ValueType n, typename Env>
    struct Eval<Eq<Integral<n>, Integral<n>>, Env> {
        using result = typename Eval<Lit<True>, Env>::result;
    };

    template<uint64_t varId, typename Env>
    struct Eval<Ref<varId>, Env> {
        using result = typename findVarValue<varId, Env>::result;
    };

    template<uint64_t varId, typename Value, typename Expression, typename Env>
    struct Eval<Let<varId, Value, Expression>, Env> {
        using result = typename Eval<Expression, Environment<varId, typename Eval<Value, Env>::result, Env>>::result;
    };

    template<uint64_t varId, typename Body, typename Env>
    struct Eval<Lambda<varId, Body>, Env> {
        using result = Func<varId, Body, Env>;
    };

    template<typename Fun, typename Param, typename Env>
    struct Eval<Invoke<Fun, Param>, Env> {
        using result = typename Eval<Invoke<typename Eval<Fun, Env>::result, Param>, Env>::result;
    };

    template<uint64_t varId, typename Body, typename funcEnv, typename Param, typename Env>
    struct Eval<Invoke<Func<varId, Body, funcEnv>, Param>, Env> {
        using result = typename Eval<Body, Environment<varId, typename Eval<Param, Env>::result, funcEnv>>::result;
    };

public:
    template<typename Expr, typename V = ValueType>
    static constexpr typename std::enable_if<std::is_integral<V>::value, V>::type
    eval() {
        return Eval<Expr, EmptyEnvironment>::result::value;
    }

    template<typename Expr, typename V = ValueType>
    static constexpr typename std::enable_if<!std::is_integral<V>::value>::type
    eval() {
        std::cout << "Fibin doesn't support: " << typeid(V).name() << std::endl;
    }
};

#endif


