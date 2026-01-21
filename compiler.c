#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void compile(char *i_input_file_data, int i_memsize, FILE *io_output_file){

    //Generate initialization code

    fprintf(io_output_file,
        "#include <stdlib.h>\n"
        "#include <stdio.h>\n\n"

        "int main(){\n"
        "unsigned char *mem = calloc(%d, sizeof(char));\n"
        "unsigned char *mem_ptr = mem;\n\n",
        i_memsize
    );

    for(int i = 0; i_input_file_data[i] != 0; i++){
        if(i_input_file_data[i] == '>') fprintf(io_output_file, "mem_ptr++;\n");
        if(i_input_file_data[i] == '<') fprintf(io_output_file, "mem_ptr--;\n");
        if(i_input_file_data[i] == '+') fprintf(io_output_file, "(*mem_ptr)++;\n");
        if(i_input_file_data[i] == '-') fprintf(io_output_file, "(*mem_ptr)--;\n");
        if(i_input_file_data[i] == '.') fprintf(io_output_file, "putc(*mem_ptr, stdout);\n");
        if(i_input_file_data[i] == ',') fprintf(io_output_file, "*mem_ptr = getc(stdin);\n");
        if(i_input_file_data[i] == '[') fprintf(io_output_file, "while(*mem_ptr != 0){\n");
        if(i_input_file_data[i] == ']') fprintf(io_output_file, "}\n");
    }

    fprintf(io_output_file,
        "free(mem);\n"
        "return 0;\n"
        "}\n"
    );

}