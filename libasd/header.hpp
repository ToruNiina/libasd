#ifndef LIBASD_HEADER_H
#define LIBASD_HEADER_H
#include <libasd/tag.hpp>
#include <libasd/exception_thrower.hpp>
#include <libasd/container_dispatcher.hpp>
#include <cstdint>
#include <cmath>

namespace asd
{

enum class AD_range : std::uint32_t
{
    unipolar_1_0V = 0x00000001, //!<  0.0 ~  1.0 [V]
    unipolar_2_5V = 0x00000002, //!<  0.0 ~  2.5 [V]
    unipolar_5_0V = 0x00000004, //!<  0.0 ~  5.0 [V]
    bipolar_1_0V  = 0x00010000, //!< -1.0 ~ +1.0 [V]
    bipolar_2_5V  = 0x00020000, //!< -2.5 ~ +2.5 [V]
    bipolar_5_0V  = 0x00040000, //!< -5.0 ~ +5.0 [V]
    dummy_value   = 0x00800000, //!< dummy value(80[V]). & resolution -> 16bit.
};

template<typename Real>
std::pair<Real, Real> to_voltage(const AD_range range)
{
    switch(range)
    {
        case AD_range::unipolar_1_0V: {return std::make_pair(  0.0,  1.0);}
        case AD_range::unipolar_2_5V: {return std::make_pair(  0.0,  2.5);}
        case AD_range::unipolar_5_0V: {return std::make_pair(  0.0,  5.0);}
        case AD_range::bipolar_1_0V : {return std::make_pair( -1.0,  1.0);}
        case AD_range::bipolar_2_5V : {return std::make_pair( -2.5,  2.5);}
        case AD_range::bipolar_5_0V : {return std::make_pair( -5.0,  5.0);}
        case AD_range::dummy_value  : {return std::make_pair(-80.0, 80.0);}
        default:
        {
            throw std::runtime_error("invalid AD_range: " +
                     std::to_string(static_cast<std::uint32_t>(range)));
        }
    }
}

template<typename charT, typename traitsT>
std::basic_ostream<charT, traitsT>&
operator<<(std::basic_ostream<charT, traitsT>& os, const AD_range adr)
{
    switch(adr)
    {
        case AD_range::unipolar_1_0V: return os << "0.0 ~ 1.0 [V]";
        case AD_range::unipolar_2_5V: return os << "0.0 ~ 2.5 [V]";
        case AD_range::unipolar_5_0V: return os << "0.0 ~ 5.0 [V]";
        case AD_range::bipolar_1_0V : return os << "-1.0 ~ +1.0 [V]";
        case AD_range::bipolar_2_5V : return os << "-2.5 ~ +2.5 [V]";
        case AD_range::bipolar_5_0V : return os << "-5.0 ~ +5.0 [V]";
        case AD_range::dummy_value  : return os << "dummy (-80 ~ 80) [V]";
        default: return os << "invalid AD_range value : "
                           << static_cast<std::uint32_t>(adr);
    }
}

enum class scan_direction : std::int32_t
{
    x_forward  = 1,
    x_backward = 2,
    y_forward  = 4,
    y_backward = 8,
};

template<typename charT, typename traitsT>
std::basic_ostream<charT, traitsT>&
operator<<(std::basic_ostream<charT, traitsT>& os, const scan_direction scd)
{
    switch(scd)
    {
        case scan_direction::x_forward : return os << "x forward";
        case scan_direction::x_backward: return os << "x backward";
        case scan_direction::y_forward : return os << "y forward";
        case scan_direction::y_backward: return os << "y backward";
        default: return os << "invalid scan_direction value : "
                           << static_cast<std::uint32_t>(scd);
    }
}

enum class encode : std::int32_t
{
    Shift_JIS =   932,
    UTF_16    =  1200,
    EUC_JP    = 20932,
    UTF_8     = 65001,
    JP_MAC    = 10001,
    ASCII     = 20127,
};

template<typename charT, typename traitsT>
std::basic_ostream<charT, traitsT>&
operator<<(std::basic_ostream<charT, traitsT>& os, const encode scd)
{
    switch(scd)
    {
        case encode::Shift_JIS : return os << "Shift_JIS";
        case encode::UTF_16    : return os << "UTF_16";
        case encode::EUC_JP    : return os << "EUC_JP";
        case encode::UTF_8     : return os << "UTF_8";
        case encode::JP_MAC    : return os << "JP_MAC";
        case encode::ASCII     : return os << "ASCII";
        default: return os << "invalid encode value : "
                           << static_cast<std::uint32_t>(scd);
    }
}

enum class data_kind : std::int32_t
{
    topography = 0x5054, //!< from ascii code "TP" (reversed because of the endian).
    error      = 0x5245, //!< from ascii code "ER" (reversed because of the endian).
    phase      = 0x4850, //!< from ascii code "PH" (reversed because of the endian).
    none       = 0x0000, //!< nothing.
};

template<typename charT, typename traitsT>
std::basic_ostream<charT, traitsT>&
operator<<(std::basic_ostream<charT, traitsT>& os, const data_kind dk)
{
    switch(dk)
    {
        case data_kind::topography : return os << "topography";
        case data_kind::error      : return os << "error";
        case data_kind::phase      : return os << "phase";
        case data_kind::none       : return os << "none";
        default: return os << "invalid data_kind value : "
                           << static_cast<std::uint16_t>(dk);
    }
}

template<typename verT>
struct Header
{
    static_assert(std::is_same<verT, version<0>>::value ||
                  std::is_same<verT, version<1>>::value ||
                  std::is_same<verT, version<2>>::value,
                  "currently, version 0, 1, and 2 are supported.");
};

template<>
struct Header<version<0>>
{
    typedef version<0> version_tag;
    static constexpr std::size_t num_version = version_tag::value;
    typedef std::vector<std::int8_t> byte_array;

