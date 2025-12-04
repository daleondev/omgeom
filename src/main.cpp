#include <variant>
import std;
import omg;

using Tuple1 = std::tuple<int, double, const char*>;
using Tuple2 = std::tuple<int, float, bool>;
using Tuple3 = omg::meta::tuple::tuple_cat_t<Tuple1, Tuple2>;
using Tuple4 = omg::meta::tuple::make_unique_t<Tuple3>;

using Variant = omg::meta::tuple::to_variant_t<Tuple3>;
using Variant2 = omg::meta::variant::make_unique_t<Variant>;

void print(const Variant2& v)
{
    std::visit([](const auto& t) {
        std::println("Hello Variant {}", t);
    },
        v);
}

int main()
{
    Tuple3 tuple { 1, 2.2, "Test", 5, 3.3f, true };

    omg::meta::tuple::forEach([](auto I, auto& t) {
        std::println("Hello Tuple {} - {}", I, t);
        print(t);
    },
        tuple);

    return 0;
}

// template <typename T, std::size_t N>
// struct Vector {
//     std::array<T, N> data;
//     constexpr T operator[](std::size_t i) const noexcept { return data[i]; }
//     constexpr T& operator[](std::size_t i) noexcept { return data[i]; }
//     static constexpr std::size_t size() noexcept { return N; }
// };

// template <typename Expr>
// struct ExprWrapper {
//     const Expr& expr;
//     constexpr ExprWrapper(const Expr& e) noexcept
//         : expr(e)
//     {
//     }
//     constexpr auto operator[](std::size_t i) const noexcept { return expr[i]; }
//     static constexpr std::size_t size() noexcept { return Expr::size(); }
// };