#ifndef LIBASD_HEADER_H
#define LIBASD_HEADER_H
#include <libasd/debug_macro.hpp>
#include <libasd/tag.hpp>
#include <libasd/container_dispatcher.hpp>
#include <iostream>
#include <cstdint>

namespace asd
{

template<typename chT, typename verT, typename contT = container::vec>
struct Header
{
    static_assert(std::is_same<chT, channel<1>>::value ||
                  std::is_same<chT, channel<2>>::value,
                  "currently, channel 1 and 2 are supported.");
    static_assert(std::is_same<verT, version<0>>::value ||
                  std::is_same<verT, version<1>>::value ||
                  std::is_same<verT, version<2>>::value,
                  "currently, version 0, 1, and 2 are supported.");
};

template<std::size_t N, typename contT>
struct Header<channel<N>, version<0>, contT>
{
    typedef typename contT::template rebind<std::int8_t>::other byte_array;

    std::int32_t version;             //!< file version
    std::int16_t data_type_1ch;       //!< AFM data type of 1ch
    std::int16_t data_type_2ch;       //!< AFM data type of 2ch
    std::int32_t file_header_size;    //!< Size of the file header
    std::int32_t frame_header_size;   //!< Size of the frame header
    std::int32_t operator_name_size;  //!< Size of the operator name
    std::int32_t comment_offset_size; //!< Size of the offset to the comment
    std::int32_t comment_size;        //!< Size of the comment
    std::int32_t x_pixel;             //!< X pixel
    std::int32_t y_pixel;             //!< Y pixel (or number of lines)
    std::int32_t x_scanning_range;    //!< X scanning range (nm)
    std::int32_t y_scanning_range;    //!< Y scanning range (nm)
    float        frame_rate;          //!< Frame rate (ms/frame)
    float        z_piezo_extension;   //!< Z piezo extension coefficient (nm/V)
    float        z_piexo_gain;        //!< Z piezo driver gain
    std::int32_t ad_range;            //!< Code showing AD range (AD_1V, AD_2P5V, AD_5V or AD_10V)
    std::int32_t bits_data;           //!< Number of bits of Data(typically, 12)
    bool	     is_averaged;         //!< Flag of the averaging
    std::int32_t average_window;      //!< Number of data for the averaging
    std::int32_t _legacy;             //!< Not booked-region, just a compliance for the previous Kodec program
    std::int16_t year;                //!< Year when this asd file was recorded
    std::uint8_t month;               //!< Month when this asd file was recorded
    std::uint8_t day;                 //!< Day when this asd file was recorded
    std::uint8_t hour;                //!< Hour when this asd file was recorded
    std::uint8_t minute;              //!< Minute when this asd file was recorded
    std::uint8_t second;              //!< Second when this asd file was recorded
    std::int32_t rounding_degree;     //!< Degree of the rounding of x-scanning signal (%)
    float        x_max_range;         //!< X maximum scanning range (nm)
    float        y_max_range;         //!< Y maximum scanning range (nm)
    std::int32_t _booked1;            //!< Booked-region
    std::int32_t _booked2;            //!< Booked-region
    std::int32_t _booked3;            //!< Booked-region
    std::int32_t init_frame;          //!< Number of frames when this asd file was recorded
    std::int32_t num_frames;          //!< Number of frames contained in this asd file
    std::int32_t machine_id;          //!< Number of the imaging machine
    std::int32_t file_id;             //!< Name of the asd file
    byte_array   operator_name;       //!< Name of the operator
    float        sensor_sensitivity;  //!< Sensor sensitivity (nm/V)
    float        phase_sensitivity;   //!< Phase sensitivity (deg/V)
    std::int32_t scannig_direction;   //!< Number showing the scanning direction
    byte_array   comment;             //!< Comment
};

template<std::size_t N, typename contT>
struct Header<channel<N>, version<1>, contT>
{
    typedef contT container_dispatcher_type;
    typedef typename contT::template rebind<std::int8_t>::other byte_array;
    //! XXX because text encoding is not known here, we can't use std::string.

