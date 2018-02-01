/*!
@file  frame.hpp
@brief definition of a class that contains frame (frame header and frame data)

For 1 channel case, it contains just one FrameData.
Otherwise, it contains an array of FrameData, std::array<FrameData, Channel>.

@author Toru Niina (niina.toru.68u@gmail.com)
@date 2017-11-06 14:30
@copyright Toru Niina
*/

#ifndef LIBASD_FRAME_H
#define LIBASD_FRAME_H
#include <libasd/exception_thrower.hpp>
#include <libasd/container_dispatcher.hpp>
#include <libasd/frame_header.hpp>
#include <libasd/frame_data.hpp>
#include <libasd/tag.hpp>
#include <type_traits>
#include <stdexcept>
#include <array>
#include <cstdint>

namespace asd
{

//! .asd frame class containing frame header and frame data information.
/*!
 * It contains an array of FrameData, std::array<FrameData, Channel>.
 * @tparam valueT type of a value corresponding to each pixel
 * @tparam chT    a class having the number of channel
 * @tparam contT  a container-dispatcher that defines container class to be used
 * */
template<typename valueT, typename chT, typename contT = container::vec>
struct Frame
{
    typedef chT channel_tag;
    static constexpr std::size_t num_channel = channel_tag::value;
    typedef valueT                                     pixel_type;
    typedef FrameHeader                                header_type;
    typedef FrameData<pixel_type, contT>               frame_data_type;
    typedef std::array<frame_data_type, num_channel>   data_type;
    typedef typename data_type::iterator               iterator;
    typedef typename data_type::const_iterator         const_iterator;
    typedef typename data_type::reverse_iterator       reverse_iterator;
    typedef typename data_type::const_reverse_iterator const_reverse_iterator;

    header_type header; //!< frame-header information
    data_type   data;   //!< frame-data information

    // ------------------------------------------------------------------------
    // container-interface as an array of frames (not as a framedata)
    // ------------------------------------------------------------------------

    //! return size of data (in this case, channel)
    constexpr std::size_t size() const noexcept {return data.size();}

    frame_data_type&       operator[](std::size_t i)       noexcept {return data[i];}
    frame_data_type const& operator[](std::size_t i) const noexcept {return data[i];}
    frame_data_type&       at(std::size_t i)       {return data.at(i);}
    frame_data_type const& at(std::size_t i) const {return data.at(i);}
    frame_data_type&       front()       noexcept {return data.front();}
    frame_data_type const& front() const noexcept {return data.front();}
    frame_data_type&       back()        noexcept {return data.back();}
    frame_data_type const& back()  const noexcept {return data.back();}

    iterator       begin()         noexcept {return data.begin();}
    iterator       end()           noexcept {return data.end();}
    const_iterator begin()   const noexcept {return data.begin();}
    const_iterator end()     const noexcept {return data.end();}
    const_iterator cbegin()  const noexcept {return data.cbegin();}
    const_iterator cend()    const noexcept {return data.cend();}

    reverse_iterator       rbegin()        noexcept {return data.rbegin();}
    reverse_iterator       rend()          noexcept {return data.rend();}
    const_reverse_iterator rbegin()  const noexcept {return data.rbegin();}
    const_reverse_iterator rend()    const noexcept {return data.rend();}
    const_reverse_iterator crbegin() const noexcept {return data.crbegin();}
    const_reverse_iterator crend()   const noexcept {return data.crend();}

};
template<typename valueT, typename chT, typename contT>
constexpr std::size_t Frame<valueT, chT, contT>::num_channel;

//! .asd frame class containing frame header and frame data information.
/*!
 * It contains just one FrameData. The interfaces are forwarded to FrameData.
 * @tparam contT a container-dispatcher that defines container class to be used
 */
template<typename valueT, typename contT>
struct Frame<valueT, channel<1>, contT>
{
    typedef channel<1> channel_tag;
    static constexpr std::size_t num_channel = channel_tag::value;
    typedef valueT                                     pixel_type;
    typedef FrameHeader                                header_type;
    typedef FrameData<valueT, contT>                   frame_data_type;
    typedef typename frame_data_type::proxy_type             proxy_type;
    typedef typename frame_data_type::const_proxy_type       const_proxy_type;
    typedef typename frame_data_type::iterator               iterator;
    typedef typename frame_data_type::const_iterator         const_iterator;
    typedef typename frame_data_type::reverse_iterator       reverse_iterator;
    typedef typename frame_data_type::const_reverse_iterator const_reverse_iterator;

    header_type     header;
    frame_data_type data;

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

    reverse_iterator       rbegin()        noexcept {return data.rbegin();}
    reverse_iterator       rend()          noexcept {return data.rend();}
    const_reverse_iterator rbegin()  const noexcept {return data.rbegin();}
    const_reverse_iterator rend()    const noexcept {return data.rend();}
    const_reverse_iterator crbegin() const noexcept {return data.crbegin();}
    const_reverse_iterator crend()   const noexcept {return data.crend();}
};
template<typename valueT, typename contT>
constexpr std::size_t Frame<valueT, channel<1>, contT>::num_channel;

} // asd
#endif//LIBASD_FRAME_TAG_H
