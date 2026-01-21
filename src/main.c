#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cmd_processor.h"
#include "compiler.h"

#define MEM_SIZE (1<<16)

int main(int argc, char **argv){

    char *input_file_data;
    FILE *output_file;

    bool use_gcc;
    char *output_c_file_path;
    char *output_file_path;

    if(process_cmd(argc, argv, &input_file_data, &output_file, &output_file_path, &output_c_file_path, &use_gcc) == 1){
        printf("Compilation failed.\n");
        return 1;
    }

    compile(input_file_data, MEM_SIZE, output_file);

    fclose(output_file);
    free(input_file_data);

    if(use_gcc){
        char *gcc_cmd_string = calloc(64 + strlen(output_c_file_path) + strlen(output_file_path), sizeof(char));
        sprintf(gcc_cmd_string, "gcc %s -o %s", output_c_file_path, output_file_path);
        if(system(gcc_cmd_string) != 0){
            printf("GCC ERROR\nCompilation failed.\n");
            return 1;
        }
        char *file_rem_string = calloc(32 + strlen(output_c_file_path), sizeof(char));
        sprintf(file_rem_string, "rm -f %s", output_c_file_path);
        system(file_rem_string);
    }
    

    printf("Compilation success.\n");
    return 0;
}