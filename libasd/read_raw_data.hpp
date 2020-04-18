#ifndef LIBASD_READ_RAW_DATA_H
#define LIBASD_READ_RAW_DATA_H
#include <libasd/read_header.hpp>
#include <libasd/read_frame.hpp>
#include <libasd/data.hpp>

namespace asd
{
namespace detail
{
template<typename chT, typename verT, typename contT>
struct read_raw_frames_impl
{
    template<typename sourceT>
    static Data<std::int16_t, chT, verT, contT> invoke(sourceT& src)
    {
        Data<std::int16_t, chT, verT, contT> data;

        detail::read_header_impl(data.header, src);
        const std::size_t x = data.header.x_pixel;
        const std::size_t y = data.header.y_pixel;
        const std::size_t n = data.header.num_frames;

        for(auto& frames : data.channels)
        {
            container::resize(frames, n);
            for(std::size_t i=0; i<n; ++i)
            {
                frames[i] = read_frame<contT>(src, x, y);
            }
        }
        return data;
    }
};
template<typename verT, typename contT>
struct read_raw_frames_impl<channel<1>, verT, contT>
{
    template<typename sourceT>
    static Data<std::int16_t, channel<1>, verT, contT> invoke(sourceT& src)
    {
        Data<std::int16_t, channel<1>, verT, contT> data;

        detail::read_header_impl(data.header, src);
        const std::size_t x = data.header.x_pixel;
        const std::size_t y = data.header.y_pixel;
        const std::size_t n = data.header.num_frames;

        container::resize(data.frames, n);
        for(std::size_t i=0; i<n; ++i)
        {
            data.frames[i] = read_frame<contT>(src, x, y);
        }
        return data;
    }
};
} // detail

template<typename chT = channel<1>, typename verT = version<1>,
         typename contT = container::vec, typename sourceT>
Data<std::int16_t, chT, verT, contT> read_raw_data(sourceT& src)
{
    return detail::read_raw_frames_impl<chT, verT, contT>::invoke(src);
}

} // asd
#endif// LIBASD_READ_RAW_DATA_H
