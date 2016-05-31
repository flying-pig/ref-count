#ifndef __REF_COUNT_CONFIG_H_
#define __REF_COUNT_CONFIG_H_

#include <config/select_compiler_config.h>
#ifdef REF_COUNT_COMPILER_CONFIG
#  include REF_COUNT_COMPILER_CONFIG
#endif

#include <config/select_stdlib_config.h>
#ifdef REF_COUNT_STDLIB_CONFIG
#include REF_COUNT_STDLIB_CONFIG
#endif

#include <config/select_platform_config.h>
#ifdef REF_COUNT_PLATFORM_CONFIG
#include REF_COUNT_PLATFORM_CONFIG
#endif

#ifdef REF_COUNT_HAS_PRAGMA_ONCE
#pragma once
#endif

#endif  // __REF_COUNT_CONFIG_H_
