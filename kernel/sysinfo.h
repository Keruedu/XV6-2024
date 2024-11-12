#include "types.h"

struct sysinfo {
    uint64 freemem;
    uint64 nproc;
};

uint64 freemem(void);
uint64 getnproc(void);