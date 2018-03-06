#ifndef LIBASD_CONTAINER_DISPATCHER_H
#define LIBASD_CONTAINER_DISPATCHER_H
#include <libasd/exception_thrower.hpp>
#include <libasd/container_traits.hpp>
#include <vector>
#include <deque>

namespace asd
{
namespace container
{

template<typename containerT, typename std::enable_if<
    ::asd::detail::has_mem_func_data<containerT>::value,
    std::nullptr_t>::type = nullptr>
inline typename container_traits<containerT>::value_type const*
get_ptr(const containerT& c) noexcept(noexcept(c.data()))
{
    return c.data();
}

template<typename containerT, typename std::enable_if<
    ::asd::detail::has_mem_func_data<containerT>::value,
    std::nullptr_t>::type = nullptr>
inline typename container_traits<containerT>::value_type*
get_ptr(containerT& c) noexcept(noexcept(c.data()))
{
    return c.data();
}

template<typename containerT, typename std::enable_if<
    ::asd::detail::has_mem_func_size<containerT>::value,
    std::nullptr_t>::type = nullptr>
inline std::size_t size(const containerT& c) noexcept(noexcept(c.size()))
{
    return c.size();
}

template<typename containerT, typename std::enable_if<
    ::asd::detail::has_mem_func_resize<containerT>::value,
    std::nullptr_t>::type = nullptr>
inline void resize(containerT& c, std::size_t N)
{
    c.resize(N);
    return;
}

template<typename containerT, typename std::enable_if<
    ::asd::detail::has_mem_func_clear<containerT>::value,
    std::nullptr_t>::type = nullptr>
inline void clear(containerT& c) noexcept(noexcept(c.clear()))
{
    return c.clear();
}

struct vec
{
    template<typename T>
    struct rebind
    {
        typedef std::vector<T, std::allocator<T>> other;
    };
};

struct deq
{
    template<typename T>
    struct rebind
    {
        typedef std::deque<T, std::allocator<T>> other;
    };
};

} // container
} // asd
#endif//LIBASD_CONTAINER_DISPATCHER_H
