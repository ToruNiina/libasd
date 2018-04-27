#ifndef LIBASD_WRITE_FRAME_H
#define LIBASD_WRITE_FRAME_H
#include <libasd/container_traits.hpp>
#include <libasd/write_as_binary.hpp>
#include <libasd/frame_header.hpp>
#include <libasd/frame_data.hpp>
#include <libasd/frame.hpp>

namespace asd
{
namespace detail
{

template<typename sourceT>
void write_frame_header_impl(sourceT& source, const FrameHeader& fh)
{
    write_as_binary<std::int32_t>(source, fh.number       );
    write_as_binary<std::int16_t>(source, fh.max_data     );
    write_as_binary<std::int16_t>(source, fh.min_data     );
    write_as_binary<std::int16_t>(source, fh.x_offset     );
    write_as_binary<std::int16_t>(source, fh.y_offset     );
    write_as_binary<float       >(source, fh.x_tilt       );
    write_as_binary<float       >(source, fh.y_tilt       );
    write_as_binary<bool        >(source, fh.is_stimulated);
    write_as_binary<std::int8_t >(source, fh._booked1     );
    write_as_binary<std::int16_t>(source, fh._booked2     );
    write_as_binary<std::int32_t>(source, fh._booked3     );
    write_as_binary<std::int32_t>(source, fh._booked4     );
    return;
}

template<typename sourceT, typename contT>
void write_frame_data_impl(sourceT& source, const FrameData<contT>& fd)
{
    write_as_binary(source, fd.data, typename container_traits<
            typename FrameData<contT>::container_type>::ptr_accessibility{});
    return;
}

template<typename channelT, typename contT>
struct write_frame_impl
{
    template<typename sourceT>
    static void invoke(sourceT& source, const Frame<channelT, contT>& frm)
    {
        write_frame_header_impl(source, frm.header);
        for(std::size_t i=0; i<frm.data.size(); ++i) // for each channel
        {
            write_frame_data_impl(source, frm.data[i]);
        }
        return;
    }
};

template<typename contT>
struct write_frame_impl<channel<1>, contT>
{
    template<typename sourceT>
    static void invoke(sourceT& source, const Frame<channel<1>, contT>& frm)
    {
        write_frame_header_impl(source, frm.header);
        write_frame_data_impl(source, frm.data);
        return;
    }
};
} // detail

inline char*& write_frame_header(char*& ptr, const FrameHeader& fh)
{
    detail::write_frame_header_impl(ptr, fh);
    return ptr;
}
inline std::ostream& write_frame_header(std::ostream& os, const FrameHeader& fh)
{
    detail::write_frame_header_impl(os, fh);
    return os;
}

template<typename contT = container::vec>
char*& write_frame_data(char*& ptr, const FrameData<contT>& fd)
{
    detail::write_frame_data_impl<contT>(ptr, fd);
    return ptr;
}
template<typename contT = container::vec>
std::ostream&
write_frame_data(std::ostream& os, const FrameData<contT>& fd)
{
    detail::write_frame_data_impl<contT>(os, fd);
    return os;
}

template<typename channelT, typename contT = container::vec>
char*& write_frame(char*& ptr, const Frame<channelT, contT>& frm)
{
    detail::write_frame_impl<channelT, contT>::invoke(ptr, frm);
    return ptr;
}
template<typename channelT, typename contT = container::vec>
std::ostream& write_frame(std::ostream& os, const Frame<channelT, contT>& frm)
{
    detail::write_frame_impl<channelT, contT>::invoke(os, frm);
    return os;
}

}// asd
#endif// LIBASD_WRITE_FRAME_H
