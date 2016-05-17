#if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC)
// linux, also other platforms (Hurd etc) that use GLIBC, should these really have their own config headers though?
#  define REF_COUNT_PLATFORM_CONFIG "config/platform/linux.h"

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// MacOS
#  define REF_COUNT_PLATFORM_CONFIG "config/platform/macos.h"

#else

#  if defined(unix) \
      || defined(__unix) \
      || defined(_XOPEN_SOURCE) \
      || defined(_POSIX_SOURCE)

   // generic unix platform:

#  ifndef REF_COUNT_HAS_UNISTD_H
#     define REF_COUNT_HAS_UNISTD_H
#  endif

#  include <config/posix_features.hpp>

#  endif

#  if defined (REF_COUNT_ASSERT_CONFIG)
#     error "Unknown platform"
#  endif

#endif
