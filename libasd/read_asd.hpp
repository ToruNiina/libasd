#ifndef LIBASD_READ_ASD_DATA_H
#define LIBASD_READ_ASD_DATA_H
#include <libasd/stream_checkpoint.hpp>
#include <libasd/read_header.hpp>
#include <libasd/read_frame.hpp>
#include <libasd/data.hpp>
#include <libasd/convert.hpp>
#include <libasd/read_raw_data.hpp>

namespace asd
{

namespace detail
{

// for N-channel files, read N pair of frame-header and frame-data.
template<typename realT, typename channelT, typename versionT, typename contT>
struct read_asd_impl
{
    template<typename sourceT>
    static Data<realT, channelT, versionT, contT> invoke(sourceT& src)
    {
        Data<realT, channelT, versionT, contT> data;

        detail::read_header_impl(data.header, src);

        const std::size_t n = data.header.num_frames;
        const std::size_t x = data.header.x_pixel;
        const std::size_t y = data.header.y_pixel;

        const std::size_t frame_header_size = data.header.frame_header_size;
        for(std::size_t ch = 0; ch < channelT::value; ++ch)
        {
            auto& frames = data.channels[ch];
            container::resize(frames, n);
            for(std::size_t i=0; i<n; ++i)
            {
                auto& frame = frames[i];

                const auto frame_starting_point = checkpoint(src);
                read_frame_header_impl(frame.header, src);
                // force skip even if something is broken
                restart_from(src, frame_starting_point);
                ignore_bytes(src, frame_header_size);

                FrameData<std::int16_t, contT> raw_data;
                read_frame_data_impl(raw_data, src, x, y);

                frame.data = convert_data<realT>(raw_data, data.header, ch);
            }
        }
        return data;
    }
};

template<typename realT, typename versionT, typename contT>
struct read_asd_impl<realT, channel<1>, versionT, contT>
{
    template<typename sourceT>
    static Data<realT, channel<1>, versionT, contT> invoke(sourceT& src)
    {
        Data<realT, channel<1>, versionT, contT> data;

        detail::read_header_impl(data.header, src);

        const std::size_t n = data.header.num_frames;
        const std::size_t x = data.header.x_pixel;
        const std::size_t y = data.header.y_pixel;

        container::resize(data.frames, n);
        const std::size_t frame_header_size = data.header.frame_header_size;
        for(std::size_t i=0; i<n; ++i)
        {
            auto& frame = data.frames[i];

            const auto frame_starting_point = checkpoint(src);
            read_frame_header_impl(frame.header, src);
            // force skip even if something is broken
            restart_from(src, frame_starting_point);
            ignore_bytes(src, frame_header_size);

            FrameData<std::int16_t, contT> raw_data;
            read_frame_data_impl(raw_data, src, x, y);

            frame.data = convert_data<realT>(raw_data, data.header, 0);
        }
        return data;
    }
};

}// detail

template<typename realT, typename chT = channel<1>, typename verT = version<1>,
         typename contT = container::vec, typename sourceT = std::istream>
Data<realT, chT, verT, contT> read_asd(sourceT& src)
{
    return detail::read_asd_impl<realT, chT, verT, contT>::invoke(src);
}

} // asd
#endif// LIBASD_READ_ASD_DATA_H
