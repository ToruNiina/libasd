#ifndef LIBASD_READ_HEADER_H
#define LIBASD_READ_HEADER_H
#include <libasd/debug_macro.hpp>
#include <libasd/read_binary_as.hpp>
#include <libasd/header.hpp>
#include <istream>

namespace asd
{
namespace detail
{

template<std::size_t N, typename contT>
void
read_header_impl(Header<channel<N>, version<0>, contT>& header,
                 const char* const ptr)
{
// TODO
    return;
}

template<std::size_t N, typename contT = container::vec>
void
read_header_impl(Header<channel<N>, version<0>, contT>& header, std::istream& is)
{
// TODO
    return;
}

template<std::size_t N, typename contT>
const char*
read_header_impl(Header<channel<N>, version<1>, contT>& header, const char* ptr)
{
    typedef Header<channel<N>, version<1>, contT> header_type;
    header.version              = read_binary_as<std::int32_t>(ptr);
    header.file_header_size     = read_binary_as<std::int32_t>(ptr);
    header.frame_header_size    = read_binary_as<std::int32_t>(ptr);
    header.text_encoding        = read_binary_as<std::int32_t>(ptr);
    header.operator_name_size   = read_binary_as<std::int32_t>(ptr);
    header.comment_size         = read_binary_as<std::int32_t>(ptr);
    header.data_kind_1ch        = read_binary_as<std::int32_t>(ptr);
    header.data_kind_2ch        = read_binary_as<std::int32_t>(ptr);
    header.init_frame           = read_binary_as<std::int32_t>(ptr);
    header.frame_size           = read_binary_as<std::int32_t>(ptr);
    header.scanning_direction   = read_binary_as<std::int32_t>(ptr);
    header.file_id              = read_binary_as<std::int32_t>(ptr);
    header.x_pixel              = read_binary_as<std::int32_t>(ptr);
    header.y_pixel              = read_binary_as<std::int32_t>(ptr);
    header.x_scanning_range     = read_binary_as<std::int32_t>(ptr);
    header.y_scanning_range     = read_binary_as<std::int32_t>(ptr);
    header.is_averaged          = read_binary_as<bool        >(ptr);
    header.average_window       = read_binary_as<std::int32_t>(ptr);
    header.year                 = read_binary_as<std::int32_t>(ptr);
    header.month                = read_binary_as<std::int32_t>(ptr);
    header.day                  = read_binary_as<std::int32_t>(ptr);
    header.hour                 = read_binary_as<std::int32_t>(ptr);
    header.minute               = read_binary_as<std::int32_t>(ptr);
    header.second               = read_binary_as<std::int32_t>(ptr);
    header.x_rounding_degree    = read_binary_as<std::int32_t>(ptr);
    header.y_rounding_degree    = read_binary_as<std::int32_t>(ptr);
    header.frame_acquision_time = read_binary_as<float       >(ptr);
    header.sensor_sensitiviy    = read_binary_as<float       >(ptr);
    header.phase_sensitivity    = read_binary_as<float       >(ptr);
    header.offset               = read_binary_as<std::int32_t>(ptr);
    ptr += 12; // booked region
    header.machine_id           = read_binary_as<std::int32_t>(ptr);
    header.AD_range             = read_binary_as<std::int32_t>(ptr);
    header.AD_resolution        = read_binary_as<std::int32_t>(ptr);
    header.x_max_scanning_range = read_binary_as<float       >(ptr);
    header.y_max_scanning_range = read_binary_as<float       >(ptr);
    header.x_piezo_extension    = read_binary_as<float       >(ptr);
    header.y_piezo_extension    = read_binary_as<float       >(ptr);
    header.z_piezo_extension    = read_binary_as<float       >(ptr);
    header.z_piezo_gain         = read_binary_as<float       >(ptr);

    const std::size_t op_name_size =
        static_cast<std::size_t>(header.operator_name_size);
    header_type::container_dispatcher_type::resize(header.operator_name, op_name_size);
    for(std::size_t i=0; i<op_name_size; ++i)
    {
        header.operator_name[i] += read_binary_as<std::int8_t>(ptr);
    }

    const std::size_t cm_size =
        static_cast<std::size_t>(header.comment_size);
    header_type::container_dispatcher_type::resize(header.comment, cm_size);
    for(std::size_t i=0; i<cm_size; ++i)
    {
        header.comment[i] += read_binary_as<std::int8_t>(ptr);
    }
    return ptr;
}

template<std::size_t N, typename contT = container::vec>
std::istream&
read_header_impl(Header<channel<N>, version<1>, contT>& header, std::istream& is)
{
    typedef Header<channel<N>, version<1>, contT> header_type;
    header.version              = read_binary_as<std::int32_t>(is);
    header.file_header_size     = read_binary_as<std::int32_t>(is);
    header.frame_header_size    = read_binary_as<std::int32_t>(is);
    header.text_encoding        = read_binary_as<std::int32_t>(is);
    header.operator_name_size   = read_binary_as<std::int32_t>(is);
    header.comment_size         = read_binary_as<std::int32_t>(is);
    header.data_kind_1ch        = read_binary_as<std::int32_t>(is);
    header.data_kind_2ch        = read_binary_as<std::int32_t>(is);
    header.init_frame           = read_binary_as<std::int32_t>(is);
    header.frame_size           = read_binary_as<std::int32_t>(is);
    header.scanning_direction   = read_binary_as<std::int32_t>(is);
    header.file_id              = read_binary_as<std::int32_t>(is);
    header.x_pixel              = read_binary_as<std::int32_t>(is);
    header.y_pixel              = read_binary_as<std::int32_t>(is);
    header.x_scanning_range     = read_binary_as<std::int32_t>(is);
    header.y_scanning_range     = read_binary_as<std::int32_t>(is);
    header.is_averaged          = read_binary_as<bool        >(is);
    header.average_window       = read_binary_as<std::int32_t>(is);
    header.year                 = read_binary_as<std::int32_t>(is);
    header.month                = read_binary_as<std::int32_t>(is);
    header.day                  = read_binary_as<std::int32_t>(is);
    header.hour                 = read_binary_as<std::int32_t>(is);
    header.minute               = read_binary_as<std::int32_t>(is);
    header.second               = read_binary_as<std::int32_t>(is);
    header.x_rounding_degree    = read_binary_as<std::int32_t>(is);
    header.y_rounding_degree    = read_binary_as<std::int32_t>(is);
    header.frame_acquision_time = read_binary_as<float       >(is);
    header.sensor_sensitiviy    = read_binary_as<float       >(is);
    header.phase_sensitivity    = read_binary_as<float       >(is);
    header.offset               = read_binary_as<std::int32_t>(is);
    is.ignore(12); // booked region
    header.machine_id           = read_binary_as<std::int32_t>(is);
    header.AD_range             = read_binary_as<std::int32_t>(is);
    header.AD_resolution        = read_binary_as<std::int32_t>(is);
    header.x_max_scanning_range = read_binary_as<float       >(is);
    header.y_max_scanning_range = read_binary_as<float       >(is);
    header.x_piezo_extension    = read_binary_as<float       >(is);
    header.y_piezo_extension    = read_binary_as<float       >(is);
    header.z_piezo_extension    = read_binary_as<float       >(is);
    header.z_piezo_gain         = read_binary_as<float       >(is);

    const std::size_t op_name_size =
        static_cast<std::size_t>(header.operator_name_size);
    header_type::container_dispatcher_type::resize(
            header.operator_name, op_name_size);
    for(std::size_t i=0; i<op_name_size; ++i)
    {
        header.operator_name[i] += read_binary_as<std::int8_t>(is);
    }

    const std::size_t cm_size =
        static_cast<std::size_t>(header.comment_size);
    header_type::container_dispatcher_type::resize(header.comment, cm_size);
    for(std::size_t i=0; i<cm_size; ++i)
    {
        header.comment[i] += read_binary_as<std::int8_t>(is);
    }
    return is;
}

template<std::size_t N, typename contT>
void read_header_impl(Header<channel<N>, version<2>, contT>& header,
                 const char* const ptr)
{
// TODO
    return;
}

template<std::size_t N, typename contT = container::vec>
void
read_header_impl(Header<channel<N>, version<2>, contT>& header, std::istream& is)
{
// TODO
    return;
}

} // detail

template<typename chT, typename verT, typename contT = container::vec>
Header<chT, verT, contT> read_header(const char* const ptr)
{
    Header<chT, verT, contT> header;
    detail::read_header_impl(header, ptr);
    return header;
}

template<typename chT, typename verT, typename contT = container::vec>
Header<chT, verT, contT> read_header(std::istream& is)
{
    Header<chT, verT, contT> header;
    detail::read_header_impl(header, is);
    return header;
}

} // asd
#endif// LIBASD_READ_HEADER_H
