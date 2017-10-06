#ifndef LIBASD_HEADER_H
#define LIBASD_HEADER_H
#include <libasd/tag.hpp>
#include <libasd/container_dispatcher.hpp>
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
    typedef channel<N> channel_tag;
    typedef version<0> version_tag;
    static constexpr std::size_t num_channel = channel_tag::value;
    static constexpr std::size_t num_version = version_tag::value;

    typedef contT container_dispatcher_type;
    typedef typename contT::template rebind<std::int8_t>::other byte_array;

    std::int32_t file_version;        //!< file version
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
    typedef channel<N> channel_tag;
    typedef version<1> version_tag;
    static constexpr std::size_t num_channel = channel_tag::value;
    static constexpr std::size_t num_version = version_tag::value;

    typedef contT container_dispatcher_type;
    typedef typename contT::template rebind<std::int8_t>::other byte_array;
    //! XXX because text encoding is not known here, we can't use std::string.

    std::int32_t file_version;         //!< File version
    std::int32_t file_header_size;     //!< Size of the file header
    std::int32_t frame_header_size;    //!< Size of the frame header
    std::int32_t text_encoding;        //!< Characteristic number for text encoding (ex. “932” in case of Shift-JIS)
    std::int32_t operator_name_size;   //!< Size of the operator name
    std::int32_t comment_size;         //!< Size of the comment
    std::int32_t data_kind_1ch;        //!< Kind of AFM data of 1ch (Topography, Error or Phase) written by enum array (ex. Topography 0x5054)
    std::int32_t data_kind_2ch;        //!< Kind of AFM data of 2ch (Topography, Error or Phase) written by enum array (ex. Topography 0x5054)
    std::int32_t init_frame;           //!< Number of frames when this asd file was recorded
    std::int32_t num_frames;           //!< Number of frames contained in this asd file
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
    // Booked region of 12 byte
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
    typedef channel<N> channel_tag;
    typedef version<2> version_tag;
    static constexpr std::size_t num_channel = channel_tag::value;
    static constexpr std::size_t num_version = version_tag::value;

    typedef contT container_dispatcher_type;
    typedef typename contT::template rebind<std::int8_t>::other byte_array;
    typedef typename contT::template rebind<std::int32_t>::other i32_array;

    std::int32_t file_version;         //!< File version
    std::int32_t file_header_size;     //!< Size of the file header
    std::int32_t frame_header_size;    //!< Size of the frame header
    std::int32_t text_encoding;        //!< Characteristic number for text encoding (ex. “932” in case of Shift-JIS)
    std::int32_t operator_name_size;   //!< Size of the operator name
    std::int32_t comment_size;         //!< Size of the comment
    std::int32_t data_kind_1ch;        //!< Kind of AFM data of 1ch (Topography, Error or Phase) written by enum array (ex. Topography 0x5054)
    std::int32_t data_kind_2ch;        //!< Kind of AFM data of 2ch (Topography, Error or Phase) written by enum array (ex. Topography 0x5054)
    std::int32_t init_frame;           //!< Number of frames when this asd file was recorded
    std::int32_t num_frames;           //!< Number of frames contained in this asd file
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
    // Booked region of 12 byte
    std::int32_t machine_id;           //!< Number of the imaging machine
    std::int32_t AD_range;             //!< Code showing AD range (AD_1V, AD_2P5V, AD_5V or AD_10V)
    std::int32_t AD_resolution;        //!< AD resolution (When this value is 12, the AD resolution is 4096 (2^12).)
    float        x_max_scanning_range; //!< X maximum scanning range (nm)
    float        y_max_scanning_range; //!< Y maximum scanning range (nm)
    float        x_piezo_extension;    //!< X piezo extension coefficient (nm/V)
    float        y_piezo_extension;    //!< Y piezo extension coefficient (nm/V)
    float        z_piezo_extension;    //!< Z piezo extension coefficient (nm/V)
    float        z_piezo_gain;         //!< Z piezo driver gain
    byte_array   operator_name;        //!< Name of the operator
    byte_array   comment;              //!< Comment

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
