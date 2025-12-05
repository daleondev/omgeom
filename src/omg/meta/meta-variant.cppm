export module omg.meta:variant;

import std;
import :common;
import :tuple;

export namespace omg::meta::variant {
//------------------------------------------------------
//                    is variant
//------------------------------------------------------

template <typename T>
struct is_variant : std::false_type { };

template <typename... Ts>
struct is_variant<std::variant<Ts...>> : std::true_type { };

template <typename T>
constexpr bool is_variant_v = is_variant<std::remove_cvref_t<T>>::value;

template <typename T>
concept Variant = is_variant_v<T>;

//------------------------------------------------------
//                    to tuple
//------------------------------------------------------

template <typename T>
struct to_tuple;

template <typename... Ts>
struct to_tuple<std::variant<Ts...>> {
    using type = std::tuple<Ts...>;
};

template <Variant T>
using to_tuple_t = typename to_tuple<T>::type;

//------------------------------------------------------
//                     make unique
//------------------------------------------------------

template <typename T>
struct make_unique;

template <typename... Ts>
struct make_unique<std::variant<Ts...>> {
    using type = tuple::to_variant_t<typename tuple::unique_filter<std::tuple<Ts...>, std::tuple<>>::type>;
};

// Helper alias
template <typename T>
using make_unique_t = typename make_unique<T>::type;

//------------------------------------------------------
//                     to references
//------------------------------------------------------

template <typename T>
struct to_references;

template <typename... Ts>
struct to_references<std::variant<Ts...>> {
    using type = std::variant<std::reference_wrapper<Ts>...>;
};

// Helper alias
template <typename T>
using to_references_t = typename to_references<T>::type;

}