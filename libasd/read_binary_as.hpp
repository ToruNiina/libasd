#ifndef LIBASD_READ_BINARY_AS_H
#define LIBASD_READ_BINARY_AS_H
#include <istream>
#include <libasd/container_dispatcher.hpp>

namespace asd
{
namespace detail
{

template<typename T>
T read_binary_as(const char*& ptr) noexcept
{
    const T retval = *reinterpret_cast<const T*>(ptr);
    ptr += sizeof(T);
    return retval;
}

template<typename T>
T read_binary_as(std::istream& is)
{
    constexpr std::size_t sz = sizeof(T);
    alignas(alignof(T)) char cbytes[sz];
    is.read(cbytes, sz);
    const T retval = *reinterpret_cast<const T*>(cbytes);
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
    is.read(reinterpret_cast<char*>(::asd::container::get_ptr(retval)),
            sz * N);
    return retval;
}

template<typename Value, typename ContainerDispatcher>
typename ContainerDispatcher::template rebind<Value>::other
read_binary_as(std::istream& is, const std::size_t N, std::false_type)
{
    // the traditional C-array form is not available, use Iterator.
    constexpr std::size_t sz = sizeof(Value);
    std::vector<char> buffer(sz * N);
    is.read(buffer.data(), sz * N);

    const Value* const first = reinterpret_cast<const Value*>(buffer.data());
    const Value* const last  = reinterpret_cast<const Value*>(buffer.data())+N;

    return typename ContainerDispatcher::template
        rebind<Value>::other(first, last);
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
read_binary_as(const char*& ptr, const std::size_t N)
{
    const Value* const first = reinterpret_cast<const Value*>(ptr);
    const Value* const last  = reinterpret_cast<const Value*>(ptr) + N;

    return typename ContainerDispatcher::template
        rebind<Value>::other(first, last);
}

} // detail
}// asd
#endif// LIBASD_READ_BINARY_AS_H
