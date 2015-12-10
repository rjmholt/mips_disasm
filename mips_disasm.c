#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mips.h"

#define START_ADDR 0x400000
#define BYTE 4

void print_instr(unsigned addr, Instr inst_word);

void print_r_instr(const char *name, Instr in);

void print_j_instr(const char *name, Instr in);

void print_i_instr(const char *name, Instr in);

void print_jr_instr(Instr in);

void print_jalr_instr(Instr in);

void print_sl_instr(const char *name, Instr in);

void print_branch_instr(const char *name, Instr in, bool use_rt);

void print_shift_instr(const char *name, Instr in);

void print_lui_instr(Instr in);

int main(int argc, char **argv)
{
    // Allocate the array to fill
    InstructionArray instr_arr;

    int i = 0;
    unsigned int instruction;
    while ((scanf("%x ", &instruction)) == 1) {
        if (instruction == 0xffffffff) {
            break;
        }

        instr_arr.ins[i] = mips_machine_read(instruction);
        i++;
    }
    instr_arr.len = i;

    unsigned curr_addr = START_ADDR;
    for (i = 0; i < instr_arr.len; i++) {
        print_instr(curr_addr, instr_arr.ins[i]);
        curr_addr += BYTE;
    }

    return 0;
}

void print_instr(unsigned addr, Instr in)
{
    printf("[0x%08x] 0x%08x ", addr, in.word);

    switch (in.type) {
        /* immediate-type instructions */
        case ADDI:
            print_i_instr("addi", in);
            break;
        case ADDIU:
            print_i_instr("addiu", in);
            break;
        case ANDI:
            print_i_instr("andi", in);
            break;
        case BEQ:
            print_branch_instr("beq", in, true);
            break;
        case BGTZ:
            print_branch_instr("bgtz", in, false);
            break;
        case BGEZ:
            print_branch_instr("bgez", in, false);
            break;
        case BLEZ:
            print_branch_instr("blez", in, false);
            break;
        case BLTZ:
            print_branch_instr("bltz", in, false);
            break;
        case BNE:
            print_branch_instr("bne", in, true);
            break;
        case LB:
            print_sl_instr("lb", in);
            break;
        case LUI:
            print_lui_instr(in);
            break;
        case LW:
            print_sl_instr("lw", in);
            break;
        case ORI:
            print_i_instr("ori", in);
            break;
        case SB:
            print_sl_instr("sb", in);
            break;
        case SW:
            print_sl_instr("sw", in);
            break;
        /* jump-type instructions */
        case J:
            print_j_instr("j", in);
            break;
        case JAL:
            print_j_instr("jal", in);
            break;
        /* register-type instructions */
        case ADD:
            print_r_instr("add", in);
            break;
        case ADDU:
            print_r_instr("addu", in);
            break;
        case AND:
            print_r_instr("and", in);
            break;
        case JALR:
            print_jalr_instr(in);
            break;
        case JR:
            print_jr_instr(in);
            break;
        case MUL:
            print_r_instr("mul", in);
            break;
        case NOP:
            printf("nop\n");
            break;
        case SLT:
            print_r_instr("slt", in);
            break;
        case SRA:
            print_shift_instr("sra", in);
            break;
        case SUB:
            print_r_instr("sub", in);
            break;
        case SYSCALL:
            printf("syscall\n");
            break;
        case SLL:
            print_shift_instr("sll", in);
            break;
        default:
            fprintf(stderr, "ERROR: Unrecognised instruction type\n");
    }
}

void print_r_instr(const char *name, Instr instr)
{
    R_Instr in = instr.in.r;
    printf("%s $%d, $%d, $%d\n", name, in.rd, in.rs, in.rt);
}

void print_i_instr(const char *name, Instr instr)
{
    I_Instr in = instr.in.i;
    printf("%s $%d, $%d, %d\n", name, in.rt, in.rs, in.imm);
}

void print_j_instr(const char *name, Instr instr)
{
    printf("%s 0x%08x\n", name, instr.in.j.addr << 2);
}

void print_jr_instr(Instr instr)
{
    printf("jr $%d\n", instr.in.r.rs);
}

void print_jalr_instr(Instr instr)
{
    printf("jalr $%d, $%d\n", instr.in.r.rd, instr.in.r.rs);
}

void print_sl_instr(const char *name, Instr instr)
{
    I_Instr in = instr.in.i;
    printf("%s $%d, %d($%d)\n", name, in.rt, BYTE*in.imm, in.rs);
}

void print_branch_instr(const char *name, Instr instr, bool use_rt)
{
    I_Instr in = instr.in.i;
    if (use_rt) {
        printf("%s $%d, $%d, %d\n", name, in.rs, in.rt, BYTE*in.imm);
    }
    else {
        printf("%s $%d %d\n", name, in.rs, BYTE*in.imm);
    }
}

void print_shift_instr(const char *name, Instr instr)
{
    R_Instr in = instr.in.r;
    printf("%s $%d, $%d, %d\n", name, in.rd, in.rt, in.shamt);
}

void print_lui_instr(Instr instr)
{
    printf("lui $%d, %d\n", instr.in.i.rt, instr.in.i.imm);
}
