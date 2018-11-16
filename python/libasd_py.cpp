#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <libasd/libasd.hpp>
#include <fstream>

namespace py = pybind11;

void add_header_enums(py::module& mod) // {{{
{
    py::enum_<asd::AD_range>(mod, "AD_range", py::arithmetic())
        .value("unipolar_1.0V", asd::AD_range::unipolar_1_0V)
        .value("unipolar_2.5V", asd::AD_range::unipolar_2_5V)
        .value("unipolar_5.0V", asd::AD_range::unipolar_5_0V)
        .value("bipolar_1.0V",  asd::AD_range::bipolar_1_0V)
        .value("bipolar_2.5V",  asd::AD_range::bipolar_2_5V)
        .value("bipolar_5.0V",  asd::AD_range::bipolar_5_0V)
        .value("dummy_value",   asd::AD_range::dummy_value)
        .export_values();

    py::enum_<asd::scan_direction>(mod, "scan_direction", py::arithmetic())
        .value("x_forward",  asd::scan_direction::x_forward)
        .value("x_backward", asd::scan_direction::x_backward)
        .value("y_forward",  asd::scan_direction::y_forward)
        .value("y_backward", asd::scan_direction::y_backward)
        .export_values();

    py::enum_<asd::encode>(mod, "encode", py::arithmetic())
        .value("Shift_JIS", asd::encode::Shift_JIS)
        .value("UTF_16",    asd::encode::UTF_16)
        .value("EUC_JP",    asd::encode::EUC_JP)
        .value("UTF_8",     asd::encode::UTF_8)
        .value("JP_MAC",    asd::encode::JP_MAC)
        .value("ASCII",     asd::encode::ASCII)
        .export_values();

    py::enum_<asd::data_kind>(mod, "data_kind", py::arithmetic())
        .value("topography", asd::data_kind::topography)
        .value("error",      asd::data_kind::error)
        .value("phase",      asd::data_kind::phase)
        .value("none",       asd::data_kind::none)
        .export_values();

    return;
} // }}}

