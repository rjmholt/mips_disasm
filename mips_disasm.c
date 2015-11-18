#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mips_read.h"

#define NUM_INSTR 1000
#define START_ADDR 0x400000
#define BYTE 4

void print_instr(unsigned, uint32_t);

void print_r_instr(Instr);

void print_j_instr(Instr);

void print_i_instr(Instr);

void print_jr_instr(Instr);

void print_sl_instr(Instr);

void print_branch_instr(Instr, bool);

void print_shift_instr(Instr);

typedef struct instrs_t
{
    uint32_t ins[NUM_INSTR];
    unsigned short len;
} InstructionArray;

int main(int argc, char **argv)
{
    // Allocate the array to fill
    InstructionArray instr_arr;

    int i = 0;
    unsigned int instruction;
    while ((scanf("%x ", &instruction)) == 1) {
#ifdef DEBUG
        printf("Instruction: 0x%x\n", instruction);
#endif
        if (instruction == 0xffffffff) {
#ifdef DEBUG
            printf("Breaking\n");
#endif
            break;
        }
#ifdef DEBUG
        printf("Looping\n");
#endif

        instr_arr.ins[i] = instruction;
        i++;
    }
    instr_arr.len = i;

    unsigned curr_addr = START_ADDR;
    for (i = 0; i < instr_arr.len; i++) {
        curr_addr += BYTE;
        print_instr(curr_addr, instr_arr.ins[i]);
    }

    return 0;
}

void print_instr(unsigned addr, uint32_t word)
{
    printf("[0x%08x] 0x%08x ", addr, word);

    Instr in = mips_machine_read(word);

    switch (in.type) {
        /* immediate-type instructions */
        case ADDI:
            printf("addi");
            print_i_instr(in);
            break;
        case ADDIU:
            printf("addiu");
            print_i_instr(in);
            break;
        case ANDI:
            printf("andi");
            print_i_instr(in);
            break;
        case BEQ:
            printf("beq");
            print_branch_instr(in, true);
            break;
        case BGTZ:
            printf("bgtz");
            print_branch_instr(in, false);
            break;
        case BGEZ:
            printf("bgez");
            print_branch_instr(in, false);
            break;
        case BLEZ:
            printf("blez");
            print_branch_instr(in, false);
            break;
        case BLTZ:
            printf("bltz");
            print_branch_instr(in, false);
            break;
        case BNE:
            printf("bne");
            print_branch_instr(in, true);
            break;
        case LB:
            printf("lb");
            print_sl_instr(in);
            break;
        case LUI:
            printf("lui");
            print_i_instr(in);
            break;
        case LW:
            printf("lw");
            print_sl_instr(in);
            break;
        case ORI:
            printf("ori");
            print_i_instr(in);
            break;
        case SB:
            printf("sb");
            print_sl_instr(in);
            break;
        case SW:
            printf("sw");
            print_sl_instr(in);
            break;
        /* jump-type instructions */
        case J:
            printf("j");
            print_j_instr(in);
            break;
        case JAL:
            printf("jal");
            print_j_instr(in);
            break;
        /* register-type instructions */
        case ADD:
            printf("add");
            print_r_instr(in);
            break;
        case ADDU:
            printf("addu");
            print_r_instr(in);
            break;
        case AND:
            printf("and");
            print_r_instr(in);
            break;
        case JALR:
            printf("jalr");
            print_jr_instr(in);
            break;
        case JR:
            printf("jr");
            print_jr_instr(in);
            break;
        case MUL:
            printf("mul");
            print_r_instr(in);
            break;
        case NOP:
            printf("nop\n");
            break;
        case SLT:
            printf("slt");
            print_r_instr(in);
            break;
        case SRA:
            printf("sra");
            print_shift_instr(in);
            break;
        case SUB:
            printf("sub");
            print_r_instr(in);
            break;
        case SYSCALL:
            printf("syscall\n");
            break;
        case SLL:
            printf("sll");
            print_shift_instr(in);
            break;
        default:
            fprintf(stderr, "ERROR: Unrecognised instruction type\n");
    }
}

void print_r_instr(Instr instr)
{
    R_Instr in = instr.in.r;
    printf(" $%d, $%d, $%d\n", in.rd, in.rs, in.rt);
}

void print_i_instr(Instr instr)
{
    I_Instr in = instr.in.i;
    printf(" $%d, $%d, %d\n", in.rt, in.rs, in.imm);
}

void print_j_instr(Instr instr)
{
    printf(" 0x%08x\n", instr.in.j.addr << 2);
}

void print_jr_instr(Instr instr)
{
    printf(" $%d\n", instr.in.r.rs);
}

void print_sl_instr(Instr instr)
{
    I_Instr in = instr.in.i;
    printf(" $%d, %d($%d)\n", in.rt, BYTE*in.imm, in.rs);
}

void print_branch_instr(Instr instr, bool use_rt)
{
    I_Instr in = instr.in.i;
    if (use_rt) {
        printf(" $%d, $%d, %d\n", in.rs, in.rt, BYTE*in.imm);
    }
    else {
        printf(" $%d %d\n", in.rs, BYTE*in.imm);
    }
}

void print_shift_instr(Instr instr)
{
    R_Instr in = instr.in.r;
    printf(" $%d, $%d, %d\n", in.rd, in.rt, in.shamt);
}
