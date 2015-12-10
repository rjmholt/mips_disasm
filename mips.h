//#define DEBUG

/* Enumerated MIPS instuctions
 * to be used in place of opcodes */
typedef enum instr_name
{
    ADD,
    ADDI,
    ADDIU,
    ADDU,
    AND,
    ANDI,
    BEQ,
    BGEZ,
    BGTZ,
    BLEZ,
    BLTZ,
    BNE,
    J,
    JAL,
    JALR,
    JR,
    LB,
    LUI,
    LW,
    MUL,
    NOP,
    ORI,
    SB,
    SLL,
    SLT,
    SRA,
    SUB,
    SW,
    SYSCALL
} Instr_Type;

/* A MIPS R-type (register) instruction
 * of the form:
 * [opcode (6)][rs (5)][rt (5)][rd (5)][shamt (5)][funct (6)]
 * e.g. add, and
 * The opcode is discarded (stored instead as an Enum).
 */
typedef struct R_type_instruction
{
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
    uint8_t shamt;
} R_Instr;

/* A MIPS I-type (immediate) instruction
 * of the form:
 * [opcode (6)][rs (5)][rd (5)][immediate (16)]
 * e.g. addi, beq
 */
typedef struct I_type_instruction
{
    uint8_t rs;
    uint8_t rt;
    uint16_t imm;
} I_Instr;

/* MIPS J-type (jump) instruction
 * of the form:
 * [opcode (6)][address (26)]
 * e.g. j, jal
 */
typedef struct J_type_instruction
{
    uint32_t addr;
} J_Instr;

/* General instruction storage.
 * More efficient than a struct.
 */
typedef union instruction_union
{
    R_Instr r;
    I_Instr i;
    J_Instr j;   
} Instr_U;

/* Final storage for an instruction.
 * The opcode/MIPS name is encoded in type,
 * and the instruction contents are in the union field.
 */
typedef struct Instruction
{
    Instr_Type type;
    Instr_U in;
    uint32_t word;
} Instr;

/* Max size of a MIPS program */
#define NUM_INSTR 1000
/* Group struct for instructions -- a MIPS program */
typedef struct instrs_t
{
    Instr ins[NUM_INSTR];
    unsigned short len;
} InstructionArray;


/* Reads a 32-bit integer encoding a MIPS instruction,
 * and returns an Instr which stores that instruction
 */
Instr mips_machine_read(uint32_t);
