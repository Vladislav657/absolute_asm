#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hashTable.h"
#include "stringList.h"

#define OPERANDS    "E:\\project\\C_labs_2\\absolute_asm\\operands"
#define PROGRAM     "E:\\project\\C_labs_2\\absolute_asm\\program.abs"
#define LISTING     "E:\\project\\C_labs_2\\absolute_asm\\listing"
#define OBJECT      "E:\\project\\C_labs_2\\absolute_asm\\object"

void splitAsmString(char buffer[], char label[128], char operator[128], char operand[128], char comment[128]) {
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
    } else
        comment[0] = '\0';

    char *operator_ptr;
    char *label_ptr = strchr(buffer, ':');
    if (label_ptr != NULL){
        *label_ptr = '\0';
        strcpy(label, buffer);
        operator_ptr = label_ptr + 1;
    } else {
        operator_ptr = buffer;
        label[0] = '\0';
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
}

void getCode(struct List *labels, struct List *operators, struct List *operands, struct List *comments, FILE* prg){
    char buff[256];
    char label[128]={}, operator[128]={}, operand[128]={}, comment[128]={};

    while (fgets(buff, sizeof(buff), prg)){
        splitAsmString(buff, label, operator, operand, comment);
        if (strcmp(operator, "end") == 0)
            break;
        else{
            push(labels, label);
            push(operators, operator);
            push(operands, operand);
            push(comments, comment);
        }
    }
}

void getOperands(struct HashTable *table, FILE *file){
    char buff[128];
    char command[10];
    char type;
    unsigned long code;

    while(fgets(buff, sizeof(buff), file)){
        char *ptr = strchr(buff, '\n');
        if(ptr != NULL)
            *ptr = '\0';

        for (char *ch = buff; *ch != '\0'; ch++)
            *ch = tolower(*ch);

        sscanf(buff, "%s %c %lx", command, &type, &code);

        addKeyValue(table, command, code);
    }
}

unsigned long getAddresses(struct HashTable *marks, struct List *labels, unsigned long start){
    char *current;
    for (int i = 1; i < labels->size; ++i) {
        current = get(labels, i);
        if (current[0] != '\0')
            addKeyValue(marks, current, start);
        start += 3;
    }
    return start;
}

void getListing(struct HashTable *commands, struct HashTable *addresses, struct List *labels, struct List *operators,
        struct List *operands, struct List *comments, unsigned long start){
    FILE* out = fopen(LISTING, "w");

    char *label, *operator, *operand, *comment;
    for (int i = 1; i < operators->size; ++i) {
        label = get(labels, i);
        operator = get(operators, i);
        operand = get(operands, i);
        comment = get(comments, i);

        unsigned long address = getValueByKey(addresses, operand);
        if (address == 0 && strcmp(operator, "resb") != 0 && strcmp(operator, "resw") != 0 &&
        strcmp(operator, "hlt") != 0)
            sscanf(operand, "%lu", &address);

        fprintf(out, "%d\t%04lx %02lx %04lx\t\t|%s\t\t%s\t\t%s\t\t;%s\n", i, start,
                getValueByKey(commands, operator),
                address, label, operator, operand, comment);
        start += 3;
    }

    fclose(out);
}

void getObject(struct HashTable *commands, struct HashTable *addresses, struct List *operators, struct List *operands,
        unsigned long start, unsigned long size){
    FILE *out = fopen(OBJECT, "w");
    fprintf(out, "0301%02lx%04lxXX\n", size, start);
    unsigned long current = start;

    int i = 1;
    while (i < operators->size){
        if (i + 3 <= operators->size){
            fprintf(out, "0900%04lx", current);
            for (int j = 0; j < 3; ++j)
                fprintf(out, "%02lx%04lx",
                        getValueByKey(commands, get(operators, i + j)),
                        getValueByKey(addresses, get(operands, i + j)));
            fprintf(out, "XX\n");

            current += 9;
            i += 3;
        } else {
            fprintf(out, "%02lx00%04lx", (operators->size - i) * 3lu, current);
            for (int j = 0; j < operators->size - i; ++j)
                fprintf(out, "%02lx%04lx",
                        getValueByKey(commands, get(operators, i + j)),
                        getValueByKey(addresses, get(operands, i + j)));
            fprintf(out, "XX\n");
            i = operators->size;
        }
    }
    fprintf(out, "0202%04lx\n", start + size);
}

int main(void) {
    // get operands machine codes:
    struct HashTable commands;
    initHashTable(&commands);

    FILE* ops = fopen(OPERANDS, "r");
    getOperands(&commands, ops);
    fclose(ops);

    printHashTable(&commands);

    // get source code:
    FILE *prg = fopen(PROGRAM, "r");

    struct List labels, operators, operands, comments;
    initList(&labels);
    initList(&operators);
    initList(&operands);
    initList(&comments);

    getCode(&labels, &operators, &operands, &comments, prg);

    fclose(prg);

    printList(&labels);
    printList(&operators);
    printList(&operands);
    printList(&comments);

    // get labels addresses:
    struct HashTable marks;
    initHashTable(&marks);

    unsigned long start;
    sscanf(get(&operands, 0), "%lx", &start);
    unsigned long size = getAddresses(&marks, &labels, start);

    // get listing:
    getListing(&commands, &marks, &labels, &operators, &operands, &comments, start);

    // get object:
    getObject(&commands, &marks, &operators, &operands, start, size);

    return 0;
}
