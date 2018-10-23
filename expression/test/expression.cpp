#include <alps/expression.hpp>

#include <boost/throw_exception.hpp>
#include <iostream>
#include <stdexcept>

void test(std::string const& exp, double res) {
  alps::expression::Expression<double> ex(exp);
  std::cout << ex << std::endl;
  double val = alps::evaluate<double>(exp);
  std::cout << "[" << exp << "] is evaluated as " << val << std::endl;
  if (!alps::numeric::is_zero(val - res)) {
    std::cerr << "error!\n";
  }
}

int main() {
  test("3", 3);
  test("3 + 1", 4);
  test("3.5 * 2.1", 3.5 * 2.1);
  test("3.5 / 2.1", 3.5 / 2.1);
  test("2 * Pi", 2 * M_PI);
  test("2 * sin(Pi/4)", 2 * std::sin(M_PI/4));
  test("1/sqrt(3)", 1 / std::sqrt(3));
  return 0;
}
