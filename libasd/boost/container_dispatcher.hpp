#ifndef LIBASD_BOOST_CONTAINER_DISPATCHER_H
#define LIBASD_BOOST_CONTAINER_DISPATCHER_H

#include <boost/container/vector.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/container/deque.hpp>
#include <boost/array.hpp>

namespace asd
{

struct boost_vec
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::vector<T> other;
    };

    typedef std::true_type ptr_accessibility;

    template<typename T>
    static void resize(boost::container::vector<T>& cont, std::size_t N)
    {
        cont.resize(N);
        return;
    }

    template<typename T>
    static const T* get_ptr(const boost::container::vector<T>& cont) noexcept
    {
        return cont.data();
    }

    template<typename T>
    static std::size_t size(const boost::container::vector<T>& cont) noexcept
    {
        return cont.size();
    }
};

struct boost_stable_vec
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::stable_vector<T> other;
    };

    typedef std::false_type ptr_accessibility;

    template<typename T>
    static void resize(boost::container::stable_vector<T>& cont, std::size_t N)
    {
        cont.resize(N);
        return;
    }

    template<typename T, typename Alloc>
    static std::size_t size(const std::vector<T, Alloc>& cont) noexcept
    {
        return cont.size();
    }
};

template<std::size_t N>
struct boost_static_vec
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::static_vector<T, N> other;
    };

    typedef std::true_type ptr_accessibility;

    template<typename T>
    static void resize(boost::container::static_vector<T, N>& cont, std::size_t M)
    {
        cont.resize(M);
        return;
    }

    template<typename T>
    static const T*
    get_ptr(const boost::container::static_vector<T, N>& cont) noexcept
    {
        return cont.data();
    }

    template<typename T>
    static std::size_t
    size(const boost::container::static_vector<T, N>& cont) noexcept
    {
        return cont.size();
    }
};

template<std::size_t N>
struct boost_small_vec
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::small_vector<T, N> other;
    };

    typedef std::true_type ptr_accessibility;

    template<typename T>
    static void resize(boost::container::small_vector<T, N>& cont, std::size_t M)
    {
        cont.resize(M);
        return;
    }

    template<typename T>
    static const T*
    get_ptr(const boost::container::small_vector<T, N>& cont) noexcept
    {
        return cont.data();
    }

    template<typename T>
    static std::size_t
    size(const boost::container::small_vector<T, N>& cont) noexcept
    {
        return cont.size();
    }
};

struct boost_deq
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::deque<T> other;
    };

    typedef std::false_type ptr_accessibility;

    template<typename T>
    static void resize(boost::container::deque<T, N>& cont, std::size_t N)
    {
        cont.resize(N);
        return;
    }

    template<typename T>
    static std::size_t
    size(const boost::container::deque<T, N>& cont) noexcept
    {
        return cont.size();
    }
};

template<std::size_t N>
struct boost_arr
{
    template<typename T>
    struct rebind
    {
        typedef boost::array<T, N> other;
    };

    template<typename T>
    static void resize(boost::array<T, N>& cont, std::size_t M)
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
    static const T*
    get_ptr(const boost::array<T, N>& cont) noexcept
    {
        return cont.data();
    }

    template<typename T>
    static std::size_t
    size(const boost::array<T, N>& cont) noexcept
    {
        return cont.size();
    }
};

} // asd
#endif//LIBASD_BOOST_CONTAINER_DISPATCHER_H
