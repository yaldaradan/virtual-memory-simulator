#include <stdio.h>
#include <stdlib.h>

struct Page {
    int frame;
    int valid;
    int dirty;
    int swap;
};

struct Node {
    int p;
    struct Node *next;
};
struct Node *head = NULL;

void push(int p){
    struct Node *n = malloc(sizeof(struct Node));
    n->p = p;
    n->next = NULL;
    if (head == NULL) {
        head = n;
        return;
    }
    struct Node *cur = head;
    while (cur->next != NULL) cur = cur->next;
    cur->next = n;
}

int pop() {
    if (head == NULL) return -1;
    struct Node *tmp = head;
    int p = tmp->p;
    head = head->next;
    free(tmp);
    return p;
}

void removeNode(int p) {
    if (head == NULL) return;
    if (head->p == p) {
        struct Node *tmp = head;
        head = head->next;
        free(tmp);
        return;
    }
    struct Node *cur = head;
    while (cur->next != NULL) {
        if (cur->next->p == p) {
            struct Node *tmp = cur->next;
            cur->next = tmp->next;
            free(tmp);
            return;
        }
        cur = cur->next;
    }
}

int main(int argc, char *argv[]) {

    int frames = atoi(argv[1]); 
    int offset = atoi(argv[2]); 
    int p = 32 - offset; 
    int size = 1 << p;
    struct Page *pageTable = calloc(size, sizeof(struct Page));
    int framesTable[frames];
    int hits = 0;
    int minor = 0;
    int major = 0;
    int swapSpace = 0;
    
    // 0 means invalid, clean, not in the swap space. 1 is valid, dirty, in the swap space
    for (int i = 0; i < size; i++){
        pageTable[i].frame = 0;
        pageTable[i].valid = 0;
        pageTable[i].dirty = 0;
        pageTable[i].swap = 0;
    }
    //0 means free. 1 is not free
    for (int i = 0; i < frames; i++){
        framesTable[i] = 0;
    }

    char operation;
    int addr;
    int freeFrame = -1;
    while (scanf(" %c %d", &operation, &addr) == 2){
        int offsetVal = addr % (1 << offset);
        int page = addr >> offset;
        if (operation == 'w'){
            if (pageTable[page].valid == 1){
                hits++;
                pageTable[page].dirty = 1;
                removeNode(page);
                push(page);
                int physicalAddr = (pageTable[page].frame << offset) + offsetVal;
                printf("\nphysical address is frame %d", physicalAddr);
            } 
            else{
                for (int i = 0; i < frames; i++){
                    if (framesTable[i]==0){
                        freeFrame = i;
                        break;
                    } 
                }
                if (freeFrame<0){
                    int swapOut = pop();
                    if (pageTable[swapOut].dirty == 0){
                        pageTable[page].frame = pageTable[swapOut].frame;
                        pageTable[swapOut].valid = 0;
                        pageTable[swapOut].dirty = 0;
                        pageTable[page].dirty = 1;
                        pageTable[page].valid = 1;
                        push(page);
                        minor++;
                    }
                    else{
                        pageTable[page].frame = pageTable[swapOut].frame;
                        pageTable[swapOut].valid = 0;
                        pageTable[swapOut].dirty = 0;
                        pageTable[swapOut].swap = 1;
                        swapSpace++;
                        pageTable[page].dirty = 1;
                        pageTable[page].valid = 1;
                        push(page);
                        major++;
                    }
                    //remember if it ois a dirty page youre taking out you would have to set the dirty bit to false
                }
                else{
                    pageTable[page].frame = freeFrame;
                    framesTable[freeFrame] = 1;
                    pageTable[page].valid = 1;
                    pageTable[page].dirty = 1;
                    minor++;
                    freeFrame = -1;
                    push(page);
                }
                if (pageTable[page].swap == 1){
                    pageTable[page].swap = 0;
                    swapSpace--;
                }
                int physicalAddr = (pageTable[page].frame << offset) + offsetVal;
                printf("\nphysical address is frame %d", physicalAddr);
            }
        }






        else{
            if (pageTable[page].valid == 1){
                hits++;
                removeNode(page);
                push(page);
                int physicalAddr = (pageTable[page].frame << offset) + offsetVal;
                printf("\nphysical address is frame %d", physicalAddr);
            }
            else{
                for (int i = 0; i < frames; i++){
                    if (framesTable[i]==0){ 
                        freeFrame = i;
                        break;
                    }
                }
                if (freeFrame<0){
                    int swapOut = pop();
                    if (pageTable[swapOut].dirty == 0){
                        pageTable[page].frame = pageTable[swapOut].frame;
                        pageTable[swapOut].valid = 0;
                        pageTable[swapOut].dirty = 0;
                        pageTable[page].dirty = 0;
                        pageTable[page].valid = 1;
                        push(page);
                        minor++;
                    }
                    else{
                        pageTable[page].frame = pageTable[swapOut].frame;
                        pageTable[swapOut].valid = 0;
                        pageTable[swapOut].dirty = 0;
                        pageTable[swapOut].swap = 1;
                        swapSpace++;
                        pageTable[page].dirty = 0;
                        pageTable[page].valid = 1;
                        push(page);
                        major++;
                    }
                    //remember if it ois a dirty page youre taking out you would have to set the dirty bit to false
                }
                else{
                    pageTable[page].frame = freeFrame;
                    framesTable[freeFrame] = 1;
                    pageTable[page].valid = 1;
                    pageTable[page].dirty = 0;
                    minor++;
                    freeFrame = -1;
                    push(page);
                }
                if (pageTable[page].swap == 1){
                    pageTable[page].swap = 0;
                    swapSpace--;
                }
                int physicalAddr = (pageTable[page].frame << offset) + offsetVal;
                printf("\nphysical address is frame %d", physicalAddr);
            }
        }
    }
    printf("major faults come from dirty pages swaped out, minor faults are from clean pages replaces or new pages loaded.");
    printf("\nmajor faults = %d, minor faults = %d, hits = %d, pages stored in the swap space = %d.", major, minor, hits, swapSpace);
    return 0;
}