#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

typedef void (*instr_func)(void); 

uint32_t instruction_pointer = 0;

uint32_t accu = 0;

uint32_t memory[16777216];

instr_func instructions[16];

void hexDump(char *desc, void *addr, int len)
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf("  %s\n", buff);

            // Output the offset.
            printf("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf(" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = pc[i];
        }

        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf("  %s\n", buff);
}

#include "instructions.c"

int main(int argc, char* argv[]) {

    for (int i = 0; i<16; i++) {
        instructions[i] = illegal_instruction;
    }

    instructions[0x00] = halt;
    instructions[0x01] = ldc;
    instructions[0x02] = ldv;
    instructions[0x03] = stv;
    instructions[0x04] = ldiv_inst;
    instructions[0x05] = stiv;
    instructions[0x06] = add;
    instructions[0x07] = and;
    instructions[0x08] = or;
    instructions[0x09] = xor;
    instructions[0x0a] = not;
//    instructions[0x0b] = rar;
//    instructions[0x0c] = eql;
    instructions[0x0d] = jmp;
//    instructions[0x0e] = jmn;
    instructions[0x0f] = dump;

    if (argc < 2) {
        printf("Not enough arguments\n");
        exit(1);
    }

    FILE *f = fopen(argv[1], "rb");

    if (f == 0) {
        printf("error opening file\n");
        exit(1);
    }

    fread(memory, 0xffff, 1, f);
    fclose(f);

    printf("Starting execution\n");

    while(1) {
        printf("Reading instruction from 0x%08x, ",instruction_pointer);
        printf("Content is 0x%08x\n",memory[instruction_pointer]);
        uint32_t instruction = memory[instruction_pointer] & 0xf;
        uint32_t argument = (memory[instruction_pointer] & 0xfffff0) >> 4;
        printf("instruction: 0x%01x; arguemnt: 0x%05x\n", instruction, argument);
        instructions[instruction]();
        printf("Accu: 0x%05x\n", accu);
        printf("\n");
    }

}
