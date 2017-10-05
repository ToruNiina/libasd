#ifndef LIBASD_READ_BINARY_AS_H
#define LIBASD_READ_BINARY_AS_H
#include <istream>

namespace asd
{
namespace detail
{

template<typename T>
T read_binary_as(const char*& ptr)
{
    const T retval = *reinterpret_cast<const T*>(ptr);
    ptr += sizeof(T);
    return retval;
}

template<typename T>
T read_binary_as(std::istream& is)
{
    const std::size_t sz = sizeof(T);
    char cbytes[sz];
    is.read(cbytes, sz);
    const T retval = *reinterpret_cast<const T*>(cbytes);
    return retval;
}

} // detail
}// asd
#endif// LIBASD_READ_BINARY_AS_H
