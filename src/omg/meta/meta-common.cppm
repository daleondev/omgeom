export module omg.meta:common;

import std;

export namespace omg::meta::common {

template <typename T, typename... Ts>
struct contains : std::disjunction<std::is_same<T, Ts>...> { };

template <typename T, typename... Ts>
inline constexpr bool contains_v = contains<T, Ts...>::value;

}