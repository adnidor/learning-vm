#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

typedef void (*instr_func)(void); 

uint16_t instruction_pointer = 0;

uint8_t reg0 = 0;
uint8_t reg1 = 0;
uint8_t reg2 = 0;
uint8_t reg3 = 0;
uint8_t reg4 = 0;
uint8_t reg5 = 0;
uint8_t reg6 = 0;
uint8_t reg7 = 0;
uint8_t reg8 = 0;
uint8_t reg9 = 0;

uint8_t memory[65536];

instr_func instructions[256];

uint8_t* get_reg(uint8_t input) {
    switch (input) {
        case 0:
            return &reg0;
        case 1:
            return &reg1;
        case 2:
            return &reg2;
        case 3:
            return &reg3;
        case 4:
            return &reg4;
        case 5:
            return &reg5;
        case 6:
            return &reg6;
        case 7:
            return &reg7;
        case 8:
            return &reg8;
        case 9:
            return &reg9;
    }
    return NULL;
}


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

void nop(void) {
    //no op
    instruction_pointer++;
}

void hlt(void) {
    //halt execution
    printf("Halting execution\n");
    exit(0);
}

void dmp(void) {
    //dump memory
    //hexDump("dmp instruction called, dumping memory:", &memory, sizeof(memory));
    hexDump("dmp instruction called, dumping memory:", &memory, 256);
    instruction_pointer++;
}

void dbg(void) {
    //print debug info
    printf("dbg instruction called\n");
    printf("    reg0: 0x%02x",reg0);
    printf("    reg1: 0x%02x",reg1);
    printf("    reg2: 0x%02x",reg2);
    printf("    reg3: 0x%02x",reg3);
    printf("    reg4: 0x%02x",reg4);
    printf("    reg5: 0x%02x",reg5);
    printf("    reg6: 0x%02x",reg6);
    printf("    reg7: 0x%02x",reg7);
    printf("    reg8: 0x%02x",reg8);
    printf("    reg9: 0x%02x",reg9);
    printf("\n    instruction_pointer: 0x%04x\n",instruction_pointer);
    instruction_pointer++;
}

void prm_old(void) {
    //print character from mem
    instruction_pointer++;
    uint16_t memory_location = *((uint16_t*) &memory[instruction_pointer]);
    printf("prm from: 0x%04x: 0x%02x\n",memory_location,memory[memory_location]);
    putchar(memory[memory_location]);
    instruction_pointer++;
    instruction_pointer++;
} 

void prm(void) {
    //print character from mem
    instruction_pointer++;
    uint8_t* high_byte = get_reg(memory[instruction_pointer]);
    instruction_pointer++;
    uint8_t* low_byte = get_reg(memory[instruction_pointer]);
    uint16_t memory_location = *low_byte | (*high_byte << 8);
    //printf("prm from: 0x%04x: 0x%02x\n",memory_location,memory[memory_location]);
    putchar(memory[memory_location]);
    instruction_pointer++;
} 

void pr0(void) {
    //print character from reg0
    putchar(reg0);
    instruction_pointer++;
} 

void ldc(void) { 
    //load constant to reg
    instruction_pointer++;
    uint8_t* reg = get_reg(memory[instruction_pointer]);
    if (reg == NULL) {
        printf("Illegal Argument\n");
        exit(1);
    }
    instruction_pointer++;
    *reg = memory[instruction_pointer];
    instruction_pointer++;
}

void add(void) {
    //add reg0 and reg1, store in reg0
    reg0 = reg0+reg1;
    instruction_pointer++;
}

void sub(void) {
    //subtract reg1 from reg0, store in reg0
    reg0 = reg0-reg1;
    instruction_pointer++;
}

void jmp(void) {
    //unconditional jump
    instruction_pointer++;
    uint16_t memory_location = *((uint16_t*) &memory[instruction_pointer]);
    instruction_pointer = memory_location;
}

void jlt(void) {
    //jump if less than
    instruction_pointer++;
    uint8_t comparison = memory[instruction_pointer];
    instruction_pointer++;
    if (reg0 < comparison) {
        uint16_t memory_location = *((uint16_t*) &memory[instruction_pointer]);
        instruction_pointer = memory_location;
    } else {
        instruction_pointer++;
        instruction_pointer++;
    }
}

void jgt(void) {
    //jump if greater than
    instruction_pointer++;
    uint8_t comparison = memory[instruction_pointer];
    instruction_pointer++;
    if (reg0 > comparison) {
        uint16_t memory_location = *((uint16_t*) &memory[instruction_pointer]);
        instruction_pointer = memory_location;
    } else {
        instruction_pointer++;
        instruction_pointer++;
    }
}

void jeq(void) {
    //jump if equal
    instruction_pointer++;
    uint8_t comparison = memory[instruction_pointer];
    instruction_pointer++;
    if (reg0 == comparison) {
        uint16_t memory_location = *((uint16_t*) &memory[instruction_pointer]);
        instruction_pointer = memory_location;
    } else {
        instruction_pointer++;
        instruction_pointer++;
    }
}

void ldy(void) {
    //load from memory
    instruction_pointer++;
    uint8_t* reg = get_reg(memory[instruction_pointer]);
    if (reg == NULL) {
        printf("Illegal Argument\n");
        exit(1);
    }
    instruction_pointer++;
    uint8_t* mregh = get_reg(memory[instruction_pointer]);
    uint8_t* mregl = get_reg(memory[instruction_pointer]+1);
    if (mregl == NULL) {
        printf("Illegal Argument\n");
        exit(1);
    }
    uint16_t memloc = *mregh<<8 | *mregl;
    printf("memloc: 0x%04x\n",memloc);
    *reg = memory[memloc];
    instruction_pointer++;
}

void illegal_instruction(void) {
    printf("Illegal Instruction at 0x%04x\n",instruction_pointer);
    exit(1);
}

void sto(void) {
    //store reg in mem
    instruction_pointer++;
    uint8_t* reg = get_reg(memory[instruction_pointer]);
    if (reg == NULL) {
        printf("Illegal Argument\n");
        exit(1);
    }
    instruction_pointer++;
    uint16_t memory_location = *((uint16_t*) &memory[instruction_pointer]);
    instruction_pointer++;
    instruction_pointer++;
    memory[memory_location] = *reg;
}

int main(int argc, char* argv[]) {
    for (int i = 0; i<256; i++) {
        instructions[i] = illegal_instruction;
    }
    instructions[0x00] = nop;
    instructions[0x01] = dbg;
    instructions[0x02] = dmp;
    instructions[0x03] = hlt;
    instructions[0x04] = prm;
    instructions[0x05] = pr0;
    instructions[0x06] = ldc;
    instructions[0x07] = ldy;
    instructions[0x08] = add;
    instructions[0x09] = sub;
    instructions[0x0a] = jmp;
    instructions[0x0b] = jlt;
    instructions[0x0c] = jgt;
    instructions[0x0d] = jeq;
    instructions[0x0e] = sto;

    if (argc < 2) {
        printf("Not enought arguments\n");
        exit(1);
    }

    FILE *f = fopen(argv[1], "rb");

    if (f == 0) {
        printf("error opening file\n");
        exit(1);
    }

    fread(memory, 0xffff, 1, f);

    memory[0xffff] = 0xff;

    printf("Starting execution\n");

    while(1) {
        //printf("Reading instruction from 0x%04x, ",instruction_pointer);
        //printf("Content is 0x%02x\n",memory[instruction_pointer]);
        instructions[memory[instruction_pointer]]();
    }

}