void add_header_classes(py::module& mod) // {{{
{
    py::class_<asd::Header<asd::version<0>>>(mod, "Header_v0", "header of asd version 0 file.")
        .def(py::init<>())
        .def_readwrite("file_version"       , &asd::Header<asd::version<0>>::file_version,
                       "type: Integer\nversion of the file. by definition, the value is 0.")
        .def_readwrite("data_type_1ch"      , &asd::Header<asd::version<0>>::data_type_1ch,
                       "type: libasd.data_kind\ndata type of channel 1. [topography|error|phase|none]")
        .def_readwrite("data_type_2ch"      , &asd::Header<asd::version<0>>::data_type_2ch,
                       "type: libasd.data_kind\ndata type of channel 2. [topography|error|phase|none]")
        .def_readwrite("file_header_size"   , &asd::Header<asd::version<0>>::file_header_size,
                       "type: Integer\nbyte size of a header of a file.")
        .def_readwrite("frame_header_size"  , &asd::Header<asd::version<0>>::frame_header_size,
                       "type: Integer\nbyte size of headers of frames.")
        .def_readwrite("operator_name_size" , &asd::Header<asd::version<0>>::operator_name_size,
                       "type: Integer\nlength of operator's name.")
        .def_readwrite("comment_offset_size", &asd::Header<asd::version<0>>::comment_offset_size,
                       "type: Integer\nbyte size of offset.")
        .def_readwrite("comment_size"       , &asd::Header<asd::version<0>>::comment_size,
                       "type: Integer\nlength of comments.")
        .def_readwrite("x_pixel"            , &asd::Header<asd::version<0>>::x_pixel,
                       "type: Integer\nnumber of pixels along the x axis in one frame")
        .def_readwrite("y_pixel"            , &asd::Header<asd::version<0>>::y_pixel,
                       "type: Integer\nnumber of pixels along the y axis in one frame")
        .def_readwrite("x_scanning_range"   , &asd::Header<asd::version<0>>::x_scanning_range,
                       "type: Integer\nscanning range along x axis [nm]")
        .def_readwrite("y_scanning_range"   , &asd::Header<asd::version<0>>::y_scanning_range,
                       "type: Integer\nscanning range along y axis [nm]")
        .def_readwrite("frame_rate"         , &asd::Header<asd::version<0>>::frame_rate,
                       "type: Float\nthe time it takes to obtain one frame [ms/frame]")
        .def_readwrite("z_piezo_extension"  , &asd::Header<asd::version<0>>::z_piezo_extension,
                       "type: Float\npiezo extention coefficient [nm/V]")
        .def_readwrite("z_piezo_gain"       , &asd::Header<asd::version<0>>::z_piezo_gain,
                       "type: Float\npiezo driver gain (electronics)")
        .def_readwrite("ad_range"           , &asd::Header<asd::version<0>>::ad_range,
                       "type: libasd.AD_range\nrange of the A-D converter used")
        .def_readwrite("bits_data"          , &asd::Header<asd::version<0>>::bits_data,
                       "type: Integer\nnumber of bits used in the frame data (usually 12)")
        .def_readwrite("is_averaged"        , &asd::Header<asd::version<0>>::is_averaged,
                       "type: Boolean\nwhether the frames are averaged or not")
        .def_readwrite("average_window"     , &asd::Header<asd::version<0>>::average_window,
                       "type: Integer\nnumber of frames used to average the frame data")
        .def_readwrite("year"               , &asd::Header<asd::version<0>>::year,
                       "type: Integer\ntime stamp")
        .def_readwrite("month"              , &asd::Header<asd::version<0>>::month,
                       "type: Integer\ntime stamp")
        .def_readwrite("day"                , &asd::Header<asd::version<0>>::day,
                       "type: Integer\ntime stamp")
        .def_readwrite("hour"               , &asd::Header<asd::version<0>>::hour,
                       "type: Integer\ntime stamp")
        .def_readwrite("minute"             , &asd::Header<asd::version<0>>::minute,
                       "type: Integer\ntime stamp")
        .def_readwrite("second"             , &asd::Header<asd::version<0>>::second,
                       "type: Integer\ntime stamp")
        .def_readwrite("rounding_degree"    , &asd::Header<asd::version<0>>::rounding_degree,
                       "type: Integer\ndegree of the rounding of x-scanning signal [%]")
        .def_readwrite("x_max_range"        , &asd::Header<asd::version<0>>::x_max_range,
                       "type: Float\nmaximum scanning range in the X axis[nm]")
        .def_readwrite("y_max_range"        , &asd::Header<asd::version<0>>::y_max_range,
                       "type: Float\nmaximum scanning range in the Y axis[nm]")
        .def_readwrite("init_frame"         , &asd::Header<asd::version<0>>::init_frame,
                       "type: Integer\nnumber of frames when this file was recorded")
        .def_readwrite("num_frames"         , &asd::Header<asd::version<0>>::num_frames,
                       "type: Integer\nnumber of frames contained in this file")
        .def_readwrite("machine_id"         , &asd::Header<asd::version<0>>::machine_id,
                       "type: Integer\nidentifier of the AFM")
        .def_readwrite("file_id"            , &asd::Header<asd::version<0>>::file_id,
                       "type: Integer\nidentifier of the file")
        .def_readwrite("operator_name"      , &asd::Header<asd::version<0>>::operator_name,
                       "type: byte array\nwho operated the AFM while imaging")
        .def_readwrite("sensor_sensitivity" , &asd::Header<asd::version<0>>::sensor_sensitivity,
                       "type: Float\nsensitivity of the sensor of canti-lever [nm/V]")
        .def_readwrite("phase_sensitivity"  , &asd::Header<asd::version<0>>::phase_sensitivity,
                       "type: Float\nsensitivity of the sensor of the phase [deg/V]")
        .def_readwrite("scannig_direction"  , &asd::Header<asd::version<0>>::scannig_direction,
                       "type: libasd.scan_direction\nscanning direction. [x|y]_[forward|backward]")
        .def_readwrite("comment"            , &asd::Header<asd::version<0>>::comment,
                       "type: byte array\ncomments!")
        ;

    py::class_<asd::Header<asd::version<1>>>(mod, "Header_v1")
        .def(py::init<>())
        .def_readwrite("file_version"        , &asd::Header<asd::version<1>>::file_version)
        .def_readwrite("file_header_size"    , &asd::Header<asd::version<1>>::file_header_size)
        .def_readwrite("frame_header_size"   , &asd::Header<asd::version<1>>::frame_header_size)
        .def_readwrite("text_encoding"       , &asd::Header<asd::version<1>>::text_encoding)
        .def_readwrite("operator_name_size"  , &asd::Header<asd::version<1>>::operator_name_size)
        .def_readwrite("comment_size"        , &asd::Header<asd::version<1>>::comment_size)
        .def_readwrite("data_kind_1ch"       , &asd::Header<asd::version<1>>::data_kind_1ch)
        .def_readwrite("data_kind_2ch"       , &asd::Header<asd::version<1>>::data_kind_2ch)
        .def_readwrite("init_frame"          , &asd::Header<asd::version<1>>::init_frame)
        .def_readwrite("num_frames"          , &asd::Header<asd::version<1>>::num_frames)
        .def_readwrite("scanning_direction"  , &asd::Header<asd::version<1>>::scanning_direction)
        .def_readwrite("file_id"             , &asd::Header<asd::version<1>>::file_id)
        .def_readwrite("x_pixel"             , &asd::Header<asd::version<1>>::x_pixel)
        .def_readwrite("y_pixel"             , &asd::Header<asd::version<1>>::y_pixel)
        .def_readwrite("x_scanning_range"    , &asd::Header<asd::version<1>>::x_scanning_range)
        .def_readwrite("y_scanning_range"    , &asd::Header<asd::version<1>>::y_scanning_range)
        .def_readwrite("is_averaged"         , &asd::Header<asd::version<1>>::is_averaged)
        .def_readwrite("average_window"      , &asd::Header<asd::version<1>>::average_window)
        .def_readwrite("year"                , &asd::Header<asd::version<1>>::year)
        .def_readwrite("month"               , &asd::Header<asd::version<1>>::month)
        .def_readwrite("day"                 , &asd::Header<asd::version<1>>::day)
        .def_readwrite("hour"                , &asd::Header<asd::version<1>>::hour)
        .def_readwrite("minute"              , &asd::Header<asd::version<1>>::minute)
        .def_readwrite("second"              , &asd::Header<asd::version<1>>::second)
        .def_readwrite("x_rounding_degree"   , &asd::Header<asd::version<1>>::x_rounding_degree)
        .def_readwrite("y_rounding_degree"   , &asd::Header<asd::version<1>>::y_rounding_degree)
        .def_readwrite("frame_acquision_time", &asd::Header<asd::version<1>>::frame_acquision_time)
        .def_readwrite("sensor_sensitivity"  , &asd::Header<asd::version<1>>::sensor_sensitivity)
        .def_readwrite("phase_sensitivity"   , &asd::Header<asd::version<1>>::phase_sensitivity)
        .def_readwrite("offset"              , &asd::Header<asd::version<1>>::offset)
        .def_readwrite("machine_id"          , &asd::Header<asd::version<1>>::machine_id)
        .def_readwrite("ad_range"            , &asd::Header<asd::version<1>>::ad_range)
        .def_readwrite("ad_resolution"       , &asd::Header<asd::version<1>>::ad_resolution)
        .def_readwrite("x_max_scanning_range", &asd::Header<asd::version<1>>::x_max_scanning_range)
        .def_readwrite("y_max_scanning_range", &asd::Header<asd::version<1>>::y_max_scanning_range)
        .def_readwrite("x_piezo_extension"   , &asd::Header<asd::version<1>>::x_piezo_extension)
        .def_readwrite("y_piezo_extension"   , &asd::Header<asd::version<1>>::y_piezo_extension)
        .def_readwrite("z_piezo_extension"   , &asd::Header<asd::version<1>>::z_piezo_extension)
        .def_readwrite("z_piezo_gain"        , &asd::Header<asd::version<1>>::z_piezo_gain)
        .def_readwrite("operator_name"       , &asd::Header<asd::version<1>>::operator_name)
        .def_readwrite("comment"             , &asd::Header<asd::version<1>>::comment)
        ;

    py::class_<asd::Header<asd::version<2>>>(mod, "Header_v2")
        .def(py::init<>())
        .def_readwrite("file_version"           , &asd::Header<asd::version<2>>::file_version)
        .def_readwrite("file_header_size"       , &asd::Header<asd::version<2>>::file_header_size)
        .def_readwrite("frame_header_size"      , &asd::Header<asd::version<2>>::frame_header_size)
        .def_readwrite("text_encoding"          , &asd::Header<asd::version<2>>::text_encoding)
        .def_readwrite("operator_name_size"     , &asd::Header<asd::version<2>>::operator_name_size)
        .def_readwrite("comment_size"           , &asd::Header<asd::version<2>>::comment_size)
        .def_readwrite("data_kind_1ch"          , &asd::Header<asd::version<2>>::data_kind_1ch)
        .def_readwrite("data_kind_2ch"          , &asd::Header<asd::version<2>>::data_kind_2ch)
        .def_readwrite("init_frame"             , &asd::Header<asd::version<2>>::init_frame)
        .def_readwrite("num_frames"             , &asd::Header<asd::version<2>>::num_frames)
        .def_readwrite("scanning_direction"     , &asd::Header<asd::version<2>>::scanning_direction)
        .def_readwrite("file_id"                , &asd::Header<asd::version<2>>::file_id)
        .def_readwrite("x_pixel"                , &asd::Header<asd::version<2>>::x_pixel)
        .def_readwrite("y_pixel"                , &asd::Header<asd::version<2>>::y_pixel)
        .def_readwrite("x_scanning_range"       , &asd::Header<asd::version<2>>::x_scanning_range)
        .def_readwrite("y_scanning_range"       , &asd::Header<asd::version<2>>::y_scanning_range)
        .def_readwrite("is_averaged"            , &asd::Header<asd::version<2>>::is_averaged)
        .def_readwrite("average_window"         , &asd::Header<asd::version<2>>::average_window)
        .def_readwrite("year"                   , &asd::Header<asd::version<2>>::year)
        .def_readwrite("month"                  , &asd::Header<asd::version<2>>::month)
        .def_readwrite("day"                    , &asd::Header<asd::version<2>>::day)
        .def_readwrite("hour"                   , &asd::Header<asd::version<2>>::hour)
        .def_readwrite("minute"                 , &asd::Header<asd::version<2>>::minute)
        .def_readwrite("second"                 , &asd::Header<asd::version<2>>::second)
        .def_readwrite("x_rounding_degree"      , &asd::Header<asd::version<2>>::x_rounding_degree)
        .def_readwrite("y_rounding_degree"      , &asd::Header<asd::version<2>>::y_rounding_degree)
        .def_readwrite("frame_acquision_time"   , &asd::Header<asd::version<2>>::frame_acquision_time)
        .def_readwrite("sensor_sensitivity"     , &asd::Header<asd::version<2>>::sensor_sensitivity)
        .def_readwrite("phase_sensitivity"      , &asd::Header<asd::version<2>>::phase_sensitivity)
        .def_readwrite("offset"                 , &asd::Header<asd::version<2>>::offset)
        .def_readwrite("machine_id"             , &asd::Header<asd::version<2>>::machine_id)
        .def_readwrite("ad_range"               , &asd::Header<asd::version<2>>::ad_range)
        .def_readwrite("ad_resolution"          , &asd::Header<asd::version<2>>::ad_resolution)
        .def_readwrite("x_max_scanning_range"   , &asd::Header<asd::version<2>>::x_max_scanning_range)
        .def_readwrite("y_max_scanning_range"   , &asd::Header<asd::version<2>>::y_max_scanning_range)
        .def_readwrite("x_piezo_extension"      , &asd::Header<asd::version<2>>::x_piezo_extension)
        .def_readwrite("y_piezo_extension"      , &asd::Header<asd::version<2>>::y_piezo_extension)
        .def_readwrite("z_piezo_extension"      , &asd::Header<asd::version<2>>::z_piezo_extension)
        .def_readwrite("z_piezo_gain"           , &asd::Header<asd::version<2>>::z_piezo_gain)
        .def_readwrite("operator_name"          , &asd::Header<asd::version<2>>::operator_name)
        .def_readwrite("comment"                , &asd::Header<asd::version<2>>::comment)
        .def_readwrite("number_of_frames"       , &asd::Header<asd::version<2>>::number_of_frames)
        .def_readwrite("is_x_feed_forward"      , &asd::Header<asd::version<2>>::is_x_feed_forward)
        .def_readwrite("x_feed_forward_i"       , &asd::Header<asd::version<2>>::x_feed_forward_i)
        .def_readwrite("x_feed_forward_d"       , &asd::Header<asd::version<2>>::x_feed_forward_d)
        .def_readwrite("max_color_scale"        , &asd::Header<asd::version<2>>::max_color_scale)
        .def_readwrite("min_color_scale"        , &asd::Header<asd::version<2>>::min_color_scale)
        .def_readwrite("anchor_point_size_red"  , &asd::Header<asd::version<2>>::anchor_point_size_red)
        .def_readwrite("anchor_point_size_green", &asd::Header<asd::version<2>>::anchor_point_size_green)
        .def_readwrite("anchor_point_size_blue ", &asd::Header<asd::version<2>>::anchor_point_size_blue)
        .def_readwrite("x_anchor_points_red"    , &asd::Header<asd::version<2>>::x_anchor_points_red)
        .def_readwrite("y_anchor_points_red"    , &asd::Header<asd::version<2>>::y_anchor_points_red)
        .def_readwrite("x_anchor_points_green"  , &asd::Header<asd::version<2>>::x_anchor_points_green)
        .def_readwrite("y_anchor_points_green"  , &asd::Header<asd::version<2>>::y_anchor_points_green)
        .def_readwrite("x_anchor_points_blue"   , &asd::Header<asd::version<2>>::x_anchor_points_blue)
        .def_readwrite("y_anchor_points_blue"   , &asd::Header<asd::version<2>>::y_anchor_points_blue)
        ;

    return;
} // }}}

