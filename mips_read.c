#include <stdlib.h>
#include <stdio.h>
#include "mips_read.h"

/* Sizes and bit regions of MIPS machine words */
#define OPCODE 6
#define RS 5
#define RT 5
#define RD 5
#define SHAMT 5
#define FUNCT 6
#define IMM 16
#define ADDR 26
#define SIZE 32

/* Opcodes encoding multiple MIPS assembly functions */
#define OP_NOP      0x0
#define OP_B        0x1

/* Other unique opcodes */
#define OP_ADDI     0x8
#define OP_ADDIU    0x9
#define OP_ANDI     0xc
#define OP_BEQ      0x4
#define OP_BGTZ     0x7
#define OP_BLEZ     0x6
#define OP_BNE      0x5
#define OP_J        0x2
#define OP_JAL      0x3
#define OP_LB       0x20
#define OP_LUI      0xf
#define OP_LW       0x23
#define OP_MUL      0x1c
#define OP_ORI      0xd
#define OP_SB       0x28
#define OP_SW       0x2b

/* Function codes */
#define F_ADD       0x20
#define F_ADDU      0x21
#define F_AND       0x24
#define F_JALR      0x9
#define F_JR        0x8
#define F_MUL       0x2
#define F_SLL       0x0
#define F_SLT       0x2a
#define F_SRA       0x3
#define F_SUB       0x22
#define F_SYSCALL   0xc

unsigned get_bits(uint32_t, unsigned, unsigned);

Instr immed_instr(Instr_Type, uint32_t);

Instr jump_instr(Instr_Type, uint32_t);

Instr reg_instr(Instr_Type, uint32_t);

Instr_Type get_r_instr_type(uint32_t word);

Instr mips_machine_read(uint32_t word)
{
    unsigned opcode;
    opcode = get_bits(word, OPCODE, SIZE - OPCODE);

    switch (opcode) {
        /* Deal with immediate-type instructions */
        case OP_ADDI:
            return immed_instr(ADDI, word);
        case OP_ADDIU:
            return immed_instr(ADDIU, word);
        case OP_ANDI:
            return immed_instr(ANDI, word);
        case OP_BEQ:
            return immed_instr(BEQ, word);
        case OP_BGTZ:
            return immed_instr(BGTZ, word);
        case OP_BLEZ:
            return immed_instr(BLEZ, word);
        case OP_BNE:
            return immed_instr(BNE, word);
        case OP_LB:
            return immed_instr(LB, word);
        case OP_LUI:
            return immed_instr(LUI, word);
        case OP_LW:
            return immed_instr(LW, word);
        case OP_ORI:
            return immed_instr(ORI, word);
        case OP_SB:
            return immed_instr(SB, word);
        case OP_SW:
            return immed_instr(SW, word);
        case OP_B:
            if (get_bits(word, RT, IMM)) {
                return immed_instr(BGEZ, word);
            }
            return immed_instr(BLTZ, word);
        /* Deal with jump-type instructions */
        case OP_J:
            return jump_instr(J, word);
        case OP_JAL:
            return jump_instr(JAL, word);
        /* Deal with register-type instructions */
        case OP_MUL:
            return reg_instr(MUL, word);
        case OP_NOP:
            return reg_instr(get_r_instr_type(word), word);
        default:
            fprintf(stderr, "ERROR: Unrecognised instruction\n");
    }
}

unsigned get_bits(uint32_t word, unsigned number, unsigned offset)
{
    unsigned mask       = (1 << (number + offset)) - 1;
    unsigned offmask    = (1 << offset) - 1;
    return (word & mask & ~offmask) >> offset;
}

Instr immed_instr(Instr_Type type, uint32_t word)
{
    I_Instr body;
    body.rs = get_bits(word, RS, RT+IMM);
    body.rt = get_bits(word, RT, IMM);
    body.imm = get_bits(word, IMM, 0);

    Instr instruction;
    instruction.type = type;
    instruction.in.i = body;

    return instruction;
}

Instr jump_instr(Instr_Type type, uint32_t word)
{
    J_Instr body;
    body.addr = get_bits(word, ADDR, 0);

    Instr instruction;
    instruction.type = type;
    instruction.in.j = body;

    return instruction;
}

Instr reg_instr(Instr_Type type, uint32_t word)
{
    R_Instr body;
    body.rs = get_bits(word, RS, RT+RD+SHAMT+FUNCT);
    body.rt = get_bits(word, RT, RD+SHAMT+FUNCT);
    body.rd = get_bits(word, RD, SHAMT+FUNCT);
    body.shamt = get_bits(word, SHAMT, FUNCT);

    Instr instruction;
    instruction.type = type;
    instruction.in.r = body;

    return instruction;
}

Instr_Type get_r_instr_type(uint32_t word)
{
    uint8_t funct = get_bits(word, FUNCT, 0);

    switch (funct) {
        case F_ADD:
            return ADD;
        case F_ADDU:
            return ADDU;
        case F_AND:
            return AND;
        case F_JALR:
            return JALR;
        case F_JR:
            return JR;
        case F_MUL:
            fprintf(stderr, "ERROR: Invalid MUL detected\n");
            exit(1);
        case F_SLT:
            return SLT;
        case F_SRA:
            return SRA;
        case F_SUB:
            return SUB;
        case F_SYSCALL:
            return SYSCALL;
        case F_SLL:
            if (word) {
                return SLL;
            }
            return NOP;
        default:
            fprintf(stderr, "ERROR: Invalid register-type word\n");
    }
}
