#include <iostream>
#include <type_traits>

namespace {
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
    if (!arg) throw std::invalid_argument("Invalid argument provided.");

    constexpr int maxLen = 6;
    int len = 0, c = 0;

    uint64_t id = 0;
    while (len < maxLen && (c = arg[len]) != '\0') {
        if (!isAlnum(c)) throw std::invalid_argument("Non-alphanumeric character provided.");

        id = (id << 8) | toLower(c);
        ++len;
    }

    if (!len || (len == maxLen && arg[maxLen] != '\0')) throw std::range_error("Invalid variable name size provided.");
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

    template<ValueType n>
    struct Boolean {
        static constexpr ValueType value = n;
    };

    template<typename Expr, typename Env>
    struct Eval {};

    template<typename Env>
    struct Eval<Lit<True>, Env> {
        using result = Boolean<1>;
    };

    template<typename Env>
    struct Eval<Lit<False>, Env> {
        using result = Boolean<0>;
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
        using result = Integral<Eval<Lit<Fib<n - 1>>, Env>::result::value + Eval<Lit<Fib<n - 2>>, Env>::result::value>;
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

    template<typename First, typename Second, typename ... Args, typename Env>
    struct Eval<Sum<First, Second, Args ...>, Env> {
        using result = Integral<Eval<First, Env>::result::value + Eval<Sum<Second, Args ...>, Env>::result::value>;
    };

    template<typename First, typename Second, typename Env>
    struct Eval<Sum<First, Second>, Env> {
        using result = Integral<Eval<First, Env>::result::value + Eval<Second, Env>::result::value>;
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
        using result = typename Eval<Eq<typename Eval<Left, Env>::result, typename Eval<Right, Env>::result>, Env>::result;
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

//    template<uint64_t varId, typename Body, typename Env>
//    struct Eval<Lambda<>, Env> { };
//
//    template<typename Fun, typename Param, typename Env>
//    struct Eval<Invoke<Fun, Param>, Env> {};

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


