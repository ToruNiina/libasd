#ifndef LIBASD_FRAME_DATA_H
#define LIBASD_FRAME_DATA_H
#include <libasd/container_dispatcher.hpp>
#include <libasd/exception_thrower.hpp>
#include <type_traits>
#include <iterator>
#include <cstdint>

namespace asd
{

template<typename contT> struct FrameData;

namespace detail
{

template<bool IsConst, typename contT>
struct LineProxyIterator;

template<bool IsConst, typename contT>
struct LineProxy
{
    static constexpr bool is_constant = IsConst;
    friend LineProxyIterator<is_constant, contT>;

    typedef FrameData<contT> origin_type;
    typedef typename origin_type::raw_iterator               origin_iterator;
    typedef typename origin_type::raw_const_iterator         origin_const_iterator;
    typedef typename origin_type::raw_reverse_iterator       origin_reverse_iterator;
    typedef typename origin_type::raw_const_reverse_iterator origin_const_reverse_iterator;

    typedef typename std::conditional<is_constant,
            origin_const_iterator, origin_iterator>::type iterator;
    typedef origin_const_iterator const_iterator;
    typedef typename std::conditional<is_constant,
            origin_const_reverse_iterator, origin_reverse_iterator
            >::type reverse_iterator;
    typedef origin_const_reverse_iterator const_reverse_iterator;

    typedef typename std::iterator_traits<iterator> iterator_tratis_type;

    static_assert(std::is_same<typename iterator_tratis_type::iterator_category,
            std::random_access_iterator_tag>::value,
            "container should be randomly accessible");

    typedef typename iterator_tratis_type::difference_type difference_type;
    typedef typename iterator_tratis_type::value_type      value_type;
    typedef typename iterator_tratis_type::pointer         pointer;
    typedef typename iterator_tratis_type::reference       reference;

    LineProxy(iterator b, iterator e,
              std::size_t l, std::size_t x, std::size_t y) noexcept
        : begin_(b), end_(e), line(l), x_pixel(x), y_pixel(y)
    {}

    reference operator[](const std::size_t i) const noexcept
    {
        return *(this->begin_ + i);
    }

    reference at(const std::size_t i) const
    {
        if(i >= x_pixel)
        {
            throw_exception<std::out_of_range>(
                "libasd: Frame::LineProxy::at: line(%): index(%) exceeds x_pixel(%)",
                this->line, i, this->x_pixel);
        }
        return *(this->begin_ + i);
    }

    std::size_t size()       const noexcept {return x_pixel;}
    std::size_t max_size()   const noexcept {return x_pixel;}
    bool        empty()      const noexcept {return false;}
    std::size_t y_position() const noexcept {return line;}

    iterator begin() const noexcept {return begin_;}
    iterator end()   const noexcept {return end_;}
    const_iterator cbegin() const noexcept {return begin_;}
    const_iterator cend()   const noexcept {return end_;}

    reverse_iterator rbegin() const noexcept {return reverse_iterator(end_);}
    reverse_iterator rend()   const noexcept {return reverse_iterator(begin_);}

    const_reverse_iterator crbegin() const noexcept
    {return const_reverse_iterator(end_);}
    const_reverse_iterator crend()   const noexcept
    {return const_reverse_iterator(begin_);}

    bool operator==(LineProxy const& rhs) const noexcept
    {
        return this->begin_  == rhs.begin_  && this->end_ == rhs.end_ &&
               this->line == rhs.line &&
               this->x_pixel == rhs.x_pixel && this->y_pixel == rhs.y_pixel;
    }
    bool operator!=(LineProxy const& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    bool operator<(LineProxy const& rhs) const noexcept
    {
        return this->x_pixel == rhs.x_pixel && this->y_pixel == rhs.y_pixel &&
               this->line    <  rhs.line    && this->begin_  <  rhs.begin_;
    }
    bool operator<=(LineProxy const& rhs) const noexcept
    {
        return (*this == rhs) || *this < rhs;
    }

    bool operator>(LineProxy const& rhs) const noexcept
    {
        return this->x_pixel == rhs.x_pixel && this->y_pixel == rhs.y_pixel &&
               this->line    >  rhs.line    && this->begin_  >  rhs.begin_;
    }
    bool operator>=(LineProxy const& rhs) const noexcept
    {
        return (*this == rhs) || *this > rhs;
    }

  private:
    iterator begin_, end_;
    std::size_t line;
    std::size_t x_pixel;
    std::size_t y_pixel;
};

template<bool IsConst, typename contT>
struct LineProxyIterator
{
    static constexpr bool is_constant = IsConst;
    typedef LineProxy<is_constant, contT> proxy_type;
    typedef proxy_type             value_type;
    typedef value_type const*      pointer;
    typedef value_type const&      reference;
    typedef std::ptrdiff_t         difference_type;
    typedef std::random_access_iterator_tag iterator_category;

