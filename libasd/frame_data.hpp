#ifndef LIBASD_FRAME_DATA_H
#define LIBASD_FRAME_DATA_H
#include <libasd/exception_thrower.hpp>
#include <libasd/line_proxy.hpp>
#include <libasd/container_dispatcher.hpp>
#include <cstdint>

namespace asd
{

template<typename valueT, typename contT = container::vec>
struct FrameData
{
    typedef valueT value_type;
    typedef contT container_dispatcher_type;
    typedef FrameData<value_type, container_dispatcher_type> self_type;
    typedef typename contT::template rebind<value_type>::other container_type;
    typedef typename container_type::iterator               raw_iterator;
    typedef typename container_type::const_iterator         raw_const_iterator;
    typedef typename container_type::reverse_iterator       raw_reverse_iterator;
    typedef typename container_type::const_reverse_iterator raw_const_reverse_iterator;

    typedef detail::LineProxy<false, self_type>         proxy_type;
    typedef detail::LineProxy<true,  self_type>         const_proxy_type;
    typedef detail::LineProxyIterator<false, self_type> proxy_iterator;
    typedef detail::LineProxyIterator<true,  self_type> const_proxy_iterator;
    typedef proxy_iterator       iterator;
    typedef const_proxy_iterator const_iterator;

    // XXX
    // Since std::reverse_iterator returns address of local value, ProxyIterator
    // cannot be wrapped by std::reverse_iterator. New ProxyReverseIterator
    // type is needed.
    typedef detail::LineProxyReverseIterator<false, self_type>
            reverse_proxy_iterator;
    typedef detail::LineProxyReverseIterator<true,  self_type>
            const_reverse_proxy_iterator;
    typedef reverse_proxy_iterator       reverse_iterator;
    typedef const_reverse_proxy_iterator const_reverse_iterator;

    FrameData() = default;
    ~FrameData() = default;

    FrameData(const std::size_t x, const std::size_t y)
        : x_pixel_(x), y_pixel_(y), data_(x * y)
    {}

    FrameData(const FrameData&) = default;
    FrameData(FrameData&&)      = default;
    FrameData& operator=(const FrameData&) = default;
    FrameData& operator=(FrameData&&)      = default;

    void reset(const std::size_t x, const std::size_t y)
    {
        this->x_pixel_ = x;
        this->y_pixel_ = y;
        container::clear(this->data_);
        container::resize(this->data_, x * y);
        return;
    }

    // ------------------------------------------------------------------------
    // raw interface
    // ------------------------------------------------------------------------

    raw_iterator       raw_begin()        noexcept {return data_.begin();}
    raw_iterator       raw_end()          noexcept {return data_.end();}
    raw_const_iterator raw_begin()  const noexcept {return data_.begin();}
    raw_const_iterator raw_end()    const noexcept {return data_.end();}
    raw_const_iterator raw_cbegin() const noexcept {return data_.cbegin();}
    raw_const_iterator raw_cend()   const noexcept {return data_.cend();}

    raw_reverse_iterator       raw_rbegin()        noexcept {return data_.rbegin();}
    raw_reverse_iterator       raw_rend()          noexcept {return data_.rend();}
    raw_const_reverse_iterator raw_rbegin()  const noexcept {return data_.rbegin();}
    raw_const_reverse_iterator raw_rend()    const noexcept {return data_.rend();}
    raw_const_reverse_iterator raw_crbegin() const noexcept {return data_.crbegin();}
    raw_const_reverse_iterator raw_crend()   const noexcept {return data_.crend();}

    value_type& raw_at(std::size_t i)       {return data_.at(i);}
    value_type  raw_at(std::size_t i) const {return data_.at(i);}
    value_type& raw_at(std::size_t i, std::nothrow_t)       noexcept {return data_[i];}
    value_type  raw_at(std::size_t i, std::nothrow_t) const noexcept {return data_[i];}

    std::size_t raw_size() const noexcept {return container::size(this->data_);}

    container_type&       base()       noexcept {return this->data_;}
    container_type const& base() const noexcept {return this->data_;}

    // ------------------------------------------------------------------------
    // proxy interface
    // ------------------------------------------------------------------------

    std::size_t x_pixel() const noexcept {return x_pixel_;}
    std::size_t y_pixel() const noexcept {return y_pixel_;}
    std::size_t size()    const noexcept {return y_pixel_;}

    proxy_type at(std::size_t i)
    {
        if(i >= y_pixel_)
        {
            throw_exception<std::out_of_range>(
                "libasd: Frame::at: index(%) exceeds y_pixel(%)",
                i, this->y_pixel_);
        }
        const std::size_t offset = i * x_pixel_;
        return proxy_type(this->raw_begin() + offset,
                          this->raw_begin() + offset + x_pixel_,
                          i, x_pixel_, y_pixel_);
    }
    const_proxy_type at(std::size_t i) const
    {
        if(i >= y_pixel_)
        {
            throw_exception<std::out_of_range>(
                "libasd: Frame::at: index(%) exceeds y_pixel(%)",
                i, this->y_pixel_);
        }
        const std::size_t offset = i * x_pixel_;
        return const_proxy_type(this->raw_cbegin() + offset,
                                this->raw_cbegin() + offset + x_pixel_,
                                i, x_pixel_, y_pixel_);
    }
    proxy_type operator[](std::size_t i) noexcept
    {
        const std::size_t offset = i * x_pixel_;
        return proxy_type(this->raw_begin() + offset,
                          this->raw_begin() + offset + x_pixel_,
                          i, x_pixel_, y_pixel_);
    }
    const_proxy_type operator[](std::size_t i) const noexcept
    {
        const std::size_t offset = i * x_pixel_;
        return const_proxy_type(this->raw_cbegin() + offset,
                                this->raw_cbegin() + offset + x_pixel_,
                                i, x_pixel_, y_pixel_);
    }

    proxy_type       front()       noexcept {return (*this)[0];}
    const_proxy_type front() const noexcept {return (*this)[0];}
    proxy_type       back()        noexcept {return (*this)[y_pixel_ - 1];}
    const_proxy_type back()  const noexcept {return (*this)[y_pixel_ - 1];}

    iterator       begin()        noexcept {return iterator((*this)[0]);}
    iterator       end()          noexcept {return iterator((*this)[y_pixel_]);}
    const_iterator begin()  const noexcept {return const_iterator((*this)[0]);}
    const_iterator end()    const noexcept {return const_iterator((*this)[y_pixel_]);}
    const_iterator cbegin() const noexcept {return const_iterator((*this)[0]);}
    const_iterator cend()   const noexcept {return const_iterator((*this)[y_pixel_]);}

    reverse_iterator       rbegin()        noexcept {return reverse_iterator((*this)[y_pixel_]);}
    reverse_iterator       rend()          noexcept {return reverse_iterator((*this)[0]       );}
    const_reverse_iterator rbegin()  const noexcept {return const_reverse_iterator((*this)[y_pixel_]);}
    const_reverse_iterator rend()    const noexcept {return const_reverse_iterator((*this)[0]       );}
    const_reverse_iterator crbegin() const noexcept {return const_reverse_iterator((*this)[y_pixel_]);}
    const_reverse_iterator crend()   const noexcept {return const_reverse_iterator((*this)[0]       );}

  private:
    std::size_t    x_pixel_, y_pixel_;
    container_type data_;
};

} // asd
#endif// LIBASD_FRAME_DATA_H
