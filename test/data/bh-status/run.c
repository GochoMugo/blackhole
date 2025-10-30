#include "../../../src/main.h"

int main(void) {
    bh_daemon *daemon = NULL;
    contra_assert_int_gte(bh_daemon_new(&daemon, getenv("TEST_DATA_DIR")), 0);
    contra_assert_int_gte(bh_status_print(daemon), 0);
    return 0;
}
