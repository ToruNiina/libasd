#ifndef LIBASD_READ_HEADER_H
#define LIBASD_READ_HEADER_H
#include <libasd/read_binary_as.hpp>
#include <libasd/stream_checkpoint.hpp>
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
    const auto starting_point = checkpoint(source);

    header.file_version        = read_binary_as<decltype(header.file_version       )>(source);
    header.data_type_1ch       = static_cast<decltype(header.data_type_1ch)>(read_binary_as<std::int16_t>(source));
    header.data_type_2ch       = static_cast<decltype(header.data_type_2ch)>(read_binary_as<std::int16_t>(source));
    header.file_header_size    = read_binary_as<decltype(header.file_header_size   )>(source);
    header.frame_header_size   = read_binary_as<decltype(header.frame_header_size  )>(source);
    header.operator_name_size  = read_binary_as<decltype(header.operator_name_size )>(source);
    header.comment_offset_size = read_binary_as<decltype(header.comment_offset_size)>(source);
    header.comment_size        = read_binary_as<decltype(header.comment_size       )>(source);
    header.x_pixel             = read_binary_as<decltype(header.x_pixel            )>(source);
    header.y_pixel             = read_binary_as<decltype(header.y_pixel            )>(source);
    header.x_scanning_range    = read_binary_as<decltype(header.x_scanning_range   )>(source);
    header.y_scanning_range    = read_binary_as<decltype(header.y_scanning_range   )>(source);
    header.frame_rate          = read_binary_as<decltype(header.frame_rate         )>(source);
    header.z_piezo_extension   = read_binary_as<decltype(header.z_piezo_extension  )>(source);
    header.z_piezo_gain        = read_binary_as<decltype(header.z_piezo_gain       )>(source);
    header.ad_range            = read_binary_as<decltype(header.ad_range           )>(source);
    header.bits_data           = read_binary_as<decltype(header.bits_data          )>(source);
    header.is_averaged         = read_binary_as<decltype(header.is_averaged        )>(source);
    header.average_window      = read_binary_as<decltype(header.average_window     )>(source);
    header._legacy             = read_binary_as<decltype(header._legacy            )>(source);
    header.year                = read_binary_as<decltype(header.year               )>(source);
    header.month               = read_binary_as<decltype(header.month              )>(source);
    header.day                 = read_binary_as<decltype(header.day                )>(source);
    header.hour                = read_binary_as<decltype(header.hour               )>(source);
    header.minute              = read_binary_as<decltype(header.minute             )>(source);
    header.second              = read_binary_as<decltype(header.second             )>(source);
    header.rounding_degree     = read_binary_as<decltype(header.rounding_degree    )>(source);
    header.x_max_range         = read_binary_as<decltype(header.x_max_range        )>(source);
    header.y_max_range         = read_binary_as<decltype(header.y_max_range        )>(source);
    header._booked1            = read_binary_as<decltype(header._booked1           )>(source);
    header._booked2            = read_binary_as<decltype(header._booked2           )>(source);
    header._booked3            = read_binary_as<decltype(header._booked3           )>(source);
    header.init_frame          = read_binary_as<decltype(header.init_frame         )>(source);
    header.num_frames          = read_binary_as<decltype(header.num_frames         )>(source);
    header.machine_id          = read_binary_as<decltype(header.machine_id         )>(source);
    header.file_id             = read_binary_as<decltype(header.file_id            )>(source);

    const std::size_t op_nm_sz =
        static_cast<std::size_t>(header.operator_name_size);

    ::asd::container::resize(header.operator_name, op_nm_sz);
    for(std::size_t i=0; i<op_nm_sz; ++i)
    {
        header.operator_name[i] = read_binary_as<std::int8_t>(source);
    }

    header.sensor_sensitivity  = read_binary_as<decltype(header.sensor_sensitivity)>(source);
    header.phase_sensitivity   = read_binary_as<decltype(header.phase_sensitivity )>(source);
    header.scannig_direction   = read_binary_as<decltype(header.scannig_direction )>(source);

    // comment offset is not included in the comment size
    ignore_bytes(source, static_cast<std::size_t>(header.comment_offset_size));

    const std::size_t cm_sz = static_cast<std::size_t>(header.comment_size);
    ::asd::container::resize(header.comment, cm_sz);
    for(std::size_t i=0; i<cm_sz; ++i)
    {
        header.comment[i] = read_binary_as<std::int8_t>(source);
    }

    // sometimes version 0 file contains incorrect file_header_size, so here
    // we don't rely on the `file_header_size`

//     // force read even if file header or spec is broken
//     restart_from(source, starting_point);
//     ignore_bytes(source, static_cast<std::size_t>(header.file_header_size));

    return source;
}

