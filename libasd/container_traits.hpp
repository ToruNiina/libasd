#ifndef LIBASD_CONTAINER_TRAITS_H
#define LIBASD_CONTAINER_TRAITS_H
#include <type_traits>
#include <vector>
#include <array>

namespace asd
{

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
