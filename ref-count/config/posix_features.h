// All POSIX feature tests go in this file,
// Note that we test _POSIX_C_SOURCE and _XOPEN_SOURCE as well
// _POSIX_VERSION and _XOPEN_VERSION: on some systems POSIX API's
// may be present but none-functional unless _POSIX_C_SOURCE and
// _XOPEN_SOURCE have been defined to the right value (it's up
// to the user to do this *before* including any header, although
// in most cases the compiler will do this for you).

#  if defined(REF_COUNT_HAS_UNISTD_H)
#     include <unistd.h>

      // XOpen has <nl_types.h>, but is this the correct version check?
#     if defined(_XOPEN_VERSION) && (_XOPEN_VERSION >= 3)
#        define REF_COUNT_HAS_NL_TYPES_H
#     endif

      // POSIX version 6 requires <stdint.h>
#     if defined(_POSIX_VERSION) && (_POSIX_VERSION >= 200100)
#        define REF_COUNT_HAS_STDINT_H
#     endif

      // POSIX version 2 requires <dirent.h>
#     if defined(_POSIX_VERSION) && (_POSIX_VERSION >= 199009L)
#        define REF_COUNT_HAS_DIRENT_H
#     endif

      // POSIX version 3 requires <signal.h> to have sigaction:
#     if defined(_POSIX_VERSION) && (_POSIX_VERSION >= 199506L)
#        define REF_COUNT_HAS_SIGACTION
#     endif
      // POSIX defines _POSIX_THREADS > 0 for pthread support,
      // however some platforms define _POSIX_THREADS without
      // a value, hence the (_POSIX_THREADS+0 >= 0) check.
      // Strictly speaking this may catch platforms with a
      // non-functioning stub <pthreads.h>, but such occurrences should
      // occur very rarely if at all.
#     if defined(_POSIX_THREADS) && (_POSIX_THREADS+0 >= 0) && !defined(REF_COUNT_HAS_WINTHREADS) && !defined(REF_COUNT_HAS_MPTASKS)
#        define REF_COUNT_HAS_PTHREADS
#     endif

      // REF_COUNT_HAS_NANOSLEEP:
      // This is predicated on _POSIX_TIMERS or _XOPEN_REALTIME:
#     if (defined(_POSIX_TIMERS) && (_POSIX_TIMERS+0 >= 0)) \
             || (defined(_XOPEN_REALTIME) && (_XOPEN_REALTIME+0 >= 0))
#        define REF_COUNT_HAS_NANOSLEEP
#     endif

      // REF_COUNT_HAS_CLOCK_GETTIME:
      // This is predicated on _POSIX_TIMERS (also on _XOPEN_REALTIME
      // but at least one platform - linux - defines that flag without
      // defining clock_gettime):
#     if (defined(_POSIX_TIMERS) && (_POSIX_TIMERS+0 >= 0))
#        define REF_COUNT_HAS_CLOCK_GETTIME
#     endif

      // REF_COUNT_HAS_SCHED_YIELD:
      // This is predicated on _POSIX_PRIORITY_SCHEDULING or
      // on _POSIX_THREAD_PRIORITY_SCHEDULING or on _XOPEN_REALTIME.
#     if defined(_POSIX_PRIORITY_SCHEDULING) && (_POSIX_PRIORITY_SCHEDULING+0 > 0)\
            || (defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && (_POSIX_THREAD_PRIORITY_SCHEDULING+0 > 0))\
            || (defined(_XOPEN_REALTIME) && (_XOPEN_REALTIME+0 >= 0))
#        define REF_COUNT_HAS_SCHED_YIELD
#     endif

      // REF_COUNT_HAS_GETTIMEOFDAY:
      // REF_COUNT_HAS_PTHREAD_MUTEXATTR_SETTYPE:
      // These are predicated on _XOPEN_VERSION, and appears to be first released
      // in issue 4, version 2 (_XOPEN_VERSION > 500).
      // Likewise for the functions log1p and expm1.
#     if defined(_XOPEN_VERSION) && (_XOPEN_VERSION+0 >= 500)
#        define REF_COUNT_HAS_GETTIMEOFDAY
#        if defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE+0 >= 500)
#           define REF_COUNT_HAS_PTHREAD_MUTEXATTR_SETTYPE
#        endif
#        ifndef REF_COUNT_HAS_LOG1P
#           define REF_COUNT_HAS_LOG1P
#        endif
#        ifndef REF_COUNT_HAS_EXPM1
#           define REF_COUNT_HAS_EXPM1
#        endif
#     endif

#  endif




