#ifndef LIBASD_WRITE_ASD_H
#define LIBASD_WRITE_ASD_H
#include <libasd/write_header.hpp>
#include <libasd/write_frame.hpp>
#include <libasd/data.hpp>

namespace asd
{

template<typename chT, typename verT, typename contT>
char*& write_asd(char*& ptr, const Data<chT, verT, contT>& dat)
{
    detail::write_header_impl(ptr, dat.header);
    for(const auto& frm : dat.frames)
    {
        detail::write_frame_impl<chT, contT>::invoke(ptr, frm);
    }
    return ptr;
}

template<typename chT, typename verT, typename contT>
std::ostream& write_asd(std::ostream& os, const Data<chT, verT, contT>& dat)
{
    detail::write_header_impl(os, dat.header);
    for(const auto& frm : dat.frames)
    {
        detail::write_frame_impl<chT, contT>::invoke(os, frm);
    }
    return os;
}
} // asd
#endif// LIBASD_WRITE_ASD_H
