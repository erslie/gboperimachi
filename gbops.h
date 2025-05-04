#include <stdint.h>

//HLD or HLI
typedef enum {
  hl_type_normanl,
  hl_type_increment,
  hl_type_decrement,
} hl_type;

//mnemonic
typedef enum {
  //Load instructions
  ld,
  ld16,
  //8-bit arithmetic instructions
  adc,
  cp,
  add,
  dec,
  inc,
  sbc,
  sub,
  //16-bit arithmetic instructions
  add16,
  dec16,
  inc16,
  //Bitwise logic instructions
  and,
  cpl,
  or,
  xor,
  //Bit flag instructions
  bit,
  set,
  //Bit shift instructions
  rl,
  rlc,
  rr,
  rra,
  rrc,
  rrca,
  sla,
  sra,
  srl,
  swap,
  //Jamps and subroutine instructions
  call,
  call_c,
  jp,
  jp_c,
  jp_hl,
  ret,
  ret_c,
  reti,
  rst,
  //Carry flag instructinos
  ccf,
  scf,
  //Stack manipulation instructions
  add_sp_e,
  add_hl_reg16,
  ld_sp_hl,
  ld_hl_sp_e,
  pop,
  push,
  //Miscellaneous instructions
  daa,
  nop,
  stop,
} mnemonic;

const char* const mnemonic_array[47][12] = {
  "ld",
  "ld16",
  "adc",
  "cp",
  "add",
  "dec",
  "inc",
  "sbc",
  "sub",
  "add16",
  "dec16",
  "inc16",
  "and",
  "cpl",
  "or",
  "xor",
  "bit",
  "set",
  "rl",
  "rlc",
  "rr",
  "rra",
  "rrc",
  "rrca",
  "sla",
  "sra",
  "srl",
  "swap",
  "call",
  "call_c",
  "jp",
  "jp_c",
  "jp_hl",
  "ret",
  "ret_c",
  "reti",
  "rst",
  "ccf"
  "scf",
  "add_sp_e",
  "add_hl_reg16",
  "ld_sp_hl",
  "ld_hl_sp_e",
  "pop",
  "push",
  "daa",
  "nop",
  "stop"
};

typedef struct {
  char mnemonic[8];
  char dst[16];
  char src[16];
} gb_instruction_t;

gb_instruction_t generate_gb_ld_operands(uint8_t opcode);
