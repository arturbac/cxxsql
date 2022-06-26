#include <boost/ut.hpp>

constexpr auto sum(auto... args) { return (args + ...); }

int main() {
  using boost::ut::operator""_test;
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;

  "sum"_test = [] {
    sum(0) == 0_i;
    sum(1, 2) == 3_i;
//     sum(1, 2) > 0_i and 41_i == sum(40, 2);
  };
}
