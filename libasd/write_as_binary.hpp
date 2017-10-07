#ifndef LIBASD_WRITE_BINARY_AS_H
#define LIBASD_WRITE_BINARY_AS_H
#include <ostream>
#include <utility>
#include <memory>

namespace asd
{
namespace detail
{

template<typename T>
void write_as_binary(char*& ptr, const T& value) noexcept
{
    constexpr std::size_t sz = sizeof(T);
    ptr = std::copy(reinterpret_cast<const char*>(std::addressof(value)),
                    reinterpret_cast<const char*>(std::addressof(value)) + sz,
                    ptr);
    return;
}

template<typename T>
void write_as_binary(std::ostream& os, const T& value)
{
    os.write(reinterpret_cast<const char*>(std::addressof(value)), sizeof(T));
    return;
}

// for array with length
template<typename containerT, typename ContainerDispatcher>
void write_as_binary(char*& ptr, const containerT& c,
                     ContainerDispatcher, std::true_type) noexcept
{
    ptr = std::copy(
        reinterpret_cast<const char*>(ContainerDispatcher::get_ptr(c)),
        reinterpret_cast<const char*>(ContainerDispatcher::get_ptr(c))
            + (ContainerDispatcher::size(c) * sizeof(typename containerT::value_type)),
        ptr);
    return;
}

template<typename containerT, typename ContainerDispatcher>
void write_as_binary(std::ostream& os, const containerT& c,
                     ContainerDispatcher, std::true_type)
{
    os.write(reinterpret_cast<const char*>(ContainerDispatcher::get_ptr(c)),
             (ContainerDispatcher::size(c) * sizeof(typename containerT::value_type)));
    return;
}

template<typename containerT, typename ContainerDispatcher>
void write_as_binary(char*& ptr, const containerT& c,
                     ContainerDispatcher, std::false_type) noexcept
{
    for(auto const& item : c)
    {
        write_as_binary(ptr, item);
    }
    return;
}

template<typename containerT, typename ContainerDispatcher>
void write_as_binary(std::ostream& os, const containerT& c,
                     ContainerDispatcher, std::false_type)
{
    for(auto const& item : c)
    {
        write_as_binary(os, item);
    }
    return;
}

inline void skip_bytes(char*& ptr, std::size_t sz) noexcept
{
    for(std::size_t i=0; i<sz; ++i)
    {
        *ptr = '\0';
        ++ptr;
    }
    return;
}
inline void skip_bytes(std::ostream& os, std::size_t sz)
{
    for(std::size_t i=0; i<sz; ++i)
    {
        os.put('\0');
    }
    return;
}


} // detail
}// asd
#endif// LIBASD_WRITE_BINARY_AS_H