void add_read_header(py::module& mod) // {{{
{
    mod.def("read_header_v0",
            [](const std::string& fname) -> asd::Header<asd::ver<0>> {
                std::ifstream ifs(fname);
                if(!ifs.good())
                {
                    throw std::runtime_error("file open error: " + fname);
                }
                return asd::read_header<asd::ver<0>>(ifs);
            },
            py::arg("file_name"),
            "A function which reads only header information of asd file.");

    mod.def("read_header_v1",
            [](const std::string& fname) -> asd::Header<asd::ver<1>> {
                std::ifstream ifs(fname);
                if(!ifs.good())
                {
                    throw std::runtime_error("file open error: " + fname);
                }
                return asd::read_header<asd::ver<1>>(ifs);
            },
            py::arg("file_name"),
            "A function which reads only header information of asd file.");

    mod.def("read_header_v2",
            [](const std::string& fname) -> asd::Header<asd::ver<2>> {
                std::ifstream ifs(fname);
                if(!ifs.good())
                {
                    throw std::runtime_error("file open error: " + fname);
                }
                return asd::read_header<asd::ver<2>>(ifs);
            },
            py::arg("file_name"),
            "A function which reads only header information of asd file.");

    mod.def("read_header",
        [](const std::string& fname) -> py::object {
            std::ifstream ifs(fname);
            if(!ifs.good())
            {
                throw std::runtime_error("file open error: " + fname);
            }

            const auto v = asd::read_version(ifs);
            switch(v)
            {
                case 0: return py::cast(asd::read_header<asd::version<0>>(ifs));
                case 1: return py::cast(asd::read_header<asd::version<1>>(ifs));
                case 2: return py::cast(asd::read_header<asd::version<2>>(ifs));
                default: throw std::invalid_argument(
                             "invalid asd version: " + std::to_string(v));
            }
        },
        py::arg("file_name"),
        "A function which reads only header information of asd file.\n"
        "It automatically reads the version information, and returns an "
        "appropreate header class.\n"
        "If the file you passed has invalid signature or some error occured "
        "while opening file, ValueError will be thrown.");
} // }}}