template<typename sourceT>
sourceT&
read_header_impl(Header<version<1>>& header, sourceT& source)
{
    const auto starting_point = checkpoint(source);

    header.file_version         = read_binary_as<decltype(header.file_version        )>(source);
    header.file_header_size     = read_binary_as<decltype(header.file_header_size    )>(source);
    header.frame_header_size    = read_binary_as<decltype(header.frame_header_size   )>(source);
    header.text_encoding        = read_binary_as<decltype(header.text_encoding       )>(source);
    header.operator_name_size   = read_binary_as<decltype(header.operator_name_size  )>(source);
    header.comment_size         = read_binary_as<decltype(header.comment_size        )>(source);
    header.data_kind_1ch        = read_binary_as<decltype(header.data_kind_1ch       )>(source);
    header.data_kind_2ch        = read_binary_as<decltype(header.data_kind_2ch       )>(source);
    header.init_frame           = read_binary_as<decltype(header.init_frame          )>(source);
    header.num_frames           = read_binary_as<decltype(header.num_frames          )>(source);
    header.scanning_direction   = read_binary_as<decltype(header.scanning_direction  )>(source);
    header.file_id              = read_binary_as<decltype(header.file_id             )>(source);
    header.x_pixel              = read_binary_as<decltype(header.x_pixel             )>(source);
    header.y_pixel              = read_binary_as<decltype(header.y_pixel             )>(source);
    header.x_scanning_range     = read_binary_as<decltype(header.x_scanning_range    )>(source);
    header.y_scanning_range     = read_binary_as<decltype(header.y_scanning_range    )>(source);
    header.is_averaged          = read_binary_as<decltype(header.is_averaged         )>(source);
    header.average_window       = read_binary_as<decltype(header.average_window      )>(source);
    header.year                 = read_binary_as<decltype(header.year                )>(source);
    header.month                = read_binary_as<decltype(header.month               )>(source);
    header.day                  = read_binary_as<decltype(header.day                 )>(source);
    header.hour                 = read_binary_as<decltype(header.hour                )>(source);
    header.minute               = read_binary_as<decltype(header.minute              )>(source);
    header.second               = read_binary_as<decltype(header.second              )>(source);
    header.x_rounding_degree    = read_binary_as<decltype(header.x_rounding_degree   )>(source);
    header.y_rounding_degree    = read_binary_as<decltype(header.y_rounding_degree   )>(source);
    header.frame_acquision_time = read_binary_as<decltype(header.frame_acquision_time)>(source);
    header.sensor_sensitivity   = read_binary_as<decltype(header.sensor_sensitivity  )>(source);
    header.phase_sensitivity    = read_binary_as<decltype(header.phase_sensitivity   )>(source);
    header.offset               = read_binary_as<decltype(header.offset              )>(source);
    ignore_bytes(source, 12); // booked region
    header.machine_id           = read_binary_as<decltype(header.machine_id          )>(source);
    header.ad_range             = read_binary_as<decltype(header.ad_range            )>(source);
    header.ad_resolution        = read_binary_as<decltype(header.ad_resolution       )>(source);
    header.x_max_scanning_range = read_binary_as<decltype(header.x_max_scanning_range)>(source);
    header.y_max_scanning_range = read_binary_as<decltype(header.y_max_scanning_range)>(source);
    header.x_piezo_extension    = read_binary_as<decltype(header.x_piezo_extension   )>(source);
    header.y_piezo_extension    = read_binary_as<decltype(header.y_piezo_extension   )>(source);
    header.z_piezo_extension    = read_binary_as<decltype(header.z_piezo_extension   )>(source);
    header.z_piezo_gain         = read_binary_as<decltype(header.z_piezo_gain        )>(source);

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

    // force read even if file header or spec is broken
    restart_from(source, starting_point);
    ignore_bytes(source, static_cast<std::size_t>(header.file_header_size));

    return source;
}

