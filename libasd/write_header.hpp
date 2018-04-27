#ifndef LIBASD_WRITE_HEADER_H
#define LIBASD_WRITE_HEADER_H
#include <libasd/write_as_binary.hpp>
#include <libasd/header.hpp>
#include <ostream>

namespace asd
{
namespace detail
{

template<typename sourceT>
void write_header_impl(sourceT& source, const Header<version<0>>& header)
{
    write_as_binary<std::int32_t>(source, header.file_version       );
    write_as_binary<std::int16_t>(source, header.data_type_1ch      );
    write_as_binary<std::int16_t>(source, header.data_type_2ch      );
    write_as_binary<std::int32_t>(source, header.file_header_size   );
    write_as_binary<std::int32_t>(source, header.frame_header_size  );
    write_as_binary<std::int32_t>(source, header.operator_name_size );
    write_as_binary<std::int32_t>(source, header.comment_offset_size);
    write_as_binary<std::int32_t>(source, header.comment_size       );
    write_as_binary<std::int32_t>(source, header.x_pixel            );
    write_as_binary<std::int32_t>(source, header.y_pixel            );
    write_as_binary<std::int32_t>(source, header.x_scanning_range   );
    write_as_binary<std::int32_t>(source, header.y_scanning_range   );
    write_as_binary<float       >(source, header.frame_rate         );
    write_as_binary<float       >(source, header.z_piezo_extension  );
    write_as_binary<float       >(source, header.z_piezo_gain       );
    write_as_binary<std::int32_t>(source, header.ad_range           );
    write_as_binary<std::int32_t>(source, header.bits_data          );
    write_as_binary<bool        >(source, header.is_averaged        );
    write_as_binary<std::int32_t>(source, header.average_window     );
    write_as_binary<std::int32_t>(source, header._legacy            );
    write_as_binary<std::int16_t>(source, header.year               );
    write_as_binary<std::uint8_t>(source, header.month              );
    write_as_binary<std::uint8_t>(source, header.day                );
    write_as_binary<std::uint8_t>(source, header.hour               );
    write_as_binary<std::uint8_t>(source, header.minute             );
    write_as_binary<std::uint8_t>(source, header.second             );
    write_as_binary<std::int32_t>(source, header.rounding_degree    );
    write_as_binary<float       >(source, header.x_max_range        );
    write_as_binary<float       >(source, header.y_max_range        );
    write_as_binary<std::int32_t>(source, header._booked1           );
    write_as_binary<std::int32_t>(source, header._booked2           );
    write_as_binary<std::int32_t>(source, header._booked3           );
    write_as_binary<std::int32_t>(source, header.init_frame         );
    write_as_binary<std::int32_t>(source, header.num_frames         );
    write_as_binary<std::int32_t>(source, header.machine_id         );
    write_as_binary<std::int32_t>(source, header.file_id            );

    write_as_binary(source, header.operator_name, std::true_type{});

    write_as_binary<float       >(source, header.sensor_sensitivity);
    write_as_binary<float       >(source, header.phase_sensitivity);
    write_as_binary<std::int32_t>(source, header.scannig_direction);

    write_as_binary(source, header.comment, std::true_type{});
    return;
}

template<typename sourceT>
void write_header_impl(sourceT& source, const Header<version<1>>& header)
{
    write_as_binary<std::int32_t>(source, header.file_version         );
    write_as_binary<std::int32_t>(source, header.file_header_size     );
    write_as_binary<std::int32_t>(source, header.frame_header_size    );
    write_as_binary<std::int32_t>(source, header.text_encoding        );
    write_as_binary<std::int32_t>(source, header.operator_name_size   );
    write_as_binary<std::int32_t>(source, header.comment_size         );
    write_as_binary<std::int32_t>(source, header.data_kind_1ch        );
    write_as_binary<std::int32_t>(source, header.data_kind_2ch        );
    write_as_binary<std::int32_t>(source, header.init_frame           );
    write_as_binary<std::int32_t>(source, header.num_frames           );
    write_as_binary<std::int32_t>(source, header.scanning_direction   );
    write_as_binary<std::int32_t>(source, header.file_id              );
    write_as_binary<std::int32_t>(source, header.x_pixel              );
    write_as_binary<std::int32_t>(source, header.y_pixel              );
    write_as_binary<std::int32_t>(source, header.x_scanning_range     );
    write_as_binary<std::int32_t>(source, header.y_scanning_range     );
    write_as_binary<bool        >(source, header.is_averaged          );
    write_as_binary<std::int32_t>(source, header.average_window       );
    write_as_binary<std::int32_t>(source, header.year                 );
    write_as_binary<std::int32_t>(source, header.month                );
    write_as_binary<std::int32_t>(source, header.day                  );
    write_as_binary<std::int32_t>(source, header.hour                 );
    write_as_binary<std::int32_t>(source, header.minute               );
    write_as_binary<std::int32_t>(source, header.second               );
    write_as_binary<std::int32_t>(source, header.x_rounding_degree    );
    write_as_binary<std::int32_t>(source, header.y_rounding_degree    );
    write_as_binary<float       >(source, header.frame_acquision_time );
    write_as_binary<float       >(source, header.sensor_sensitivity   );
    write_as_binary<float       >(source, header.phase_sensitivity    );
    write_as_binary<std::int32_t>(source, header.offset               );
    skip_bytes(source, 12); // booked region
    write_as_binary<std::int32_t>(source, header.machine_id          );
    write_as_binary<std::int32_t>(source, header.ad_range            );
    write_as_binary<std::int32_t>(source, header.ad_resolution       );
    write_as_binary<float       >(source, header.x_max_scanning_range);
    write_as_binary<float       >(source, header.y_max_scanning_range);
    write_as_binary<float       >(source, header.x_piezo_extension   );
    write_as_binary<float       >(source, header.y_piezo_extension   );
    write_as_binary<float       >(source, header.z_piezo_extension   );
    write_as_binary<float       >(source, header.z_piezo_gain        );

    write_as_binary(source, header.operator_name, std::true_type{});
    write_as_binary(source, header.comment,       std::true_type{});

    return;
}

template<typename sourceT>
void write_header_impl(sourceT& source, const Header<version<2>>& header)
{
    write_as_binary<std::int32_t>(source, header.file_version         );
    write_as_binary<std::int32_t>(source, header.file_header_size     );
    write_as_binary<std::int32_t>(source, header.frame_header_size    );
    write_as_binary<std::int32_t>(source, header.text_encoding        );
    write_as_binary<std::int32_t>(source, header.operator_name_size   );
    write_as_binary<std::int32_t>(source, header.comment_size         );
    write_as_binary<std::int32_t>(source, header.data_kind_1ch        );
    write_as_binary<std::int32_t>(source, header.data_kind_2ch        );
    write_as_binary<std::int32_t>(source, header.init_frame           );
    write_as_binary<std::int32_t>(source, header.num_frames           );
    write_as_binary<std::int32_t>(source, header.scanning_direction   );
    write_as_binary<std::int32_t>(source, header.file_id              );
    write_as_binary<std::int32_t>(source, header.x_pixel              );
    write_as_binary<std::int32_t>(source, header.y_pixel              );
    write_as_binary<std::int32_t>(source, header.x_scanning_range     );
    write_as_binary<std::int32_t>(source, header.y_scanning_range     );
    write_as_binary<bool        >(source, header.is_averaged          );
    write_as_binary<std::int32_t>(source, header.average_window       );
    write_as_binary<std::int32_t>(source, header.year                 );
    write_as_binary<std::int32_t>(source, header.month                );
    write_as_binary<std::int32_t>(source, header.day                  );
    write_as_binary<std::int32_t>(source, header.hour                 );
    write_as_binary<std::int32_t>(source, header.minute               );
    write_as_binary<std::int32_t>(source, header.second               );
    write_as_binary<std::int32_t>(source, header.x_rounding_degree    );
    write_as_binary<std::int32_t>(source, header.y_rounding_degree    );
    write_as_binary<float       >(source, header.frame_acquision_time );
    write_as_binary<float       >(source, header.sensor_sensitivity   );
    write_as_binary<float       >(source, header.phase_sensitivity    );
    write_as_binary<std::int32_t>(source, header.offset               );
    skip_bytes(source, 12); // booked region
    write_as_binary<std::int32_t>(source, header.machine_id          );
    write_as_binary<std::int32_t>(source, header.ad_range            );
    write_as_binary<std::int32_t>(source, header.ad_resolution       );
    write_as_binary<float       >(source, header.x_max_scanning_range);
    write_as_binary<float       >(source, header.y_max_scanning_range);
    write_as_binary<float       >(source, header.x_piezo_extension   );
    write_as_binary<float       >(source, header.y_piezo_extension   );
    write_as_binary<float       >(source, header.z_piezo_extension   );
    write_as_binary<float       >(source, header.z_piezo_gain        );

    write_as_binary(source, header.operator_name, std::true_type{});
    write_as_binary(source, header.comment,       std::true_type{});

    write_as_binary<std::int32_t>(source, header.number_of_frames       );
    write_as_binary<std::int32_t>(source, header.is_x_feed_forward      );
    write_as_binary<std::int32_t>(source, header.x_feed_forward_i       );
    write_as_binary<std::int32_t>(source, header.x_feed_forward_d       );
    write_as_binary<std::int32_t>(source, header.max_color_scale        );
    write_as_binary<std::int32_t>(source, header.min_color_scale        );
    write_as_binary<std::int32_t>(source, header.anchor_point_size_red  );
    write_as_binary<std::int32_t>(source, header.anchor_point_size_green);
    write_as_binary<std::int32_t>(source, header.anchor_point_size_blue );

    write_as_binary(source, header.x_anchor_points_red, std::true_type{});
    write_as_binary(source, header.y_anchor_points_red, std::true_type{});

    write_as_binary(source, header.x_anchor_points_green, std::true_type{});
    write_as_binary(source, header.y_anchor_points_green, std::true_type{});

    write_as_binary(source, header.x_anchor_points_blue, std::true_type{});
    write_as_binary(source, header.y_anchor_points_blue, std::true_type{});

    return;
}
}// detail

template<typename verT>
char*& write_header(char*& ptr, const Header<verT>& header)
{
    detail::write_header_impl(ptr, header);
    return ptr;
}

template<typename verT>
std::ostream& write_header(std::ostream& os, const Header<verT>& header)
{
    detail::write_header_impl(os, header);
    return os;
}

} // asd
#endif// LIBASD_WRITE_HEADER_H
