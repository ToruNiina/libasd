#ifndef LIBASD_READ_DATA_H
#define LIBASD_READ_DATA_H

namespace asd
{

template<typename chT   = channel<1>,
         typename verT  = version<1>,
         typename contT = container::vec>
struct Data
{
    typedef chT  channel_tag;
    typedef verT version_tag;
    static constexpr std::size_t num_channel = channel_tag::value;
    static constexpr std::size_t num_version = version_tag::value;

    typedef Header<version_tag, contT> header_type;
    typedef Frame<channel_tag, contT>  frame_type;
    typedef typename frame_type::header_type     frame_header_type;
    typedef typename frame_type::frame_data_type frame_data_type;

    typedef contT container_dispatcher_type;
    typedef typename contT::template rebind<frame_type>::other frame_container_type;

    header_type header;
    frame_container_type frames;
};

}
#endif//LIBASD_READ_DATA_H
