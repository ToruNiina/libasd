#ifndef LIBASD_CHANNEL_VERSION_TAG_H
#define LIBASD_CHANNEL_VERSION_TAG_H
#include <type_traits>
#include <cstddef>

namespace asd
{

template<std::size_t N>
struct channel : std::integral_constant<std::size_t, N>{};
template<std::size_t N>
struct version : std::integral_constant<std::size_t, N>{};

template<std::size_t N> using ch  = channel<N>;
template<std::size_t N> using ver = version<N>;

#if __cplusplus >= 201703L
constexpr inline auto ch1 = channel<1>{};
constexpr inline auto ch2 = channel<2>{};
constexpr inline auto v0  = version<0>{};
constexpr inline auto v1  = version<1>{};
constexpr inline auto v2  = version<2>{};
#endif

} // asd
#endif//LIBASD_CHANNEL_TAG_H
