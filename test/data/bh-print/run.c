#include "../../../src/main.h"

int main(void) {
    bh_print_marker("Marker");
    bh_print_ok();
    bh_print_status(NULL);
    bh_error_set(BH_ERR);
    bh_print_status(bh_error_get());
    return 0;
}
