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


//! .asd data class.
/*!
 * class Data contains header, frame-header, frame-data informations from asd files.
 * @tparam dataT a value type corresponds to each pixel
 * @tparam chT   a class having the number of channel
 * @tparam verT  a class having the version information
 * @tparam contT a container-dispatcher that defines container class to be used
 * */
template<typename dataT,
         typename chT   = channel<1>,
         typename verT  = version<1>,
         typename contT = container::vec>
struct Data
{
    typedef chT  channel_tag;
    typedef verT version_tag;
    static constexpr std::size_t num_channel = channel_tag::value;
    static constexpr std::size_t num_version = version_tag::value;

    typedef dataT data_type;
    typedef Header<version_tag> header_type;
    typedef Frame<dataT, channel_tag, contT> frame_type;
    typedef typename frame_type::header_type        frame_header_type;
    typedef typename frame_type::frame_data_type    frame_data_type;

    typedef contT container_dispatcher_type;
    typedef typename contT::template rebind<frame_type>::other
            frame_container_type;

    header_type          header; //!< header information
    frame_container_type frames; //!< an array of frame
};

}
#endif//LIBASD_READ_DATA_H
