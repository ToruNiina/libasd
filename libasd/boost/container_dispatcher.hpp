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

struct boost_vec
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::vector<T> other;
    };
};

struct boost_stable_vec
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::stable_vector<T> other;
    };
};

template<std::size_t N>
struct boost_static_vec
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::static_vector<T, N> other;
    };
};

template<std::size_t N>
struct boost_small_vec
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::small_vector<T, N> other;
    };

};

struct boost_deq
{
    template<typename T>
    struct rebind
    {
        typedef boost::container::deque<T> other;
    };

};

template<std::size_t N>
struct boost_arr
{
    template<typename T>
    struct rebind
    {
        typedef boost::array<T, N> other;
    };
};

template<typename T, std::size_t N>
inline void resize(boost::array<T, N>& cont, std::size_t M)
{
    if(cont.size() < M)
    {
        throw_exception<std::out_of_range>("libasd::container::resize: "
                "boost::array(size = %) has no enough storage(%)",
                cont.size(), M);
    }
    return;
}

} // container
} // asd
#endif//LIBASD_BOOST_CONTAINER_DISPATCHER_H
