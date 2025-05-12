#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Rust = 1,
#define LANG_VALUE 1

#if LANG_VALUE == 1
#define REG8 "Reg8::"
#define REG16 "Reg16::"
#define INDIRECT "Indirect::"
#define COND "Cond::"
#define DIRECT8 "Direct8::"
#define DIRECT16 "Direct16"
#define IMM8 "Imm8"
#define IMM16 "Imm16"
#endif

//HLD or HLI
typedef enum {
  hl_type_normanl,
  hl_type_increment,
  hl_type_decrement,
} hl_type;


//Load instructions
#define LD "ld"
#define LD16 "ld16"
//8-bit arithmetic instructions
#define ADC "adc"
#define CP "cp"
#define ADD "add"
#define DEC "dec"
#define INC "inc"
#define SBC "sbc"
#define SUB "sub"
//16-bit arithmetic instructions
#define ADD16 "add16"
#define DEC16 "dec16"
#define INC16 "inc16"
//Bitwise logic instructions
#define AND "and"
#define CPL "cpl"
#define OR "or"
#define XOR "xor"

//Bit shift instructions
#define RRA "rra"
#define RRCA "rrca"

#define CB_RES "res"
#define CB_RL "rl"
#define CB_RLC "rlc"
#define CB_RR "rr"
#define CB_RRC "rrc"
#define CB_SLA "sla"
#define CB_SRA "sra"
#define CB_SRL "srl"
#define CB_SWAP "swap"
//Bit flag instructions
#define CB_BIT "bit"
#define CB_SET "set"

//Jamps and subroutine instructions
#define CALL "call"
#define CALL_C "call_c"
#define JP "jp"
#define JP_C "jp_c"
#define JP_HL "jp_hl"
#define RET "ret"
#define RET_C "ret_c"
#define RETI "reti"
#define RST "rst"
//Carry flag instructinos
#define CCF "ccf"
#define SCF "scf"
//Stack manipulation instructions
#define ADD_SP_E "add_sp_e"
#define ADD_HL_REG16 "add_hl_reg16"
#define LD_SP_HL "ld_sp_hl"
#define LD_HL_SP_E "ld_hl_sp_e"
#define POP "pop"
#define PUSH "push"
//Miscellaneous instructions
#define DAA "daa"
#define NOP "nop"
#define STOP "stop"

typedef struct {
  char mnemonic[16];
  char dst[16];
  char src[16];
} gb_instruction_t;

gb_instruction_t generate_gb_instrucion(uint8_t opcode, char *mnemonic);
