#ifndef LIBASD_CONTAINER_TRAITS_H
#define LIBASD_CONTAINER_TRAITS_H
#include <type_traits>
#include <vector>
#include <array>

namespace asd
{
namespace detail
{
template<typename T>
class has_mem_func_data
{
    template<typename U>
    static auto check(const U& u) -> decltype(
        u.data() == declval<typename U::value_type const*>(), std::true_type{});
    static auto check(...) -> std::false_type;
  public:
    using type = decltype(has_mem_func_data::check(std::declval<T>()));
    static constexpr bool value = type::value;
};

template<typename T>
class has_mem_func_size
{
    template<typename U>
    static auto check(const U& u) -> decltype(
        u.size() == declval<std::size_t>(), std::true_type{});
    static auto check(...) -> std::false_type;
  public:
    using type = decltype(has_mem_func_size::check(std::declval<T>()));
    static constexpr bool value = type::value;
};

template<typename T>
class has_mem_func_resize
{
    template<typename U>
    static auto check(U& u) -> decltype(u.resize(), std::true_type{});
    static auto check(...)  -> std::false_type;
  public:
    using type = decltype(has_mem_func_resize::check(std::declval<T>()));
    static constexpr bool value = type::value;
};

template<typename T>
class has_mem_func_clear
{
    template<typename U>
    static auto check(U& u) -> decltype(u.clear(), std::true_type{});
    static auto check(...)  -> std::false_type;
  public:
    using type = decltype(has_mem_func_clear::check(std::declval<T>()));
    static constexpr bool value = type::value;
};
} // detail

template<typename T>
struct container_traits
{
    using ptr_accessibility = std::false_type;
};

template<typename T, typename Alloc>
struct container_traits<std::vector<T, Alloc>>
{
    using ptr_accessibility = std::true_type;
};
template<typename T, std::size_t N>
struct container_traits<std::array<T, N>>
{
    using ptr_accessibility = std::true_type;
};

} //asd
#endif// LIBASD_CONTAINER_TRAITS_H
