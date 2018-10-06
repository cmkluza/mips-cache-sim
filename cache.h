#ifndef CACHE_H
#define CACHE_H

/////////////////////////////
// Structs, enums, typdefs //
/////////////////////////////

/** 
 * A collection of ccahe configuration info and arrays storing relevant cache data.
 */
typedef struct {
    int numSets; /**< number of sets within the cache */
    int numBlocks; /**< number of blocks within the cache */
    int setAssoc; /**< set associativity of the cache */

    int offsetBits; /**< number of offset bits in address */
    int indexBits; /**< number of index bits in address */
    int tagBits; /**< number of tag bits in address */

    /**
     * Array of block tags. Subscripted as `[i][j]` where `i` refers to the set and `j`
     * refers to the block.
     */
    unsigned int **tagArray;
    /**
     * Array of block LRU statuses. Subscripted as `[i][j]` where `i` refers to the set
     * and `j` refers to the block.
     */
    int **lruArray;
} Cache;

///////////////////////////////////////
// Functions specified in assignment //
///////////////////////////////////////

/**
 * Outputs the cache set in which the address falls.
 *
 * @param cache a pointer to a configured cache 
 * @param addr the address to check
 * @return a number [0,numSets) that indicates which set the address falls in
 */
int whichSet(Cache *cache, int addr){

    int setNum=-1; 
    for (int i=0; i< cache.numSets; i++) {
        if ( tagBits(cache, addr) == cache.tagArray[cache.index][i] ) {
        setNum= i;
        }
    }
    
    return setNum;
}

/**
 * Outputs the number of bits in the set index field of the address.
 *
 * @param cache a pointer to a configured cache
 * @return the number of bits in the set index field for the given cache
 */
int setIndexLength(Cache *cache);

/**
 * Outputs the number of bits in the line offset field of the address.
 *
 * @param cache a pointer to a configured cache
 * @return the number of bits in the line offset field for the given cache
 */
int offsetLength(Cache *cache){
   return cache.offsetBits;          // IDK how the struct thing works
}

/**
 * Outputs the tag bits associated with the address.
 *
 * @param cache a pointer to a configured cache
 * @param addr the address to get the tag bits from
 * @return the tag bits associated with the address for the given cache
 */
int tagBits(Cache *cache, int addr);

/**
 * If there is a hit, this outputs the cache way in which the accessed line can be found; it
 * returns -1 if there is a cache miss.
 *
 * @param cache a pointer to a configured cache
 * @param addr the address to attempt to pull from the cache
 * @return the set in which the accessed block can be found or -1 for a miss
 */
int hitWay(Cache *cache, int addr){
    
    if (whichSet(Cache *cache, int addr)==-1){    //if whichSet returns -1 it is a miss
        updateOnMiss(Cache *cache, int addr);
        return -1;
    }
    
    else{                                         //else it is a hit
        updateOnHit(Cache *cache, int addr);
        return whichSet(Cache *cache, int addr)+1;    
    }
}
/**
 * Updates the `tagArray` and `lruArray` upon a hit. This function is only called on a cache
 * hit.
 *
 * @param cache a pointer to a configured cache
 * @param addr the address where the hit occurred
 */
void updateOnHit(Cache *cache, int addr);

/**
 * Updates the `tagArray` and `lruArray` upon a miss. This function is only called on a cache
 * miss.
 *
 * @param cache a pointer to a configured cache
 * @param addr the address where the miss occurred
 */
void updateOnMiss(Cache *cache, int addr);

/**
 * Allocates and configures a cache struct given the cache parameters.
 *
 * @param setAssoc the set associativity for the cache
 * @param blockSize the block size for the cache (bytes)
 * @param cacheSize the total cache size (kilobytes)
 * @return a pointer to a cache configured for the given parameters
 */
Cache *cacheAlloc(int setAssoc, int blockSize, int cacheSize);

#endif
