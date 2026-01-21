#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    CMD_HELP,
    CMD_ERROR,
    CMD_OUTPUT_SEL,
    CMD_INPUT_SEL,
    CMD_GCC_MODE
} arg_mode_t;

static arg_mode_t get_arg_id(char *i_cmd_str){

    struct {const char *cmd_str; arg_mode_t id;} commands [] = {
        {"--help", CMD_HELP},
        {"-o", CMD_OUTPUT_SEL},
        {"-i", CMD_INPUT_SEL},
        {"-gcc", CMD_GCC_MODE}
    };

    for(int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++){
        if(strcmp(i_cmd_str, commands[i].cmd_str) == 0) return commands[i].id;
    }

    //Return CMD_ERROR if there is no match

    return CMD_ERROR;

}

typedef enum {
    ERR_INVALID_CMD,
    ERR_O_NOTFOUND,
    ERR_I_NOTFOUND,
    ERR_INVALID_O_PATH,
    ERR_INVALID_I_PATH,
    ERR_FILE_HANDLER_ERROR
} error_t;

static void print_err(error_t id, char *illegal_str){
    switch(id){
        case ERR_INVALID_CMD: printf("CMD_ERROR: command '%s' does not exist.\n", illegal_str); break;
        case ERR_O_NOTFOUND: printf("CMD_ERROR: Output file was not specified.\n"); break;
        case ERR_I_NOTFOUND: printf("CMD_ERROR: Input file was not specified.\n"); break;
        case ERR_INVALID_O_PATH: printf("CMD_ERROR: Output path '%s' does not exist or cannot be accessed.\n", illegal_str); break;
        case ERR_INVALID_I_PATH: printf("CMD_ERROR: Input path '%s' does not exist or cannot be accessed.\n", illegal_str); break;
        case ERR_FILE_HANDLER_ERROR: printf("INTERNAL: File handler error.\n"); break;
    }
}

static void print_help(){
    printf("--help         : print this message.\n");
    printf("-i <FILE-PATH> : specify input file path.\n");
    printf("-o <FILE-PATH> : specify output file path.\n");
    printf("-gcc           : compile output C file and then delete it.\n");
}

//Return 0 if success. Return 1 if error
int process_cmd(int i_argc, char **i_argv, char **o_input_file_data, FILE **o_output_c_file, char **output_file_path, char **output_c_file_path, bool* use_gcc){

    int input_path_idx = 0;
    int output_path_idx = 0;

    *use_gcc = false;

    if(i_argc == 1) {
        print_help();
        return 0;
    } else
    for(int i = 1; i < i_argc;){
        switch(get_arg_id(i_argv[i])){
            case CMD_HELP: print_help(); return 0;
            case CMD_ERROR: print_err(ERR_INVALID_CMD, i_argv[i]); return 1;
            case CMD_OUTPUT_SEL: {
                if(i == (i_argc-1)) {
                    print_err(ERR_O_NOTFOUND, NULL);
                    return 1;
                } else {
                    output_path_idx = i + 1;
                    i += 2;
                    break;
                }
            }
            case CMD_INPUT_SEL: {
                if(i == (i_argc-1)) {
                    print_err(ERR_I_NOTFOUND, NULL);
                    return 1;
                } else {
                    input_path_idx = i + 1;
                    i += 2;
                    break;
                }
            }
            case CMD_GCC_MODE: *use_gcc = true; i++; break;
            default: /*nothing*/;
        }
    }

    if(output_path_idx == 0) {
        print_err(ERR_O_NOTFOUND, NULL);
        return 1;
    }

    *output_c_file_path = calloc(strlen(i_argv[output_path_idx]) + sizeof(".c"), sizeof(char));
    if(*use_gcc) sprintf(*output_c_file_path, "%s.c", i_argv[output_path_idx]);
    else *output_c_file_path = i_argv[output_path_idx];

    *output_file_path = i_argv[output_path_idx];

    if(input_path_idx == 0) {
        print_err(ERR_I_NOTFOUND, NULL);
        return 1;
    }

    //Open output file
    *o_output_c_file = fopen(*output_c_file_path, "w");
    if(*o_output_c_file == NULL){
        print_err(ERR_INVALID_O_PATH, i_argv[output_path_idx]);
        return 1;
    }

    //Open input file
    FILE *input_file = fopen(i_argv[input_path_idx], "r");
    if(input_file == NULL){
        print_err(ERR_INVALID_I_PATH, i_argv[input_path_idx]);
        return 1;
    }

    //Get size of file and allocate an array of bytes that includes a null terminator
    int tmp = ftell(input_file);
    fseek(input_file, 0, SEEK_END);
    int input_file_size = ftell(input_file) - tmp;
    *o_input_file_data = calloc(input_file_size + 1, sizeof(char));
    fseek(input_file, 0, SEEK_SET);

    //Transfer input file data to the array of bytes
    if(fread(*o_input_file_data, sizeof(char), input_file_size, input_file) != input_file_size){
        print_err(ERR_FILE_HANDLER_ERROR, NULL);
        return 1;
    }

    fclose(input_file);

    return 0;
}