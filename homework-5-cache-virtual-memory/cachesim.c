#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int log2(int n);
//define frame structure
typedef struct frame{
    char data[1024];
    int tag;
    int valid;
    int lru;
} frame;
//create set structure
typedef struct set{
    frame *frames;
} set;
//create cache structure
typedef struct cache{
    int capacity;
    int ways;
    int block_size;
    int numSets;
    int offNum;  
    int setShift;
    int setMask;
    int tagShift;  
    set *sets;
} cache;
//make memory as a 1d array
char memory[16*1024*1024];

//helper function for evicting using LRU
void useLRU(set* sets, int tag, int offset_bits, int address_bits, int setIndex, unsigned char* datum, cache *mycache, int address, int access_size){
    int lruCounter = 0;
    int numLRU = 0;
    //iterate through cache
    for (int i = 0; i < mycache->ways; i++){
        //check if not valid within set
        if (!sets->frames[i].valid){
            //if not valid within a set
            memcpy(sets->frames[i].data, &memory[address_bits], mycache->block_size);
            sets->frames[i].valid = 1;
            sets->frames[i].tag = tag;
            sets->frames[i].lru = 0;
            //this is a load miss
            printf("miss ");
            for (int j = 0; j < access_size; j++){
                //print data
                printf("%02hhx", sets->frames[i].data[offset_bits + j]);
            }
            //print newline
            printf("\n");
            //exit
            return;
        }
        //if all frames full
        else if (sets->frames[i].lru > lruCounter){
            numLRU = i;
            lruCounter = sets->frames[i].lru;
        }
    }
    //after iterating through set and all are valid 
    int diffLRU = sets->frames[numLRU].tag << mycache->tagShift;
    diffLRU |= setIndex << mycache->setShift;

    memcpy(&memory[diffLRU], sets->frames[numLRU].data, mycache->block_size);
    memcpy(sets->frames[numLRU].data, &memory[address_bits], mycache->block_size);
    memcpy(datum, &sets->frames[numLRU].data[offset_bits], access_size);

    sets->frames[numLRU].lru = 0;
    sets->frames[numLRU].tag = tag;
    sets->frames[numLRU].valid = 1;

    printf("miss ");
    for (int j = 0; j < access_size; j++){
        printf("%02hhx", datum[j]);
    }
    printf("\n");
}

//helper function for storing
void storefunc(set* sets, int tag, int offset_bits, int address_bits, int setIndex, char* data, cache *mycache, int address, int access_size){
    //initialize hit as false
    bool hit = false;
    //iterate through cache
    for (int i = 0; i < mycache->ways; i++){
        if (sets->frames[i].valid && sets->frames[i].tag == tag){
            //hit case (store hit)
            memcpy(&sets->frames[i].data[offset_bits], data, access_size);
            memcpy(&memory[address_bits], sets->frames[i].data, mycache->block_size);
            printf("hit\n");
            sets->frames[i].lru = 0;
            //set hit to true
            hit = true;
            }
        else{
            //if not a hit update lru
            sets->frames[i].lru++;
        }
    }
    //breaks out if hit, now moving into store case
    if (hit) return;
    printf("miss\n");
    //for a store miss
    char offData[16 * 4 * 1024];
    memcpy(offData, &memory[address_bits], mycache->block_size);
    memcpy(&offData[offset_bits], data, access_size);
    memcpy(&memory[address_bits], offData, mycache->block_size);
}
//helper function for loading
void loadfunc(set* sets, int tag, int offset_bits, int address_bits, int setIndex, cache *mycache, int address, int access_size){
    //initialize hit to fale
    bool hit = false;
    //will be used for loading in data
    unsigned char datum[512];
    //iterate through cache
    for (int i = 0; i < mycache->ways; i++){
        if (sets->frames[i].valid && sets->frames[i].tag == tag){
            //hit case
            memcpy(datum, &sets->frames[i].data[offset_bits], access_size);
            sets->frames[i].lru = 0;
            hit = true;
            //print hit and then data
            printf("hit ");
            for (int j = 0; j < access_size; j++){
                printf("%02hhx", datum[j]);
            }
            //newline
            printf("\n");
        }
        else {
            //if not a hit then update lru in case
            sets->frames[i].lru++;
        }
    }
    //break out if hit is true, move onto  miss case
    if (hit) return;
    useLRU(sets, tag, offset_bits, address_bits, setIndex, datum, mycache, address, access_size);
    }  
//helper function for initializing cache
void buildCache(cache *mycache){
    //create all parts of cache using given capacity, ways, and block_size
    mycache->numSets = mycache->capacity / mycache->block_size / mycache->ways;
    mycache->offNum = mycache->block_size - 1;  
    mycache->setShift = log2(mycache->block_size);
    mycache->setMask = ((1 << log2(mycache->numSets)) - 1);
    mycache->tagShift = log2(mycache->numSets) + mycache->setShift;
    mycache->sets = (set *)malloc(mycache->numSets * sizeof(set));

    //iterate through and calloc
    for (int i = 0; i < mycache->numSets; i++){
        mycache->sets[i].frames = (frame *)calloc(mycache->ways, sizeof(frame));
        if (!mycache->sets[i].frames)
            //control for errors
            exit(-1);
    }
    if (!mycache->sets)
        //control for errors
        exit(-1);
}

int main(int argc, char *argv[]){
    //open file and check if can work with it
    FILE *file = fopen(argv[1], "r");  
    if (file == NULL){
        perror("Failed to open file");  
        return EXIT_SUCCESS;
    }
    //create this cache structure
    cache mycache;
    mycache.capacity = atoi(argv[2])<<10;
    mycache.ways = atoi(argv[3]);
    mycache.block_size = atoi(argv[4]);

    //initialize cache
    buildCache(&mycache);

    //store or load, data, address, access_size
    char type[10];
    char data[32];
    int address;
    int access_size;

    while (fscanf(file, "%s", type) != EOF)  {
        fscanf(file, "%x", &address);
        fscanf(file, "%d", &access_size);
        cache* pointer = &mycache;
        int setIndex = (address >> pointer->setShift) & pointer->setMask;
        int tag = address >> pointer->tagShift;
        int offset_bits = address & pointer->offNum;  
        int address_bits = address & ~pointer->offNum;  
        set *sets = &pointer->sets[setIndex];

        if (!strcmp(type, "load")){
            //load type, print load and address
            printf("%s 0x%x ", type, address);
            //call load helper function
            loadfunc(sets, tag, offset_bits, address_bits, setIndex, &mycache, address, access_size);
        }
        else{
            //because store will need to read the data
            for (int i = 0; i < access_size; i++){
                fscanf(file, "%2hhx", &data[i]);
            }
            //store type, print store and address
            printf("%s 0x%x ", type, address);
            //call store helper function
            storefunc(sets, tag, offset_bits, address_bits, setIndex, data, &mycache, address, access_size);
        }
    }
    //close file (done reading)
    fclose(file);  
    //go through and free at each way, then at each set
    cache* pointer = &mycache;
    for (int i = 0; i < pointer->numSets; i++){
        free(pointer->sets[i].frames);
    }
    free(pointer->sets);
    return EXIT_SUCCESS;
}

//log2 helper function
int log2(int n){
    int result = 0;
    while (n >>= 1)
        result++;
    return result;
}