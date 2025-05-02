#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringList.h"

//struct List{
//    int size;
//    int capacity;
//    char **array;
//};

void initList(struct List* list){
    list->size = 0;
    list->capacity = 10;
    list->array = malloc(list->capacity * sizeof(char *));
    for (int i = 0; i < list->capacity; ++i)
        list->array[i] = calloc(128, 1);
}

void push(struct List* list, char *element){
    strcpy(list->array[list->size++], element);
    if (list->size == list->capacity){
        list->array = realloc(list->array, (list->capacity + 10) * sizeof(char *));
        for (int i = list->capacity; i < list->capacity + 10; ++i)
            list->array[i] = calloc(128, 1);
        list->capacity += 10;
    }
}

char *get(struct List* list, int index){
    if (index >= 0 && index < list->size)
        return list->array[index];
    return "\0";
}

void printList(struct List* list){
    for (int i = 0; i < list->size; ++i)
        printf("'%s' ", list->array[i]);
    printf("\n");
}
