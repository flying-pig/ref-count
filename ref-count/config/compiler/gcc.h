//  GNU C++ compiler setup.

#define REF_COUNT_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if !defined(__CUDACC__)
#define REF_COUNT_GCC REF_COUNT_GCC_VERSION
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103L)
#  define REF_COUNT_GCC_CXX11
#endif

#if __GNUC__ == 3
#  if defined (__PATHSCALE__)
#     define REF_COUNT_NO_TWO_PHASE_NAME_LOOKUP
#     define REF_COUNT_NO_IS_ABSTRACT
#  endif

#  if __GNUC_MINOR__ < 4
#     define REF_COUNT_NO_IS_ABSTRACT
#  endif
#  define REF_COUNT_NO_CXX11_EXTERN_TEMPLATE
#endif
#if __GNUC__ < 4
//
// All problems to gcc-3.x and earlier here:
//
#define REF_COUNT_NO_TWO_PHASE_NAME_LOOKUP
#  ifdef __OPEN64__
#     define REF_COUNT_NO_IS_ABSTRACT
#  endif
#endif

// GCC prior to 3.4 had #pragma once too but it didn't work well with filesystem links
#if REF_COUNT_GCC_VERSION >= 30400
#define REF_COUNT_HAS_PRAGMA_ONCE
#endif

#if REF_COUNT_GCC_VERSION < 40400
// Previous versions of GCC did not completely implement value-initialization:
// GCC Bug 30111, "Value-initialization of POD base class doesn't initialize
// members", reported by Jonathan Wakely in 2006,
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=30111 (fixed for GCC 4.4)
// GCC Bug 33916, "Default constructor fails to initialize array members",
// reported by Michael Elizabeth Chastain in 2007,
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=33916 (fixed for GCC 4.2.4)
// See also: http://www.REF_COUNT.org/libs/utility/value_init.htm#compiler_issues
#define REF_COUNT_NO_COMPLETE_VALUE_INITIALIZATION
#endif

#if !defined(__EXCEPTIONS) && !defined(REF_COUNT_NO_EXCEPTIONS)
# define REF_COUNT_NO_EXCEPTIONS
#endif


//
// Threading support: Turn this on unconditionally here (except for
// those platforms where we can know for sure). It will get turned off again
// later if no threading API is detected.
//
#if !defined(__MINGW32__) && !defined(linux) && !defined(__linux) && !defined(__linux__)
# define REF_COUNT_HAS_THREADS
#endif

//
// gcc has "long long"
// Except on Darwin with standard compliance enabled (-pedantic)
// Apple gcc helpfully defines this macro we can query
//
#if !defined(__DARWIN_NO_LONG_LONG)
# define REF_COUNT_HAS_LONG_LONG
#endif

//
// gcc implements the named return value optimization since version 3.1
//
#define REF_COUNT_HAS_NRVO

// Branch prediction hints
#define REF_COUNT_LIKELY(x) __builtin_expect(x, 1)
#define REF_COUNT_UNLIKELY(x) __builtin_expect(x, 0)

//
// Dynamic shared object (DSO) and dynamic-link library (DLL) support
//
#if __GNUC__ >= 4
#  if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32)) && !defined(__CYGWIN__)
     // All Win32 development environments, including 64-bit Windows and MinGW, define
     // _WIN32 or one of its variant spellings. Note that Cygwin is a POSIX environment,
     // so does not define _WIN32 or its variants.
#    define REF_COUNT_HAS_DECLSPEC
#    define REF_COUNT_SYMBOL_EXPORT __attribute__((__dllexport__))
#    define REF_COUNT_SYMBOL_IMPORT __attribute__((__dllimport__))
#  else
#    define REF_COUNT_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#    define REF_COUNT_SYMBOL_IMPORT
#  endif
#  define REF_COUNT_SYMBOL_VISIBLE __attribute__((__visibility__("default")))
#else
// config/platform/win32.hpp will define REF_COUNT_SYMBOL_EXPORT, etc., unless already defined
#  define REF_COUNT_SYMBOL_EXPORT
#endif

//
// RTTI and typeinfo detection is possible post gcc-4.3:
//
#if REF_COUNT_GCC_VERSION > 40300
#  ifndef __GXX_RTTI
#     ifndef REF_COUNT_NO_TYPEID
#        define REF_COUNT_NO_TYPEID
#     endif
#     ifndef REF_COUNT_NO_RTTI
#        define REF_COUNT_NO_RTTI
#     endif
#  endif
#endif

