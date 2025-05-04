#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "gbops.h"

// Rust = 1,
#define LANG_VALUE 1

#if LANG_VALUE == 1
#define REG8 "Reg8::"
#define REG16 "Reg16::"
#define INDIRECT "Indirect::"
#define COND "Cond::"
#define DIRECT8 "Direct8::"
#define IMM8 "Imm8"
#define IMM16 "Imm16"
#endif

int bytes;
int cycles;
hl_type hl;

gb_instruction_t gbins;

gb_instruction_t generate_gb_ld_operands(uint8_t opcode) {

  gb_instruction_t result;
  strcpy(result.mnemonic, "");
  strcpy(result.dst, "");
  strcpy(result.src, "");

  switch (opcode) {

    const char* initial_dst = "";
    const char* initial_src = "";
    const char* dst;
    const char* src;

    //8bit転送(imm -> Reg or mem)
    case 0x06:
    case 0x16:
    case 0x26:
    case 0x36:
    case 0x46:
    case 0x56:
    case 0x66:
    case 0x77: {
      switch ((opcode >> 3) & 0x07) {
        case 0x00: dst = "B"; break;
        case 0x01: dst = "C"; break;
        case 0x02: dst = "D"; break;
        case 0x03: dst = "E"; break;
        case 0x04: dst = "H"; break;
        case 0x05: dst = "L"; break;
        case 0x06: dst = "HL"; break;
        case 0x07: dst = "A"; break;
        default: return result;
      }

      switch ((opcode >> 3) & 0x07) {
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x07:
          initial_dst = REG8; break;
        case 0x06: 
          initial_dst = INDIRECT; break;//ddd = 110 ならIndirect::HL
        default: return result;
      }
      strcpy(result.mnemonic, "LD");
      strcat(initial_dst, dst);
      strncpy(result.dst, initial_dst, sizeof(result.dst) - 1);
      result.dst[sizeof(result.dst) - 1] = '\0';
      strncpy(result.src, IMM8, sizeof(result.src) - 1);
      result.src[sizeof(result.src) - 1] = '\0';
      return result;
    }

    //8bit転送(Reg -> Reg)
    case 0x40 ... 0x45:
    case 0x47 ... 0x55:
    case 0x57 ... 0x65:
    case 0x67 ... 0x76:
    case 0x78 ... 0x7F: {

      //0b_xx_ddd_sssからdst以外ビットマスク
      //destination
      switch ((opcode >> 3) & 0x07) {
        case 0x00: dst = "B"; break;
        case 0x01: dst = "C"; break;
        case 0x02: dst = "D"; break;
        case 0x03: dst = "E"; break;
        case 0x04: dst = "H"; break;
        case 0x05: dst = "L"; break;
        case 0x06: dst = "HL"; break;
        case 0x07: dst = "A"; break;
        default: return result;
      }

      switch ((opcode >> 3) & 0x07) {
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x07:
          initial_dst = REG8; break;
        case 0x06: 
          initial_dst = INDIRECT; break;//ddd = 110 ならIndirect::HL
        default: return result;
      }

      //source
      switch (opcode & 0x07){
      case 0x00: src = "B"; break;
      case 0x01: src = 'C'; break;
      case 0x02: src = "D"; break;
      case 0x03: src = "E"; break;
      case 0x04: src = "H"; break;
      case 0x05: src = "L"; break;
      case 0x06: src = "HL"; break;
      case 0x07: src = "A"; break;
      default: return result;
      }


      switch (opcode & 0x07) {
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x07:
          initial_src = REG8; break;
        case 0x06: 
          initial_src = INDIRECT; break;//ddd = 110 ならIndirect::HL
        default: return result;
      }
      strcpy(result.mnemonic, "LD");
      strcat(initial_dst, dst);
      strncpy(result.dst, initial_dst, sizeof(result.dst) - 1);
      result.dst[sizeof(result.dst) - 1] = '\0';
      strcat(initial_src, src);
      strncpy(result.src, initial_src, sizeof(result.src) - 1);
      result.src[sizeof(result.src) - 1] = '\0';
      return result;
    }

    //8bit転送(A -> BC, DE, HL+, HL- , $FF00+C, a8)
    case 0x02: {
      initial_dst = INDIRECT;
      strcat(initial_dst, "BC");
      strcpy(result.dst, initial_dst);
      initial_src = REG8;
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }     
    case 0x12:{
      initial_dst = INDIRECT;
      strcat(initial_dst, "DE");
      strcpy(result.dst, initial_dst);
      initial_src = REG8;
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }
    case 0x22:{
      initial_dst = INDIRECT;
      strcat(initial_dst, "HLI");
      strcpy(result.dst, initial_dst);
      initial_src = REG8;
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }
    case 0x32:{
      initial_dst = INDIRECT;
      strcat(initial_dst, "HLD");
      strcpy(result.dst, initial_dst);
      initial_src = REG8;
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }
    case 0xE2:{
      initial_dst = INDIRECT;
      strcat(initial_dst, "CFF");
      strcpy(result.dst, initial_dst);
      initial_src = REG8;
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }
    case 0xE0: {
      initial_dst = DIRECT8;
      strcat(initial_dst, "DFF");
      strcpy(result.dst, initial_dst);
      initial_src = REG8;
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }

    //8bit転送 (BC, DE, HL+, HL- , $FF00+C, a8 -> A)
    case 0x0A: {
      initial_dst = REG8;
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      initial_src = INDIRECT;
      strcat(initial_src, "BC");
      strcpy(result.src ,initial_src);
      return result;
    }
    case 0x1A: {
      initial_dst = REG8;
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      initial_src = INDIRECT;
      strcat(initial_src, "DE");
      strcpy(result.src ,initial_src);
      return result;
    }
    case 0x2A: {
      initial_dst = REG8;
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      initial_src = INDIRECT;
      strcat(initial_src, "HLI");
      strcpy(result.src ,initial_src);
      return result;
    }
    case 0x3A: {
      initial_dst = REG8;
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      initial_src = INDIRECT;
      strcat(initial_src, "HLD");
      strcpy(result.src ,initial_src);
      return result;
    }
    case 0xF2: {
      initial_dst = REG8;
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      initial_src = INDIRECT;
      strcat(initial_src, "CFF");
      strcpy(result.src ,initial_src);
      return result;
    }
    case 0xF0: {
      initial_dst = REG8;
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      initial_src = DIRECT8;
      strcat(initial_src, "DFF");
      strcpy(result.src ,initial_src);
      return result;
    }
  }
  return result;
}