    //! XXX It should be noted that asd version 0 contains data_type_Nch as
    //      int16_t. But the latter versions contain data_kind as int32_t.
    //      Since underlying type of data_kind is defined as int32_t in this
    //      library, the data size should be cared in function read_header().

    std::int32_t   file_version;        //!< file version
    data_kind      data_type_1ch;       //!< AFM data type of 1ch
    data_kind      data_type_2ch;       //!< AFM data type of 2ch
    std::int32_t   file_header_size;    //!< Size of the file header
    std::int32_t   frame_header_size;   //!< Size of the frame header
    std::int32_t   operator_name_size;  //!< Size of the operator name
    std::int32_t   comment_offset_size; //!< Size of the offset to the comment
    std::int32_t   comment_size;        //!< Size of the comment
    std::int16_t   x_pixel;             //!< X pixel
    std::int16_t   y_pixel;             //!< Y pixel (or number of lines)
    std::int16_t   x_scanning_range;    //!< X scanning range (nm)
    std::int16_t   y_scanning_range;    //!< Y scanning range (nm)
    float          frame_rate;          //!< Frame rate (ms/frame)
    float          z_piezo_extension;   //!< Z piezo extension coefficient (nm/V)
    float          z_piezo_gain;        //!< Z piezo driver gain
    AD_range       ad_range;            //!< Code showing AD range (AD_1V, AD_2P5V, AD_5V or AD_10V)
    std::int32_t   bits_data;           //!< Number of bits of Data(typically, 12 -> [0, 4095])
    bool	       is_averaged;         //!< Flag of the averaging
    std::int32_t   average_window;      //!< Number of data for the averaging
    std::int16_t   _legacy;             //!< Not booked-region, just a compliance for the previous Kodec program
    std::int16_t   year;                //!< Year when this asd file was recorded
    std::uint8_t   month;               //!< Month when this asd file was recorded
    std::uint8_t   day;                 //!< Day when this asd file was recorded
    std::uint8_t   hour;                //!< Hour when this asd file was recorded
    std::uint8_t   minute;              //!< Minute when this asd file was recorded
    std::uint8_t   second;              //!< Second when this asd file was recorded
    std::uint8_t   rounding_degree;     //!< Degree of the rounding of x-scanning signal (%)
    float          x_max_range;         //!< X maximum scanning range (nm)
    float          y_max_range;         //!< Y maximum scanning range (nm)
    std::int32_t   _booked1;            //!< Booked-region
    std::int32_t   _booked2;            //!< Booked-region
    std::int32_t   _booked3;            //!< Booked-region
    std::int32_t   init_frame;          //!< Number of frames when this asd file was recorded
    std::int32_t   num_frames;          //!< Number of frames contained in this asd file
    std::int32_t   machine_id;          //!< Number of the imaging machine
    std::int16_t   file_id;             //!< Name of the asd file
    byte_array     operator_name;       //!< Name of the operator
    float          sensor_sensitivity;  //!< Sensor sensitivity (nm/V)
    float          phase_sensitivity;   //!< Phase sensitivity (deg/V)
    scan_direction scannig_direction;   //!< Number showing the scanning direction
    byte_array     comment;             //!< Comment
};

template<>
struct Header<version<1>>
{
    typedef version<1> version_tag;
    static constexpr std::size_t num_version = version_tag::value;

