#include "kernel/types.h"
#include "kernel/sysinfo.h"
#include "user/user.h"
#include "kernel/stat.h"

void sysinfotest() {
    printf( "sysinfotest: Starting tests\n");
    struct sysinfo info;

    if (sysinfo(&info) < 0) {
        printf("sysinfotest: sysinfo failed\n");
        exit(1);
    }
    if (info.freemem == 0) {
        printf("sysinfotest: freemem is 0\n");
        exit(1);
    }
    if (info.nproc == 0) {
        printf("sysinfotest: nproc is 0\n");
        exit(1);
    }
    printf("sysinfotest: OK\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    sysinfotest();
    return 0;
}
