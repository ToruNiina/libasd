#ifndef LIBASD_FRAME_DATA_H
#define LIBASD_FRAME_DATA_H
#include <libasd/container_dispatcher.hpp>
#include <libasd/exception_thrower.hpp>
#include <libasd/line_proxy.hpp>
#include <cstdint>

namespace asd
{

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
        return const_proxy_type(
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