//
// Recent GCC versions have __int128 when in 64-bit mode.
//
// We disable this if the compiler is really nvcc as it
// doesn't actually support __int128 as of CUDA_VERSION=5000
// even though it defines __SIZEOF_INT128__.
// See https://svn.REF_COUNT.org/trac/REF_COUNT/ticket/8048
// Only re-enable this for nvcc if you're absolutely sure
// of the circumstances under which it's supported:
//
#if defined(__SIZEOF_INT128__) && !defined(__CUDACC__)
#  define REF_COUNT_HAS_INT128
#endif
//
// Recent GCC versions have a __float128 native type, we need to
// include a std lib header to detect this - not ideal, but we'll
// be including <cstddef> later anyway when we select the std lib.
//
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif
#if defined(_GLIBCXX_USE_FLOAT128) && !defined(__STRICT_ANSI__)
# define REF_COUNT_HAS_FLOAT128
#endif

// C++0x features in 4.3.n and later
//
#if (REF_COUNT_GCC_VERSION >= 40300) && defined(REF_COUNT_GCC_CXX11)
// C++0x features are only enabled when -std=c++0x or -std=gnu++0x are
// passed on the command line, which in turn defines
// __GXX_EXPERIMENTAL_CXX0X__.
#  define REF_COUNT_HAS_DECLTYPE
#  define REF_COUNT_HAS_RVALUE_REFS
#  define REF_COUNT_HAS_STATIC_ASSERT
#  define REF_COUNT_HAS_VARIADIC_TMPL
#else
#  define REF_COUNT_NO_CXX11_DECLTYPE
#  define REF_COUNT_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#  define REF_COUNT_NO_CXX11_RVALUE_REFERENCES
#  define REF_COUNT_NO_CXX11_STATIC_ASSERT
#endif

// C++0x features in 4.4.n and later
//
#if (REF_COUNT_GCC_VERSION < 40400) || !defined(REF_COUNT_GCC_CXX11)
#  define REF_COUNT_NO_CXX11_AUTO_DECLARATIONS
#  define REF_COUNT_NO_CXX11_AUTO_MULTIDECLARATIONS
#  define REF_COUNT_NO_CXX11_CHAR16_T
#  define REF_COUNT_NO_CXX11_CHAR32_T
#  define REF_COUNT_NO_CXX11_HDR_INITIALIZER_LIST
#  define REF_COUNT_NO_CXX11_DEFAULTED_FUNCTIONS
#  define REF_COUNT_NO_CXX11_DELETED_FUNCTIONS
#  define REF_COUNT_NO_CXX11_TRAILING_RESULT_TYPES
#  define REF_COUNT_NO_CXX11_INLINE_NAMESPACES
#  define REF_COUNT_NO_CXX11_VARIADIC_TEMPLATES
#endif

#if REF_COUNT_GCC_VERSION < 40500
#  define REF_COUNT_NO_SFINAE_EXPR
#endif

// GCC 4.5 forbids declaration of defaulted functions in private or protected sections
#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ == 5) || !defined(REF_COUNT_GCC_CXX11)
#  define REF_COUNT_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS
#endif

// C++0x features in 4.5.0 and later
//
#if (REF_COUNT_GCC_VERSION < 40500) || !defined(REF_COUNT_GCC_CXX11)
#  define REF_COUNT_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
#  define REF_COUNT_NO_CXX11_LAMBDAS
#  define REF_COUNT_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
#  define REF_COUNT_NO_CXX11_RAW_LITERALS
#  define REF_COUNT_NO_CXX11_UNICODE_LITERALS
#endif

// C++0x features in 4.5.1 and later
//
#if (REF_COUNT_GCC_VERSION < 40501) || !defined(REF_COUNT_GCC_CXX11)
// scoped enums have a serious bug in 4.4.0, so define REF_COUNT_NO_CXX11_SCOPED_ENUMS before 4.5.1
// See http://gcc.gnu.org/bugzilla/show_bug.cgi?id=38064
#  define REF_COUNT_NO_CXX11_SCOPED_ENUMS
#endif

