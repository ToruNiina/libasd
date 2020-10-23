#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <libasd/libasd.hpp>
#include <fstream>
#include <sstream>

namespace py = pybind11;

void add_header_enums(py::module& mod) // {{{
{
    py::enum_<asd::AD_range>(mod, "AD_range", py::arithmetic())
        .value("unipolar_1_0V", asd::AD_range::unipolar_1_0V)
        .value("unipolar_2_5V", asd::AD_range::unipolar_2_5V)
        .value("unipolar_5_0V", asd::AD_range::unipolar_5_0V)
        .value("bipolar_1_0V",  asd::AD_range::bipolar_1_0V)
        .value("bipolar_2_5V",  asd::AD_range::bipolar_2_5V)
        .value("bipolar_5_0V",  asd::AD_range::bipolar_5_0V)
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

    mod.def("to_voltage",
        [](const asd::AD_range range) -> std::pair<double, double> {
            return asd::to_voltage<double>(range);
        }, py::arg("AD_range"), "This function converts AD_range enum into a "
        "pair of floats that correspond to the minimum and maximum voltage [V]."
        );

    return;
} // }}}

void add_header_classes(py::module& mod) // {{{
{
    py::class_<asd::Header<asd::version<0>>>(mod, "Header_v0", "header of asd file version 0.")
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
        .def("__repr__", [](const asd::Header<asd::version<0>>& self){
                std::ostringstream oss; oss << std::fixed << std::boolalpha;
                oss << "version           : " << self.file_version       << '\n';
                oss << "timestamp         : " << self.year   << '/' << self.month  << '/'
                                              << self.day    << ' ' << self.hour   << ':'
                                              << self.minute << ':' << self.second << '\n';
                oss << "data_kind_1ch     : " << self.data_type_1ch      << '\n';
                oss << "data_kind_2ch     : " << self.data_type_2ch      << '\n';
                oss << "num_frames        : " << self.num_frames         << '\n';
                oss << "x_pixel           : " << self.x_pixel            << '\n';
                oss << "y_pixel           : " << self.y_pixel            << '\n';
                oss << "x_scanning_range  : " << self.x_scanning_range   << '\n';
                oss << "y_scanning_range  : " << self.y_scanning_range   << '\n';
                oss << "x_max_range       : " << self.x_max_range        << '\n';
                oss << "y_max_range       : " << self.y_max_range        << '\n';
                oss << "is_averaged       : " << self.is_averaged        << '\n';
                oss << "average_window    : " << self.average_window     << '\n';
                oss << "rounding_degree   : " << self.rounding_degree  << '\n';
                oss << "machine_id        : " << self.machine_id         << '\n';
                oss << "ad_range          : " << self.ad_range           << '\n';
                oss << "bits_data         : " << self.bits_data          << '\n';
                oss << "z_piezo_extension : " << self.z_piezo_extension  << '\n';
                oss << "z_piezo_gain      : " << self.z_piezo_gain       << '\n';
                return oss.str();
            })
        ;

    py::class_<asd::Header<asd::version<1>>>(mod, "Header_v1", "header of asd file version 1.")
        .def(py::init<>())
        .def_readwrite("file_version"        , &asd::Header<asd::version<1>>::file_version,
                       "type: Integer\nversion of the file. by definition, the value is 1.")
        .def_readwrite("file_header_size"    , &asd::Header<asd::version<1>>::file_header_size,
                       "type: Integer\nsize of header region.")
        .def_readwrite("frame_header_size"   , &asd::Header<asd::version<1>>::frame_header_size,
                       "type: Integer\nsize of frame header region.")
        .def_readwrite("text_encoding"       , &asd::Header<asd::version<1>>::text_encoding,
                       "type: libasd.encode\ntext encoding used in this file")
        .def_readwrite("operator_name_size"  , &asd::Header<asd::version<1>>::operator_name_size,
                       "type: Integer\nlength of operator name")
        .def_readwrite("comment_size"        , &asd::Header<asd::version<1>>::comment_size,
                       "type: Integer\nlength of comment")
        .def_readwrite("data_kind_1ch"       , &asd::Header<asd::version<1>>::data_kind_1ch,
                       "type: libasd.data_kind\ndata type of channel 1. [topography|error|phase|none]")
        .def_readwrite("data_kind_2ch"       , &asd::Header<asd::version<1>>::data_kind_2ch,
                       "type: libasd.data_kind\ndata type of channel 2. [topography|error|phase|none]")
        .def_readwrite("init_frame"          , &asd::Header<asd::version<1>>::init_frame,
                       "type: Integer\nnumber of frames when this file was recorded")
        .def_readwrite("num_frames"          , &asd::Header<asd::version<1>>::num_frames,
                       "type: Integer\nnumber of frames contained in this file")
        .def_readwrite("scanning_direction"  , &asd::Header<asd::version<1>>::scanning_direction,
                       "type: libasd.scan_direction\nscanning direction. [x|y]_[forward|backward]")
        .def_readwrite("file_id"             , &asd::Header<asd::version<1>>::file_id,
                       "type: Integer\nidentifier of the file")
        .def_readwrite("x_pixel"             , &asd::Header<asd::version<1>>::x_pixel,
                       "type: Integer\nnumber of pixels along the x axis in one frame")
        .def_readwrite("y_pixel"             , &asd::Header<asd::version<1>>::y_pixel,
                       "type: Integer\nnumber of pixels along the y axis in one frame")
        .def_readwrite("x_scanning_range"    , &asd::Header<asd::version<1>>::x_scanning_range,
                       "type: Integer\nscanning range along x axis [nm]")
        .def_readwrite("y_scanning_range"    , &asd::Header<asd::version<1>>::y_scanning_range,
                       "type: Integer\nscanning range along y axis [nm]")
        .def_readwrite("is_averaged"         , &asd::Header<asd::version<1>>::is_averaged,
                       "type: Boolean\nwhether the frames are averaged or not")
        .def_readwrite("average_window"      , &asd::Header<asd::version<1>>::average_window,
                       "type: Integer\nnumber of frames used to average the frame data")
        .def_readwrite("year"                , &asd::Header<asd::version<1>>::year,
                       "type: Integer\ntime stamp")
        .def_readwrite("month"               , &asd::Header<asd::version<1>>::month,
                       "type: Integer\ntime stamp")
        .def_readwrite("day"                 , &asd::Header<asd::version<1>>::day,
                       "type: Integer\ntime stamp")
        .def_readwrite("hour"                , &asd::Header<asd::version<1>>::hour,
                       "type: Integer\ntime stamp")
        .def_readwrite("minute"              , &asd::Header<asd::version<1>>::minute,
                       "type: Integer\ntime stamp")
        .def_readwrite("second"              , &asd::Header<asd::version<1>>::second,
                       "type: Integer\ntime stamp")
        .def_readwrite("x_rounding_degree"   , &asd::Header<asd::version<1>>::x_rounding_degree,
                       "type: Integer\ndegree of the rounding of x-scanning signal [%]")
        .def_readwrite("y_rounding_degree"   , &asd::Header<asd::version<1>>::y_rounding_degree,
                       "type: Integer\ndegree of the rounding of y-scanning signal [%]")
        .def_readwrite("frame_acquision_time", &asd::Header<asd::version<1>>::frame_acquision_time,
                       "type: Float\nthe time it takes to obtain one frame [ms]")
        .def_readwrite("sensor_sensitivity"  , &asd::Header<asd::version<1>>::sensor_sensitivity,
                       "type: Float\nsensitivity of the sensor of canti-lever [nm/V]")
        .def_readwrite("phase_sensitivity"   , &asd::Header<asd::version<1>>::phase_sensitivity,
                       "type: Float\nsensitivity of the sensor of the phase [deg/V]")
        .def_readwrite("offset"              , &asd::Header<asd::version<1>>::offset,
                       "type: Integer\noffset. internal use only")
        .def_readwrite("machine_id"          , &asd::Header<asd::version<1>>::machine_id,
                       "type: Integer\nidentifier of the AFM")
        .def_readwrite("ad_range"            , &asd::Header<asd::version<1>>::ad_range,
                       "type: libasd.AD_range\nrange of the A-D converter used")
        .def_readwrite("ad_resolution"       , &asd::Header<asd::version<1>>::ad_resolution,
                       "type: Integer\nnumber of bits used in the frame data (usually 12)")
        .def_readwrite("x_max_scanning_range", &asd::Header<asd::version<1>>::x_max_scanning_range,
                       "type: Float\nmaximum scanning range in the X axis[nm]")
        .def_readwrite("y_max_scanning_range", &asd::Header<asd::version<1>>::y_max_scanning_range,
                       "type: Float\nmaximum scanning range in the Y axis[nm]")
        .def_readwrite("x_piezo_extension"   , &asd::Header<asd::version<1>>::x_piezo_extension,
                       "type: Float\npiezo extention coefficient [nm/V]")
        .def_readwrite("y_piezo_extension"   , &asd::Header<asd::version<1>>::y_piezo_extension,
                       "type: Float\npiezo extention coefficient [nm/V]")
        .def_readwrite("z_piezo_extension"   , &asd::Header<asd::version<1>>::z_piezo_extension,
                       "type: Float\npiezo extention coefficient [nm/V]")
        .def_readwrite("z_piezo_gain"        , &asd::Header<asd::version<1>>::z_piezo_gain,
                       "type: Float\npiezo driver gain (electronics)")
        .def_readwrite("operator_name"       , &asd::Header<asd::version<1>>::operator_name,
                       "type: byte array\nwho operated the AFM while imaging")
        .def_readwrite("comment"             , &asd::Header<asd::version<1>>::comment,
                       "type: byte array\ncomments!")
        .def("__repr__", [](const asd::Header<asd::version<1>>& self){
                std::ostringstream oss; oss << std::fixed << std::boolalpha;
                oss << "version             : " << self.file_version       << '\n';
                oss << "timestamp           : " << self.year   << '/' << self.month  << '/'
                                                << self.day    << ' ' << self.hour   << ':'
                                                << self.minute << ':' << self.second << '\n';
                oss << "data_kind_1ch       : " << self.data_kind_1ch      << '\n';
                oss << "data_kind_2ch       : " << self.data_kind_2ch      << '\n';
                oss << "num_frames          : " << self.num_frames         << '\n';
                oss << "scanning_direction  : " << self.scanning_direction << '\n';
                oss << "x_pixel             : " << self.x_pixel            << '\n';
                oss << "y_pixel             : " << self.y_pixel            << '\n';
                oss << "x_scanning_range    : " << self.x_scanning_range   << '\n';
                oss << "y_scanning_range    : " << self.y_scanning_range   << '\n';
                oss << "x_max_scanning_range: " << self.x_max_scanning_range << '\n';
                oss << "y_max_scanning_range: " << self.y_max_scanning_range << '\n';
                oss << "is_averaged         : " << self.is_averaged        << '\n';
                oss << "average_window      : " << self.average_window     << '\n';
                oss << "x_rounding_degree   : " << self.x_rounding_degree  << '\n';
                oss << "y_rounding_degree   : " << self.y_rounding_degree  << '\n';
                oss << "machine_id          : " << self.machine_id         << '\n';
                oss << "ad_range            : " << self.ad_range           << '\n';
                oss << "ad_resolution       : " << self.ad_resolution      << '\n';
                oss << "x_piezo_extension   : " << self.x_piezo_extension  << '\n';
                oss << "y_piezo_extension   : " << self.y_piezo_extension  << '\n';
                oss << "z_piezo_extension   : " << self.z_piezo_extension  << '\n';
                oss << "z_piezo_gain        : " << self.z_piezo_gain       << '\n';
                return oss.str();
            })
        ;

    py::class_<asd::Header<asd::version<2>>>(mod, "Header_v2")
        .def(py::init<>())
        .def_readwrite("file_version"           , &asd::Header<asd::version<2>>::file_version,
                       "type: Integer\nversion of the file. by definition, the value is 1.")
        .def_readwrite("file_header_size"       , &asd::Header<asd::version<2>>::file_header_size,
                       "type: Integer\nsize of header region.")
        .def_readwrite("frame_header_size"      , &asd::Header<asd::version<2>>::frame_header_size,
                       "type: Integer\nsize of frame header region.")
        .def_readwrite("text_encoding"          , &asd::Header<asd::version<2>>::text_encoding,
                       "type: libasd.encode\ntext encoding used in this file")
        .def_readwrite("operator_name_size"     , &asd::Header<asd::version<2>>::operator_name_size,
                       "type: Integer\nlength of operator name")
        .def_readwrite("comment_size"           , &asd::Header<asd::version<2>>::comment_size,
                       "type: Integer\nlength of comment")
        .def_readwrite("data_kind_1ch"          , &asd::Header<asd::version<2>>::data_kind_1ch,
                       "type: libasd.data_kind\ndata type of channel 1. [topography|error|phase|none]")
        .def_readwrite("data_kind_2ch"          , &asd::Header<asd::version<2>>::data_kind_2ch,
                       "type: libasd.data_kind\ndata type of channel 2. [topography|error|phase|none]")
        .def_readwrite("init_frame"             , &asd::Header<asd::version<2>>::init_frame,
                       "type: Integer\nnumber of frames when this file was recorded")
        .def_readwrite("num_frames"             , &asd::Header<asd::version<2>>::num_frames,
                       "type: Integer\nnumber of frames contained in this file")
        .def_readwrite("scanning_direction"     , &asd::Header<asd::version<2>>::scanning_direction,
                       "type: libasd.scan_direction\nscanning direction. [x|y]_[forward|backward]")
        .def_readwrite("file_id"                , &asd::Header<asd::version<2>>::file_id,
                       "type: Integer\nidentifier of the file")
        .def_readwrite("x_pixel"                , &asd::Header<asd::version<2>>::x_pixel,
                       "type: Integer\nnumber of pixels along the x axis in one frame")
        .def_readwrite("y_pixel"                , &asd::Header<asd::version<2>>::y_pixel,
                       "type: Integer\nnumber of pixels along the y axis in one frame")
        .def_readwrite("x_scanning_range"       , &asd::Header<asd::version<2>>::x_scanning_range,
                       "type: Integer\nscanning range along x axis [nm]")
        .def_readwrite("y_scanning_range"       , &asd::Header<asd::version<2>>::y_scanning_range,
                       "type: Integer\nscanning range along y axis [nm]")
        .def_readwrite("is_averaged"            , &asd::Header<asd::version<2>>::is_averaged,
                       "type: Boolean\nwhether the frames are averaged or not")
        .def_readwrite("average_window"         , &asd::Header<asd::version<2>>::average_window,
                       "type: Integer\nnumber of frames used to average the frame data")
        .def_readwrite("year"                   , &asd::Header<asd::version<2>>::year,
                       "type: Integer\ntime stamp")
        .def_readwrite("month"                  , &asd::Header<asd::version<2>>::month,
                       "type: Integer\ntime stamp")
        .def_readwrite("day"                    , &asd::Header<asd::version<2>>::day,
                       "type: Integer\ntime stamp")
        .def_readwrite("hour"                   , &asd::Header<asd::version<2>>::hour,
                       "type: Integer\ntime stamp")
        .def_readwrite("minute"                 , &asd::Header<asd::version<2>>::minute,
                       "type: Integer\ntime stamp")
        .def_readwrite("second"                 , &asd::Header<asd::version<2>>::second,
                       "type: Integer\ntime stamp")
        .def_readwrite("x_rounding_degree"      , &asd::Header<asd::version<2>>::x_rounding_degree,
                       "type: Integer\ndegree of the rounding of x-scanning signal [%]")
        .def_readwrite("y_rounding_degree"      , &asd::Header<asd::version<2>>::y_rounding_degree,
                       "type: Integer\ndegree of the rounding of y-scanning signal [%]")
        .def_readwrite("frame_acquision_time"   , &asd::Header<asd::version<2>>::frame_acquision_time,
                       "type: Float\nthe time it takes to obtain one frame [ms]")
        .def_readwrite("sensor_sensitivity"     , &asd::Header<asd::version<2>>::sensor_sensitivity,
                       "type: Float\nsensitivity of the sensor of canti-lever [nm/V]")
        .def_readwrite("phase_sensitivity"      , &asd::Header<asd::version<2>>::phase_sensitivity,
                       "type: Float\nsensitivity of the sensor of the phase [deg/V]")
        .def_readwrite("offset"                 , &asd::Header<asd::version<2>>::offset,
                       "type: Integer\noffset. internal use only")
        .def_readwrite("machine_id"             , &asd::Header<asd::version<2>>::machine_id,
                       "type: Integer\nidentifier of the AFM")
        .def_readwrite("ad_range"               , &asd::Header<asd::version<2>>::ad_range,
                       "type: libasd.AD_range\nrange of the A-D converter used")
        .def_readwrite("ad_resolution"          , &asd::Header<asd::version<2>>::ad_resolution,
                       "type: Integer\nnumber of bits used in the frame data (usually 12)")
        .def_readwrite("x_max_scanning_range"   , &asd::Header<asd::version<2>>::x_max_scanning_range,
                       "type: Float\nmaximum scanning range in the X axis[nm]")
        .def_readwrite("y_max_scanning_range"   , &asd::Header<asd::version<2>>::y_max_scanning_range,
                       "type: Float\nmaximum scanning range in the Y axis[nm]")
        .def_readwrite("x_piezo_extension"      , &asd::Header<asd::version<2>>::x_piezo_extension,
                       "type: Float\npiezo extention coefficient [nm/V]")
        .def_readwrite("y_piezo_extension"      , &asd::Header<asd::version<2>>::y_piezo_extension,
                       "type: Float\npiezo extention coefficient [nm/V]")
        .def_readwrite("z_piezo_extension"      , &asd::Header<asd::version<2>>::z_piezo_extension,
                       "type: Float\npiezo extention coefficient [nm/V]")
        .def_readwrite("z_piezo_gain"           , &asd::Header<asd::version<2>>::z_piezo_gain,
                       "type: Float\npiezo driver gain (electronics)")
        .def_readwrite("operator_name"          , &asd::Header<asd::version<2>>::operator_name,
                       "type: byte array\nwho operated the AFM while imaging")
        .def_readwrite("comment"                , &asd::Header<asd::version<2>>::comment,
                       "type: byte array\ncomments!")
        .def_readwrite("number_of_frames"       , &asd::Header<asd::version<2>>::number_of_frames,
                       "type: Integer\nnumber of frames contained in this file (same as num_frames)")
        .def_readwrite("is_x_feed_forward"      , &asd::Header<asd::version<2>>::is_x_feed_forward,
                       "type: Boolean\nflag for X-line feed-forward")
        .def_readwrite("x_feed_forward_i"       , &asd::Header<asd::version<2>>::x_feed_forward_i,
                       "type: Integer\nparameter for X-line feed-forward")
        .def_readwrite("x_feed_forward_d"       , &asd::Header<asd::version<2>>::x_feed_forward_d,
                       "type: Float\nparameter for X-line feed-forward")
        .def_readwrite("max_color_scale"        , &asd::Header<asd::version<2>>::max_color_scale,
                       "type: Integer\nmaximum value of color scale (0-255)")
        .def_readwrite("min_color_scale"        , &asd::Header<asd::version<2>>::min_color_scale,
                       "type: Integer\nminimum value of color scale (0-255)")
        .def_readwrite("anchor_point_size_red"  , &asd::Header<asd::version<2>>::anchor_point_size_red,
                       "type: Integer\nnumber of anchor points for color collection (red)")
        .def_readwrite("anchor_point_size_green", &asd::Header<asd::version<2>>::anchor_point_size_green,
                       "type: Integer\nnumber of anchor points for color collection (green)")
        .def_readwrite("anchor_point_size_blue ", &asd::Header<asd::version<2>>::anchor_point_size_blue,
                       "type: Integer\nnumber of anchor points for color collection (blue)")
        .def_readwrite("x_anchor_points_red"    , &asd::Header<asd::version<2>>::x_anchor_points_red,
                       "type: List of Integers\nX value of the anchor points (red)")
        .def_readwrite("y_anchor_points_red"    , &asd::Header<asd::version<2>>::y_anchor_points_red,
                       "type: List of Integers\nY value of the anchor points (red)")
        .def_readwrite("x_anchor_points_green"  , &asd::Header<asd::version<2>>::x_anchor_points_green,
                       "type: List of Integers\nX value of the anchor points (green)")
        .def_readwrite("y_anchor_points_green"  , &asd::Header<asd::version<2>>::y_anchor_points_green,
                       "type: List of Integers\nY value of the anchor points (green)")
        .def_readwrite("x_anchor_points_blue"   , &asd::Header<asd::version<2>>::x_anchor_points_blue,
                       "type: List of Integers\nX value of the anchor points (blue)")
        .def_readwrite("y_anchor_points_blue"   , &asd::Header<asd::version<2>>::y_anchor_points_blue,
                       "type: List of Integers\nY value of the anchor points (blue)")
        .def("__repr__", [](const asd::Header<asd::version<2>>& self){
                std::ostringstream oss; oss << std::fixed << std::boolalpha;
                oss << "version             : " << self.file_version       << '\n';
                oss << "timestamp           : " << self.year   << '/' << self.month  << '/'
                                                << self.day    << ' ' << self.hour   << ':'
                                                << self.minute << ':' << self.second << '\n';
                oss << "data_kind_1ch       : " << self.data_kind_1ch      << '\n';
                oss << "data_kind_2ch       : " << self.data_kind_2ch      << '\n';
                oss << "num_frames          : " << self.num_frames         << '\n';
                oss << "scanning_direction  : " << self.scanning_direction << '\n';
                oss << "x_pixel             : " << self.x_pixel            << '\n';
                oss << "y_pixel             : " << self.y_pixel            << '\n';
                oss << "x_scanning_range    : " << self.x_scanning_range   << '\n';
                oss << "y_scanning_range    : " << self.y_scanning_range   << '\n';
                oss << "x_max_scanning_range: " << self.x_max_scanning_range << '\n';
                oss << "y_max_scanning_range: " << self.y_max_scanning_range << '\n';
                oss << "is_averaged         : " << self.is_averaged        << '\n';
                oss << "average_window      : " << self.average_window     << '\n';
                oss << "x_rounding_degree   : " << self.x_rounding_degree  << '\n';
                oss << "y_rounding_degree   : " << self.y_rounding_degree  << '\n';
                oss << "machine_id          : " << self.machine_id         << '\n';
                oss << "ad_range            : " << self.ad_range           << '\n';
                oss << "ad_resolution       : " << self.ad_resolution      << '\n';
                oss << "x_piezo_extension   : " << self.x_piezo_extension  << '\n';
                oss << "y_piezo_extension   : " << self.y_piezo_extension  << '\n';
                oss << "z_piezo_extension   : " << self.z_piezo_extension  << '\n';
                oss << "z_piezo_gain        : " << self.z_piezo_gain       << '\n';
                return oss.str();
            })

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
            "A function which reads only header information of asd file.\n"
            "Return:\n-- Header_v0: header information of version 0");

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
            "A function which reads only header information of asd file.\n"
            "Return:\n-- Header_v1: header information of version 1");

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
            "A function which reads only header information of asd file."
            "Return:\n-- Header_v2: header information of version 2");

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
        "Return:\n-- Header: header information.\n"
        "It automatically reads the version information, and returns an "
        "appropreate header class.\n"
        "If the file you passed has invalid signature or some error occured "
        "while opening file, ValueError will be thrown.");
} // }}}

