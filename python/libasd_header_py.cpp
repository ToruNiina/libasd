#include "libasd_header_py.hpp"
#include <fstream>

namespace py = pybind11;

void add_header_enums(py::module& mod)
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
}

void add_header_classes(py::module& mod)
{
    py::class_<asd::Header<asd::version<0>>> header_v0(mod, "Header_v0");
    header_v0.def(py::init<>())
        .def_readwrite("file_version"       , &asd::Header<asd::version<0>>::file_version)
        .def_readwrite("data_type_1ch"      , &asd::Header<asd::version<0>>::data_type_1ch)
        .def_readwrite("data_type_2ch"      , &asd::Header<asd::version<0>>::data_type_2ch)
        .def_readwrite("file_header_size"   , &asd::Header<asd::version<0>>::file_header_size)
        .def_readwrite("frame_header_size"  , &asd::Header<asd::version<0>>::frame_header_size)
        .def_readwrite("operator_name_size" , &asd::Header<asd::version<0>>::operator_name_size)
        .def_readwrite("comment_offset_size", &asd::Header<asd::version<0>>::comment_offset_size)
        .def_readwrite("comment_size"       , &asd::Header<asd::version<0>>::comment_size)
        .def_readwrite("x_pixel"            , &asd::Header<asd::version<0>>::x_pixel)
        .def_readwrite("y_pixel"            , &asd::Header<asd::version<0>>::y_pixel)
        .def_readwrite("x_scanning_range"   , &asd::Header<asd::version<0>>::x_scanning_range)
        .def_readwrite("y_scanning_range"   , &asd::Header<asd::version<0>>::y_scanning_range)
        .def_readwrite("frame_rate"         , &asd::Header<asd::version<0>>::frame_rate)
        .def_readwrite("z_piezo_extension"  , &asd::Header<asd::version<0>>::z_piezo_extension)
        .def_readwrite("z_piezo_gain"       , &asd::Header<asd::version<0>>::z_piezo_gain)
        .def_readwrite("ad_range"           , &asd::Header<asd::version<0>>::ad_range)
        .def_readwrite("bits_data"          , &asd::Header<asd::version<0>>::bits_data)
        .def_readwrite("is_averaged"        , &asd::Header<asd::version<0>>::is_averaged)
        .def_readwrite("average_window"     , &asd::Header<asd::version<0>>::average_window)
        .def_readwrite("year"               , &asd::Header<asd::version<0>>::year)
        .def_readwrite("month"              , &asd::Header<asd::version<0>>::month)
        .def_readwrite("day"                , &asd::Header<asd::version<0>>::day)
        .def_readwrite("hour"               , &asd::Header<asd::version<0>>::hour)
        .def_readwrite("minute"             , &asd::Header<asd::version<0>>::minute)
        .def_readwrite("second"             , &asd::Header<asd::version<0>>::second)
        .def_readwrite("rounding_degree"    , &asd::Header<asd::version<0>>::rounding_degree)
        .def_readwrite("x_max_range"        , &asd::Header<asd::version<0>>::x_max_range)
        .def_readwrite("y_max_range"        , &asd::Header<asd::version<0>>::y_max_range)
        .def_readwrite("init_frame"         , &asd::Header<asd::version<0>>::init_frame)
        .def_readwrite("num_frames"         , &asd::Header<asd::version<0>>::num_frames)
        .def_readwrite("machine_id"         , &asd::Header<asd::version<0>>::machine_id)
        .def_readwrite("file_id"            , &asd::Header<asd::version<0>>::file_id)
        .def_readwrite("operator_name"      , &asd::Header<asd::version<0>>::operator_name)
        .def_readwrite("sensor_sensitivity" , &asd::Header<asd::version<0>>::sensor_sensitivity)
        .def_readwrite("phase_sensitivity"  , &asd::Header<asd::version<0>>::phase_sensitivity)
        .def_readwrite("scannig_direction"  , &asd::Header<asd::version<0>>::scannig_direction)
        .def_readwrite("comment"            , &asd::Header<asd::version<0>>::comment)
        ;

    py::class_<asd::Header<asd::version<1>>> header_v1(mod, "Header_v1");
    header_v1.def(py::init<>())
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

    py::class_<asd::Header<asd::version<2>>> header_v2(mod, "Header_v2");
    header_v2.def(py::init<>())
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
}

template<typename Version>
asd::Header<Version> py_read_header(const std::string& filename)
{
    std::ifstream ifs(filename);
    return asd::read_header<Version>(ifs);
}

void add_header_io_funcs(py::module& mod)
{
    mod.def("read_header_v0", &py_read_header<asd::version<0>>,
            py::arg("file_name"),
            "A function which reads only header information of asd file.");
    mod.def("read_header_v1", &py_read_header<asd::version<1>>,
            py::arg("file_name"),
            "A function which reads only header information of asd file.");
    mod.def("read_header_v2", &py_read_header<asd::version<2>>,
            py::arg("file_name"),
            "A function which reads only header information of asd file.");

    const auto py_read_header_dynamic =
        [](const std::string& fname, std::uint32_t v) -> py::object {
            switch(v)
            {
                case 0: return py::cast(py_read_header<asd::version<0>>(fname));
                case 1: return py::cast(py_read_header<asd::version<1>>(fname));
                case 2: return py::cast(py_read_header<asd::version<2>>(fname));
                default: throw std::invalid_argument(
                             "the available versions are only 0, 1, and 2.");
            }
        };

    mod.def("read_header", py_read_header_dynamic,
            "A function which reads only header information of asd file.\n"
            "When 0 is passed as a version, it reads file as asd version 0"
            "and returns Header_v0.\nThere are only version 0, 1, and 2. "
            "If you passed the other value as the version of the file, "
            "ValueError will be thrown.\n",
            py::arg("file_name"), py::arg("version") = 1);
}