void add_frame_headers(py::module& mod) // {{{
{
    py::class_<asd::FrameHeader>(mod, "FrameHeader")
        .def(py::init<>())
        .def_readwrite("number",        &asd::FrameHeader::number)
        .def_readwrite("max_data",      &asd::FrameHeader::max_data)
        .def_readwrite("max_data",      &asd::FrameHeader::max_data)
        .def_readwrite("x_offset",      &asd::FrameHeader::x_offset)
        .def_readwrite("y_offset",      &asd::FrameHeader::y_offset)
        .def_readwrite("x_tilt",        &asd::FrameHeader::x_tilt)
        .def_readwrite("y_tilt",        &asd::FrameHeader::y_tilt)
        .def_readwrite("is_stimulated", &asd::FrameHeader::is_stimulated)
        ;
    return;
} // }}}

void add_frame_data(py::module& mod) // {{{
{
    py::class_<asd::FrameData<std::int16_t, asd::container::vec>
        >(mod, "RawFrameData", py::buffer_protocol())
        .def_buffer([](asd::FrameData<std::int16_t, asd::container::vec>& fd)
            -> py::buffer_info {
            return py::buffer_info(
                fd.base().data(), sizeof(std::int16_t),
                py::format_descriptor<std::int16_t>::format(),
                2, {fd.x_pixel(), fd.y_pixel()},
                {sizeof(std::int16_t) * fd.x_pixel(), sizeof(std::int16_t)}
            );
        });

    py::class_<asd::FrameData<double, asd::container::vec>
        >(mod, "FrameData", py::buffer_protocol())
        .def_buffer([](asd::FrameData<double, asd::container::vec>& fd)
            -> py::buffer_info {
            return py::buffer_info(
                fd.base().data(), sizeof(double),
                py::format_descriptor<double>::format(),
                2, {fd.x_pixel(), fd.y_pixel()},
                {sizeof(double) * fd.x_pixel(), sizeof(double)}
            );
        });
    return;
} // }}}

