#ifndef LIBASD_READ_ASD_DATA_H
#define LIBASD_READ_ASD_DATA_H
#include <libasd/read_raw_data.hpp>
#include <libasd/convert.hpp>

namespace asd
{

namespace detail
{

// for N-channel files, read N pair of frame-header and frame-data.
template<typename realT, typename versionT, typename channelT, typename contT>
struct read_and_convert_frame_impl
{
    template<typename sourceT>
    static std::array<Frame<realT, contT>, channelT::value>
    invoke(sourceT& source, const Header<versionT>& hdr,
           const std::size_t x, const std::size_t y)
    {
        std::array<Frame<realT, contT>, channelT::value> channels;

        for(auto& frame : channels)
        {
            read_frame_header_impl(frame.header, source);

            FrameData<std::int16_t, contT> raw_data;
            read_frame_data_impl(raw_data, source, x, y);

            frame.data = convert_data<realT>(raw_data, hdr, 0);
        }
        return channels;
    }
};

template<typename realT, typename versionT, typename contT>
struct read_and_convert_frame_impl<realT, versionT, channel<1>, contT>
{
    template<typename sourceT>
    static Frame<realT, contT>
    invoke(sourceT& source, const Header<versionT>& hdr,
           const std::size_t x, const std::size_t y)
    {
        Frame<realT, contT> frame;
        read_frame_header_impl(frame.header, source);

        FrameData<std::int16_t, contT> raw_data;
        read_frame_data_impl(raw_data, source, x, y);

        frame.data = convert_data<realT>(raw_data, hdr, 0);
        return frame;
    }
};

}// detail

template<typename realT, typename chT = channel<1>, typename verT = version<1>,
         typename contT = container::vec, typename sourceT = std::istream>
Data<realT, chT, verT, contT> read_asd(sourceT& is)
{
    typedef Data<realT, chT, verT, contT> data_type;
    data_type data;

    detail::read_header_impl(data.header, is);

    const std::size_t n = data.header.num_frames;
    container::resize(data.frames, n);

    const std::size_t x = data.header.x_pixel;
    const std::size_t y = data.header.y_pixel;
    for(std::size_t i=0; i<n; ++i)
    {
        data.frames[i] = detail::read_and_convert_frame_impl<
            realT, verT, chT, contT>::invoke(is, data.header, x, y);
    }
    return data;
}

} // asd
#endif// LIBASD_READ_ASD_DATA_H
