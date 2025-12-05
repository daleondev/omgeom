import std;
import omg;

int main() {
  using MyTuple = std::tuple<int, float, const char *>;
  MyTuple tuple{24, -235.632f, "Message"};

  omg::meta::tuple::forEach(
      []<std::size_t I>(const auto &elem) {
        std::println("Tuple element {}: {}", I, elem);
      },
      tuple);

  struct MyStruct {
    int integer_field{24};
    float float_flied{-235.632f};
    const char *string_field{"Message"};
  };
  MyStruct data_struct;

  omg::meta::data_struct::forEach(
      []<std::size_t I>(const auto &elem) {
        std::println("Struct element {} ({}): {}", I,
                     omg::meta::data_struct::field_name<MyStruct, I>(), elem);
      },
      data_struct);

  return 0;
}
