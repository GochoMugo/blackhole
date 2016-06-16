#include "main.h"

int main(void) {
    cmocka_run_group_tests(tests_bh_error, NULL, NULL);
    return 0;
}
