export module omg.meta:tuple;

import std;

export namespace omg::meta::tuple {
//------------------------------------------------------
//                    is Tuple
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
//                 Tuple has type
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
consteval std::size_t size() { return std::tuple_size_v<std::remove_cvref_t<T>>; }

template <typename T, typename Callable>
concept is_tuple_iterable_v = []<std::size_t... I>(std::index_sequence<I...>) {
    return (std::invocable<Callable, std::size_t, decltype(std::get<I>(std::declval<T>()))> && ...);
}(std::make_index_sequence<size<T>()> {});

template <Tuple T, std::invocable<std::size_t> Callable>
constexpr void forEach(Callable&& callable)
{
    [&]<std::size_t... I>(std::index_sequence<I...>) {
        (std::invoke(std::forward<Callable>(callable), I), ...);
    }(std::make_index_sequence<size<T>()> {});
}

template <Tuple T, typename Callable>
    requires is_tuple_iterable_v<T, Callable>
constexpr void forEach(Callable&& callable, T&& tuple)
{
    [&]<std::size_t... I>(std::index_sequence<I...>) {
        (std::invoke(std::forward<Callable>(callable), I, std::get<I>(tuple)), ...);
    }(std::make_index_sequence<size<T>()> {});
}
}