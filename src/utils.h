#pragma once

#include <concepts>

namespace dsun {
  template<typename T>
  struct is_comparable {
  private:
    template<typename U>
    static auto test(int) -> decltype(std::declval<U>() < std::declval<U>(), std::declval<U>() > std::declval<U>(), std::true_type{});

    template<typename>
    static auto test(...) -> std::false_type;

  public:
    static constexpr bool value = decltype(test<T>(0))::value;
  };
}