    //XXX because text encoding is unknown here, std::string and its variants cannot be used.
    typedef std::vector<std::int8_t> byte_array;

    std::int32_t   file_version;         //!< File version
    std::int32_t   file_header_size;     //!< Size of the file header
    std::int32_t   frame_header_size;    //!< Size of the frame header
    encode         text_encoding;        //!< Characteristic number for text encoding
    std::int32_t   operator_name_size;   //!< Size of the operator name
    std::int32_t   comment_size;         //!< Size of the comment
    data_kind      data_kind_1ch;        //!< Kind of AFM data of 1ch
    data_kind      data_kind_2ch;        //!< Kind of AFM data of 2ch
    std::int32_t   init_frame;           //!< Number of frames when this asd file was recorded
    std::int32_t   num_frames;           //!< Number of frames contained in this asd file
    scan_direction scanning_direction;   //!< Number showing the scanning direction
    std::int32_t   file_id;              //!< Name of the asd file
    std::int32_t   x_pixel;              //!< X pixel
    std::int32_t   y_pixel;              //!< Y pixel
    std::int32_t   x_scanning_range;     //!< X scanning range (nm)
    std::int32_t   y_scanning_range;     //!< Y scanning range (nm)
    bool           is_averaged;          //!< Flag of the averaging
    std::int32_t   average_window;       //!< Number of data for the averaging
    std::int32_t   year;                 //!< Year when this asd file was recorded
    std::int32_t   month;                //!< Month when this asd file was recorded
    std::int32_t   day;                  //!< Day when this asd file was recorded
    std::int32_t   hour;                 //!< Hour when this asd file was recorded
    std::int32_t   minute;               //!< Minuit when this asd file was recorded
    std::int32_t   second;               //!< Second when this asd file was recorded
    std::int32_t   x_rounding_degree;    //!< Degree of the rounding of x-scanning signal (%)
    std::int32_t   y_rounding_degree;    //!< Degree of the rounding of y-scanning signal (%)
    float          frame_acquision_time; //!< Frame acquisition time (ms)
    float          sensor_sensitivity;   //!< Sensor sensitivity (nm/V)
    float          phase_sensitivity;    //!< Phase sensitivity (deg/V)
    std::int32_t   offset;               //!< Offset (This number is “0” in the case of file ver. 2)
    // Booked region of 12 byte here
    std::int32_t   machine_id;           //!< Number of the imaging machine
    AD_range       ad_range;             //!< Code showing AD range (AD_1V, AD_2P5V, AD_5V or AD_10V)
    std::int32_t   ad_resolution;        //!< AD resolution (When this value is 12, the AD resolution is 4096 (2^12).)
    float          x_max_scanning_range; //!< X maximum scanning range (nm)
    float          y_max_scanning_range; //!< Y maximum scanning range (nm)
    float          x_piezo_extension;    //!< X piezo extension coefficient (nm/V)
    float          y_piezo_extension;    //!< Y piezo extension coefficient (nm/V)
    float          z_piezo_extension;    //!< Z piezo extension coefficient (nm/V)
    float          z_piezo_gain;         //!< Z piezo driver gain
    byte_array     operator_name;        //!< Name of the operator
    byte_array     comment;              //!< Comment
};

template<>
struct Header<version<2>>
{
    typedef version<2> version_tag;
    static constexpr std::size_t num_version = version_tag::value;

    typedef std::vector<std::int8_t> byte_array;
    typedef std::vector<std::int32_t> i32_array;

