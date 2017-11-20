#ifndef LIBASD_READ_HEADER_H
#define LIBASD_READ_HEADER_H
#include <libasd/read_binary_as.hpp>
#include <libasd/header.hpp>
#include <istream>

namespace asd
{
namespace detail
{

template<typename sourceT>
sourceT&
read_header_impl(Header<version<0>>& header, sourceT& source)
{
    header.file_version        = read_binary_as<std::int32_t>(source);
    header.data_type_1ch       = read_binary_as<std::int16_t>(source);
    header.data_type_2ch       = read_binary_as<std::int16_t>(source);
    header.file_header_size    = read_binary_as<std::int32_t>(source);
    header.frame_header_size   = read_binary_as<std::int32_t>(source);
    header.operator_name_size  = read_binary_as<std::int32_t>(source);
    header.comment_offset_size = read_binary_as<std::int32_t>(source);
    header.comment_size        = read_binary_as<std::int32_t>(source);
    header.x_pixel             = read_binary_as<std::int32_t>(source);
    header.y_pixel             = read_binary_as<std::int32_t>(source);
    header.x_scanning_range    = read_binary_as<std::int32_t>(source);
    header.y_scanning_range    = read_binary_as<std::int32_t>(source);
    header.frame_rate          = read_binary_as<float       >(source);
    header.z_piezo_extension   = read_binary_as<float       >(source);
    header.z_piezo_gain        = read_binary_as<float       >(source);
    header.ad_range            = read_binary_as<std::int32_t>(source);
    header.bits_data           = read_binary_as<std::int32_t>(source);
    header.is_averaged         = read_binary_as<bool	    >(source);
    header.average_window      = read_binary_as<std::int32_t>(source);
    header._legacy             = read_binary_as<std::int32_t>(source);
    header.year                = read_binary_as<std::int16_t>(source);
    header.month               = read_binary_as<std::uint8_t>(source);
    header.day                 = read_binary_as<std::uint8_t>(source);
    header.hour                = read_binary_as<std::uint8_t>(source);
    header.minute              = read_binary_as<std::uint8_t>(source);
    header.second              = read_binary_as<std::uint8_t>(source);
    header.rounding_degree     = read_binary_as<std::int32_t>(source);
    header.x_max_range         = read_binary_as<float       >(source);
    header.y_max_range         = read_binary_as<float       >(source);
    header._booked1            = read_binary_as<std::int32_t>(source);
    header._booked2            = read_binary_as<std::int32_t>(source);
    header._booked3            = read_binary_as<std::int32_t>(source);
    header.init_frame          = read_binary_as<std::int32_t>(source);
    header.num_frames          = read_binary_as<std::int32_t>(source);
    header.machine_id          = read_binary_as<std::int32_t>(source);
    header.file_id             = read_binary_as<std::int32_t>(source);

    const std::size_t op_nm_sz =
        static_cast<std::size_t>(header.operator_name_size);

    ::asd::container::resize(header.operator_name, op_nm_sz);
    for(std::size_t i=0; i<op_nm_sz; ++i)
    {
        header.operator_name[i] = read_binary_as<std::int8_t>(source);
    }

    header.sensor_sensitivity  = read_binary_as<float       >(source);
    header.phase_sensitivity   = read_binary_as<float       >(source);
    header.scannig_direction   = read_binary_as<std::int32_t>(source);

    const std::size_t cm_sz = static_cast<std::size_t>(header.comment_size);
    ::asd::container::resize(header.comment, cm_sz);
    for(std::size_t i=0; i<cm_sz; ++i)
    {
        header.comment[i] = read_binary_as<std::int8_t>(source);
    }

    return source;
}

template<typename sourceT>
sourceT&
read_header_impl(Header<version<1>>& header, sourceT& source)
{
    header.file_version         = read_binary_as<std::int32_t>(source);
    header.file_header_size     = read_binary_as<std::int32_t>(source);
    header.frame_header_size    = read_binary_as<std::int32_t>(source);
    header.text_encoding        = read_binary_as<std::int32_t>(source);
    header.operator_name_size   = read_binary_as<std::int32_t>(source);
    header.comment_size         = read_binary_as<std::int32_t>(source);
    header.data_kind_1ch        = read_binary_as<std::int32_t>(source);
    header.data_kind_2ch        = read_binary_as<std::int32_t>(source);
    header.init_frame           = read_binary_as<std::int32_t>(source);
    header.num_frames           = read_binary_as<std::int32_t>(source);
    header.scanning_direction   = read_binary_as<std::int32_t>(source);
    header.file_id              = read_binary_as<std::int32_t>(source);
    header.x_pixel              = read_binary_as<std::int32_t>(source);
    header.y_pixel              = read_binary_as<std::int32_t>(source);
    header.x_scanning_range     = read_binary_as<std::int32_t>(source);
    header.y_scanning_range     = read_binary_as<std::int32_t>(source);
    header.is_averaged          = read_binary_as<bool        >(source);
    header.average_window       = read_binary_as<std::int32_t>(source);
    header.year                 = read_binary_as<std::int32_t>(source);
    header.month                = read_binary_as<std::int32_t>(source);
    header.day                  = read_binary_as<std::int32_t>(source);
    header.hour                 = read_binary_as<std::int32_t>(source);
    header.minute               = read_binary_as<std::int32_t>(source);
    header.second               = read_binary_as<std::int32_t>(source);
    header.x_rounding_degree    = read_binary_as<std::int32_t>(source);
    header.y_rounding_degree    = read_binary_as<std::int32_t>(source);
    header.frame_acquision_time = read_binary_as<float       >(source);
    header.sensor_sensitiviy    = read_binary_as<float       >(source);
    header.phase_sensitivity    = read_binary_as<float       >(source);
    header.offset               = read_binary_as<std::int32_t>(source);
    ignore_bytes(source, 12); // booked region
    header.machine_id           = read_binary_as<std::int32_t>(source);
    header.AD_range             = read_binary_as<std::int32_t>(source);
    header.AD_resolution        = read_binary_as<std::int32_t>(source);
    header.x_max_scanning_range = read_binary_as<float       >(source);
    header.y_max_scanning_range = read_binary_as<float       >(source);
    header.x_piezo_extension    = read_binary_as<float       >(source);
    header.y_piezo_extension    = read_binary_as<float       >(source);
    header.z_piezo_extension    = read_binary_as<float       >(source);
    header.z_piezo_gain         = read_binary_as<float       >(source);

    const auto op_name_size = static_cast<std::size_t>(header.operator_name_size);
    ::asd::container::resize(header.operator_name, op_name_size);
    for(std::size_t i=0; i<op_name_size; ++i)
    {
        header.operator_name[i] = read_binary_as<std::int8_t>(source);
    }

    const std::size_t cm_size = static_cast<std::size_t>(header.comment_size);
    ::asd::container::resize(header.comment, cm_size);
    for(std::size_t i=0; i<cm_size; ++i)
    {
        header.comment[i] = read_binary_as<std::int8_t>(source);
    }
    return source;
}

template<typename sourceT>
sourceT&
read_header_impl(Header<version<2>>& header, sourceT& source)
{
    header.file_version         = read_binary_as<std::int32_t>(source);
    header.file_header_size     = read_binary_as<std::int32_t>(source);
    header.frame_header_size    = read_binary_as<std::int32_t>(source);
    header.text_encoding        = read_binary_as<std::int32_t>(source);
    header.operator_name_size   = read_binary_as<std::int32_t>(source);
    header.comment_size         = read_binary_as<std::int32_t>(source);
    header.data_kind_1ch        = read_binary_as<std::int32_t>(source);
    header.data_kind_2ch        = read_binary_as<std::int32_t>(source);
    header.init_frame           = read_binary_as<std::int32_t>(source);
    header.num_frames           = read_binary_as<std::int32_t>(source);
    header.scanning_direction   = read_binary_as<std::int32_t>(source);
    header.file_id              = read_binary_as<std::int32_t>(source);
    header.x_pixel              = read_binary_as<std::int32_t>(source);
    header.y_pixel              = read_binary_as<std::int32_t>(source);
    header.x_scanning_range     = read_binary_as<std::int32_t>(source);
    header.y_scanning_range     = read_binary_as<std::int32_t>(source);
    header.is_averaged          = read_binary_as<bool        >(source);
    header.average_window       = read_binary_as<std::int32_t>(source);
    header.year                 = read_binary_as<std::int32_t>(source);
    header.month                = read_binary_as<std::int32_t>(source);
    header.day                  = read_binary_as<std::int32_t>(source);
    header.hour                 = read_binary_as<std::int32_t>(source);
    header.minute               = read_binary_as<std::int32_t>(source);
    header.second               = read_binary_as<std::int32_t>(source);
    header.x_rounding_degree    = read_binary_as<std::int32_t>(source);
    header.y_rounding_degree    = read_binary_as<std::int32_t>(source);
    header.frame_acquision_time = read_binary_as<float       >(source);
    header.sensor_sensitiviy    = read_binary_as<float       >(source);
    header.phase_sensitivity    = read_binary_as<float       >(source);
    header.offset               = read_binary_as<std::int32_t>(source);
    ignore_bytes(source, 12);
    header.machine_id           = read_binary_as<std::int32_t>(source);
    header.AD_range             = read_binary_as<std::int32_t>(source);
    header.AD_resolution        = read_binary_as<std::int32_t>(source);
    header.x_max_scanning_range = read_binary_as<float       >(source);
    header.y_max_scanning_range = read_binary_as<float       >(source);
    header.x_piezo_extension    = read_binary_as<float       >(source);
    header.y_piezo_extension    = read_binary_as<float       >(source);
    header.z_piezo_extension    = read_binary_as<float       >(source);
    header.z_piezo_gain         = read_binary_as<float       >(source);

    const std::size_t op_nm_sz =
        static_cast<std::size_t>(header.operator_name_size);
    ::asd::container::resize(header.operator_name, op_nm_sz);
    for(std::size_t i=0; i<op_nm_sz; ++i)
    {
        header.operator_name[i] = read_binary_as<std::int8_t>(source);
    }

    const std::size_t cm_size = static_cast<std::size_t>(header.comment_size);
    ::asd::container::resize(header.comment, cm_size);
    for(std::size_t i=0; i<cm_size; ++i)
    {
        header.comment[i] = read_binary_as<std::int8_t>(source);
    }

    header.number_of_frames        = read_binary_as<std::int32_t>(source);
    header.is_x_feed_forward       = read_binary_as<std::int32_t>(source);
    header.x_feed_forward_i        = read_binary_as<std::int32_t>(source);
    header.x_feed_forward_d        = read_binary_as<std::int32_t>(source);
    header.max_color_scale         = read_binary_as<std::int32_t>(source);
    header.min_color_scale         = read_binary_as<std::int32_t>(source);
    header.anchor_point_size_red   = read_binary_as<std::int32_t>(source);
    header.anchor_point_size_green = read_binary_as<std::int32_t>(source);
    header.anchor_point_size_blue  = read_binary_as<std::int32_t>(source);

    /* anchor_red */{
        const std::size_t sz =
            static_cast<std::size_t>(header.anchor_point_size_red);
        ::asd::container::resize(header.x_anchor_points_red, sz);
        ::asd::container::resize(header.y_anchor_points_red, sz);
        for(std::size_t i=0; i<sz; ++i)
        {
            header.x_anchor_points_red[i] = read_binary_as<std::int32_t>(source);
        }
        for(std::size_t i=0; i<sz; ++i)
        {
            header.y_anchor_points_red[i] = read_binary_as<std::int32_t>(source);
        }
    }
    /* anchor_green */{
        const std::size_t sz =
            static_cast<std::size_t>(header.anchor_point_size_green);
        ::asd::container::resize(header.x_anchor_points_green, sz);
        ::asd::container::resize(header.y_anchor_points_green, sz);
        for(std::size_t i=0; i<sz; ++i)
        {
            header.x_anchor_points_green[i] = read_binary_as<std::int32_t>(source);
        }
        for(std::size_t i=0; i<sz; ++i)
        {
            header.y_anchor_points_green[i] = read_binary_as<std::int32_t>(source);
        }
    }
    /* anchor_blue */{
        const std::size_t sz =
            static_cast<std::size_t>(header.anchor_point_size_blue);
        ::asd::container::resize(header.x_anchor_points_blue, sz);
        ::asd::container::resize(header.y_anchor_points_blue, sz);
        for(std::size_t i=0; i<sz; ++i)
        {
            header.x_anchor_points_blue[i] = read_binary_as<std::int32_t>(source);
        }
        for(std::size_t i=0; i<sz; ++i)
        {
            header.y_anchor_points_blue[i] = read_binary_as<std::int32_t>(source);
        }
    }
    return source;
}

} // detail

template<typename verT = version<1>>
Header<verT> read_header(const char* ptr)
{
    Header<verT> header;
    detail::read_header_impl(header, ptr);
    return header;
}

template<typename verT = version<1>>
Header<verT> read_header(std::istream& is)
{
    Header<verT> header;
    detail::read_header_impl(header, is);
    return header;
}

} // asd
#endif// LIBASD_READ_HEADER_H
