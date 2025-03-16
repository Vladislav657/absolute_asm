#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    FILE* fp = fopen("program.abs", "r");
    char buffer[1024];
    while(fgets(buffer, sizeof(buffer), fp)) {
        char label[1024]={}, operator[1024]={}, operand[1024]={}, comment[1024]={};

        char *ptr = strchr(buffer, '\n');
        if(ptr != NULL)
            *ptr = '\0';
        for (char *ch = buffer; *ch != '\0'; ch++)
            *ch = tolower(*ch);

        char *comment_ptr = strrchr(buffer, '/');
        if (comment_ptr != NULL){
            comment_ptr++;
            strcpy(comment, comment_ptr);
            comment_ptr--;
            *comment_ptr = '\0';
        }

        char *operator_ptr;
        char *label_ptr = strchr(buffer, '-');
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

        *operand_ptr = '\0';
        strcpy(operator, operator_ptr);
        operand_ptr++;

        while (*operand_ptr == ' ' || *operand_ptr == '\t')
            operand_ptr++;
        char *end_ptr = strchr(operand_ptr, ' ');
        if (end_ptr == NULL)
            end_ptr = strchr(operand_ptr, '\t');
        if (end_ptr != NULL)
            *end_ptr = '\0';

        strcpy(operand, operand_ptr);

        printf("label: %s\t\toperator: %s\t\toperand: %s\t\tcomment: %s\n", label, operator, operand, comment);

//        puts(buffer);
    }

    fclose(fp);
    return 0;
}
