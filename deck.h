#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>

#define DEBUG 0
#define MAX_VAL 65535

struct card {
        uint32_t index; /* Original Index */
        struct card *next; /* Next Card in Deck */
};
struct index_map {
        uint32_t next; /* Position in next round */
        uint8_t cycle_mark; /* If cycle is already calculated */
};

uint64_t gcd(uint64_t num1,uint64_t num2);
