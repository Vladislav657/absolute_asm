#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>
#include "hashTable.h"

void splitAsmString(FILE* code){
    char buffer[1024];
    while(fgets(buffer, sizeof(buffer), code)) {
        char label[1024]={}, operator[1024]={}, operand[1024]={}, comment[1024]={};

        char *ptr = strchr(buffer, '\n');
        if(ptr != NULL)
            *ptr = '\0';
        for (char *ch = buffer; *ch != '\0'; ch++)
            *ch = tolower(*ch);

        char *comment_ptr = strrchr(buffer, ';');
        if (comment_ptr != NULL){
            comment_ptr++;
            strcpy(comment, comment_ptr);
            comment_ptr--;
            *comment_ptr = '\0';
        }

        char *operator_ptr;
        char *label_ptr = strchr(buffer, ':');
        if (label_ptr != NULL){
            *label_ptr = '\0';
            strcpy(label, buffer);
            operator_ptr = label_ptr + 1;
        } else {
            operator_ptr = buffer;
        }

        while (*operator_ptr == ' ' || *operator_ptr == '\t')
            operator_ptr++;
        char *operand_ptr = strchr(operator_ptr, ' ');
        if (operand_ptr == NULL)
            operand_ptr = strchr(operator_ptr, '\t');
        if (operand_ptr != NULL) {
            *operand_ptr = '\0';
            operand_ptr++;
        }
        strcpy(operator, operator_ptr);

        if (operand_ptr != NULL) {
            while (*operand_ptr == ' ' || *operand_ptr == '\t')
                operand_ptr++;
            char *end_ptr = strchr(operand_ptr, ' ');
            if (end_ptr == NULL)
                end_ptr = strchr(operand_ptr, '\t');
            if (end_ptr != NULL)
                *end_ptr = '\0';

            strcpy(operand, operand_ptr);
        }

        printf("label: %s\t\toperator: %s\t\toperand: %s\t\tcomment: %s\n\n", label, operator, operand, comment);
    }
}

int main(void) {
    struct HashTable table;
    initHashTable(&table);

    addKeyValue(&table, "start", 1);
    addKeyValue(&table, "end", 2);
    addKeyValue(&table, "byte", 3);
    addKeyValue(&table, "word", 4);
    addKeyValue(&table, "resb", 5);
    addKeyValue(&table, "resw", 6);
    addKeyValue(&table, "mov", 7);
    addKeyValue(&table, "add", 8);
    addKeyValue(&table, "sub", 9);
    addKeyValue(&table, "mul", 10);
    addKeyValue(&table, "div", 11);
    addKeyValue(&table, "orr", 12);
    addKeyValue(&table, "and", 13);
    addKeyValue(&table, "xor", 14);
    addKeyValue(&table, "not", 15);
    addKeyValue(&table, "jmp", 16);
    addKeyValue(&table, "loop", 17);

    printHashTable(&table);

    printf("\n%d\n", getValueByKey(&table, "and"));

    destroyTable(&table);

    return 0;
}
