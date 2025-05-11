
#include "gbops.h"

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

    char initial_dst[16];
    char initial_src[16];
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
          strcpy(initial_dst, REG8); break;
        case 0x06: 
          strcpy(initial_dst, INDIRECT); break;//ddd = 110 ならIndirect::HL
        default: return result;
      }
      strcpy(result.mnemonic, LD);
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
          strcpy(initial_dst, REG8); break;
        case 0x06: 
          strcpy(initial_dst, INDIRECT); break;//ddd = 110 ならIndirect::HL
        default: return result;
      }

      //source
      switch (opcode & 0x07){
      case 0x00: src = "B"; break;
      case 0x01: src = "C"; break;
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
          strcpy(initial_src, REG8); break;
        case 0x06: 
          strcpy(initial_src, INDIRECT); break;//ddd = 110 ならIndirect::HL
        default: return result;
      }
      strcpy(result.mnemonic, LD);
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
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, INDIRECT);
      strcat(initial_dst, "BC");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, REG8);
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }     
    case 0x12:{
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, INDIRECT);
      strcat(initial_dst, "DE");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, REG8);
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }
    case 0x22:{
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, INDIRECT);
      strcat(initial_dst, "HLI");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, REG8);
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }
    case 0x32:{
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, INDIRECT);
      strcat(initial_dst, "HLD");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, REG8);
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }
    case 0xE2:{
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, INDIRECT);
      strcat(initial_dst, "CFF");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, REG8);
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }
    case 0xE0: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, DIRECT8 );
      strcat(initial_dst, "DFF");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, REG8);
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }

    //8bit転送 (BC, DE, HL+, HL- , $FF00+C, a8 -> A)
    case 0x0A: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, REG8);
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, INDIRECT);
      strcat(initial_src, "BC");
      strcpy(result.src ,initial_src);
      return result;
    }
    case 0x1A: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, REG8);
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, INDIRECT);
      strcat(initial_src, "DE");
      strcpy(result.src ,initial_src);
      return result;
    }
    case 0x2A: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, REG8);
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, INDIRECT);
      strcat(initial_src, "HLI");
      strcpy(result.src ,initial_src);
      return result;
    }
    case 0x3A: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, REG8);
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, INDIRECT);
      strcat(initial_src, "HLD");
      strcpy(result.src ,initial_src);
      return result;
    }
    case 0xF2: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, REG8);
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, INDIRECT);
      strcat(initial_src, "CFF");
      strcpy(result.src ,initial_src);
      return result;
    }
    case 0xF0: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, REG8);
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, DIRECT8);
      strcat(initial_src, "DFF");
      strcpy(result.src ,initial_src);
      return result;
    }

    //16bit転送(Imm16 -> Reg) 
    case 0x01: {
      strcpy(result.mnemonic, LD16);
      strcpy(initial_dst, REG16);
      strcat(initial_dst, "BC");
      strcpy(result.dst, initial_dst);
      strcpy(result.src, IMM16);
      return result;
    }
    case 0x11: {
      strcpy(result.mnemonic, LD16);
      strcpy(initial_dst, REG16);
      strcat(initial_dst, "DE");
      strcpy(result.dst, initial_dst);
      strcpy(result.src, IMM16);
      return result;
    }
    case 0x21: {
      strcpy(result.mnemonic, LD16);
      strcpy(initial_dst, REG16);
      strcat(initial_dst, "HL");
      strcpy(result.dst, initial_dst);
      strcpy(result.src, IMM16);
      return result;
    }
    case 0x31: {
      strcpy(result.mnemonic, LD16);
      strcpy(initial_dst, REG16);
      strcat(initial_dst, "SP");
      strcpy(result.dst, initial_dst);
      strcpy(result.src, IMM16);
      return result;
    }

    case 0x08: {
      strcpy(result.mnemonic, LD16);
      strcpy(result.dst, DIRECT16);
      strcpy(initial_src, REG16);
      strcat(initial_src, "SP");
      strcpy(result.src, initial_src);
      return result;
    }

    case 0x0E: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, REG8);
      strcat(initial_dst, "C");
      strcpy(result.dst, initial_dst);
      strcpy(result.src, IMM8);
      return result;
    }
    case 0x1E: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, REG8);
      strcat(initial_dst, "E");
      strcpy(result.dst, initial_dst);
      strcpy(result.src, IMM8);
      return result;
    }
    case 0x2E: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, REG8);
      strcat(initial_dst, "L");
      strcpy(result.dst, initial_dst);
      strcpy(result.src, IMM8);
      return result;
    }
    case 0x3E: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, REG8);
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      strcpy(result.src, IMM8);
      return result;
    }

    case 0xEA: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, DIRECT8);
      strcat(initial_dst, "D");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, REG8);
      strcat(initial_src, "A");
      strcpy(result.src, initial_src);
      return result;
    }

    case 0xFA: {
      strcpy(result.mnemonic, LD);
      strcpy(initial_dst, REG8);
      strcat(initial_dst, "A");
      strcpy(result.dst, initial_dst);
      strcpy(initial_src, DIRECT8);
      strcat(initial_src, "D");
      strcpy(result.src,initial_src);
      return result;
    }

    case 0xF8: {
      strcpy(result.mnemonic, LD_HL_SP_E);
      return result;
    }

    case 0xF9: {
      strcpy(result.mnemonic, LD_SP_HL);
      return result;
    }

    return result;

  }

  return result;
}

gb_instruction_t generate_gb_instrucion(char arg_mnemonic, 
                                        char *dst_name,
                                        char *src_name,
                                        uint8_t opcode,
                                        hl_type arg_hl) {
  
  int dst_name_len = strlen(dst_name);
  int src_name_len = strlen(src_name);
  hl = arg_hl;
  char ret_gb_mnemonic;
  int ret_gb_opcode;
  gb_instruction_t gb_inst;

  //ld ld16
  if (strcmp("LD", arg_mnemonic) == 0 || strcmp("LDH", arg_mnemonic) == 0) {
    gb_inst = generate_gb_ld_operands(opcode);
    //srcかdstどちらが8bitレジスタなら8bit転送、ただし0x36はHLに対してImm8をロードしているためsrcがn8で判断
    if (dst_name_len == 1 
      ||src_name_len == 1
      ||strcmp("n8", src_name) == 0) {
        strcpy(gb_inst.mnemonic, "ld");
    } else {
      strcpy(gb_inst.mnemonic, "ld16");
    }

    return gb_inst;

  }


}

