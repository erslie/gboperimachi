#include <stdio.h>
#include <string.h>
#include "gbops.h"



gb_instruction_t generate_gb_instrucion(char arg_mnemonic, 
                                        int arg_bytes,
                                        int arg_opcode,
                                        char arg_d_name,
                                        char arg_s_name) {
  
  int d_name_len = sizeof(arg_d_name) / sizeof(arg_d_name[0]);
  int s_name_len = sizeof(arg_s_name) / sizeof(arg_s_name[0]);

  //return gb instrucions elements
  mnemonic gb_mnemonic;
  int gb_opcode;
  char gb_dst;
  char gb_src;
  
  gb_instruction_t gbins;
  
  
  //Load instruction
  if (strcmp("LD", arg_mnemonic) == 0) {  
    if ((strcmp("n8",arg_s_name) == 0
        || strcmp("n8", arg_d_name) == 0
        || d_name_len == 1
        || s_name_len == 1) {
      gb_mnemonic = ld;
    } else {
    gb_mnemonic = ld16;
    gb_src = arg_s_name;
    }
  }

  else if (strcmp("ADC",arg_mnemonic) == 0) {
  } 

