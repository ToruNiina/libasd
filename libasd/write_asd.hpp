#ifndef LIBASD_WRITE_ASD_H
#define LIBASD_WRITE_ASD_H
#include <libasd/write_header.hpp>
#include <libasd/write_frame.hpp>
#include <libasd/data.hpp>

namespace asd
{

template<typename chT = channel<1>, typename verT = version<1>,
         typename contT = container::vec>
char* write_asd(char* ptr, const Data<chT, verT, contT>& dat)
{
    detail::write_header_impl(ptr, dat.header);
    for(const auto& frm : dat.frames)
    {
        detail::write_frame_impl<chT, contT>::invoke(ptr, frm);
    }
    return ptr;
}

template<typename chT = channel<1>, typename verT = version<1>,
         typename contT = container::vec>
std::ostream& write_asd(std::ostream& is, const Data<chT, verT, contT>& dat)
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
