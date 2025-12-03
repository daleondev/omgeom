import std;
import omg.meta;

int main()
{
    using Tuple = std::tuple<int, double, const char*>;
    Tuple tuple { 1, 2.2, "Test" };

    omg::meta::tuple::forEach([](auto I, auto& t) {
        std::println("Hello Tuple {} - {}", I, t);
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