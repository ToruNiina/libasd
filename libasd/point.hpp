#ifndef LIBASD_POINT_H
#define LIBASD_POINT_H
#include <libasd/type_traits.hpp>
#include <utility>
#include <array>
#include <ostream>
#include <cmath>

namespace asd
{

template<typename realT, std::size_t N>
class point
{
  public:
    typedef realT real_type;
    typedef real_type   value_type;
    typedef std::size_t size_type;
    constexpr static std::size_t dim = N;
    typedef std::array<value_type, dim> container_type;
    typedef typename container_type::iterator iterator;
    typedef typename container_type::const_iterator const_iterator;

    point()  noexcept {container_.fill(0);}
    ~point() noexcept = default;

    point(const point& rhs) noexcept = default;
    point(point&& rhs)      noexcept = default;
    point& operator=(const point& rhs) noexcept = default;
    point& operator=(point&& rhs)      noexcept = default;

    template<typename ... Ts, typename std::enable_if<
        conjunction<std::is_convertible<Ts, real_type>...>::value,
        std::nullptr_t>::type = nullptr>
    point(const Ts&& ... xs) noexcept
        : container_{{static_cast<real_type>(xs)...}}
    {}

    point& operator+=(const point& rhs) noexcept
    {
        for(std::size_t i=0; i<N; ++i)
        {
            container_[i] += rhs.container_[i];
        }
        return *this;
    }
    point& operator+=(const value_type rhs) noexcept
    {
        for(std::size_t i=0; i<N; ++i)
        {
            container_[i] += rhs;
        }
        return *this;
    }
    point& operator-=(const point& rhs) noexcept
    {
        for(std::size_t i=0; i<N; ++i)
        {
            container_[i] -= rhs.container_[i];
        }
        return *this;
    }
    point& operator-=(const value_type rhs) noexcept
    {
        for(std::size_t i=0; i<N; ++i)
        {
            container_[i] -= rhs;
        }
        return *this;
    }
    point& operator*=(const value_type rhs) noexcept
    {
        for(std::size_t i=0; i<N; ++i)
        {
            container_[i] *= rhs;
        }
        return *this;
    }
    point& operator/=(const value_type rhs) noexcept
    {
        for(std::size_t i=0; i<N; ++i)
        {
            container_[i] /= rhs;
        }
        return *this;
    }

    constexpr size_type size() const noexcept {return dim;}

    value_type& operator[](const size_type i)       noexcept {return container_[i];}
    value_type  operator[](const size_type i) const noexcept {return container_[i];}
    value_type& at(const size_type i)       {return container_.at(i);}
    value_type  at(const size_type i) const {return container_.at(i);}

    iterator        begin()       noexcept {return container_.begin();}
    iterator        end()         noexcept {return container_.end();}
    const_iterator  begin() const noexcept {return container_.begin();}
    const_iterator  end()   const noexcept {return container_.end();}
    const_iterator cbegin() const noexcept {return container_.cbegin();}
    const_iterator cend()   const noexcept {return container_.cend();}

  private:
    container_type container_;
};

template<typename realT, std::size_t N>
inline point<realT, N>
operator+(const point<realT, N>& lhs, const point<realT, N>& rhs) noexcept
{
    point<realT, N> p(lhs);
    p += rhs;
    return p;
}
template<typename realT, std::size_t N, typename T, typename std::enable_if<
    std::is_convertible<T, realT>::value, std::nullptr_t>::type = nullptr>
inline point<realT, N>
operator+(const point<realT, N>& lhs, const T rhs) noexcept
{
    point<realT, N> p(lhs);
    p += rhs;
    return p;
}
template<typename realT, std::size_t N>
inline point<realT, N>
operator-(const point<realT, N>& lhs, const point<realT, N>& rhs) noexcept
{
    point<realT, N> p(lhs);
    p -= rhs;
    return p;
}

template<typename realT, std::size_t N, typename T, typename std::enable_if<
    std::is_convertible<T, realT>::value, std::nullptr_t>::type = nullptr>
inline point<realT, N>
operator-(const point<realT, N>& lhs, const T rhs) noexcept
{
    point<realT, N> p(lhs);
    p -= rhs;
    return p;
}
template<typename realT, std::size_t N, typename T, typename std::enable_if<
    std::is_convertible<T, realT>::value, std::nullptr_t>::type = nullptr>
inline point<realT, N>
operator*(const point<realT, N>& lhs, const T rhs) noexcept
{
    point<realT, N> p(lhs);
    p *= rhs;
    return p;
}
template<typename realT, std::size_t N, typename T, typename std::enable_if<
    std::is_convertible<T, realT>::value, std::nullptr_t>::type = nullptr>
inline point<realT, N>
operator*(const T lhs, const point<realT, N>& rhs) noexcept
{
    point<realT, N> p(rhs);
    p *= lhs;
    return p;
}
template<typename realT, std::size_t N, typename T, typename std::enable_if<
    std::is_convertible<T, realT>::value, std::nullptr_t>::type = nullptr>
inline point<realT, N>
operator/(const point<realT, N>& lhs, const T rhs) noexcept
{
    point<realT, N> p(lhs);
    p /= rhs;
    return p;
}

template<typename realT, std::size_t N>
inline bool
operator==(const point<realT, N>& lhs, const point<realT, N>& rhs) noexcept
{
    for(std::size_t i=0; i<N; ++i)
    {
        if(lhs[i] != rhs[i]){return false;}
    }
    return true;
}
template<typename realT, std::size_t N>
inline bool
operator!=(const point<realT, N>& lhs, const point<realT, N>& rhs) noexcept
{
    return !(lhs == rhs);
}

template<typename charT, typename traits, typename realT, std::size_t N>
inline std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os,
           const point<realT, N>& pos)
{
    for(auto i : pos)
    {
        os << i << ", ";
    }
    return os;
}

template<typename realT, std::size_t N>
inline point<realT, N>
abs(const point<realT, N>& lhs) noexcept
{
    point<realT, N> p;
    for(std::size_t i=0; i<N; ++i)
    {
        p[i] = std::abs(lhs[i]);
    }
    return p;
}

template<typename realT, std::size_t N>
inline realT
dot_product(const point<realT, N>& lhs, const point<realT, N>& rhs) noexcept
{
    realT d = 0;
    for(std::size_t i=0; i<N; ++i)
    {
        d = lhs[i] * rhs[i];
    }
    return d;
}

template<typename realT>
inline point<realT, 3>
cross_product(const point<realT, 3>& lhs, const point<realT, 3>& rhs) noexcept
{
    return point<realT, 3>(lhs[1] * rhs[2] - lhs[2] * rhs[1],
                           lhs[2] * rhs[0] - lhs[0] * rhs[2],
                           lhs[0] * rhs[1] - lhs[1] * rhs[0]);
}

template<typename realT, std::size_t N>
inline realT length_sq(const point<realT, N>& lhs) noexcept
{
    return dot_product(lhs, lhs);
}

template<typename realT, std::size_t N>
inline realT length(const point<realT, N>& lhs) noexcept
{
    return std::sqrt(length_sq(lhs));
}

} // asd
#endif// LIBASD_POINT_H
