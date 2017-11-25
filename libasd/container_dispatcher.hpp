#ifndef LIBASD_CONTAINER_DISPATCHER_H
#define LIBASD_CONTAINER_DISPATCHER_H
#include <vector>
#include <deque>
#include <array>
#include <libasd/exception_thrower.hpp>
#include <libasd/container_traits.hpp>

namespace asd
{
namespace container
{

template<typename containerT, typename std::enable_if<
    ::asd::detail::has_mem_func_data<containerT>::value, std::nullptr_t
    >::type = nullptr>
inline typename containerT::value_type const* get_ptr(const containerT& c)
    noexcept(noexcept(c.data()))
{
    return c.data();
}

template<typename containerT, typename std::enable_if<
    ::asd::detail::has_mem_func_size<containerT>::value, std::nullptr_t
    >::type = nullptr>
inline std::size_t size(const containerT& c) noexcept(noexcept(c.size()))
{
    return c.size();
}

template<typename containerT, typename std::enable_if<
    ::asd::detail::has_mem_func_resize<containerT>::value, std::nullptr_t
    >::type = nullptr>
inline void resize(containerT& c, std::size_t N)
{
    return c.resize(N);
}

template<typename containerT, typename std::enable_if<
    ::asd::detail::has_mem_func_clear<containerT>::value, std::nullptr_t
    >::type = nullptr>
inline void clear(containerT& c) noexcept(noexcept(c.clear()))
{
    return c.clear();
}

// ---------------------------------- vector ---------------------------------//

struct vec
{
    template<typename T>
    struct rebind
    {
        typedef std::vector<T, std::allocator<T>> other;
    };
};
// not needed
//
// template<typename T, typename Alloc>
// inline T const* get_ptr(const std::vector<T, Alloc>& v) noexcept
// {
//     return v.data();
// }
// template<typename T, typename Alloc>
// inline std::size_t size(const std::vector<T, Alloc>& v) noexcept
// {
//     return v.size();
// }
// template<typename T, typename Alloc>
// inline void resize(std::vector<T, Alloc>& v, const std::size_t N)
// {
//     return v.resize(N);
// }
// template<typename T, typename Alloc>
// inline void clear(std::vector<T, Alloc>& v)
// {
//     return v.clear();
// }

// ---------------------------------- deque ----------------------------------//

struct deq
{
    template<typename T>
    struct rebind
    {
        typedef std::deque<T, std::allocator<T>> other;
    };
};

// ---------------------------------- array ----------------------------------//

template<std::size_t N>
struct arr
{
    template<typename T>
    struct rebind
    {
        typedef std::array<T, N> other;
    };
};

template<typename T, std::size_t N>
inline void resize(std::array<T, N>& v, const std::size_t M)
{
    if(v.size() < M)
    {
        throw_exception<std::bad_alloc>(
                "array(size = %) has no enough storage(%)", v.size(), M);
    }
    return;
}

template<typename T, std::size_t N>
inline void clear(std::array<T, N>& v)
{
    // do nothing
    return;
}

} // container
} // asd
#endif//LIBASD_CONTAINER_DISPATCHER_H