    std::int32_t version;              //!< File version
    std::int32_t file_header_size;     //!< Size of the file header
    std::int32_t frame_header_size;    //!< Size of the frame header
    std::int32_t text_encoding;        //!< Characteristic number for text encoding (ex. “932” in case of Shift-JIS)
    std::int32_t operator_name_size;   //!< Size of the operator name
    std::int32_t comment_size;         //!< Size of the comment
    std::int32_t data_kind_1ch;        //!< Kind of AFM data of 1ch (Topography, Error or Phase) written by enum array (ex. Topography 0x5054)
    std::int32_t data_kind_2ch;        //!< Kind of AFM data of 2ch (Topography, Error or Phase) written by enum array (ex. Topography 0x5054)
    std::int32_t init_frame;           //!< Number of frames when this asd file was recorded
    std::int32_t frame_size;           //!< Number of frames contained in this asd file
    std::int32_t scanning_direction;   //!< Number showing the scanning direction
    std::int32_t file_id;              //!< Name of the asd file
    std::int32_t x_pixel;              //!< X pixel
    std::int32_t y_pixel;              //!< Y pixel
    std::int32_t x_scanning_range;     //!< X scanning range (nm)
    std::int32_t y_scanning_range;     //!< Y scanning range (nm)
    bool         is_averaged;          //!< Flag of the averaging
    std::int32_t average_window;       //!< Number of data for the averaging
    std::int32_t year;                 //!< Year when this asd file was recorded
    std::int32_t month;                //!< Month when this asd file was recorded
    std::int32_t day;                  //!< Day when this asd file was recorded
    std::int32_t hour;                 //!< Hour when this asd file was recorded
    std::int32_t minute;               //!< Minuit when this asd file was recorded
    std::int32_t second;               //!< Second when this asd file was recorded
    std::int32_t x_rounding_degree;    //!< Degree of the rounding of x-scanning signal (%)
    std::int32_t y_rounding_degree;    //!< Degree of the rounding of y-scanning signal (%)
    float        frame_acquision_time; //!< Frame acquisition time (ms)
    float        sensor_sensitiviy;    //!< Sensor sensitivity (nm/V)
    float        phase_sensitivity;    //!< Phase sensitivity (deg/V)
    std::int32_t offset;               //!< Offset (This number is “0” in the case of file ver. 2)
    // 12 byte Booked region of 12 byte
    std::int32_t machine_id;           //!< Number of the imaging machine
    std::int32_t AD_range;             //!< Code showing AD range (AD_1V, AD_2P5V, AD_5V or AD_10V)
    std::int32_t AD_resolution;        //!< AD resolution (When this value is 12, the AD resolution is 4096 (2^12).)
    float x_max_scanning_range;        //!< X maximum scanning range (nm)
    float y_max_scanning_range;        //!< Y maximum scanning range (nm)
    float x_piezo_extension;           //!< X piezo extension coefficient (nm/V)
    float y_piezo_extension;           //!< Y piezo extension coefficient (nm/V)
    float z_piezo_extension;           //!< Z piezo extension coefficient (nm/V)
    float z_piezo_gain;                //!< Z piezo driver gain
    byte_array operator_name;          //!< Name of the operator
    byte_array comment;                //!< Comment
};

template<std::size_t N, typename contT>
struct Header<channel<N>, version<2>, contT>
{
    typedef typename contT::template rebind<std::int8_t>::other byte_array;
    typedef typename contT::template rebind<std::int32_t>::other i32_array;

