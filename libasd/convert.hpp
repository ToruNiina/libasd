#ifndef LIBASD_CONVERT_H
#define LIBASD_CONVERT_H
#include <libasd/exception_thrower.hpp>
#include <libasd/header.hpp>
#include <libasd/frame_data.hpp>
#include <algorithm>

namespace asd
{

template<typename realT>
inline realT
level_to_voltage(const std::int16_t lvl, const AD_range range,
                 const std::uint64_t resolution = 4096)
{
    switch(range)
    {
        case AD_range::unipolar_1_0V:
        {
            return static_cast<realT>(lvl) / resolution;
        }
        case AD_range::unipolar_2_5V:
        {
            return 2.5 * static_cast<realT>(lvl) / resolution;
        }
        case AD_range::unipolar_5_0V:
        {
            return 5.0 * static_cast<realT>(lvl) / resolution;
        }
        case AD_range::bipolar_1_0V :
        {
            return 1.0 - 2.0 * static_cast<realT>(lvl) / resolution;
        }
        case AD_range::bipolar_2_5V :
        {
            return 2.5 - 5.0 * static_cast<realT>(lvl) / resolution;
        }
        case AD_range::bipolar_5_0V :
        {
            return 5.0 - 10.0 * static_cast<realT>(lvl) / resolution;
        }
        case AD_range::dummy_value  :
        {
            //XXX dummy value means "this file is modified, not raw data".
            //    enable to write out as bipolar-80.0.
            return 160.0 * static_cast<realT>(lvl) / resolution - 80.0;
        }
        default:
        {
            throw_exception<std::invalid_argument>("invalid AD_range value: %",
                    static_cast<std::uint32_t>(range));
        }
    }
}

template<typename realT>
inline std::int16_t
voltage_to_level(const realT v, const AD_range range,
                  const std::uint64_t resolution = 4096)
{
    switch(range)
    {
        case AD_range::unipolar_1_0V:
        {
            return static_cast<std::int16_t>(std::floor(v * resolution));
        }
        case AD_range::unipolar_2_5V:
        {
            return static_cast<std::int16_t>(std::floor(v * resolution / 2.5));
        }
        case AD_range::unipolar_5_0V:
        {
            return static_cast<std::int16_t>(std::floor(v * resolution / 5.0));
        }
        case AD_range::bipolar_1_0V :
        {
            return static_cast<std::int16_t>(
                    std::floor(((v + 1.0) / 2.0) * resolution));
        }
        case AD_range::bipolar_2_5V :
        {
            return static_cast<std::int16_t>(
                    std::floor(((v + 2.5) / 5.0) * resolution));
        }
        case AD_range::bipolar_5_0V :
        {
            return static_cast<std::int16_t>(
                    std::floor(((v + 5.0) /10.0) * resolution));
        }
        case AD_range::dummy_value  :
        {
            //XXX dummy value means "this file is modified, not raw data".
            //    enable to write out as bipolar-80.0.
            return static_cast<std::int16_t>(
                    std::floor(((v + 80.0) /160.0) * resolution));
        }
        default:
        {
            throw_exception<std::invalid_argument>("invalid AD_range value: %",
                    static_cast<std::uint32_t>(range));
        }
    }
}

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
                       const auto volt = level_to_voltage<realT>(i, ADrg, resl);
                       return volt * coef;
                   });
    return retval;
}

} // asd
#endif// LIBASD_CONVERT_H
