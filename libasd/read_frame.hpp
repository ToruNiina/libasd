#ifndef LIBASD_READ_FRAME_H
#define LIBASD_READ_FRAME_H
#include <libasd/read_binary_as.hpp>
#include <libasd/frame_header.hpp>
#include <libasd/frame_data.hpp>
#include <libasd/frame.hpp>

namespace asd
{
namespace detail
{

template<typename sourceT>
void read_frame_header_impl(FrameHeader& fh, sourceT& source)
{
    fh.number        = read_binary_as<std::int32_t>(source);
    fh.max_data      = read_binary_as<std::int16_t>(source);
    fh.min_data      = read_binary_as<std::int16_t>(source);
    fh.x_offset      = read_binary_as<std::int16_t>(source);
    fh.y_offset      = read_binary_as<std::int16_t>(source);
    fh.x_tilt        = read_binary_as<float       >(source);
    fh.y_tilt        = read_binary_as<float       >(source);
    fh.is_stimulated = read_binary_as<bool        >(source);
    fh._booked1      = read_binary_as<std::int8_t >(source);
    fh._booked2      = read_binary_as<std::int16_t>(source);
    fh._booked3      = read_binary_as<std::int32_t>(source);
    fh._booked4      = read_binary_as<std::int32_t>(source);
    return;
}

template<typename contT>
void read_frame_data_impl(FrameData<std::int16_t, contT>& fd, const char*& ptr,
                          const std::size_t x, const std::size_t y)
{
    fd.reset(x, y);
    const std::size_t total_size = x * y;
    const std::int16_t* init = reinterpret_cast<const std::int16_t*>(ptr);
    const std::int16_t* last = reinterpret_cast<const std::int16_t*>(ptr);
    last += total_size;

    std::copy(init, last, fd.raw_begin());

    ptr += total_size;
    return;
}

template<typename contT>
void read_frame_data_impl(FrameData<std::int16_t, contT>& fd, std::istream& is,
                          const std::size_t x, const std::size_t y)
{
    fd.reset(x, y);
    fd.base() = read_binary_as<std::int16_t, contT>(is, x * y);
    return;
}
} // detail

template<typename sourceT>
FrameHeader read_frame_header(sourceT& is)
{
    FrameHeader fh;
    detail::read_frame_header_impl(fh, is);
    return fh;
}

template<typename contT = container::vec, typename sourceT>
FrameData<std::int16_t, contT>
read_frame_data(sourceT& is, std::size_t x, std::size_t y)
{
    FrameData<std::int16_t, contT> fd;
    detail::read_frame_data_impl<contT>(fd, is, x, y);
    return fd;
}

template<typename contT = container::vec, typename sourceT>
Frame<std::int16_t, contT>
read_frame(sourceT& source, std::size_t x, std::size_t y)
{
    Frame<std::int16_t, contT> f;
    detail::read_frame_header_impl(f.header, source);
    detail::read_frame_data_impl(f.data, source, x, y);
    return f;

}

}// asd
#endif// LIBASD_READ_FRAME_H
