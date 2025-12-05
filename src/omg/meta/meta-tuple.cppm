export module omg.meta:tuple;

import std;
import :common;

export namespace omg::meta::tuple {
//------------------------------------------------------
//                    is tuple
//------------------------------------------------------

template <typename T>
struct is_tuple : std::false_type { };

template <typename... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type { };

template <typename T>
constexpr bool is_tuple_v = is_tuple<std::remove_cvref_t<T>>::value;

template <typename T>
concept Tuple = is_tuple_v<T>;

//------------------------------------------------------
//                 contains type
//------------------------------------------------------

template <typename U, typename T>
struct contains : std::false_type { };

template <typename U, typename... Ts>
struct contains<U, std::tuple<Ts...>> : common::contains<U, Ts...> { };

template <typename T, typename U>
constexpr bool contains_v = contains<U, T>::value;

template <typename U, typename T>
concept Contains = contains_v<T, U>;

//------------------------------------------------------
//                     concat
//------------------------------------------------------

template <Tuple... Ts>
struct concat {
    using type = decltype(std::tuple_cat(std::declval<Ts>()...));
};

template <Tuple... Ts>
using concat_t = typename concat<Ts...>::type;

//------------------------------------------------------
//                     make unique
//------------------------------------------------------

template <typename T, typename U>
struct unique_filter;

template <typename... Ts>
struct unique_filter<std::tuple<>, std::tuple<Ts...>> {
    using type = std::tuple<Ts...>;
};

template <typename Head, typename... Tail, typename... Ts>
struct unique_filter<std::tuple<Head, Tail...>, std::tuple<Ts...>> {
    using next_tuple = std::conditional_t<
        common::contains_v<Head, Ts...>,
        std::tuple<Ts...>,
        std::tuple<Ts..., Head>>;
    using type = typename unique_filter<std::tuple<Tail...>, next_tuple>::type;
};

template <typename T>
struct make_unique;

template <typename... Ts>
struct make_unique<std::tuple<Ts...>> {
    using type = typename unique_filter<std::tuple<Ts...>, std::tuple<>>::type;
};

template <typename T>
using make_unique_t = typename make_unique<T>::type;

//------------------------------------------------------
//                     to variant
//------------------------------------------------------

template <typename T>
struct to_variant;

template <typename... Ts>
struct to_variant<std::tuple<Ts...>> {
    using type = std::variant<Ts...>;
};

template <Tuple T>
using to_variant_t = typename to_variant<T>::type;

//------------------------------------------------------
//                     for each
//------------------------------------------------------

template <Tuple T>
consteval std::size_t count() { return std::tuple_size_v<std::remove_cvref_t<T>>; }

template <typename F, typename... Args>
concept is_invocable_with_size = requires(F&& f, Args&&... args) {
    { std::forward<F>(f).template operator()<std::size_t {}>(std::forward<Args>(args)...) };
};

template <typename Callable, typename T>
concept is_tuple_iterable = []<std::size_t... I>(std::index_sequence<I...>) {
    return (is_invocable_with_size<Callable, decltype(std::get<I>(std::declval<T>()))> && ...);
}(std::make_index_sequence<std::tuple_size_v<std::remove_cvref_t<T>>> {});

template <Tuple T, is_invocable_with_size<> Callable>
constexpr void forEach(Callable&& callable)
{
    [&]<std::size_t... I>(std::index_sequence<I...>) {
        (callable.template operator()<I>(), ...);
    }(std::make_index_sequence<count<T>()> {});
}

template <Tuple T, is_tuple_iterable<T> Callable>
constexpr void forEach(Callable&& callable, T&& tuple)
{
    [&]<std::size_t... I>(std::index_sequence<I...>) {
        (callable.template operator()<I>(std::get<I>(tuple)), ...);
    }(std::make_index_sequence<count<T>()> {});
}

}

namespace tests {

struct Sample {
    int i;
    float f;
    char c;
};

using Tuple1 = std::tuple<int, double, const char*>;
using Tuple2 = std::tuple<Sample, double, bool>;

static_assert(!omg::meta::tuple::is_tuple_v<Sample>);
static_assert(omg::meta::tuple::is_tuple_v<Tuple1>);
static_assert(omg::meta::tuple::is_tuple_v<Tuple2>);

static_assert(omg::meta::tuple::contains_v<Tuple1, int>);
static_assert(!omg::meta::tuple::contains_v<Tuple2, int>);

using Tuple3 = omg::meta::tuple::concat_t<Tuple1, Tuple2>;
using Tuple4 = omg::meta::tuple::make_unique_t<Tuple3>;

static_assert(std::is_same_v<Tuple3, std::tuple<int, double, const char*, Sample, double, bool>>);
static_assert(std::is_same_v<Tuple4, std::tuple<int, double, const char*, Sample, bool>>);

using Variant = omg::meta::tuple::to_variant_t<Tuple4>;

static_assert(std::is_same_v<Variant, std::variant<int, double, const char*, Sample, bool>>);

}