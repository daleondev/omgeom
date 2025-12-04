export module omg.meta:tuple;

import std;
import :common;

export namespace omg::meta::tuple {
//------------------------------------------------------
//                    is tuple
//------------------------------------------------------

template <typename T>
struct is_tuple : std::false_type { };

template <typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type { };

template <typename T>
constexpr bool is_tuple_v = is_tuple<std::remove_cvref_t<T>>::value;

template <typename T>
concept Tuple = is_tuple_v<T>;

//------------------------------------------------------
//                 tuple has type
//------------------------------------------------------

template <typename U, typename T>
struct has_type : std::false_type { };

template <typename U, typename... T>
struct has_type<U, std::tuple<T...>> : std::disjunction<std::is_same<U, T>...> { };

template <typename T, typename U>
constexpr bool has_type_v = has_type<U, T>::value;

template <typename U, typename T>
concept HasType = has_type_v<T, U>;

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

//------------------------------------------------------
//                     concat
//------------------------------------------------------

template <Tuple... Ts>
struct tuple_cat {
    using type = decltype(std::tuple_cat(std::declval<Ts>()...));
};

template <Tuple... Ts>
using tuple_cat_t = typename tuple_cat<Ts...>::type;

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

}