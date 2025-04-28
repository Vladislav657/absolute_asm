#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashTable.h"

//struct LinkedList{
//    char *key;
//    int data;
//    struct LinkedList* next;
//};

void initLinkedList(struct LinkedList* ll){
    ll->key = NULL;
    ll->data = 0;
    ll->next = NULL;
}

void add(struct LinkedList* ll, char *key, int data){
    if (ll->key == NULL){
        ll->key = calloc(10, 1);
        strcpy(ll->key, key);
        ll->data = data;
        struct LinkedList *new = malloc(sizeof(struct LinkedList));
        initLinkedList(new);
        ll->next = new;
    } else
        add(ll->next, key, data);
}

int getDataByKey(struct LinkedList* ll, char *key){
    if (ll->key == NULL){
//        printf("No such key\n");
        return 0;
    }
    else if (strcmp(ll->key, key) == 0)
        return ll->data;
    else
        return getDataByKey(ll->next, key);
}

void printLinkedList(struct LinkedList* ll){
    if (ll->key == NULL)
        return;
    printf("%s: %d\n", ll->key, ll->data);
    printLinkedList(ll->next);
}

void destroyList(struct LinkedList* ll){
    if (ll->next != NULL)
        destroyList(ll->next);
    free(ll->key);
    free(ll);
}

//struct HashTable{
//    int size;
//    struct LinkedList** values;
//};

void initHashTable(struct HashTable* table){
    table->size = 17;
    table->values = malloc(sizeof(struct LinkedList*) * table->size);
    for (int i = 0; i < table->size; ++i) {
        table->values[i] = malloc(sizeof(struct LinkedList));
        initLinkedList(table->values[i]);
    }
}

int getIndex(int M, const char *key){
    unsigned short K = (key[0] << 8) | key[1];
    for (int i = 2; key[i] != 0; i += 2) {
        K = (K >> 1) | (K << 15);
        int word = (key[i] << 8) | key[i + 1];
        K ^= word;
    }
    return K % M;
}

void addKeyValue(struct HashTable* table, char key[], int value){ //x = (x >> k) | (x << (32 - k));
    int index = getIndex(table->size, key);
    add(table->values[index], key, value);
}

int getValueByKey(struct HashTable* table, char *key){
    int index = getIndex(table->size, key);
    return getDataByKey(table->values[index], key);
}

void printHashTable(struct HashTable* table){
    for (int i = 0; i < table->size; ++i)
        printLinkedList(table->values[i]);
}

void destroyTable(struct HashTable* table){
    for (int i = 0; i < table->size; ++i)
        destroyList(table->values[i]);
    free(table->values);
}
