#ifndef LIBASD_CONTAINER_DISPATCHER_H
#define LIBASD_CONTAINER_DISPATCHER_H
#include <vector>
#include <deque>
#include <array>
#include <libasd/exception_thrower.hpp>

namespace asd
{
namespace container
{

struct vec
{
    template<typename T>
    struct rebind
    {
        typedef std::vector<T, std::allocator<T>> other;
    };

    template<typename T, typename Alloc>
    static void resize(std::vector<T, Alloc>& cont, std::size_t N)
    {
        cont.resize(N);
        return;
    }
};

struct deq
{
    template<typename T>
    struct rebind
    {
        typedef std::deque<T, std::allocator<T>> other;
    };

    template<typename T, typename Alloc>
    static void resize(std::deque<T, Alloc>& cont, std::size_t N)
    {
        cont.resize(N);
        return;
    }
};

template<std::size_t N>
struct arr
{
    template<typename T>
    struct rebind
    {
        typedef std::array<T, N> other;
    };

    template<typename T>
    static void resize(std::array<T, N>& cont, std::size_t M)
    {
        if(cont.size() < M)
        {
            throw_exception<std::bad_alloc>(
                    "array(size = %) has no enough storage(size = %)",
                    cont.size(), M);
        }
        return;
    }
};

} // container
} // asd
#endif//LIBASD_CONTAINER_DISPATCHER_H
