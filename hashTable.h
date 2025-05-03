#ifndef ABSOLUTE_ASM_HASHTABLE_H
#define ABSOLUTE_ASM_HASHTABLE_H

struct LinkedList{
    char *key;
    unsigned long data;
    struct LinkedList* next;
};

void initLinkedList(struct LinkedList* ll);

void add(struct LinkedList* ll, char *key, unsigned long data);

unsigned long getDataByKey(struct LinkedList* ll, char *key);

void printLinkedList(struct LinkedList* ll);

void destroyLinkedList(struct LinkedList* ll);

struct HashTable{
    int size;
    struct LinkedList** values;
};

void initHashTable(struct HashTable* table);

void addKeyValue(struct HashTable* table, char key[], unsigned long value);

unsigned long getValueByKey(struct HashTable* table, char *key);

void printHashTable(struct HashTable* table);

void destroyTable(struct HashTable* table);

#endif //ABSOLUTE_ASM_HASHTABLE_H
