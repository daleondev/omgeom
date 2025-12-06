import std;
import omg;

int main() {
  struct MyStruct {
    int integer_field{24};
    float float_flied{-235.632f};
    std::string_view string_field{"Message"};
  };
  MyStruct data_struct;

  omg::meta::data_struct::forEach(
      []<std::size_t I>(const auto &elem) {
        std::println("Struct element {} ({}): {}", I,
                     omg::meta::data_struct::field_name<MyStruct, I>(), elem);
      },
      data_struct);

  using MyTuple = omg::meta::data_struct::to_tuple_t<MyStruct>;
  MyTuple tuple{24, -235.632f, "Message"};

  omg::meta::tuple::forEach(
      []<std::size_t I>(const auto &elem) {
        std::println("Tuple element {}: {}", I, elem);
      },
      tuple);

  return 0;
}
