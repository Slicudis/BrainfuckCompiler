#ifndef CMD_PROCESSOR_H
#define CMD_PROCESSOR_H

#include <stdio.h>

int process_cmd(int i_argc, char **i_argv, char **o_input_file_data, FILE **o_output_c_file, char **output_file_path, char **output_c_file_path, bool* use_gcc);

#endif