void add_frame_classes(py::module& mod) // {{{
{
    py::class_<asd::Frame<std::int16_t, asd::ch<1>, asd::container::vec>
        >(mod, "RawFrame1ch")
        .def(py::init<>())
        .def_readwrite("header", &asd::Frame<std::int16_t, asd::ch<1>, asd::container::vec>::header)
        .def_readwrite("data",   &asd::Frame<std::int16_t, asd::ch<1>, asd::container::vec>::data)
        ;

    py::class_<asd::Frame<std::int16_t, asd::ch<2>, asd::container::vec>
        >(mod, "RawFrame2ch")
        .def(py::init<>())
        .def_readwrite("header", &asd::Frame<std::int16_t, asd::ch<2>, asd::container::vec>::header)
        .def_readwrite("data",   &asd::Frame<std::int16_t, asd::ch<2>, asd::container::vec>::data)
        ;

    py::class_<asd::Frame<double, asd::ch<1>, asd::container::vec>
        >(mod, "Frame1ch")
        .def(py::init<>())
        .def_readwrite("header", &asd::Frame<double, asd::ch<1>, asd::container::vec>::header)
        .def_readwrite("data",   &asd::Frame<double, asd::ch<1>, asd::container::vec>::data)
        ;

    py::class_<asd::Frame<double, asd::ch<2>, asd::container::vec>
        >(mod, "Frame2ch")
        .def(py::init<>())
        .def_readwrite("header", &asd::Frame<double, asd::ch<2>, asd::container::vec>::header)
        .def_readwrite("data",   &asd::Frame<double, asd::ch<2>, asd::container::vec>::data)
        ;

    return;
} // }}}

