#include "mylib.hpp"

#include <cassert>

namespace ns1 {
struct foo1 {
  friend int tag_invoke(mylib::foo_tag, foo1) noexcept { return 1; }
};
} // namespace ns1

namespace ns2 {
struct foo2 {
  friend int tag_invoke(mylib::foo_tag, foo2) noexcept { return 2; }
};
} // namespace ns2

int main() {
  ns1::foo1 f1;
  ns2::foo2 f2;
  float default_impl;

  assert(mylib::foo(f1) == 1);
  assert(mylib::foo(f2) == 2);
  assert(mylib::foo(default_impl) == 0);

  return 0;
}
