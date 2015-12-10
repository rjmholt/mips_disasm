#include <stdio.h>
#include "mips.h"

typedef struct llist
{
    unsigned addr;
    struct llist *next;
} ListNode;

int write(InstructionArray ins_arr, const char *fname)
{
    FILE *fp;
    fp = fopen(fname, "w+");

    /* TODO MIPS disassembly goes here */

    return fclose(fp);
}


