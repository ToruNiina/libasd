/*!
@file  data.hpp
@brief definition of a class that contains data from .asd files

@author Toru Niina (niina.toru.68u@gmail.com)
@date 2017-11-06 14:30
@copyright Toru Niina
*/

#ifndef LIBASD_READ_DATA_H
#define LIBASD_READ_DATA_H
#include <libasd/container_dispatcher.hpp>
#include <libasd/header.hpp>
#include <libasd/frame.hpp>
#include <libasd/tag.hpp>

namespace asd
{

template<typename valueT,
         typename chT   = channel<1>,
         typename verT  = version<1>,
         typename contT = container::vec>
struct Data;

//! .asd data class.
/*!
 * class Data contains header, frame-header, frame-data informations from asd files.
 * @tparam valueT a value type corresponds to each pixel.
 * @tparam chT    a class having the number of channel (usually, asd::channel<1>).
 * @tparam verT   a class having the version information (usually, asd::version<1>).
 * @tparam contT  a container-dispatcher that defines container class to be used (usually, container::vec).
 * */
template<typename valueT, typename chT, typename verT, typename contT>
struct Data
{
    typedef chT  channel_tag;
    typedef verT version_tag;
    static constexpr std::size_t num_channel = channel_tag::value;
    static constexpr std::size_t num_version = version_tag::value;

    typedef valueT                               value_type;
    typedef Header<version_tag>                  header_type;
    typedef Frame<value_type, contT>             frame_type;
    typedef typename frame_type::header_type     frame_header_type;
    typedef typename frame_type::frame_data_type frame_data_type;

    typedef contT container_dispatcher_type;
    typedef typename contT::template rebind<frame_type>::other
            frame_container_type;

    typedef std::array<frame_container_type, num_channel> channels_type;

    header_type   header;   //!< header information
    channels_type channels; //!< an array of (arrays of frames)

    // normally,
    // frame_container_type := std::vector<std::array<frame_type, num_channel>>
};

template<typename valueT, typename chT, typename verT, typename contT>
constexpr std::size_t Data<valueT, chT, verT, contT>::num_channel;
template<typename valueT, typename chT, typename verT, typename contT>
constexpr std::size_t Data<valueT, chT, verT, contT>::num_version;

//! .asd data class with only 1 channel.
/*!
 * class Data contains header, frame-header, frame-data informations from asd files.
 * @tparam valueT a value type corresponds to each pixel.
 * @tparam verT   a class having the version information (usually, asd::version<1>).
 * @tparam contT  a container-dispatcher that defines container class to be used (usually, container::vec).
 * */
template<typename valueT, typename verT, typename contT>
struct Data<valueT, channel<1>, verT, contT>
{
    typedef channel<1> channel_tag;
    typedef verT       version_tag;
    static constexpr std::size_t num_channel = channel_tag::value;
    static constexpr std::size_t num_version = version_tag::value;

    typedef valueT                               value_type;
    typedef Header<version_tag>                  header_type;
    typedef Frame<value_type, contT>             frame_type;
    typedef typename frame_type::header_type     frame_header_type;
    typedef typename frame_type::frame_data_type frame_data_type;

    typedef contT container_dispatcher_type;
    typedef typename contT::template rebind<frame_type>::other
            frame_container_type;

    header_type          header; //!< header information
    frame_container_type frames; //!< an array of frames.

    // normally,
    // frame_container_type := std::vector<frame_type>
};

template<typename valueT, typename verT, typename contT>
constexpr std::size_t Data<valueT, channel<1>, verT, contT>::num_channel;
template<typename valueT, typename verT, typename contT>
constexpr std::size_t Data<valueT, channel<1>, verT, contT>::num_version;

} // asd
#endif//LIBASD_READ_DATA_H
