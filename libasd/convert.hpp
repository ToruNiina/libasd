#ifndef LIBASD_CONVERT_H
#define LIBASD_CONVERT_H
#include <libasd/exception_thrower.hpp>
#include <libasd/header.hpp>
#include <libasd/frame_data.hpp>
#include <memory>
#include <iostream>
#include <algorithm>

namespace asd
{

template<typename realT>
struct voltage_level_converter
{
    typedef realT        real_type;
    typedef std::int16_t level_type;

    voltage_level_converter() noexcept = default;
    virtual ~voltage_level_converter() noexcept = default;

    virtual level_type voltage_to_level(const real_type  v) const noexcept = 0;
    virtual real_type  level_to_voltage(const level_type l) const noexcept = 0;

    virtual bool is_unipolar() const noexcept = 0;
};

template<typename realT>
struct unipolar_converter : public voltage_level_converter<realT>
{
    typedef voltage_level_converter<realT> base_type;
    typedef typename base_type::real_type  real_type;
    typedef typename base_type::level_type level_type;

    unipolar_converter(const real_type rg) noexcept
        : range(rg), resolution(4096ull)
    {}
    unipolar_converter(const real_type rg, const std::uint64_t resol) noexcept
        : range(rg), resolution(resol)
    {}
    ~unipolar_converter() noexcept override = default;

    level_type voltage_to_level(const real_type  v) const noexcept override
    {
        return static_cast<level_type>(std::floor(v * resolution));
    }
    real_type  level_to_voltage(const level_type l) const noexcept override
    {
        return range * l / resolution;
    }
    bool is_unipolar() const noexcept override {return true;}

    const real_type     range;     // 10 for range [0, 10]
    const std::uint64_t resolution;
};

template<typename realT>
struct bipolar_converter : public voltage_level_converter<realT>
{
    typedef voltage_level_converter<realT> base_type;
    typedef typename base_type::real_type  real_type;
    typedef typename base_type::level_type level_type;

    bipolar_converter(const real_type rg) noexcept
        : range(rg), resolution(4096ull)
    {}
    bipolar_converter(const real_type rg, const std::uint64_t resol) noexcept
        : range(rg), resolution(resol)
    {}
    ~bipolar_converter() noexcept override = default;

    level_type voltage_to_level(const real_type  v) const noexcept override
    {
        return static_cast<level_type>(std::floor(
                    ((v + range) / (2 * range)) * resolution));
    }
    real_type  level_to_voltage(const level_type l) const noexcept override
    {
        return range - 2 * l * range / resolution;
    }
    bool is_unipolar() const noexcept override {return false;}