    std::int32_t   file_version;         //!< File version
    std::int32_t   file_header_size;     //!< Size of the file header
    std::int32_t   frame_header_size;    //!< Size of the frame header
    encode         text_encoding;        //!< Characteristic number for text encoding
    std::int32_t   operator_name_size;   //!< Size of the operator name
    std::int32_t   comment_size;         //!< Size of the comment
    data_kind      data_kind_1ch;        //!< Kind of AFM data of 1ch
    data_kind      data_kind_2ch;        //!< Kind of AFM data of 2ch
    std::int32_t   init_frame;           //!< Number of frames when this asd file was recorded
    std::int32_t   num_frames;           //!< Number of frames contained in this asd file
    scan_direction scanning_direction;   //!< Number showing the scanning direction
    std::int32_t   file_id;              //!< Name of the asd file
    std::int32_t   x_pixel;              //!< X pixel
    std::int32_t   y_pixel;              //!< Y pixel
    std::int32_t   x_scanning_range;     //!< X scanning range (nm)
    std::int32_t   y_scanning_range;     //!< Y scanning range (nm)
    bool           is_averaged;          //!< Flag of the averaging
    std::int32_t   average_window;       //!< Number of data for the averaging
    std::int32_t   year;                 //!< Year when this asd file was recorded
    std::int32_t   month;                //!< Month when this asd file was recorded
    std::int32_t   day;                  //!< Day when this asd file was recorded
    std::int32_t   hour;                 //!< Hour when this asd file was recorded
    std::int32_t   minute;               //!< Minuit when this asd file was recorded
    std::int32_t   second;               //!< Second when this asd file was recorded
    std::int32_t   x_rounding_degree;    //!< Degree of the rounding of x-scanning signal (%)
    std::int32_t   y_rounding_degree;    //!< Degree of the rounding of y-scanning signal (%)
    float          frame_acquision_time; //!< Frame acquisition time (ms)
    float          sensor_sensitivity;   //!< Sensor sensitivity (nm/V)
    float          phase_sensitivity;    //!< Phase sensitivity (deg/V)
    std::int32_t   offset;               //!< Offset (This number is "0" in the case of file ver. 2)
    // Booked region of 12 byte
    std::int32_t   machine_id;           //!< Number of the imaging machine
    AD_range       ad_range;             //!< Code showing AD range
    std::int32_t   ad_resolution;        //!< log2 AD resolution (12 means 4096 levels)
    float          x_max_scanning_range; //!< X maximum scanning range (nm)
    float          y_max_scanning_range; //!< Y maximum scanning range (nm)
    float          x_piezo_extension;    //!< X piezo extension coefficient (nm/V)
    float          y_piezo_extension;    //!< Y piezo extension coefficient (nm/V)
    float          z_piezo_extension;    //!< Z piezo extension coefficient (nm/V)
    float          z_piezo_gain;         //!< Z piezo driver gain
    byte_array     operator_name;        //!< Name of the operator
    byte_array     comment;              //!< Comment
    // additional parameters...
    std::int32_t number_of_frames;        //!< number of frames
    bool         is_x_feed_forward;       //!< flag for X-line feed-forward
    std::int32_t x_feed_forward_i;        //!< X-line feed-forward parameter
    double       x_feed_forward_d;        //!< X-line feed-forward parameter
    std::int32_t max_color_scale;         //!< maximum value of color scale[0, 255]
    std::int32_t min_color_scale;         //!< maximum value of color scale[0, 255]
    std::int32_t anchor_point_size_red;   //!< number of anchor point for color collection (R)
    std::int32_t anchor_point_size_green; //!< number of anchor point for color collection (G)
    std::int32_t anchor_point_size_blue;  //!< number of anchor point for color collection (B)
    i32_array    x_anchor_points_red;     //!< X value of an anchor point (R)
    i32_array    y_anchor_points_red;     //!< Y value of an anchor point (R)
    i32_array    x_anchor_points_green;   //!< X value of an anchor point (G)
    i32_array    y_anchor_points_green;   //!< Y value of an anchor point (G)
    i32_array    x_anchor_points_blue;    //!< X value of an anchor point (B)
    i32_array    y_anchor_points_blue;    //!< Y value of an anchor point (B)
};

} // asd
#endif//LIBASD_FILE_HEADER_H
