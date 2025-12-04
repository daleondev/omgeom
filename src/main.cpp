import std;
import omg;

using Tuple1 = std::tuple<int, double, const char*>;
using Tuple2 = std::tuple<int, float, bool>;
using Tuple3 = omg::meta::tuple::tuple_cat_t<Tuple1, Tuple2>;
using Tuple4 = omg::meta::tuple::make_unique_t<Tuple3>;

using Variant = omg::meta::tuple::to_variant_t<Tuple3>;
using Variant2 = omg::meta::variant::make_unique_t<Variant>;

struct Struct {
    int i = 5;
    float f = 10.4;
    char c = 'x';
};

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
    Struct s;

    omg::meta::tuple::forEach([]<std::size_t I>(auto& t) {
        std::println("Tuple {}: {}", I, t);
    },
        tuple);

    omg::meta::data_struct::forEach([]<std::size_t I>(auto& t) {
        std::println("Struct {}: {}", omg::meta::data_struct::field_name<Struct, I>(), t);
    },
        s);

    // constexpr std::string_view name0 = omg::meta::data_struct::field_name<Struct, 0>();
    // constexpr std::string_view name1 = omg::meta::data_struct::field_name<Struct, 1>();
    // constexpr std::string_view name2 = Struct, 2>();

    // std::cout << "Field 0: " << name0 << "\n"; // Prints "id"
    // std::cout << "Field 1: " << name1 << "\n"; // Prints "score"
    // std::cout << "Field 2: " << name2 << "\n"; // Prints "username"

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