    std::int32_t version;              //!< File version
    std::int32_t file_header_size;     //!< Size of the file header
    std::int32_t frame_header_size;    //!< Size of the frame header
    std::int32_t text_encoding;        //!< Characteristic number for text encoding (ex. “932” in case of Shift-JIS)
    std::int32_t operator_name_size;   //!< Size of the operator name
    std::int32_t comment_size;         //!< Size of the comment
    std::int32_t data_kind_1ch;        //!< Kind of AFM data of 1ch (Topography, Error or Phase) written by enum array (ex. Topography 0x5054)
    std::int32_t data_kind_2ch;        //!< Kind of AFM data of 2ch (Topography, Error or Phase) written by enum array (ex. Topography 0x5054)
    std::int32_t init_frame;           //!< Number of frames when this asd file was recorded
    std::int32_t frame_size;           //!< Number of frames contained in this asd file
    std::int32_t scanning_direction;   //!< Number showing the scanning direction
    std::int32_t file_id;              //!< Name of the asd file
    std::int32_t x_pixel;              //!< X pixel
    std::int32_t y_pixel;              //!< Y pixel
    std::int32_t x_scanning_range;     //!< X scanning range (nm)
    std::int32_t y_scanning_range;     //!< Y scanning range (nm)
    bool         is_averaged;          //!< Flag of the averaging
    std::int32_t average_window;       //!< Number of data for the averaging
    std::int32_t year;                 //!< Year when this asd file was recorded
    std::int32_t month;                //!< Month when this asd file was recorded
    std::int32_t day;                  //!< Day when this asd file was recorded
    std::int32_t hour;                 //!< Hour when this asd file was recorded
    std::int32_t minute;               //!< Minuit when this asd file was recorded
    std::int32_t second;               //!< Second when this asd file was recorded
    std::int32_t x_rounding_degree;    //!< Degree of the rounding of x-scanning signal (%)
    std::int32_t y_rounding_degree;    //!< Degree of the rounding of y-scanning signal (%)
    float        frame_acquision_time; //!< Frame acquisition time (ms)
    float        sensor_sensitiviy;    //!< Sensor sensitivity (nm/V)
    float        phase_sensitivity;    //!< Phase sensitivity (deg/V)
    std::int32_t offset;               //!< Offset (This number is “0” in the case of file ver. 2)
    // 12 byte Booked region of 12 byte
    std::int32_t machine_id;           //!< Number of the imaging machine
    std::int32_t AD_range;             //!< Code showing AD range (AD_1V, AD_2P5V, AD_5V or AD_10V)
    std::int32_t AD_resolution;        //!< AD resolution (When this value is 12, the AD resolution is 4096 (2^12).)
    float x_max_scanning_range;        //!< X maximum scanning range (nm)
    float y_max_scanning_range;        //!< Y maximum scanning range (nm)
    float x_piezo_extension;           //!< X piezo extension coefficient (nm/V)
    float y_piezo_extension;           //!< Y piezo extension coefficient (nm/V)
    float z_piezo_extension;           //!< Z piezo extension coefficient (nm/V)
    float z_piezo_gain;                //!< Z piezo driver gain
    byte_array operator_name;          //!< Name of the operator
    byte_array comment;                //!< Comment