void add_frame_headers(py::module& mod) // {{{
{
    py::class_<asd::FrameHeader>(mod, "FrameHeader")
        .def(py::init<>())
        .def_readwrite("number",        &asd::FrameHeader::number,
                       "type: Integer\ncurrent frame index")
        .def_readwrite("max_data",      &asd::FrameHeader::max_data,
                       "type: Integer\nmaximum value included in this frame")
        .def_readwrite("min_data",      &asd::FrameHeader::min_data,
                       "type: Integer\nminimum value included in this frame")
        .def_readwrite("x_offset",      &asd::FrameHeader::x_offset,
                       "type: Integer\ncurrent location of cantilever in X axis")
        .def_readwrite("y_offset",      &asd::FrameHeader::y_offset,
                       "type: Integer\ncurrent location of cantilever in Y axis")
        .def_readwrite("x_tilt",        &asd::FrameHeader::x_tilt,
                       "type: Float\ntilt for X")
        .def_readwrite("y_tilt",        &asd::FrameHeader::y_tilt,
                       "type: Float\ntilt for Y")
        .def_readwrite("is_stimulated", &asd::FrameHeader::is_stimulated,
                       "type: Boolean\nflag for external stimuli")
        .def("__repr__", [](const asd::FrameHeader& self) {
                std::ostringstream oss; oss << std::boolalpha;
                oss << "index        : " << self.number   << '\n';
                oss << "max_data     : " << self.max_data << '\n';
                oss << "min_data     : " << self.min_data << '\n';
                oss << "x_offset     : " << self.x_offset << '\n';
                oss << "y_offset     : " << self.y_offset << '\n';
                oss << "x_tilt       : " << self.x_tilt   << '\n';
                oss << "y_tilt       : " << self.y_tilt   << '\n';
                oss << "is_stimulated: " << self.y_tilt   << '\n';
                return oss.str();
            })
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
                fd.base().data(),
                sizeof(std::int16_t),
                py::format_descriptor<std::int16_t>::format(),
                2,
                {fd.y_pixel(), fd.x_pixel()},
                {sizeof(std::int16_t) * fd.x_pixel(), sizeof(std::int16_t)}
            );
        });

    py::class_<asd::FrameData<double, asd::container::vec>
        >(mod, "FrameData", py::buffer_protocol())
        .def_buffer([](asd::FrameData<double, asd::container::vec>& fd)
            -> py::buffer_info {
            return py::buffer_info(
                fd.base().data(),
                sizeof(double),
                py::format_descriptor<double>::format(),
                2,
                {fd.y_pixel(), fd.x_pixel()},
                {sizeof(double) * fd.x_pixel(), sizeof(double)}
            );
        });
    return;
} // }}}

