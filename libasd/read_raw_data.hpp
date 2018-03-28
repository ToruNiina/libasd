#ifndef LIBASD_READ_RAW_DATA_H
#define LIBASD_READ_RAW_DATA_H
#include <libasd/read_header.hpp>
#include <libasd/read_frame.hpp>
#include <libasd/data.hpp>

namespace asd
{

template<typename chT = channel<1>, typename verT = version<1>,
         typename contT = container::vec>
Data<std::int16_t, chT, verT, contT> read_raw_data(const char*& ptr)
{
    typedef Data<std::int16_t, chT, verT, contT> data_type;
    data_type data;
    detail::read_header_impl(data.header, ptr);
    const std::size_t x = data.header.x_pixel;
    const std::size_t y = data.header.y_pixel;
    const std::size_t n = data.header.num_frames;

    container::resize(data.frames, n);

    for(std::size_t i=0; i<n; ++i)
    {
        data.frames[i] = detail::read_frame_impl<chT, contT>::invoke(ptr, x, y);
    }
    return data;
}

template<typename chT = channel<1>, typename verT = version<1>,
         typename contT = container::vec>
Data<std::int16_t, chT, verT, contT> read_raw_data(std::istream& is)
{
    typedef Data<std::int16_t, chT, verT, contT> data_type;
    data_type data;
    detail::read_header_impl(data.header, is);
    const std::size_t x = data.header.x_pixel;
    const std::size_t y = data.header.y_pixel;
    const std::size_t n = data.header.num_frames;

    container::resize(data.frames, n);

    for(std::size_t i=0; i<n; ++i)
    {
        data.frames[i] = detail::read_frame_impl<chT, contT>::invoke(is, x, y);
    }
    return data;
}

} // asd
#endif// LIBASD_READ_RAW_DATA_H
