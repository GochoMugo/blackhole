#ifndef _BH_tests_h_
#define _BH_tests_h_ 1

#define assert_ok(expr) assert_int_equal(0 <= expr, 1)

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <setjmp.h>

#include <cmocka.h>

#include "../src/main.h"
#include "util.h"

#if __has_include("config.h")
#include "config.h"
#endif

#if __has_include("counter.h")
#include "counter.h"
#endif

#if __has_include("daemon.h")
#include "daemon.h"
#endif

#if __has_include("error.h")
#include "error.h"
#endif

#if __has_include("git.h")
#include "git.h"
#endif

#if __has_include("hook.h")
#include "hook.h"
#endif

#if __has_include("print.h")
#include "print.h"
#endif

#if __has_include("status.h")
#include "status.h"
#endif

#define skip_if_filtered_out(str)                                              \
  if (NULL != getenv("TEST_FILTER") &&                                         \
      NULL == strstr(str, getenv("TEST_FILTER")))                              \
    skip();

#endif
