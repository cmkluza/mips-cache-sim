#include "cache.h"
#include "debug.h"
#include <stdio.h>
#include <mem.h>

long hits, misses;

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: ./cache-sim set_associativity block_size_b cache_size_kb tracefile\n");
        return 0;
    }
    // get parameters from command line
    long setAssoc = strtol(argv[1], NULL, 10);
    long blockSize = strtol(argv[2], NULL, 10);
    long cacheSize = strtol(argv[3], NULL, 10);
    FILE *fp = fopen(argv[4], "r"); // open trace file for reading

    // check that the file was opened
    if (!fp) {
        fprintf(stderr, "[ERROR] unable to open file: %s\n", argv[4]);
        return 1;
    }
    PRINT_DEBUG("setAssoc == %ld, blockSize == %ld, cacheSize == %ld\n",
                setAssoc, blockSize, cacheSize);

    // allocate the cache
    Cache *cache = cacheAlloc(setAssoc, blockSize, cacheSize);

    // buffer to read trace into
    char address[8]; // address is 8 bytes at most
    long addr; // current address
    int block; // which block the data is in, or else -1

    // process each line in the file
    while (fgets(address, 8, fp)) {
        // process the hex address into a number
        addr = strtol(address, NULL, 16);
        // see if we've hit
        if ((block = hitWay(cache, addr)) != -1) {
            ++hits;
            updateOnHit(cache, addr);
        } else {
            ++misses;
            updateOnMiss(cache, addr);
        }
    }

    // check for irregular stopping
    if (!feof(fp)) {
        fprintf(stderr, "[ERROR] unexpected error reading addresses: %s\n",
                strerror(errno));
    }

    // output necessary data
    printf("%s %ld %ld %ld %f\n",
            argv[4], cacheSize, setAssoc, blockSize,
           ((double) misses) / (hits + misses));

    cacheFree(cache);

    return 0;
}

    
        