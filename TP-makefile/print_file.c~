#include "print_file.h"
#include <stdio.h>

void print_file(char* filename){
    FILE* f_to_read = fopen(filename, "r");
    if(f_to_read != NULL){
        char current_char = '\0';
        while(fscanf(f_to_read, "%c", &current_char) != EOF){
            printf("%c", current_char);
        }
        //Et on n'oublie pas de refermer les fichiers
        fclose(f_to_read);
    }
    else{
        printf("File %s not found\n", filename);
    }
}
