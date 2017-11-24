#ifndef LIBASD_CONVERT_H
#define LIBASD_CONVERT_H
#include <libasd/header.hpp>
#include <libasd/frame_data.hpp>
#include <algorithm>

namespace asd
{

template<typename realT, typename contT>
FrameData<realT, contT>
convert_data(const FrameData<std::int16_t, contT>& data,
             const Header<version<1>>& header, const std::size_t ch)
{
    FrameData<realT, contT> retval(data.x_pixel(), data.y_pixel());
//     const auto kind = (ch == 1) ? header.data_kind_1ch : header.data_kind_2ch;
    // TODO switch(data_kind)

    const std::uint64_t AD_resl = (1 << header.AD_resolution);
    // assuming AD_range == 10
    const realT resolution = 10.0 / AD_resl; // [V/int12_t]
    const auto coef = resolution *
                      header.z_piezo_gain * header.z_piezo_extension;

    std::transform(data.raw_begin(), data.raw_end(), retval.raw_begin(),
                   [=](const std::int16_t i){return i * coef;});
    return retval;
}

} // asd
#endif// LIBASD_CONVERT_H
