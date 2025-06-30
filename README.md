# Tag Invoke

A modern C++20 header-only library implementing the tag dispatch pattern with customization point objects (CPOs). This library provides a generic mechanism for creating customizable function interfaces that can be extended by user types through ADL (Argument-Dependent Lookup).

## Motivation

I got tired of writing the same tag_invoke boilerplate code for every project and I'm still waiting for `std::tag_invoke` to land in the standard library. This library provides a reusable, well-tested implementation that you can drop into any project without reinventing the wheel.

## Features

- **Header-only**: No compilation required, just include and use
- **C++20 compatible**: Utilizes modern C++ features including concepts
- **Type-safe**: Compile-time checked tag dispatch mechanism
- **Extensible**: Easy to create custom dispatchers for your own types
- **Zero overhead**: All dispatch logic happens at compile time

## Quick Start

### Installation

#### CMake with FetchContent (Recommended)

Add this to your CMakeLists.txt:

```cmake
include(FetchContent)

FetchContent_Declare(
  tag_invoke
  GIT_REPOSITORY https://github.com/junduck/tag_invoke.git
  GIT_TAG        main  # or specify a version tag
)

FetchContent_MakeAvailable(tag_invoke)

# Link to your target
target_link_libraries(your_target PRIVATE ju::tag_invoke)
```

#### CMake with find_package

If you have the library installed system-wide:

```cmake
# Find the package
find_package(ju REQUIRED)

# Link to your target
target_link_libraries(your_target PRIVATE ju::tag_invoke)
```

#### Manual Installation

Simply copy `include/ju/tag_invoke.hpp` to your project's include directory.

### Basic Usage

#### Step 1: Define a tag type with a default implementation

```cpp
#include "ju/tag_invoke.hpp"

namespace mylib {
struct foo_tag {
  // Default implementation for float
  friend int tag_invoke(foo_tag, float) noexcept {
    return 0;
  }

  // Enable function call operator
  DISPATCH_FOR(foo_tag)
};

// Create a constexpr instance
constexpr inline foo_tag foo{};
}
```

#### Step 2: Customize behavior for your types

```cpp
namespace ns1 {
struct my_type {
  // Custom implementation via friend function
  friend int tag_invoke(mylib::foo_tag, my_type) noexcept {
    return 42;
  }
};
}
```

#### Step 3: Use the customizable function

```cpp
int main() {
  ns1::my_type obj;
  float f = 3.14f;

  assert(mylib::foo(obj) == 42);  // Uses custom implementation
  assert(mylib::foo(f) == 0);     // Uses default implementation

  return 0;
}
```

## API Reference

### Core Components

#### `ju::tag_invoke`

The central customization point that performs ADL lookup for `tag_invoke` functions.

```cpp
template<typename Tag, typename... Args>
auto tag_invoke(Tag tag, Args&&... args) -> /* deduced return type */;
```

#### `DISPATCH_FOR(TAG)`

A macro that generates the function call operator for tag types, enabling direct invocation.

```cpp
struct my_tag {
  friend void tag_invoke(my_tag, int) { /* implementation */ }

  DISPATCH_FOR(my_tag)  // Enables my_tag{}(42)
};
```

### Type Traits

The library provides several type traits for compile-time introspection:

#### `ju::tag_invocable<Tag, Args...>`

Concept that checks if a tag can be invoked with given arguments.

```cpp
static_assert(ju::tag_invocable<my_tag, int>);
```

#### `ju::nothrow_tag_invocable<Tag, Args...>`

Concept that checks if a tag can be invoked without throwing exceptions.

```cpp
static_assert(ju::nothrow_tag_invocable<my_tag, int>);
```

#### `ju::tag_invoke_result_t<Tag, Args...>`

Gets the return type of tag invocation.

```cpp
using result_type = ju::tag_invoke_result_t<my_tag, int>;
```

## Advanced Usage

### Custom Dispatchers

Create your own customizable function interfaces:

```cpp
namespace mylib {

struct serialize_tag {
  // Default implementation for built-in types
  template<typename T>
  friend std::string tag_invoke(serialize_tag, const T& value)
    requires std::is_arithmetic_v<T>
  {
    return std::to_string(value);
  }

  DISPATCH_FOR(serialize_tag)
};

constexpr inline serialize_tag serialize{};

}

// Custom type with serialization
struct Person {
  std::string name;
  int age;

  friend std::string tag_invoke(mylib::serialize_tag, const Person& p) {
    return p.name + " (" + std::to_string(p.age) + ")";
  }
};

// Usage
Person p{"Alice", 30};
std::cout << mylib::serialize(p) << std::endl;  // "Alice (30)"
std::cout << mylib::serialize(42) << std::endl;  // "42"
```

### Conditional Customization

Use SFINAE or concepts to provide conditional implementations:

```cpp
struct print_tag {
  // Default for types with operator<<
  template<typename T>
  friend auto tag_invoke(print_tag, const T& value)
    -> decltype(std::cout << value, void())
  {
    std::cout << value;
  }

  DISPATCH_FOR(print_tag)
};
```

## Building the Example

```bash
mkdir build && cd build
cmake ..
make
./tag_invoke_example
```

## Requirements

- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.10+ (for building examples and installation)

## Design Philosophy

This library implements the tag dispatch pattern recommended by the C++ standardization committee for creating customization point objects. While we wait for `std::tag_invoke` to be standardized and widely available, this library provides a production-ready implementation that follows the same design principles.

Key benefits:

1. **ADL-based customization**: Users can customize behavior without modifying library code
2. **Compile-time dispatch**: No runtime overhead
3. **Type safety**: Invalid customizations are caught at compile time
4. **Extensibility**: Easy to add support for new types

## Comparison with Alternatives

| Feature | Tag Invoke | Virtual Functions | Function Overloading |
|---------|------------|-------------------|---------------------|
| Runtime overhead | None | Virtual call overhead | None |
| Extensibility | High (via ADL) | Limited (inheritance) | Limited (same namespace) |
| Type safety | Compile-time | Runtime | Compile-time |
| Header-only friendly | Yes | No (vtables) | Yes |

## Contributing

Contributions are welcome! Please ensure:

1. Code follows the existing style
2. All examples compile and run correctly
3. New features include appropriate tests
4. Documentation is updated accordingly

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Inspired by the C++ standardization work on customization point objects
- Based on the tag_invoke design by Lewis Baker and Eric Niebler
- README generated by AI tools