template<typename dataT>
void add_data_class(py::module& mod, const char* name) // {{{
{
    py::class_<dataT>(mod, name)
        .def(py::init<>())
        .def_readwrite("header", &dataT::header)
        .def_readwrite("frames", &dataT::frames)
        ;
    return;
} // }}}

void add_data_classes(py::module& mod) // {{{
{
    add_data_class<
        asd::Data<std::int16_t, asd::ch<1>, asd::ver<0>, asd::container::vec>
        >(mod, "RawData1ch_v0");
    add_data_class<
        asd::Data<std::int16_t, asd::ch<1>, asd::ver<1>, asd::container::vec>
        >(mod, "RawData1ch_v1");
    add_data_class<
        asd::Data<std::int16_t, asd::ch<1>, asd::ver<2>, asd::container::vec>
        >(mod, "RawData1ch_v2");

    add_data_class<
        asd::Data<std::int16_t, asd::ch<2>, asd::ver<0>, asd::container::vec>
        >(mod, "RawData2ch_v0");
    add_data_class<
        asd::Data<std::int16_t, asd::ch<2>, asd::ver<1>, asd::container::vec>
        >(mod, "RawData2ch_v1");
    add_data_class<
        asd::Data<std::int16_t, asd::ch<2>, asd::ver<2>, asd::container::vec>
        >(mod, "RawData2ch_v2");

    add_data_class<
        asd::Data<double, asd::ch<1>, asd::ver<0>, asd::container::vec>
        >(mod, "Data1ch_v0");
    add_data_class<
        asd::Data<double, asd::ch<1>, asd::ver<1>, asd::container::vec>
        >(mod, "Data1ch_v1");
    add_data_class<
        asd::Data<double, asd::ch<1>, asd::ver<2>, asd::container::vec>
        >(mod, "Data1ch_v2");

    add_data_class<
        asd::Data<double, asd::ch<2>, asd::ver<0>, asd::container::vec>
        >(mod, "Data2ch_v0");
    add_data_class<
        asd::Data<double, asd::ch<2>, asd::ver<1>, asd::container::vec>
        >(mod, "Data2ch_v1");
    add_data_class<
        asd::Data<double, asd::ch<2>, asd::ver<2>, asd::container::vec>
        >(mod, "Data2ch_v2");
    return;
} // }}}

