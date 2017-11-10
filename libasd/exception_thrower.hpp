#ifndef LIBASD_EXCEPTION_THROWER_H
#define LIBASD_EXCEPTION_THROWER_H
#include <utility>
#include <sstream>
#include <string>
#include <stdexcept>

namespace asd
{
namespace detail
{
inline std::string formatter(const char* format)
{
    return std::string(format);
}

template<typename T, typename ... Ts>
std::string
formatter(const char* fmt, T&& head, Ts&& ... tail)
{
    std::string str;
    for(const char* i(fmt); *i != '\0'; ++i)
    {
        if(*i == '%')
        {
            std::ostringstream oss;
            oss << head;
            ++i;
            return str + oss.str() + formatter(i, std::forward<Ts>(tail)...);
        }
        else
        {
            str += *i;
        }
    }
    return str;
}
}//detail

template<typename Exception, typename ... Ts>
[[noreturn]] void throw_exception(const char* fmt, Ts&& ... strs)
{
    throw Exception(detail::formatter(fmt, std::forward<Ts>(strs)...));
}

}//asd
#endif// LIBASD_EXCEPTION_THROWER_H
