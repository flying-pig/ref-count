#ifndef REF_COUNT_SP_COUNTED_BASE_H_
#define REF_COUNT_SP_COUNTED_BASE_H_

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <config.h>

#if defined( __clang__ ) && defined( __has_extension )
# if __has_extension( __c_atomic__ )
#   define REF_COUNT_SP_HAS_CLANG_C11_ATOMICS
# endif
#endif


#if defined( REF_COUNT_SP_USE_PTHREADS )
# include <ref-count/sp_counted_base_pt.h>

#elif defined( REF_COUNT_SP_HAS_CLANG_C11_ATOMICS )
# include <ref-count/sp_counted_base_clang.h>

#elif defined( __GNUC__ ) && ( defined( __i386__ ) || defined( __x86_64__ ) ) && !defined(__PATHSCALE__)
# include <ref-count/sp_counted_base_gcc_x86.h>

#elif defined( REF_COUNT_SP_HAS_SYNC )
# include <ref-count/sp_counted_base_sync.h>

#endif

#undef REF_COUNT_SP_HAS_CLANG_C11_ATOMICS

#endif  // #ifndef REF_COUNT_SP_COUNTED_BASE_H_
