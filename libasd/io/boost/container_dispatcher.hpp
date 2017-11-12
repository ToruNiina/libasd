#ifndef LIBASD_BOOST_CONTAINER_DISPATCHER_H
#define LIBASD_BOOST_CONTAINER_DISPATCHER_H

#include <boost/container/vector.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/container/deque.hpp>
#include <boost/array.hpp>

namespace asd
{
namespace container
{

// ------------------------- boost::container::vector ------------------------//

struct boost_vec
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::vector<T> other;
    };
    using ptr_accessibility = std::true_type;
};
template<typename T, typename Alloc>
inline void resize(boost::container::vector<T, Alloc>& cont, std::size_t N)
{
    cont.resize(N);
    return;
}
template<typename T, typename Alloc>
inline const T* get_ptr(const boost::container::vector<T, Alloc>& cont) noexcept
{
    return cont.data();
}
template<typename T, typename Alloc>
inline std::size_t size(const boost::container::vector<T, Alloc>& cont) noexcept
{
    return cont.size();
}

// --------------------- boost::container::stable_vector ---------------------//

struct boost_stable_vec
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::stable_vector<T> other;
    };
    using ptr_accessibility = std::false_type;
};
template<typename T, typename Alloc>
inline void resize(boost::container::stable_vector<T, Alloc>& cont, std::size_t N)
{
    cont.resize(N);
    return;
}
template<typename T, typename Alloc>
inline std::size_t
size(const boost::container::stable_vector<T, Alloc>& cont) noexcept
{
    return cont.size();
}

// --------------------- boost::container::static_vector ---------------------//

template<std::size_t N>
struct boost_static_vec
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::static_vector<T, N> other;
    };
    using ptr_accessibility = std::true_type;
};
template<typename T, std::size_t N>
inline void resize(boost::container::static_vector<T, N>& cont, std::size_t M)
{
    cont.resize(M);
    return;
}

template<typename T, std::size_t N>
inline const T*
get_ptr(const boost::container::static_vector<T, N>& cont) noexcept
{
    return cont.data();
}

template<typename T, std::size_t N>
inline std::size_t
size(const boost::container::static_vector<T, N>& cont) noexcept
{
    return cont.size();
}

//---------------------- boost::container::small_vector ----------------------//

template<std::size_t N>
struct boost_small_vec
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::small_vector<T, N> other;
    };

    using ptr_accessibility = std::true_type;
};
template<typename T, std::size_t N, typename Alloc>
inline void resize(boost::container::small_vector<T, N, Alloc>& cont, std::size_t M)
{
    cont.resize(M);
    return;
}
template<typename T, std::size_t N, typename Alloc>
inline const T*
get_ptr(const boost::container::small_vector<T, N, Alloc>& cont) noexcept
{
    return cont.data();
}
template<typename T, std::size_t N, typename Alloc>
inline std::size_t
size(const boost::container::small_vector<T, N, Alloc>& cont) noexcept
{
    return cont.size();
}

//------------------------- boost::container::deque -------------------------//

struct boost_deq
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::deque<T> other;
    };

    using ptr_accessibility = std::false_type;
};

template<typename T, typename Alloc>
inline void resize(boost::container::deque<T, Alloc>& cont, std::size_t N)
{
    cont.resize(N);
    return;
}

template<typename T, typename Alloc>
inline std::size_t size(const boost::container::deque<T, Alloc>& cont) noexcept
{
    return cont.size();
}

// TODO : boost::container::pmr::*

//------------------------------- boost::array -------------------------------//

template<std::size_t N>
struct boost_arr
{
    template<typename T>
    struct rebind
    {
        typedef boost::array<T, N> other;
    };

    using ptr_accessibility = std::true_type;
};
template<typename T, std::size_t N>
inline void resize(boost::array<T, N>& cont, std::size_t M)
{
    if(cont.size() < M)
    {
        throw_exception<std::bad_alloc>(
                "array(size = %) has no enough storage(%)", cont.size(), M);
    }
    return;
}
template<typename T, std::size_t N>
inline const T* get_ptr(const boost::array<T, N>& cont) noexcept
{
    return cont.data();
}

template<typename T, std::size_t N>
inline std::size_t size(const boost::array<T, N>& cont) noexcept
{
    return cont.size();
}

} // container
} // asd
#endif//LIBASD_BOOST_CONTAINER_DISPATCHER_H
