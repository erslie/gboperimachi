
#include "gbops.h"

int bytes;
int cycles;
hl_type hl;

gb_instruction_t generate_gb_nop_operands(uint8_t opcode) {

  gb_instruction_t result = {0};

  if (opcode != 0x00) return result;
  strcpy(result.mnemonic, NOP);
  return result;
}

gb_instruction_t generate_gb_stop_operands(uint8_t opcode) {

  gb_instruction_t result = {0};

  if (opcode != 0x10) return result;
  strcpy(result.mnemonic, STOP);
  return result;
}

gb_instruction_t generate_gb_halt_operands(uint8_t opcode) {

  gb_instruction_t result = {0};

  if (opcode != 0x76) return result;
  strcpy(result.mnemonic, HALT);
  return result;
}

gb_instruction_t generate_gb_jr_operands(uint8_t opcode) {

  gb_instruction_t result = {0};
  const char* src;

  switch (opcode) {

    case 0x18: strcpy(result.mnemonic, JP); return result;
    case 0x20: src = "NZ"; break;
    case 0x30: src = "NC"; break;
    case 0x28: src = "Z"; break;
    case 0x38: src = "C"; break;
    default: return result;
  }
    
  strcpy(result.mnemonic, JP_C);
  strcpy(result.src,COND);
  strcat(result.src,src);
  return result;
}

gb_instruction_t generate_gb_inc_operands(uint8_t opcode) {

  gb_instruction_t result = {0};

  char initial_dst[16];
  const char* dst;

  switch (opcode) {

    //16bit転送(Reg16 increment)
    case 0x03:
    case 0x13:
    case 0x23:
    case 0x33: {
      switch ((opcode >> 2) & 0x03) {
      case 0x00: dst = "BC"; break;
      case 0x01: dst = "DE"; break;
      case 0x02: dst = "HL"; break;
      case 0x03: dst = "SP"; break;
      default: return result;
      }
      strcpy(result.mnemonic, INC16);
      strcpy(initial_dst, REG16);
      strcat(initial_dst, dst);
      strcpy(result.dst, initial_dst);
      return result;
    }
  }

  //8bit
  switch (opcode) {

    case 0x04:
    case 0x14:
    case 0x24:
    case 0x0C:
    case 0x1C:
    case 0x2C:
    case 0x3C: strcpy(initial_dst, REG8); break;
    case 0x34: strcpy(initial_dst, INDIRECT); break;
    default: return result;
  }
  switch (opcode) {

    case 0x04: strcat(initial_dst, "B"); break;
    case 0x14: strcat(initial_dst, "D"); break;
    case 0x24: strcat(initial_dst, "H"); break;
    case 0x0C: strcat(initial_dst, "C"); break;
    case 0x1C: strcat(initial_dst, "E"); break;
    case 0x2C: strcat(initial_dst, "L"); break;
    case 0x3C: strcat(initial_dst, "A"); break;
    case 0x34: strcat(initial_dst, "HL"); break;
    default: return result;
  }

  strcpy(result.mnemonic, INC);
  strcpy(result.dst, initial_dst);
  return result;
}

gb_instruction_t generate_gb_dec_operands(uint8_t opcode) {

  gb_instruction_t result = {0};

  char initial_dst[16];
  const char* dst;

  //16bit
  switch (opcode) {
    case 0x08:
    case 0x18:
    case 0x28:
    case 0x38: {
      switch ((opcode & 0xF0) >> 4) {
        case 0x00: dst = "BC"; break;
        case 0x01: dst = "DE"; break;
        case 0x02: dst = "HL"; break;
        case 0x03: dst = "SP"; break;
        default: return result;
      }
      strcpy(result.mnemonic, DEC16);
      strcpy(initial_dst, REG16);
      strcat(initial_dst, dst);
      strcpy(result.dst, initial_dst);
      return result;
    }
  }

  //8bit
  switch (opcode) {

    case 0x05:
    case 0x15:
    case 0x25:
    case 0x0D:
    case 0x1D:
    case 0x2D:
    case 0x3D: strcpy(initial_dst, REG8); break;
    case 0x35: strcpy(initial_dst, INDIRECT); break;
    default: return result;
  }
  switch (opcode) {

    case 0x05: strcat(initial_dst, "B"); break;
    case 0x15: strcat(initial_dst, "D"); break;
    case 0x25: strcat(initial_dst, "H"); break; 
    case 0x0D: strcat(initial_dst, "C"); break;
    case 0x1D: strcat(initial_dst, "E"); break;
    case 0x2D: strcat(initial_dst, "L"); break;
    case 0x3D: strcpy(initial_dst, "A"); break;
    case 0x35: strcpy(initial_dst, "HL"); break;
    default: return result;
  }
  strcpy(result.mnemonic, DEC);
  strcpy(result.dst, initial_dst);
  return result;

}

gb_instruction_t generate_gb_ld_operands(uint8_t opcode) {

  gb_instruction_t result = {0};

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

gb_instruction_t generate_gb_cp_operands(uint8_t opcode) {
  
  gb_instruction_t result = {0};

  switch (opcode) {
    char initial_src[16];
    const char* src;

    case 0xB8 ... 0xBF: {
      strcpy(result.mnemonic, CP);
      strcpy(initial_src, REG8);
      switch (opcode & 0x07) {
        case 0x00: src = "B"; break;
        case 0x01: src = "C"; break;
        case 0x02: src = "D"; break;
        case 0x03: src = "E"; break;
        case 0x04: src = "H"; break;
        case 0x05: src = "L"; break;
        case 0x06: src = "HL"; strcpy(initial_src, INDIRECT); break;
        case 0x07: src = "A"; break;
        default: return result;
      }
      strcat(initial_src, src);
      strncpy(result.src, initial_src, sizeof(result.src) - 1);
      result.src[sizeof(result.src) - 1] = '\0';
      return result;
    }

    case 0xFE: {
      strcpy(result.mnemonic, CP);
      strcpy(result.src, IMM8);
      return result;
    }

    return result;
  }
  return result;
}

gb_instruction_t generate_gb_instrucion(uint8_t opcode, const char *mnemonic) {

  if (strcmp(mnemonic, "NOP") == 0) {
    return generate_gb_nop_operands(opcode);
  }
  else if (strcmp(mnemonic, "STOP") == 0) {
    return generate_gb_stop_operands(opcode);
  }
  else if (strcmp(mnemonic, "HALT") == 0) {
    return generate_gb_halt_operands(opcode);
  }
  else if (strcmp(mnemonic, "LD") == 0) {
    return generate_gb_ld_operands(opcode);
  } 
  else if (strcmp(mnemonic, "CP") == 0) {
    return generate_gb_cp_operands(opcode);
  }
  else if (strcmp(mnemonic, "JR") == 0) {
    return generate_gb_jr_operands(opcode);
  }
  else if (strcmp(mnemonic, "INC") == 0) {
    return generate_gb_inc_operands(opcode);
  }
  else if (strcmp(mnemonic, "DEC") == 0) {
    return generate_gb_dec_operands(opcode);
  }
  else {
    gb_instruction_t result = {0};
    return result;
  }

}