    LineProxyIterator(const proxy_type& p) : proxy(p) {}
    LineProxyIterator(proxy_type&& p) : proxy(std::move(p)) {}

    reference operator* () const noexcept {return this->proxy;}
    pointer   operator->() const noexcept {return std::addressof(this->proxy);}

    LineProxyIterator& operator++() noexcept
    {
        this->proxy.line   += 1;
        this->proxy.begin_ += this->proxy.x_pixel;
        this->proxy.end_   += this->proxy.x_pixel;
    }
    LineProxyIterator& operator--() noexcept
    {
        this->proxy.line   -= 1;
        this->proxy.begin_ -= this->proxy.x_pixel;
        this->proxy.end_   -= this->proxy.x_pixel;
    }
    LineProxyIterator  operator++(int) noexcept
    {
        const auto tmp(*this); ++(*this); return tmp;
    }
    LineProxyIterator  operator--(int) noexcept
    {
        const auto tmp(*this); --(*this); return tmp;
    }

    LineProxyIterator& operator+=(const difference_type d) noexcept
    {
        this->proxy.line  += d;
        this->proxy.begin += this->proxy.x_pixel * d;
        this->proxy.end   += this->proxy.x_pixel * d;
    }
    LineProxyIterator& operator-=(const difference_type d) noexcept
    {
        this->proxy.line  -= d;
        this->proxy.begin -= this->proxy.x_pixel * d;
        this->proxy.end   -= this->proxy.x_pixel * d;
    }

    bool operator==(LineProxyIterator const& rhs) const noexcept
    {
        return this->proxy == rhs.proxy;
    }
    bool operator!=(LineProxyIterator const& rhs) const noexcept
    {
        return this->proxy != rhs.proxy;
    }

    bool operator<=(LineProxyIterator const& rhs) const noexcept
    {
        return this->proxy <= rhs.proxy;
    }
    bool operator<(LineProxyIterator const& rhs) const noexcept
    {
        return this->proxy < rhs.proxy;
    }

    bool operator>=(LineProxyIterator const& rhs) const noexcept
    {
        return this->proxy >= rhs.proxy;
    }
    bool operator>(LineProxyIterator const& rhs) const noexcept
    {
        return this->proxy > rhs.proxy;
    }

  private:

    proxy_type proxy;
};

} // detail

template<typename contT = container::vec>
struct FrameData
{
    typedef std::int16_t data_type;
    typedef contT container_dispatcher_type;
    typedef typename contT::template rebind<data_type>::other container_type;
    typedef typename container_type::iterator               raw_iterator;
    typedef typename container_type::const_iterator         raw_const_iterator;
    typedef typename container_type::reverse_iterator       raw_reverse_iterator;
    typedef typename container_type::const_reverse_iterator raw_const_reverse_iterator;

    typedef detail::LineProxy<false, container_dispatcher_type>         proxy_type;
    typedef detail::LineProxy<true,  container_dispatcher_type>         const_proxy_type;
    typedef detail::LineProxyIterator<false, container_dispatcher_type> proxy_iterator;
    typedef detail::LineProxyIterator<true,  container_dispatcher_type> const_proxy_iterator;
    typedef proxy_iterator       iterator;
    typedef const_proxy_iterator const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    std::size_t    x_pixel, y_pixel;
    container_type data;

    // ------------------------------------------------------------------------
    // raw interface
    // ------------------------------------------------------------------------

