#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>
#include "hashTable.h"

#define OPERANDS    "E:\\project\\C_labs_2\\absolute_asm\\operands"
#define PROGRAM     "E:\\project\\C_labs_2\\absolute_asm\\program.abs"

void splitAsmString(char buffer[]) {
    char label[128]={}, operator[128]={}, operand[128]={}, comment[128]={};

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

void getOperands(struct HashTable *table, FILE *file){
    char buff[128];
    char command[10];
    char type;
    int address;

    while(fgets(buff, sizeof(buff), file)){
        char *ptr = strchr(buff, '\n');
        if(ptr != NULL)
            *ptr = '\0';

        for (char *ch = buff; *ch != '\0'; ch++)
            *ch = tolower(*ch);

        sscanf(buff, "%s %c %x", command, &type, &address);

        addKeyValue(table, command, address);
    }
}

int main(void) {
    struct HashTable operands;
    initHashTable(&operands);

    FILE* ops = fopen(OPERANDS, "r");
    getOperands(&operands, ops);
    fclose(ops);

//    printHashTable(&operands);
    FILE *prg = fopen(PROGRAM, "r");
    char buff[256];

    while (fgets(buff, sizeof(buff), prg)){
        splitAsmString(buff);
    }
    fclose(prg);
    return 0;
}