void add_read_asd(py::module& mod) // {{{
{
    mod.def("read_raw_data",
        [](const std::string& fname) -> py::object {
            std::ifstream ifs(fname);
            if(!ifs.good())
            {
                throw std::runtime_error("file open error: " + fname);
            }

            const auto v = asd::read_version(ifs);
            switch(v)
            {
                case 0:
                {
                    if(asd::read_header<asd::version<0>>(ifs).data_type_2ch ==
                            asd::data_kind::none)
                    {
                        ifs.seekg(0, std::ios_base::beg);
                        return py::cast(asd::read_raw_data<
                            asd::ch<1>, asd::ver<0>, asd::container::vec>(ifs));
                    }
                    else
                    {
                        ifs.seekg(0, std::ios_base::beg);
                        return py::cast(asd::read_raw_data<
                            asd::ch<2>, asd::ver<0>, asd::container::vec>(ifs));
                    }
                }
                case 1:
                {
                    if(asd::read_header<asd::version<1>>(ifs).data_kind_2ch ==
                            asd::data_kind::none)
                    {
                        ifs.seekg(0, std::ios_base::beg);
                        return py::cast(asd::read_raw_data<
                            asd::ch<1>, asd::ver<1>, asd::container::vec>(ifs));
                    }
                    else
                    {
                        ifs.seekg(0, std::ios_base::beg);
                        return py::cast(asd::read_raw_data<
                            asd::ch<2>, asd::ver<1>, asd::container::vec>(ifs));
                    }
                }
                case 2:
                {
                    if(asd::read_header<asd::version<2>>(ifs).data_kind_2ch ==
                            asd::data_kind::none)
                    {
                        ifs.seekg(0, std::ios_base::beg);
                        return py::cast(asd::read_raw_data<
                            asd::ch<1>, asd::ver<2>, asd::container::vec>(ifs));
                    }
                    else
                    {
                        ifs.seekg(0, std::ios_base::beg);
                        return py::cast(asd::read_raw_data<
                            asd::ch<2>, asd::ver<2>, asd::container::vec>(ifs));
                    }
                }
                default: throw std::invalid_argument(
                             "invalid asd version: " + std::to_string(v));
            }
        },
        py::arg("file_name"),
        "This function reads `.asd` file without converting signal "
        "to height information."
        );

    mod.def("read_asd",
        [](const std::string& fname) -> py::object {
            std::ifstream ifs(fname);
            if(!ifs.good())
            {
                throw std::runtime_error("file open error: " + fname);
            }

            const auto v = asd::read_version(ifs);
            switch(v)
            {
                case 0:
                {
                    if(asd::read_header<asd::version<0>>(ifs).data_type_2ch ==
                            asd::data_kind::none)
                    {
                        ifs.seekg(0, std::ios_base::beg);
                        return py::cast(asd::read_asd<double,
                            asd::ch<1>, asd::ver<0>, asd::container::vec>(ifs));
                    }
                    else
                    {
                        ifs.seekg(0, std::ios_base::beg);
                        return py::cast(asd::read_asd<double,
                            asd::ch<2>, asd::ver<0>, asd::container::vec>(ifs));
                    }
                }
                case 1:
                {
                    if(asd::read_header<asd::version<1>>(ifs).data_kind_2ch ==
                            asd::data_kind::none)
                    {
                        ifs.seekg(0, std::ios_base::beg);
                        return py::cast(asd::read_asd<double,
                            asd::ch<1>, asd::ver<1>, asd::container::vec>(ifs));
                    }
                    else
                    {
                        ifs.seekg(0, std::ios_base::beg);
                        return py::cast(asd::read_asd<double,
                            asd::ch<2>, asd::ver<1>, asd::container::vec>(ifs));
                    }
                }
                case 2:
                {
                    if(asd::read_header<asd::version<2>>(ifs).data_kind_2ch ==
                            asd::data_kind::none)
                    {
                        ifs.seekg(0, std::ios_base::beg);
                        return py::cast(asd::read_asd<double,
                            asd::ch<1>, asd::ver<2>, asd::container::vec>(ifs));
                    }
                    else
                    {
                        ifs.seekg(0, std::ios_base::beg);
                        return py::cast(asd::read_asd<double,
                            asd::ch<2>, asd::ver<2>, asd::container::vec>(ifs));
                    }
                }
                default: throw std::invalid_argument(
                             "invalid asd version: " + std::to_string(v));
            }
        },
        py::arg("file_name"),
        "This function reads `.asd` file after converting signal "
        "to height information."
        );

    return;
} // }}}

PYBIND11_MODULE(libasd, mod)
{
    mod.doc() = "libasd -- library to read and write .asd format file";

    add_header_enums  (mod);
    add_header_classes(mod);
    add_frame_headers (mod);
    add_frame_data    (mod);
    add_frame_classes (mod);
    add_data_classes  (mod);
    add_read_header   (mod);
    add_read_asd      (mod);
}