void add_frame_classes(py::module& mod) // {{{
{
    py::class_<asd::Frame<std::int16_t, asd::container::vec>
        >(mod, "RawFrame")
        .def(py::init<>())
        .def_readwrite("header", &asd::Frame<std::int16_t, asd::container::vec>::header,
                       "type: FrameHeader\nheader of this frame")
        .def_readwrite("data",   &asd::Frame<std::int16_t, asd::container::vec>::data,
                       "type: FrameData<Integer>\narray that represents the image. raw data without conversion into [nm]")
        .def("image", [](asd::Frame<std::int16_t, asd::container::vec>* self)
                -> py::array_t<std::int16_t> {
                return py::array_t<std::int16_t>(py::buffer_info(
                    self->data.base().data(),
                    sizeof(std::int16_t),
                    py::format_descriptor<std::int16_t>::format(),
                    2,
                    {self->data.y_pixel(), self->data.x_pixel()},
                    {sizeof(std::int16_t) * self->data.x_pixel(), sizeof(std::int16_t)}
                ));
            },
            "array that represents the image.\n"
            "each pixel represents the heigths at that point in [nm].\n"
            "The data is the same as `data` member variable.")
        ;

    py::class_<asd::Frame<double, asd::container::vec>
        >(mod, "Frame")
        .def(py::init<>())
        .def_readwrite("header", &asd::Frame<double, asd::container::vec>::header,
                       "type: FrameHeader\nheader of this frame")
        .def_readwrite("data",   &asd::Frame<double, asd::container::vec>::data,
                       "type: FrameData<Float>\narray that represents the heights of each pixel in [nm]")
        .def("image", [](asd::Frame<double, asd::container::vec>* self)
                -> py::array_t<double> {
                return py::array_t<double>(py::buffer_info(
                    self->data.base().data(),
                    sizeof(double),
                    py::format_descriptor<double>::format(),
                    2,
                    {self->data.y_pixel(), self->data.x_pixel()},
                    {sizeof(double) * self->data.x_pixel(), sizeof(double)}
                ));
            },
            "array that represents the image.\n"
            "each pixel represents the heigths at that point in [nm].\n"
            "The data is the same as `data` member variable.")
        ;
    return;
} // }}}