    const real_type     range;      // 5 for range [-5, 5].
    const std::uint64_t resolution; // normally, 4096
};

template<typename realT>
inline std::unique_ptr<voltage_level_converter<realT>>
make_voltage_level_converter(
        const AD_range range, const std::uint64_t resolution = 4096)
{
    using result_type = std::unique_ptr<voltage_level_converter<realT>>;
    switch(range)
    {
        case AD_range::unipolar_1_0V:
        {
            return result_type(new unipolar_converter<realT>(1.0, resolution));
        }
        case AD_range::unipolar_2_5V:
        {
            return result_type(new unipolar_converter<realT>(2.5, resolution));
        }
        case AD_range::unipolar_5_0V:
        {
            return result_type(new unipolar_converter<realT>(5.0, resolution));
        }
        case AD_range::bipolar_1_0V :
        {
            return result_type(new bipolar_converter<realT>(1.0, resolution));
        }
        case AD_range::bipolar_2_5V :
        {
            return result_type(new bipolar_converter<realT>(2.5, resolution));
        }
        case AD_range::bipolar_5_0V :
        {
            return result_type(new bipolar_converter<realT>(5.0, resolution));
        }
        case AD_range::dummy_value  :
        {
            //XXX dummy value means "this file is modified, not raw data".
            //    enable to write out as bipolar-80.0.
            return result_type(new bipolar_converter<realT>(80.0, resolution));
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
             const Header<version<0>>& header, const std::size_t ch)
{
    FrameData<realT, contT> retval(data.x_pixel(), data.y_pixel());

    const auto converter = make_voltage_level_converter<realT>(
            header.ad_range, (1ul << header.bits_data));

    // voltage -> data
    if(ch != 0 && ch != 1)
    {
        throw_exception<std::invalid_argument>("channel must be 0 or 1: %", ch);
    }
    const auto kind = (ch == 0) ? header.data_type_1ch : header.data_type_2ch;

    realT coef = 0.0;
    switch(kind)
    {
        case data_kind::topography:
        {
            coef = header.z_piezo_gain * header.z_piezo_extension;
            break;
        }
        case data_kind::error:
        {
            coef = -1.0 * header.sensor_sensitivity;
            break;
        }
        case data_kind::phase:
        {
            coef = -1.0 * header.phase_sensitivity;
            break;
        }
        case data_kind::none:
        {
            std::cerr << "WARNING: data kind is 'none'. coefficient was set 0\n";
            coef = 0.0;
            break;
        }
        default:
        {
            throw_exception<std::invalid_argument>(
                    "invalid data kind(%) for channel(%)",
                    static_cast<std::int32_t>(kind), ch);
        }
    }

    std::transform(data.raw_begin(), data.raw_end(), retval.raw_begin(),
        [&converter, coef](const std::int16_t i) noexcept -> realT {
            return coef * converter->level_to_voltage(i);
        });
    return retval;
}

template<typename realT, typename contT>
FrameData<realT, contT>
convert_data(const FrameData<std::int16_t, contT>& data,
             const Header<version<1>>& header, const std::size_t ch)
{
    FrameData<realT, contT> retval(data.x_pixel(), data.y_pixel());

    const auto converter = make_voltage_level_converter<realT>(
            header.ad_range, (1ul << header.ad_resolution));

    // voltage -> data
    if(ch != 0 && ch != 1)
    {
        throw_exception<std::invalid_argument>("channel must be 0 or 1: %", ch);
    }
    const auto kind = (ch == 0) ? header.data_kind_1ch : header.data_kind_2ch;

    realT coef = 0.0;
    switch(kind)
    {
        case data_kind::topography:
        {
            coef = header.z_piezo_gain * header.z_piezo_extension;
            break;
        }
        case data_kind::error:
        {
            coef = -1.0 * header.sensor_sensitivity;
            break;
        }
        case data_kind::phase:
        {
            coef = -1.0 * header.phase_sensitivity;
            break;
        }
        case data_kind::none:
        {
            std::cerr << "WARNING: data kind is 'none'. coefficient was set 0\n";
            coef = 0.0;
            break;
        }
        default:
        {
            throw_exception<std::invalid_argument>(
                    "invalid data kind(%) for channel(%)",
                    static_cast<std::int32_t>(kind), ch);
        }
    }

    std::transform(data.raw_begin(), data.raw_end(), retval.raw_begin(),
        [&converter, coef](const std::int16_t i) noexcept -> realT {
            return coef * converter->level_to_voltage(i);
        });
    return retval;
}

template<typename realT, typename contT>
FrameData<realT, contT>
convert_data(const FrameData<std::int16_t, contT>& data,
             const Header<version<2>>& header, const std::size_t ch)
{
    FrameData<realT, contT> retval(data.x_pixel(), data.y_pixel());

    const auto converter = make_voltage_level_converter<realT>(
            header.ad_range, (1ul << header.ad_resolution));

    // voltage -> data
    if(ch != 0 && ch != 1)
    {
        throw_exception<std::invalid_argument>("channel must be 0 or 1: %", ch);
    }
    const auto kind = (ch == 0) ? header.data_kind_1ch : header.data_kind_2ch;

    realT coef = 0.0;
    switch(kind)
    {
        case data_kind::topography:
        {
            coef = header.z_piezo_gain * header.z_piezo_extension;
            break;
        }
        case data_kind::error:
        {
            coef = -1.0 * header.sensor_sensitivity;
            break;
        }
        case data_kind::phase:
        {
            coef = -1.0 * header.phase_sensitivity;
            break;
        }
        case data_kind::none:
        {
            std::cerr << "WARNING: data kind is 'none'. coefficient was set 0\n";
            coef = 0.0;
            break;
        }
        default:
        {
            throw_exception<std::invalid_argument>(
                    "invalid data kind(%) for channel(%)",
                    static_cast<std::int32_t>(kind), ch);
        }
    }

    std::transform(data.raw_begin(), data.raw_end(), retval.raw_begin(),
        [&converter, coef](const std::int16_t i) noexcept -> realT {
            return coef * converter->level_to_voltage(i);
        });
    return retval;
}

} // asd
#endif// LIBASD_CONVERT_H
