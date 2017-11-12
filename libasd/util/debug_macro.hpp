#ifndef LIBASD_DEBUG_MACRO_H
#define LIBASD_DEBUG_MACRO_H

#ifdef LIBASD_ENABLE_DEBUG
#  ifndef LIBASD_IGNORE_IF_NO_DEBUG
#    define LIBASD_IGNORE_IF_NO_DEBUG( x ) x
#  endif
#  ifndef LIBASD_DUMP_IF_DEBUG
#    define LIBASD_DUMP_IF_DEBUG( x ) std::cerr << #x << " = " << x << std::endl;
#  endif
#else
#  ifndef LIBASD_IGNORE_IF_NO_DEBUG
#    define LIBASD_IGNORE_IF_NO_DEBUG( x ) /**/
#  endif
#  ifndef LIBASD_DUMP_IF_DEBUG
#    define LIBASD_DUMP_IF_DEBUG( x ) /**/
#  endif
#endif

#endif //LIBASD_DEBUG_MACRO_H
