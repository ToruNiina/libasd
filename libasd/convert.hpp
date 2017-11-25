#ifndef LIBASD_CONVERT_H
#define LIBASD_CONVERT_H
#include <libasd/exception_thrower.hpp>
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

    // int16_t -> voltage
    const auto          ADrg = header.ad_range;
    const std::uint64_t resl = (1ul << header.ad_resolution);

    // voltage -> data
    const auto kind = [ch](const Header<version<1>>& hdr)
    {
             if(ch == 0){return hdr.data_kind_1ch;}
        else if(ch == 1){return hdr.data_kind_2ch;}
        else throw_exception<std::invalid_argument>("invalid channel: %", ch);
    }(header);
    const auto coef = [kind](const Header<version<1>>& hdr) -> realT
    {
        switch(kind)
        {
            case data_kind::topography:
                return hdr.z_piezo_gain * hdr.z_piezo_extension;
            case data_kind::error:
                return -1.0 * hdr.sensor_sensitivity;
            case data_kind::phase:
                return -1.0 * hdr.phase_sensitivity;
            case data_kind::none:
                return 0.0;
            default:
                throw_exception<std::invalid_argument>("invalid data_kind: %",
                        static_cast<std::int32_t>(kind));
        }
    }(header);

    std::transform(data.raw_begin(), data.raw_end(), retval.raw_begin(),
                   [ADrg, resl, coef](const std::int16_t i){
                       return level_to_voltage<realT>(i, ADrg, resl) * coef;
                   });
    return retval;
}

} // asd
#endif// LIBASD_CONVERT_H
