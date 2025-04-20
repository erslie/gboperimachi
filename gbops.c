#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "gbops.h"

char mnemonic[];
int bytes;
int cycles;
hl_type hl;

gb_instruction_t gbins;

void generate_gb_ld_operands(char arg_d_name, char arg_s_name) {
  
  //destination
  //HACK: サイクルとバイトで見るのやめる
  if (strlen(arg_d_name) == 1) {
    if (strcmp("D", arg_d_name) == 0) {
      strcpy(gbins.operand_array[0], sprintf("Direct8::%s", arg_d_name));
    } else {
      strcpy(gbins.operand_array[0], sprintf("Reg8::%s", arg_d_name));
    }
  } else if (cycles == 4 && bytes == 3) {//LD [n16], A
    strcpy(gbins.operand_array[0], sprintf("Indirect::%s", arg_d_name));
  } else if (cycles == 3 && bytes == 2) {//LD [HL], n8
    strcpy(gbins.operand_array[0], sprintf("Indirect::%s", arg_d_name));
  } else if (cycles == 12 && bytes == 2) {
    stpcpy(gbins.operand_array[0], sprintf("Direct8::%s", arg_d_name));
  } else {
    printf("failed generated ld's destination code\n arg_d_name:%s\nbytes:%d\ncycles:%d\n",arg_d_name,bytes,cycles);
  }

  //source
  if (strcmp("n8", arg_s_name) == 0) {
      strcpy(gbins.operand_array[1], "Imm8");
    } else if (strlen(arg_s_name) == 1) {
      if (strcmp("D", arg_d_name) == 0) {//0xFA 
        strcpy(gbins.operand_array[1], sprintf("Direct8::%s", arg_s_name));
      } else {
        strcpy(gbins.operand_array[1], sprintf("Reg8::%s", arg_s_name));
      }
    } else if (strlen(arg_s_name) >= 2) {
      //hl or hli or hld
      if (strcmp("n16", arg_s_name) == 0) {
        strcpy(gbins.operand_array[1], "Imm16");
      } else if (strcmp("HL", arg_s_name) == 0) {
        switch (hl)
        {
        case hl_type_normanl:
          strcpy(gbins.operand_array[1], "Indirect::HL");
          break;
        case hl_type_increment:
          strcpy(gbins.operand_array[1], "Indirect::HLI");
          break;
        case hl_type_decrement:
          strcpy(gbins.operand_array[1], "Indirect::HLD");
          break;
        default:
          break;
        }
      }
      //LDH
      if (strcmp("LDH", mnemonic) == 0) {
        if (strcmp("D", arg_s_name) == 0) {
          strcpy(gbins.operand_array[1], "Direct8::DFF");
        } else if (strcmp("C", arg_s_name) == 0) {
          strcpy(gbins.operand_array[1], "Indirect::CFF"); 
        }
      } 
    }
    else {
      printf("failed generated ld's source code\n arg_s_name:%s\nbytes:%d\ncycles:%d\nhl_type:%d\n",arg_s_name,bytes,cycles,hl);
    }


}

gb_instruction_t generate_gb_instrucion(char arg_mnemonic, 
                                        int arg_bytes,
                                        int arg_opcode,
                                        int arg_cycles,
                                        char *arg_fisrt_operand_name,
                                        char *arg_second_operand_name,
                                        hl_type arg_hl) {
  
  int f_name_len = strlen(arg_fisrt_operand_name);
  int s_name_len = strlen(arg_second_operand_name);
  strcpy(mnemonic, arg_mnemonic);
  bytes = arg_bytes;
  cycles = arg_cycles;
  hl = arg_hl;
  char ret_gb_mnemonic;
  int ret_gb_opcode;


  if (strcmp("n8", arg_second_operand_name) == 0) {
    strcpy(gbins.operand_array[1], arg_second_operand_name);
  }
  
  //ld ld16
  if (strcmp("LD", arg_mnemonic) == 0 || strcmp("LDH", arg_mnemonic) == 0) {
    generate_gb_ld_operands(*arg_fisrt_operand_name, *arg_second_operand_name);
    //srcかdstどちらが8bitレジスタなら8bit転送、ただし0x36はHLに対してImm8をロードしているためsrcがn8で判断
    if (f_name_len == 1 
      ||s_name_len == 1
      ||strcmp("n8", arg_second_operand_name) == 0) {
        ret_gb_mnemonic = ld;
    } else {
      ret_gb_mnemonic = ld16;
    }
  }

  else if (strcmp("ADC",arg_mnemonic) == 0) {
  }


}

