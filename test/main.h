#ifndef _BH_tests_h_
#define _BH_tests_h_ 1

#define _GNU_SOURCE

#define assert_ok(expr) assert_int_equal(0 <= expr, 1)

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>

#include <cmocka.h>

#include "../src/main.h"

#include "config.h"
#include "counter.h"
#include "daemon.h"
#include "error.h"
#include "git.h"
#include "hook.h"
#include "status.h"

#endif