// C++0x features in 4.6.n and later
//
#if (REF_COUNT_GCC_VERSION < 40600) || !defined(REF_COUNT_GCC_CXX11)
#define REF_COUNT_NO_CXX11_CONSTEXPR
#define REF_COUNT_NO_CXX11_NOEXCEPT
#define REF_COUNT_NO_CXX11_NULLPTR
#define REF_COUNT_NO_CXX11_RANGE_BASED_FOR
#define REF_COUNT_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#endif

// C++0x features in 4.7.n and later
//
#if (REF_COUNT_GCC_VERSION < 40700) || !defined(REF_COUNT_GCC_CXX11)
#  define REF_COUNT_NO_CXX11_FINAL
#  define REF_COUNT_NO_CXX11_TEMPLATE_ALIASES
#  define REF_COUNT_NO_CXX11_USER_DEFINED_LITERALS
#  define REF_COUNT_NO_CXX11_FIXED_LENGTH_VARIADIC_TEMPLATE_EXPANSION_PACKS
#endif

// C++0x features in 4.8.n and later
//
#if (REF_COUNT_GCC_VERSION < 40800) || !defined(REF_COUNT_GCC_CXX11)
#  define REF_COUNT_NO_CXX11_ALIGNAS
#endif

// C++0x features in 4.8.1 and later
//
#if (REF_COUNT_GCC_VERSION < 40801) || !defined(REF_COUNT_GCC_CXX11)
#  define REF_COUNT_NO_CXX11_DECLTYPE_N3276
#  define REF_COUNT_NO_CXX11_REF_QUALIFIERS
#  define REF_COUNT_NO_CXX14_BINARY_LITERALS
#endif

// C++14 features in 4.9.0 and later
//
#if (REF_COUNT_GCC_VERSION < 40900) || (__cplusplus < 201300)
#  define REF_COUNT_NO_CXX14_RETURN_TYPE_DEDUCTION
#  define REF_COUNT_NO_CXX14_GENERIC_LAMBDAS
#  define REF_COUNT_NO_CXX14_DIGIT_SEPARATORS
#  define REF_COUNT_NO_CXX14_DECLTYPE_AUTO
#  if !((REF_COUNT_GCC_VERSION >= 40801) && (REF_COUNT_GCC_VERSION < 40900) && defined(REF_COUNT_GCC_CXX11))
#     define REF_COUNT_NO_CXX14_INITIALIZED_LAMBDA_CAPTURES
#  endif
#endif


// C++ 14:
#if !defined(__cpp_aggregate_nsdmi) || (__cpp_aggregate_nsdmi < 201304)
#  define REF_COUNT_NO_CXX14_AGGREGATE_NSDMI
#endif
#if !defined(__cpp_constexpr) || (__cpp_constexpr < 201304)
#  define REF_COUNT_NO_CXX14_CONSTEXPR
#endif
#if !defined(__cpp_variable_templates) || (__cpp_variable_templates < 201304)
#  define REF_COUNT_NO_CXX14_VARIABLE_TEMPLATES
#endif

//
// Unused attribute:
#if __GNUC__ >= 4
#  define REF_COUNT_ATTRIBUTE_UNUSED __attribute__((__unused__))
#endif
//
// __builtin_unreachable:
#if REF_COUNT_GCC_VERSION >= 40800
#define REF_COUNT_UNREACHABLE_RETURN(x) __builtin_unreachable();
#endif

#ifndef REF_COUNT_COMPILER
#  define REF_COUNT_COMPILER "GNU C++ version " __VERSION__
#endif

// ConceptGCC compiler:
//   http://www.generic-programming.org/software/ConceptGCC/
#ifdef __GXX_CONCEPTS__
#  define REF_COUNT_HAS_CONCEPTS
#  define REF_COUNT_COMPILER "ConceptGCC version " __VERSION__
#endif

// versions check:
// we don't know gcc prior to version 3.30:
#if (REF_COUNT_GCC_VERSION< 30300)
#  error "Compiler not configured - please reconfigure"
#endif
//
// last known and checked version is 4.9:
#if (REF_COUNT_GCC_VERSION > 40900)
#  if defined(REF_COUNT_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  else
// we don't emit warnings here anymore since there are no defect macros defined for
// gcc post 3.4, so any failures are gcc regressions...
//#     warning "Unknown compiler version - please run the configure tests and report the results"
#  endif
#endif