    raw_iterator       raw_begin()        noexcept {return data.begin();}
    raw_iterator       raw_end()          noexcept {return data.end();}
    raw_const_iterator raw_begin()  const noexcept {return data.begin();}
    raw_const_iterator raw_end()    const noexcept {return data.end();}
    raw_const_iterator raw_cbegin() const noexcept {return data.cbegin();}
    raw_const_iterator raw_cend()   const noexcept {return data.cend();}

    raw_reverse_iterator       raw_rbegin()        noexcept {return data.rbegin();}
    raw_reverse_iterator       raw_rend()          noexcept {return data.rend();}
    raw_const_reverse_iterator raw_rbegin()  const noexcept {return data.rbegin();}
    raw_const_reverse_iterator raw_rend()    const noexcept {return data.rend();}
    raw_const_reverse_iterator raw_crbegin() const noexcept {return data.crbegin();}
    raw_const_reverse_iterator raw_crend()   const noexcept {return data.crend();}

    data_type& raw_at(std::size_t i)       {return data.at(i);}
    data_type  raw_at(std::size_t i) const {return data.at(i);}
    data_type& raw_access(std::size_t i)       noexcept {return data[i];}
    data_type  raw_access(std::size_t i) const noexcept {return data[i];}

    std::size_t raw_size() const noexcept {return data.size();}

    // ------------------------------------------------------------------------
    // proxy interface
    // ------------------------------------------------------------------------

    std::size_t size() const noexcept {return y_pixel;}

    proxy_type at(std::size_t i)
    {
        if(i >= y_pixel)
        {
            throw_exception<std::out_of_range>(
                "libasd: Frame::at: index(%) exceeds y_pixel(%)",
                i, this->y_pixel);
        }
        const std::size_t offset = i * x_pixel;
        return proxy_type(
            this->raw_begin() + offset, this->raw_begin() + offset + x_pixel,
            i, x_pixel, y_pixel);
    }
    const_proxy_type at(std::size_t i) const
    {
        if(i >= y_pixel)
        {
            throw_exception<std::out_of_range>(
                "libasd: Frame::at: index(%) exceeds y_pixel(%)",
                i, this->y_pixel);
        }
        const std::size_t offset = i * x_pixel;
        return const_proxy_type(
            this->raw_cbegin() + offset, this->raw_cbegin() + offset + x_pixel,
            i, x_pixel, y_pixel);
    }
    proxy_type operator[](std::size_t i) noexcept
    {
        const std::size_t offset = i * x_pixel;
        return proxy_type(
            this->raw_begin() + offset, this->raw_begin() + offset + x_pixel,
            i, x_pixel, y_pixel);
    }
    const_proxy_type operator[](std::size_t i) const noexcept
    {
        const std::size_t offset = i * x_pixel;
        return proxy_type(
            this->raw_cbegin() + offset, this->raw_cbegin() + offset + x_pixel,
            i, x_pixel, y_pixel);
    }

    proxy_type       front()       noexcept {return (*this)[0];}
    proxy_type       back()        noexcept {return (*this)[y_pixel - 1];}
    const_proxy_type front() const noexcept {return (*this)[0];}
    const_proxy_type back()  const noexcept {return (*this)[y_pixel - 1];}

    iterator begin()              noexcept {return iterator((*this)[0]);}
    iterator end()                noexcept {return iterator((*this)[y_pixel]);}
    const_iterator begin()  const noexcept {return const_iterator((*this)[0]);}
    const_iterator end()    const noexcept {return const_iterator((*this)[y_pixel]);}
    const_iterator cbegin() const noexcept {return const_iterator((*this)[0]);}
    const_iterator cend()   const noexcept {return const_iterator((*this)[y_pixel]);}

    reverse_iterator rbegin()              noexcept {return reverse_iterator(this->end());}
    reverse_iterator rend()                noexcept {return reverse_iterator(this->begin());}
    const_reverse_iterator rbegin()  const noexcept {return const_reverse_iterator(this->end());}
    const_reverse_iterator rend()    const noexcept {return const_reverse_iterator(this->begin());}
    const_reverse_iterator crbegin() const noexcept {return const_reverse_iterator(this->cend());}
    const_reverse_iterator crend()   const noexcept {return const_reverse_iterator(this->cbegin());}
};

} // asd
#endif// LIBASD_FRAME_DATA_H
