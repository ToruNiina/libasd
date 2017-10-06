#ifndef LIBASD_FRAME_HEADER_H
#define LIBASD_FRAME_HEADER_H
#include <libasd/read_binary_as.hpp>
#include <cstdint>

namespace asd
{

struct FrameHeader
{
    std::int32_t number;        //!< frame number
    std::int16_t max_data;      //!< maximum data in the frame
    std::int16_t min_data;      //!< minimum data in the frame
    std::int16_t x_offset;      //!< current position of X scanner
    std::int16_t y_offset;      //!< current position of Y scanner
    float        x_tilt;        //!< legacy from the initial version
    float        y_tilt;        //!< legacy from the initial version
    bool         is_stimulated; //!< flag of caged laser irradiation
    std::int8_t  _booked1;      //!< booked region
    std::int16_t _booked2;      //!< booked region
    std::int32_t _booked3;      //!< booked region
    std::int32_t _booked4;      //!< booked region
};

template<typename T>
FrameHeader read_frame_header(T& source)
{
    FrameHeader fh;
    fh.number        = read_binary_as<std::int32_t>(source);
    fh.max_data      = read_binary_as<std::int16_t>(source);
    fh.min_data      = read_binary_as<std::int16_t>(source);
    fh.x_offset      = read_binary_as<std::int16_t>(source);
    fh.y_offset      = read_binary_as<std::int16_t>(source);
    fh.x_tilt        = read_binary_as<float       >(source);
    fh.y_tilt        = read_binary_as<float       >(source);
    fh.is_stimulated = read_binary_as<bool        >(source);
    fh._booked1      = read_binary_as<std::int8_t >(source);
    fh._booked2      = read_binary_as<std::int16_t>(source);
    fh._booked3      = read_binary_as<std::int32_t>(source);
    fh._booked4      = read_binary_as<std::int32_t>(source);
    return fh;
}

} // asd
#endif// LIBASD_FRAME_HEADER_H
