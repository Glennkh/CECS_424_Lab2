#include "stdio.h"
#include "stdlib.h"

struct Block {
    int block_size; // # of bytes in the data section
    struct Block *next_block; // in C, you have to use struct Block as the type
};

const int OVERHEADSIZE = sizeof(struct Block);
const int MINSIZE = sizeof(void*);
struct Block *free_head;
struct Block *head;

//Initialize heap to specified size
void my_initialize_heap(int size){
    free_head = (struct Block*) malloc(size);
    (*free_head).block_size = size - OVERHEADSIZE;
    (*free_head).next_block = NULL;
}

void *create(int size, void* addr, void* next)
{
    struct Block* block = addr;
    block->block_size = size;
    block->next_block = next;
    return block;
}

void my_free(void* addr){
    
    // set the blocks next equal to the free head
    struct Block* block = addr-OVERHEADSIZE;
    (*block).next_block = free_head;
    //Set free_head equal to the the block we just freed
    free_head = block;

}

// void* my_last(){
//     Block *cur = free_head;
//     while(cur->next_block != NULL)
//        cur = cur->next_block;
//     return cur;
// }

int RoundUpToMultiple(int size){
    int remainder = abs(size) % MINSIZE;

    if (remainder == 0)
        return size;

    return size + MINSIZE - remainder;
}

void *my_alloc(int size){
    
    size = RoundUpToMultiple(size);
    // printf("\t Size: %i\n", size);

    int found = 0;

    // Walk free_head
    struct Block *cur = free_head;
    while(cur != NULL){
        if ( size <= (*cur).block_size ){
            found = 1;
            break;
        } else {
            cur = (*cur).next_block;
        }
    }
    // printf("Gaurd: %d\n", (*cur).block_size - size - OVERHEADSIZE - MINSIZE);
    // printf("Current block size: %d\n", (*cur).block_size);
    // Split or not
    if(found == 1){
        // printf("\t Split: %i\n", size);
        // current free block is large enough to fit the size being allocated
        // AND the excess space in the data portion is sufficient to fit another block with over head
        if( ((*cur).block_size - size - OVERHEADSIZE - MINSIZE) > 0 ){

            // printf("\t\tSplitting size: %d\n", size);
            // Split
            // calculate the left over space after allocating the block
            int leftOver = (*cur).block_size - size;

            // Set size and make next block null because this block is being allocated
            (*cur).block_size = size;
            (*cur).next_block = NULL;
            
            // Create a new block, set free_head pointer equal to newly created block address
            // The size of the new block is the left over space - the Overheadsize (needed to store the block size and next info)
            // Calculate the address = the current address + the OVERHEAD + the size being allocated
            // Set next equal to the next of the current block
            free_head = create( (leftOver - OVERHEADSIZE), (void*)cur+OVERHEADSIZE+size, (*cur).next_block );            

        }else{
            // printf("\t Do not Split: %i\n", size);
            // printf("\t\tNot splitting size: %d\n", size);
            // Do not split
            // printf("\t\tLeft over: %d\n", ((*cur).block_size - size - OVERHEADSIZE - MINSIZE) > 0);
            free_head = (*free_head).next_block;
            // Set next block = NULL
            (*cur).next_block = NULL;

            // Remove block from free list
            
            // printf("\t\tnot splitting allocating b: size: %d addr: %p nex_block: %p\n", (*free_head).block_size, free_head, (*free_head).next_block );

            
        }
    }

    if(found == 1){
        // return addr to new block
        return (void*)cur+OVERHEADSIZE;
    } else {
        //Return 0 if could not find a free space
        return 0;
    }

}

int main()
{
    void* a;
    void* b;
    void* c;
    void* d;
    void* e;
    int n = 3;

    my_initialize_heap(1000);
    // printf("free_head = %p\n", free_head);
    // printf("free_head: size: %d addr: %p nex_block: %p\n", (*free_head).block_size, free_head, (*free_head).next_block );
    // a = my_alloc(sizeof(int));
    // printf("a = %p\n", a);
    // printf("free_head after alloc a: size: %d addr: %p nex_block: %p\n", (*free_head).block_size, free_head, (*free_head).next_block );

    // my_free(a);
    // printf("free_head after free a: size: %d addr: %p nex_block: %p\n", (*free_head).block_size, free_head, (*free_head).next_block );
    // b = my_alloc(sizeof(int));
    // printf("b = %p\n", b);
    // printf("free_head after alloc b: size: %d addr: %p nex_block: %p\n", (*free_head).block_size, free_head, (*free_head).next_block );
    
    

    switch(n){
        case 1:
            //Test 1
            a = my_alloc(sizeof(int));
            printf("a: %p\n", a);
            my_free(a);
            b = my_alloc(sizeof(int));
            printf("b: %p\n", b);
            // The addresses should be the same.
            break;
        case 2:
            //Test 2
            a = my_alloc(sizeof(int));
            b = my_alloc(sizeof(int));
            printf("a: %p\n", a);
            printf("b: %p\n", b);
            // they should be exactly the size of your overhead plus the size of an integer apart.
            // OVERHEAD = 16
            // storage required for in is 8 because size of int is 4 must be a multiple of sizeof(void*) = 8
            // e0 + 16 + 8 = f8
            break;
        case 3:
            //Test 3
            a = my_alloc(sizeof(int));
            b = my_alloc(sizeof(int));
            c = my_alloc(sizeof(int));
            printf("a: %p\n", a);
            printf("b: %p\n", b);
            printf("c: %p\n", c);
            my_free(b);
            d = my_alloc(sizeof(double));
            printf("d: %p\n", d);
            e = my_alloc(sizeof(int));
            printf("e: %p\n", e); // Should equal b address above
            break;
        case 4:
            //Test 4
            a = my_alloc(sizeof(char));
            b = my_alloc(sizeof(int));
            printf("a: %p\n", a);
            printf("b: %p\n", b); //Should equal b in test 2
            break;
        case 5:
            //Test 5
            a = my_alloc(sizeof(int[100]));
            b = my_alloc(sizeof(int));
            
            printf("a: %p\n", a);
            printf("b: %p\n", b);
            my_free(a);
            // printf("b: %p \tShould be same as b above\n", b); // Should not change
            printf("b: %p\tShould be the same as above\n", b);
            break;
        default:
            printf("\n");
    }

    free(head);
    
    return 0;
}


// Output: 
    // Test 1
        // a: 0x7f91f4c028e0
        // b: 0x7f91f4c028e0
    // Test 2
        // a: 0x7fdbe84028e0
        // b: 0x7fdbe84028f8
    // Test 3
        // a: 0x7fb90f700920
        // b: 0x7fb90f700938
        // c: 0x7fb90f700950
        // d: 0x7fb90f700938
        // e: 0x7fb90f700968
    // Test 4
        // a: 0x7f94d7c028e0
        // b: 0x7f94d7c028f8
    // Test 5
        // a: 0x7fc8834028e0
        // b: 0x7fc883402a80
        // b: 0x7fc883402a80       Should be the same as above