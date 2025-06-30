#pragma once

#include "ju/tag_invoke.hpp"

namespace mylib {
struct foo_tag {
  friend int tag_invoke(foo_tag, float) noexcept { return 0; }

  DISPATCH_FOR(foo_tag)
};

constexpr inline foo_tag foo{};
} // namespace mylib