// gb_instruction_t generate_gb_instrucion(char arg_mnemonic, 
//                                         int arg_bytes,
//                                         int arg_opcode,
//                                         int arg_cycles,
//                                         char *arg_fisrt_operand_name,
//                                         char *arg_second_operand_name,
//                                         uint8_t opcode,
//                                         hl_type arg_hl) {
  
//   int f_name_len = strlen(arg_fisrt_operand_name);
//   int s_name_len = strlen(arg_second_operand_name);
//   strcpy(mnemonic, arg_mnemonic);
//   bytes = arg_bytes;
//   cycles = arg_cycles;
//   hl = arg_hl;
//   char ret_gb_mnemonic;
//   int ret_gb_opcode;
//   //ld ld16
//   if (strcmp("LD", arg_mnemonic) == 0 || strcmp("LDH", arg_mnemonic) == 0) {
//     generate_gb_ld_operands(*arg_fisrt_operand_name, *arg_second_operand_name);
//     //srcかdstどちらが8bitレジスタなら8bit転送、ただし0x36はHLに対してImm8をロードしているためsrcがn8で判断
//     if (f_name_len == 1 
//       ||s_name_len == 1
//       ||strcmp("n8", arg_second_operand_name) == 0) {
//         ret_gb_mnemonic = ld;
//     } else {
//       ret_gb_mnemonic = ld16;
//     }
//   }


// }

