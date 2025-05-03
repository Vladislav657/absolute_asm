#ifndef ABSOLUTE_ASM_STRINGLIST_H
#define ABSOLUTE_ASM_STRINGLIST_H

struct List{
    int size;
    int capacity;
    char **array;
};

void initList(struct List* list);

void push(struct List* list, char *element);

char *get(struct List* list, int index);

void printList(struct List* list);

void destroyList(struct List* list);

#endif //ABSOLUTE_ASM_STRINGLIST_H
