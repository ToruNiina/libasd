#ifndef LIBASD_FRAME_H
#define LIBASD_FRAME_H
#include <libasd/exception_thrower.hpp>
#include <libasd/container_dispatcher.hpp>
#include <libasd/frame_header.hpp>
#include <libasd/frame_data.hpp>
#include <libasd/channel_tag.hpp>
#include <type_traits>
#include <stdexcept>
#include <array>
#include <cstdint>

namespace asd
{

template<typename chT, typename contT = vec>
struct Frame
{
    typedef chT channel_tag;
    static constexpr std::size_t num_channel = channel_tag::value;
    typedef std::int16_t                               pixel_type;
    typedef FrameHeader                                header_type;
    typedef FrameData<contT, pixelT>                   frame_data_type;
    typedef std::array<frame_data_type, num_channel>   data_type;
    typedef typename data_type::iterator               iterator;
    typedef typename data_type::const_iterator         const_iterator;
    typedef typename data_type::reverse_iterator       reverse_iterator;
    typedef typename data_type::const_reverse_iterator const_reverse_iterator;

    header_type header;
    data_type   data;

    // ------------------------------------------------------------------------
    // container-interface as an array of frames (not as a framedata)
    // ------------------------------------------------------------------------

    constexpr std::size_t size() const noexcept {return data.size();}

    frame_data_type&       operator[](std::size_t i)       noexcept {return data[i];}
    frame_data_type const& operator[](std::size_t i) const noexcept {return data[i];}
    frame_data_type&       at(std::size_t i)       {return data.at(i);}
    frame_data_type const& at(std::size_t i) const {return data.at(i);}
    frame_data_type&       front()       noexcept {return data.front();}
    frame_data_type&       back()        noexcept {return data.back();}
    frame_data_type const& front() const noexcept {return data.front();}
    frame_data_type const& back()  const noexcept {return data.back();}

    iterator       begin()        noexcept {return data.begin();}
    iterator       end()          noexcept {return data.end();}
    const_iterator begin()  const noexcept {return data.begin();}
    const_iterator end()    const noexcept {return data.end();}
    const_iterator cbegin() const noexcept {return data.cbegin();}
    const_iterator cend()   const noexcept {return data.cend();}

    iterator       rbegin()        noexcept {return data.rbegin();}
    iterator       rend()          noexcept {return data.rend();}
    const_iterator rbegin()  const noexcept {return data.rbegin();}
    const_iterator rend()    const noexcept {return data.rend();}
    const_iterator crbegin() const noexcept {return data.crbegin();}
    const_iterator crend()   const noexcept {return data.crend();}

};

template<typename dataT, typename contT>
struct Frame<channel<1>, dataT, contT>
{
    typedef channel<1> channel_tag;
    static constexpr std::size_t num_channel = channel_tag::value;
    typedef std::int16_t                               pixel_type;
    typedef FrameHeader                                header_type;
    typedef FrameData<contT, dataT>                    frame_data_type;
    typedef frame_data_type                            data_type;
    typedef typename data_type::proxy_type             proxy_type;
    typedef typename data_type::const_proxy_type       const_proxy_type;
    typedef typename data_type::iterator               iterator;
    typedef typename data_type::const_iterator         const_iterator;
    typedef typename data_type::reverse_iterator       reverse_iterator;
    typedef typename data_type::const_reverse_iterator const_reverse_iterator;

    header_type header;
    data_type   data;

    // ------------------------------------------------------------------------
    // container-interface as a framedata
    // ------------------------------------------------------------------------

    constexpr std::size_t size() const noexcept {return 1;}

    proxy_type       operator[](std::size_t i)       noexcept {return data[i];}
    const_proxy_type operator[](std::size_t i) const noexcept {return data[i];}
    proxy_type       at(std::size_t i)       {return data.at(i);}
    const_proxy_type at(std::size_t i) const {return data.at(i);}
    proxy_type       front()       noexcept {return data.front();}
    const_proxy_type front() const noexcept {return data.front();}
    proxy_type       back()        noexcept {return data.back();}
    const_proxy_type back()  const noexcept {return data.back();}

    iterator       begin()        noexcept {return data.begin();}
    iterator       end()          noexcept {return data.end();}
    const_iterator begin()  const noexcept {return data.begin();}
    const_iterator end()    const noexcept {return data.end();}
    const_iterator cbegin() const noexcept {return data.cbegin();}
    const_iterator cend()   const noexcept {return data.cend();}

    iterator       rbegin()        noexcept {return data.rbegin();}
    iterator       rend()          noexcept {return data.rend();}
    const_iterator rbegin()  const noexcept {return data.rbegin();}
    const_iterator rend()    const noexcept {return data.rend();}
    const_iterator crbegin() const noexcept {return data.crbegin();}
    const_iterator crend()   const noexcept {return data.crend();}
};


} // asd
#endif//LIBASD_FRAME_TAG_H
