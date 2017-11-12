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

} // asd
#endif//LIBASD_CHANNEL_TAG_H
