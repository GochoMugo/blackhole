#include "../../../src/main.h"
#include "../../util.h"

int main(void) {
    const char *path = "../bh-status";
    bh_daemon *daemon = NULL;
    chdir(path_join(getenv("TEST_DIR"), "data/bh-status"));
    bh_daemon_new(&daemon, path);
    bh_status_print(daemon);
    return 0;
}
