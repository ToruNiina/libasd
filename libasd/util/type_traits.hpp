#ifndef LIBASD_TYPE_TRAITS
#define LIBASD_TYPE_TRAITS
#include <type_traits>

namespace asd
{

template<bool B>
using bool_constant = std::integral_constant<bool, B>;

template<typename ...> struct conjunction : std::true_type {};
template<typename B>   struct conjunction<B> : B {};
template<typename B, typename ... Bools>
struct conjunction<B, Bools...> : std::conditional<
    bool(B::value), conjunction<Bools...>, B>::type {};

template<typename ...> struct disjunction : std::false_type {};
template<typename B>   struct disjunction<B> : B {};
template<typename B, typename ... Bools>
struct disjunction<B, Bools...> : std::conditional<
    bool(B::value), B, disjunction<Bools...>>::type {};

template<typename B> struct negation : bool_constant<!bool(B::value)>{};

} // asd
#endif// LIBASD_TYPE_TRAITS
