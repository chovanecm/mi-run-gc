#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define HEAP_SIZE 2048
#define ROOT_SIZE 8
#define TEXT_SIZE 256

#define INSTR_SET_SIZE 2

#define NOP 0
#define OOK 1

const int instructionSet[INSTR_SET_SIZE] = { NOP, OOK };

#define CP_SIZE 3
const char constantPool[CP_SIZE][TEXT_SIZE] = {"ook", "oook", "ook!"};

/**
 * How often we should print the report
 */
#define REPORT_AFTER 10


typedef struct DataStructure Data;

struct DataStructure {

    bool marked;

    char text[TEXT_SIZE];
    Data* next;
};

Data* heap[HEAP_SIZE];
/***
 * Pointer to live objects
 */
Data* root[ROOT_SIZE];

int allocatedRecords = 0;

void reportOutOfMemoryAndExit() {
    fprintf(stderr, "Out of memory, bye!\n");
    exit(1);
};

void reportHeapStatus() {
  printf("\nalloc:\t%d\nfree:\t%d\nMAX:\t%d\n", allocatedRecords, HEAP_SIZE-allocatedRecords, HEAP_SIZE);
};
/**
 * Functiono used for reporting
 * @param data
 * @return
 */
int getTailLength(Data* data) {
    int result = 0;
    while ((data = data->next) != NULL) {
        result++;
    }
    return result;
}

void printTail(Data* data) {
    while ((data = data->next) != NULL) {
        printf("->%s", data->text);
    }
}

void reportRoots() {
    for (int i=0; i<ROOT_SIZE; i++) {
        if (root[i]) {
          printf("[%d]: %s (%d)", i, root[i]->text, getTailLength(root[i]));
          printTail(root[i]);
          printf("\n");
        }
    }
};

void mark(Data* data) {
    if (data == NULL || data->marked) {
        return;
    }
    data->marked = true;
    mark(data->next);
}

void sweep() {
    for (int i = 0; i < HEAP_SIZE; i++) {
        if (heap[i] == NULL)
            continue;
        if (heap[i]->marked) {
            heap[i]->marked = false;
        } else {
            free(heap[i]);
            heap[i] = NULL;
            allocatedRecords--;
        }
    }
}
void markAndSweep() {
    for (int i = 0; i < ROOT_SIZE; i++) {
        mark(root[i]);
    }
    sweep();
}

int findFreeSlotInHeap(){
    for (int attempt = 0; attempt < 2; attempt++) {
        for (int i = 0; i < HEAP_SIZE; i++) {
            if (heap[i] == 0) {
                return i;
            }
        }
        //when nothing found, run mark and sweep and try again
        markAndSweep();
    }
    reportOutOfMemoryAndExit();
    return -1;
};

Data* allocateNewData() {

    // find free slot in heap:
    int heapIndex = findFreeSlotInHeap();

    // claim room for the data structure
    Data* newData = (Data*)malloc(sizeof(Data));
    if (newData == NULL) {
        reportOutOfMemoryAndExit();
        return NULL;
    }

    // book the record
    heap[heapIndex] = newData;
    allocatedRecords++;

    // initialize the structure
    newData->marked = false;
    memset(newData->text, 0, TEXT_SIZE);

    return newData;
};

void ook() { // single operation of our program

    // a = new Data();
    Data* data = allocateNewData();

    // a.text = "..."
    strcpy(data->text, (char*)&constantPool[rand() % CP_SIZE]);
    // a.next = null | roots[<random index>]
    data->next = (rand() % 2) ?
              NULL
            : root[rand() % (sizeof(root) / sizeof(Data*))];

    // roots[<random index>] = a
    root[rand() % (sizeof(root) / sizeof(Data*))] = data;
}

void singleProgramStep() {

  int instruction = instructionSet[rand() % INSTR_SET_SIZE];
  switch (instruction) {
      case NOP: // do nothing
          break;
      case OOK: // do something
          ook();
          break;
      default:
          break;
  };
}

int main(int argc, char** argv) {

    // init:
    memset(heap, 0, sizeof(heap));
    memset(root, 0, sizeof(root));

    int pc = 0;


    // run
    for(;;) {
      pc++;
      singleProgramStep();
      if ((pc % REPORT_AFTER) == 0) {
        reportHeapStatus();
        reportRoots();
      }
    }
    return 0;
};
