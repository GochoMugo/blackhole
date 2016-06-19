#ifndef _BH_tests_h_
#define _BH_tests_h_ 1

#define _GNU_SOURCE

#define assert_ok(expr) assert_int_equal(expr, 0)

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>

#include <cmocka.h>

#include "../src/main.h"

#include "config.h"
#include "daemon.h"
#include "error.h"
#include "hook.h"

#endif