// add_data_class {{{
template<typename dataT>
typename std::enable_if<dataT::num_channel == 1>::type
add_data_class(py::module& mod, const char* name,
                    const char* doc_header, const char* doc_frames)
{
    py::class_<dataT>(mod, name)
        .def(py::init<>())
        .def_readwrite("header", &dataT::header, doc_header)
        .def_readwrite("frames", &dataT::frames, doc_frames)
        ;
    return;
}
template<typename dataT>
typename std::enable_if<dataT::num_channel != 1>::type
add_data_class(py::module& mod, const char* name,
                    const char* doc_header, const char* doc_frames)
{
    py::class_<dataT>(mod, name)
        .def(py::init<>())
        .def_readwrite("header",   &dataT::header,   doc_header)
        .def_readwrite("channels", &dataT::channels, doc_frames)
        ;
    return;
}

void add_data_classes(py::module& mod)
{

    add_data_class<
        asd::Data<std::int16_t, asd::ch<1>, asd::ver<0>, asd::container::vec>
        >(mod, "RawData1ch_v0", "type: Header_v0\nheader information",
          "type: Array<Array<FrameData>>\nchannels (array of frames)");
    add_data_class<
        asd::Data<std::int16_t, asd::ch<1>, asd::ver<1>, asd::container::vec>
        >(mod, "RawData1ch_v1", "type: Header_v1\nheader information",
          "type: Array<Array<FrameData>>\nchannels (array of frames)");
    add_data_class<
        asd::Data<std::int16_t, asd::ch<1>, asd::ver<2>, asd::container::vec>
        >(mod, "RawData1ch_v2", "type: Header_v2\nheader information",
          "type: Array<Array<FrameData>>\nchannels (array of frames)");

    add_data_class<
        asd::Data<std::int16_t, asd::ch<2>, asd::ver<0>, asd::container::vec>
        >(mod, "RawData2ch_v0", "type: Header_v0\nheader information", "type: Array<FrameData>\nframes");
    add_data_class<
        asd::Data<std::int16_t, asd::ch<2>, asd::ver<1>, asd::container::vec>
        >(mod, "RawData2ch_v1", "type: Header_v1\nheader information", "type: Array<FrameData>\nframes");
    add_data_class<
        asd::Data<std::int16_t, asd::ch<2>, asd::ver<2>, asd::container::vec>
        >(mod, "RawData2ch_v2", "type: Header_v2\nheader information", "type: Array<FrameData>\nframes");

    add_data_class<
        asd::Data<double, asd::ch<1>, asd::ver<0>, asd::container::vec>
        >(mod, "Data1ch_v0", "type: Header_v0\nheader information", "type: Array<FrameData>\nframes");
    add_data_class<
        asd::Data<double, asd::ch<1>, asd::ver<1>, asd::container::vec>
        >(mod, "Data1ch_v1", "type: Header_v1\nheader information", "type: Array<FrameData>\nframes");
    add_data_class<
        asd::Data<double, asd::ch<1>, asd::ver<2>, asd::container::vec>
        >(mod, "Data1ch_v2", "type: Header_v2\nheader information", "type: Array<FrameData>\nframes");

    add_data_class<
        asd::Data<double, asd::ch<2>, asd::ver<0>, asd::container::vec>
        >(mod, "Data2ch_v0", "type: Header_v0\nheader information",
          "type: Array<Array<FrameData>>\nchannels (array of frames)");
    add_data_class<
        asd::Data<double, asd::ch<2>, asd::ver<1>, asd::container::vec>
        >(mod, "Data2ch_v1", "type: Header_v1\nheader information",
          "type: Array<Array<FrameData>>\nchannels (array of frames)");
    add_data_class<
        asd::Data<double, asd::ch<2>, asd::ver<2>, asd::container::vec>
        >(mod, "Data2ch_v2", "type: Header_v2\nheader information",
          "type: Array<Array<FrameData>>\nchannels (array of frames)");
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
