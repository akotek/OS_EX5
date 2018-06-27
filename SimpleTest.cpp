#include "VirtualMemory.h"
#include "PhysicalMemory.h"

#include <cstdio>
#include <cassert>

int main(int argc, char **argv) {
    printf("%lld\n", 1LL << 10);
    printf("%d\n", TABLES_DEPTH);

    VMinitialize();
//    word_t a;
////    PMread(11, &a);
//    printf("reading %llu\n", (long long int) a);
//

    printf("writing to %llu\n", (long long int) 13);
    VMwrite(13, 3);

//    for (uint64_t i = 0; i < (2 * NUM_FRAMES); ++i) {
//        printf("writing to %llu\n", (long long int) i);
//        VMwrite(5 * i * PAGE_SIZE, i);
//    }

//    for (uint64_t i = 0; i < (2 * NUM_FRAMES); ++i) {
//        word_t value;
//        VMread(5 * i * PAGE_SIZE, &value);
//        printf("reading from %llu %d\n", (long long int) i, value);
//        assert(uint64_t(value) == i);
//    }
    printf("success\n");

    return 0;
}