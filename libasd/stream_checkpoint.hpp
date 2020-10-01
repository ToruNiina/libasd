#ifndef LIBASD_STREAM_CHECKPOINT_H
#define LIBASD_STREAM_CHECKPOINT_H
#include <istream>

// to rewind streams

namespace asd
{
namespace detail
{

inline const char* checkpoint(const char* loc) noexcept
{
    return loc;
}
inline typename std::istream::pos_type checkpoint(std::istream& is)
{
    return is.tellg();
}

inline void restart_from(const char*& loc, const char* const chkp) noexcept
{
    loc = chkp;
    return;
}
inline void restart_from(std::istream& is, const std::istream::pos_type chkp)
{
    is.seekg(chkp);
    return ;
}

} // detail
} // asd
#endif// LIBASD_STREAM_CHECKPOINT_H
