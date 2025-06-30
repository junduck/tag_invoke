#pragma once

#include <type_traits>

namespace ju {

inline namespace _tag_invoke_v1 {
namespace _tag_invoke {
struct tag_invoke_t {
#define DO_TAG_INVOKE_INNER tag_invoke(static_cast<Tag &&>(tag), static_cast<Args &&>(args)...)
  template <typename Tag, typename... Args>
  constexpr auto operator()(Tag tag, Args &&...args) const noexcept(noexcept(DO_TAG_INVOKE_INNER)) -> decltype(DO_TAG_INVOKE_INNER) {
    return DO_TAG_INVOKE_INNER;
  }
#undef DO_TAG_INVOKE_INNER
};
namespace _fn {
constexpr inline tag_invoke_t tag_invoke{};
} // namespace _fn
} // namespace _tag_invoke
} // namespace _tag_invoke_v1

using namespace _tag_invoke::_fn;

template <auto &tag>
using tag_t = std::remove_cvref_t<decltype(tag)>;

template <typename Tag, typename... Args>
using is_tag_invocable = std::is_invocable<_tag_invoke::tag_invoke_t, Tag, Args...>;

template <typename Tag, typename... Args>
constexpr inline auto is_tag_invocable_v = is_tag_invocable<Tag, Args...>::value;

template <typename Tag, typename... Args>
concept tag_invocable = is_tag_invocable_v<Tag, Args...>;

template <typename Tag, typename... Args>
using is_nothrow_tag_invocable = std::is_nothrow_invocable<_tag_invoke::tag_invoke_t, Tag, Args...>;

template <typename Tag, typename... Args>
constexpr inline auto is_nothrow_tag_invocable_v = is_nothrow_tag_invocable<Tag, Args...>::value;

template <typename Tag, typename... Args>
concept nothrow_tag_invocable = is_nothrow_tag_invocable_v<Tag, Args...>;

template <typename Tag, typename... Args>
using tag_invoke_result = std::invoke_result<_tag_invoke::tag_invoke_t, Tag, Args...>;

template <typename Tag, typename... Args>
using tag_invoke_result_t = typename tag_invoke_result<Tag, Args...>::type;

#define DISPATCH_FOR(TAG)                                                                                                                            \
  template <typename... Args>                                                                                                                        \
  auto operator()(Args &&...args) const noexcept(ju::nothrow_tag_invocable<TAG, Args...>)->ju::tag_invoke_result_t<TAG, Args...> {                   \
    return ju::tag_invoke(*this, static_cast<Args &&>(args)...);                                                                                     \
  }
} // namespace ju
