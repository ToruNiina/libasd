#ifndef LIBASD_READ_ASD_DATA_H
#define LIBASD_READ_ASD_DATA_H
#include <libasd/read_raw_data.hpp>
#include <libasd/convert.hpp>

namespace asd
{

namespace detail
{

template<typename realT, typename channelT, typename versionT, typename contT>
struct read_and_convert_frame_impl
{
    template<typename sourceT>
    static Frame<realT, channelT, contT>
    invoke(sourceT& source, const Header<versionT>& hdr,
           const std::size_t x, const std::size_t y)
    {
        Frame<realT, channelT, contT> f;
        read_frame_header_impl(f.header, source);
        for(std::size_t i=0; i<f.data.size(); ++i) // for each channel
        {
            FrameData<std::int16_t, contT> fd;
            read_frame_data_impl(fd, source, x, y);
            f[i] = convert_data<realT>(fd, hdr, i);
        }
        return f;
    }
};

template<typename realT, typename versionT, typename contT>
struct read_and_convert_frame_impl<realT, channel<1>, versionT, contT>
{
    template<typename sourceT>
    static Frame<realT, channel<1>, contT>
    invoke(sourceT& source, const Header<versionT>& hdr,
           const std::size_t x, const std::size_t y)
    {
        Frame<realT, channel<1>, contT> f;
        read_frame_header_impl(f.header, source);
        FrameData<std::int16_t, contT> fd;
        read_frame_data_impl(fd, source, x, y);
        f.data = convert_data<realT>(fd, hdr, 0);
        return f;
    }
};

}// detail

template<typename realT,
         typename chT = channel<1>, typename verT = version<1>,
         typename contT = container::vec>
Data<realT, chT, verT, contT> read_asd(const char*& ptr)
{
    typedef Data<realT, chT, verT, contT> data_type;
    data_type data;
    detail::read_header_impl(data.header, ptr);
    const std::size_t n = data.header.num_frames;
    container::resize(data.frames, n);

    const std::size_t x = data.header.x_pixel;
    const std::size_t y = data.header.y_pixel;
    for(std::size_t i=0; i<n; ++i)
    {
        data.frames[i] = detail::read_and_convert_frame_impl<
            realT, chT, verT, contT>::invoke(ptr, data.header, x, y);
    }
    return data;
}

template<typename realT,
         typename chT = channel<1>, typename verT = version<1>,
         typename contT = container::vec>
Data<realT, chT, verT, contT> read_asd(std::istream& is)
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
            realT, chT, verT, contT>::invoke(is, data.header, x, y);
    }
    return data;
}

} // asd
#endif// LIBASD_READ_ASD_DATA_H