template<typename sourceT>
sourceT&
read_header_impl(Header<version<2>>& header, sourceT& source)
{
    const auto starting_point = checkpoint(source);

    header.file_version         = read_binary_as<decltype(header.file_version        )>(source);
    header.file_header_size     = read_binary_as<decltype(header.file_header_size    )>(source);
    header.frame_header_size    = read_binary_as<decltype(header.frame_header_size   )>(source);
    header.text_encoding        = read_binary_as<decltype(header.text_encoding       )>(source);
    header.operator_name_size   = read_binary_as<decltype(header.operator_name_size  )>(source);
    header.comment_size         = read_binary_as<decltype(header.comment_size        )>(source);
    header.data_kind_1ch        = read_binary_as<decltype(header.data_kind_1ch       )>(source);
    header.data_kind_2ch        = read_binary_as<decltype(header.data_kind_2ch       )>(source);
    header.init_frame           = read_binary_as<decltype(header.init_frame          )>(source);
    header.num_frames           = read_binary_as<decltype(header.num_frames          )>(source);
    header.scanning_direction   = read_binary_as<decltype(header.scanning_direction  )>(source);
    header.file_id              = read_binary_as<decltype(header.file_id             )>(source);
    header.x_pixel              = read_binary_as<decltype(header.x_pixel             )>(source);
    header.y_pixel              = read_binary_as<decltype(header.y_pixel             )>(source);
    header.x_scanning_range     = read_binary_as<decltype(header.x_scanning_range    )>(source);
    header.y_scanning_range     = read_binary_as<decltype(header.y_scanning_range    )>(source);
    header.is_averaged          = read_binary_as<decltype(header.is_averaged         )>(source);
    header.average_window       = read_binary_as<decltype(header.average_window      )>(source);
    header.year                 = read_binary_as<decltype(header.year                )>(source);
    header.month                = read_binary_as<decltype(header.month               )>(source);
    header.day                  = read_binary_as<decltype(header.day                 )>(source);
    header.hour                 = read_binary_as<decltype(header.hour                )>(source);
    header.minute               = read_binary_as<decltype(header.minute              )>(source);
    header.second               = read_binary_as<decltype(header.second              )>(source);
    header.x_rounding_degree    = read_binary_as<decltype(header.x_rounding_degree   )>(source);
    header.y_rounding_degree    = read_binary_as<decltype(header.y_rounding_degree   )>(source);
    header.frame_acquision_time = read_binary_as<decltype(header.frame_acquision_time)>(source);
    header.sensor_sensitivity   = read_binary_as<decltype(header.sensor_sensitivity  )>(source);
    header.phase_sensitivity    = read_binary_as<decltype(header.phase_sensitivity   )>(source);
    header.offset               = read_binary_as<decltype(header.offset              )>(source);
    ignore_bytes(source, 12);
    header.machine_id           = read_binary_as<decltype(header.machine_id          )>(source);
    header.ad_range             = read_binary_as<decltype(header.ad_range            )>(source);
    header.ad_resolution        = read_binary_as<decltype(header.ad_resolution       )>(source);
    header.x_max_scanning_range = read_binary_as<decltype(header.x_max_scanning_range)>(source);
    header.y_max_scanning_range = read_binary_as<decltype(header.y_max_scanning_range)>(source);
    header.x_piezo_extension    = read_binary_as<decltype(header.x_piezo_extension   )>(source);
    header.y_piezo_extension    = read_binary_as<decltype(header.y_piezo_extension   )>(source);
    header.z_piezo_extension    = read_binary_as<decltype(header.z_piezo_extension   )>(source);
    header.z_piezo_gain         = read_binary_as<decltype(header.z_piezo_gain        )>(source);

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

    header.number_of_frames        = read_binary_as<decltype(header.number_of_frames       )>(source);
    header.is_x_feed_forward       = read_binary_as<decltype(header.is_x_feed_forward      )>(source);
    header.x_feed_forward_i        = read_binary_as<decltype(header.x_feed_forward_i       )>(source);
    header.x_feed_forward_d        = read_binary_as<decltype(header.x_feed_forward_d       )>(source);
    header.max_color_scale         = read_binary_as<decltype(header.max_color_scale        )>(source);
    header.min_color_scale         = read_binary_as<decltype(header.min_color_scale        )>(source);
    header.anchor_point_size_red   = read_binary_as<decltype(header.anchor_point_size_red  )>(source);
    header.anchor_point_size_green = read_binary_as<decltype(header.anchor_point_size_green)>(source);
    header.anchor_point_size_blue  = read_binary_as<decltype(header.anchor_point_size_blue )>(source);

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

    // force read even if file header or spec is broken
    restart_from(source, starting_point);
    ignore_bytes(source, static_cast<std::size_t>(header.file_header_size));

    return source;
}

} // detail

// XXX it does not change the position of the ptr!
inline std::int32_t read_version(const char* ptr)
{
    return detail::read_binary_as<std::int32_t>(ptr);
}
// XXX it does not change the position of the is!
inline std::int32_t read_version(std::istream& is)
{
    const auto pos = is.tellg();
    const auto v   = detail::read_binary_as<std::int32_t>(is);
    is.seekg(pos);
    return v;
}

template<typename verT = version<1>>
Header<verT> read_header(const char*& ptr)
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
