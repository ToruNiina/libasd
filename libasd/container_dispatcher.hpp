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

    typedef std::true_type ptr_accessibility;

    template<typename T, typename Alloc>
    static void resize(std::vector<T, Alloc>& cont, std::size_t N)
    {
        cont.resize(N);
        return;
    }

    template<typename T, typename Alloc>
    static const T* get_ptr(const std::vector<T, Alloc>& cont) noexcept
    {
        return cont.data();
    }

    template<typename T, typename Alloc>
    static std::size_t size(const std::vector<T, Alloc>& cont) noexcept
    {
        return cont.size();
    }
};

struct deq
{
    template<typename T>
    struct rebind
    {
        typedef std::deque<T, std::allocator<T>> other;
    };

    typedef std::false_type ptr_accessibility;

    template<typename T, typename Alloc>
    static void resize(std::deque<T, Alloc>& cont, std::size_t N)
    {
        cont.resize(N);
        return;
    }

    // pointer cannot be extracted...

    template<typename T, typename Alloc>
    static std::size_t size(const std::deque<T, Alloc>& cont)
    {
        return cont.size();
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

    typedef std::true_type ptr_accessibility;

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

    template<typename T>
    static const T* get_ptr(const std::array<T, N>& cont)
    {
        return cont.data();
    }

    template<typename T>
    static std::size_t size(const std::array<T, N>& cont)
    {
        return cont.size();
    }
};

} // container
} // asd
#endif//LIBASD_CONTAINER_DISPATCHER_H
