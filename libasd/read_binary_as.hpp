#ifndef LIBASD_READ_BINARY_AS_H
#define LIBASD_READ_BINARY_AS_H
#include <istream>
#include <cstring>
#include <libasd/container_dispatcher.hpp>

namespace asd
{
namespace detail
{

template<typename T>
T read_binary_as(const char*& ptr) noexcept
{
    T retval;
    std::memcpy(std::addressof(retval), ptr, sizeof(T));
    ptr += sizeof(T);
    return retval;
}

template<typename T>
T read_binary_as(std::istream& is)
{
    T retval;
    is.read(reinterpret_cast<char*>(std::addressof(retval)), sizeof(T));
    return retval;
}

inline void ignore_bytes(const char*& ptr, std::ptrdiff_t pdiff) noexcept
{
    ptr += pdiff;
    return;
}
inline void ignore_bytes(std::istream& is, std::ptrdiff_t pdiff)
{
    is.ignore(pdiff);
    return;
}

// read array of value

template<typename Value, typename ContainerDispatcher>
typename ContainerDispatcher::template rebind<Value>::other
read_binary_as(std::istream& is, const std::size_t N, std::true_type)
{
    // the traditional C-array form is available. write directory into the ptr.
    constexpr std::size_t sz = sizeof(Value);
    typename ContainerDispatcher::template rebind<Value>::other retval(N);
    is.read(reinterpret_cast<char*>(::asd::container::get_ptr(retval)), sz * N);
    return retval;
}

template<typename Value, typename ContainerDispatcher>
typename ContainerDispatcher::template rebind<Value>::other
read_binary_as(std::istream& is, const std::size_t N, std::false_type)
{
    // the traditional C-array form is not available, use Iterator.
    std::vector<Value> buffer(N);
    is.read(reinterpret_cast<char*>(buffer.data()), sizeof(Value) * N);

    return typename ContainerDispatcher::template
        rebind<Value>::other(buffer.begin(), buffer.end());
}

template<typename Value, typename ContainerDispatcher>
typename ContainerDispatcher::template rebind<Value>::other
read_binary_as(std::istream& is, const std::size_t N)
{
    return read_binary_as<Value, ContainerDispatcher>(is, N,
        typename container_traits<
            typename ContainerDispatcher::template rebind<Value>::other
        >::ptr_accessibility{});
}

template<typename Value, typename ContainerDispatcher>
typename ContainerDispatcher::template rebind<Value>::other
read_binary_as(const char*& ptr, const std::size_t N, std::true_type)
{
    // the traditional C-array form is available. write directory into the ptr.
    typename ContainerDispatcher::template rebind<Value>::other retval(N);
    std::memcpy(::asd::container::get_ptr(retval), ptr, sizeof(Value) * N);
    ptr += sizeof(Value) * N;
    return retval;
}
template<typename Value, typename ContainerDispatcher>
typename ContainerDispatcher::template rebind<Value>::other
read_binary_as(const char*& ptr, const std::size_t N, std::false_type)
{
    // the traditional C-array form is not available, use Iterator.
    std::vector<Value> buffer(N);
    std::memcpy(buffer.data(), ptr, sizeof(Value) * N);
    ptr += sizeof(Value) * N;
    return typename ContainerDispatcher::template
        rebind<Value>::other(buffer.begin(), buffer.end());
}

template<typename Value, typename ContainerDispatcher>
typename ContainerDispatcher::template rebind<Value>::other
read_binary_as(const char*& ptr, const std::size_t N)
{
    return read_binary_as<Value, ContainerDispatcher>(ptr, N,
        typename container_traits<
            typename ContainerDispatcher::template rebind<Value>::other
        >::ptr_accessibility{});
}

} // detail
}// asd
#endif// LIBASD_READ_BINARY_AS_H