    // additional parameters...
    std::int32_t number_of_frames;     //!< number of frames
    bool         is_x_feed_forward;    //!< flag for X-line feed-forward
    std::int32_t x_feed_forward_i;     //!< X-line feed-forward parameter
    double       x_feed_forward_d;     //!< X-line feed-forward parameter
    std::int32_t max_color_scale;      //!< maximum value of color scale[0, 255]
    std::int32_t min_color_scale;      //!< maximum value of color scale[0, 255]
    std::int32_t anchor_point_red;     //!< number of anchor point for color collection (R)
    std::int32_t anchor_point_green;   //!< number of anchor point for color collection (G)
    std::int32_t anchor_point_blue;    //!< number of anchor point for color collection (B)
    i32_array    x_anchor_points_red;  //!< X value of an anchor point (R)
    i32_array    y_anchor_points_red;  //!< Y value of an anchor point (R)
    i32_array    x_anchor_points_green;//!< X value of an anchor point (G)
    i32_array    y_anchor_points_green;//!< Y value of an anchor point (G)
    i32_array    x_anchor_points_blue; //!< X value of an anchor point (B)
    i32_array    y_anchor_points_blue; //!< Y value of an anchor point (B)
};

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

template<typename std::size_t N>
inline bool check_ptr_and_throw_if_ends_with(const char* ptr) noexcept
{
    for(std::size_t i=0; i<N; ++i)
    {
        if(*ptr == '\0'){throw std::invalid_argument("string suddenly ends");}
        else            {++ptr;}
    }
    return true;
}

template<typename T>
T read_char_ptr_as(const char*& ptr)
{
    //XXX its unsafe!
    const T retval = *reinterpret_cast<const T*>(ptr);
    ptr += sizeof(T);
    return retval;
}

template<std::size_t N, typename contT>
void read_header_impl(Header<channel<N>, version<1>, contT>& header,
                      char const* ptr)
{
    typedef Header<channel<N>, version<1>, contT> header_type;

    header.version              = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.version              )
    header.file_header_size     = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.file_header_size     )
    header.frame_header_size    = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.frame_header_size    )
    header.text_encoding        = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.text_encoding        )
    header.operator_name_size   = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.operator_name_size   )
    header.comment_size         = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.comment_size         )
    header.data_kind_1ch        = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.data_kind_1ch        )
    header.data_kind_2ch        = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.data_kind_2ch        )
    header.init_frame           = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.init_frame           )
    header.frame_size           = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.frame_size           )
    header.scanning_direction   = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.scanning_direction   )
    header.file_id              = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.file_id              )
    header.x_pixel              = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.x_pixel              )
    header.y_pixel              = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.y_pixel              )
    header.x_scanning_range     = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.x_scanning_range     )
    header.y_scanning_range     = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.y_scanning_range     )
    header.is_averaged          = read_char_ptr_as<bool        >(ptr); LIBASD_DUMP_IF_DEBUG( header.is_averaged          )
    header.average_window       = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.average_window       )
    header.year                 = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.year                 )
    header.month                = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.month                )
    header.day                  = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.day                  )
    header.hour                 = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.hour                 )
    header.minute               = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.minute               )
    header.second               = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.second               )
    header.x_rounding_degree    = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.x_rounding_degree    )
    header.y_rounding_degree    = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.y_rounding_degree    )
    header.frame_acquision_time = read_char_ptr_as<float       >(ptr); LIBASD_DUMP_IF_DEBUG( header.frame_acquision_time )
    header.sensor_sensitiviy    = read_char_ptr_as<float       >(ptr); LIBASD_DUMP_IF_DEBUG( header.sensor_sensitiviy    )
    header.phase_sensitivity    = read_char_ptr_as<float       >(ptr); LIBASD_DUMP_IF_DEBUG( header.phase_sensitivity    )
    header.offset               = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.offset               )
    ptr += 12; // booked region
    header.machine_id           = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.machine_id           )
    header.AD_range             = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.AD_range             )
    header.AD_resolution        = read_char_ptr_as<std::int32_t>(ptr); LIBASD_DUMP_IF_DEBUG( header.AD_resolution        )
    header.x_max_scanning_range = read_char_ptr_as<float       >(ptr); LIBASD_DUMP_IF_DEBUG( header.x_max_scanning_range )
    header.y_max_scanning_range = read_char_ptr_as<float       >(ptr); LIBASD_DUMP_IF_DEBUG( header.y_max_scanning_range )
    header.x_piezo_extension    = read_char_ptr_as<float       >(ptr); LIBASD_DUMP_IF_DEBUG( header.x_piezo_extension    )
    header.y_piezo_extension    = read_char_ptr_as<float       >(ptr); LIBASD_DUMP_IF_DEBUG( header.y_piezo_extension    )
    header.z_piezo_extension    = read_char_ptr_as<float       >(ptr); LIBASD_DUMP_IF_DEBUG( header.z_piezo_extension    )
    header.z_piezo_gain         = read_char_ptr_as<float       >(ptr); LIBASD_DUMP_IF_DEBUG( header.z_piezo_gain         )

    const std::size_t op_name_size =
        static_cast<std::size_t>(header.operator_name_size);
    header_type::container_dispatcher_type::resize(header.operator_name, op_name_size);
    for(std::size_t i=0; i<op_name_size; ++i)
    {
        header.operator_name[i] += read_char_ptr_as<std::int8_t>(ptr);
    }

    const std::size_t cm_size =
        static_cast<std::size_t>(header.comment_size);
    header_type::container_dispatcher_type::resize(header.comment, cm_size);
    for(std::size_t i=0; i<cm_size; ++i)
    {
        header.comment[i] += read_char_ptr_as<std::int8_t>(ptr);
    }
    return;
}

template<std::size_t N, typename contT = container::vec>
void read_header_impl(Header<channel<N>, version<1>, contT>& header,
                      std::istream& is)
{
    const auto initial = is.tellg();
    is.seekg(0, std::ios::end);
    const auto eofpos  = is.tellg();
    const std::size_t size = eofpos - initial;
    is.seekg(initial);
    std::vector<char> contents(size);
    is.read(contents.data(), size);

    read_header_impl(header, contents.data());
    return;
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
#endif//LIBASD_CHANNELS_TAG_H
