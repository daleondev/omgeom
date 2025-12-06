export module omg.meta:common;

import std;

export namespace omg::meta::common {

template <typename T, typename... Ts>
struct contains : std::disjunction<std::is_same<T, Ts>...> {};

template <typename T, typename... Ts>
inline constexpr bool contains_v = contains<T, Ts...>::value;

template <std::size_t N> struct StringLiteral {
  constexpr StringLiteral(const char (&s)[N]) {
    for (std::size_t i = 0; i < N; ++i)
      m_buffer[i] = s[i];
  }
  constexpr operator std::string_view() const {
    return {m_buffer.data(), m_buffer.size()};
  }
  std::array<char, N> m_buffer;
};

} // namespace omg::meta::common