/*!
@file  frame_header.hpp
@brief definition of a class that contains frame header information

@author Toru Niina (niina.toru.68u@gmail.com)
@date 2017-11-06 14:30
@copyright Toru Niina
*/

#ifndef LIBASD_FRAME_HEADER_H
#define LIBASD_FRAME_HEADER_H
#include <cstdint>

namespace asd
{

//! frame header class
/*!
 * Because frame header format does not depends on version or channel,
 * this class does not depends on version or channel tag.
 */
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

}// asd
#endif// LIBASD_FRAME_HEADER_H
