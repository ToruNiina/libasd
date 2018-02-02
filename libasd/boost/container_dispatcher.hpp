#ifndef LIBASD_BOOST_CONTAINER_DISPATCHER_H
#define LIBASD_BOOST_CONTAINER_DISPATCHER_H

#include <boost/container/vector.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/container/deque.hpp>

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

} // container
} // asd
#endif//LIBASD_BOOST_CONTAINER_DISPATCHER_H
