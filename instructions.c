void ldc(void) { 
    //load constant to accu
    uint32_t argument = (memory[instruction_pointer] & 0xfffff0) >> 4;
    printf("executing %s with argument 0x%07x\n", __func__, argument);
    accu = argument;
    instruction_pointer++;

}

void add(void) {
    //add value to accu
    uint32_t argument = (memory[instruction_pointer] & 0xfffff0) >> 4;
    printf("executing %s with argument 0x%07x\n", __func__, argument);
    accu = accu+argument;
    instruction_pointer++;
}

void jmp(void) {
    //unconditional jump
    uint32_t argument = (memory[instruction_pointer] & 0xfffff0) >> 4;
    printf("executing %s with argument 0x%07x\n", __func__, argument);
    instruction_pointer = argument;
}

void illegal_instruction(void) {
    printf("Illegal Instruction at 0x%04x\n",instruction_pointer);
    exit(1);
}

void stv(void) {
    //store reg in mem
    uint32_t argument = (memory[instruction_pointer] & 0xfffff0) >> 4;
    printf("executing %s with argument 0x%07x\n", __func__, argument);
    instruction_pointer++;
    memory[argument] = accu;
}

void ldv(void) {
    //store reg in mem
    uint32_t argument = (memory[instruction_pointer] & 0xfffff0) >> 4;
    printf("executing %s with argument 0x%07x\n", __func__, argument);
    instruction_pointer++;
    accu = memory[argument];
}

void ldiv_inst(void) {
    //load indirect
    uint32_t argument = (memory[instruction_pointer] & 0xfffff0) >> 4;
    printf("executing %s with argument 0x%07x\n", __func__, argument);
    instruction_pointer++;
    accu = memory[memory[argument]];
}

void stiv(void) {
    //store indirect
    uint32_t argument = (memory[instruction_pointer] & 0xfffff0) >> 4;
    printf("executing %s with argument 0x%07x\n", __func__, argument);
    instruction_pointer++;
    memory[memory[argument]] = accu;
}

void and(void) {
    //bitwise and
    uint32_t argument = (memory[instruction_pointer] & 0xfffff0) >> 4;
    printf("executing %s with argument 0x%07x\n", __func__, argument);
    instruction_pointer++;
    accu = accu & memory[argument];
}

void or(void) {
    //bitwise or
    uint32_t argument = (memory[instruction_pointer] & 0xfffff0) >> 4;
    printf("executing %s with argument 0x%07x\n", __func__, argument);
    instruction_pointer++;
    accu = accu | memory[argument];
}

void xor(void) {
    //bitwise xor
    uint32_t argument = (memory[instruction_pointer] & 0xfffff0) >> 4;
    printf("executing %s with argument 0x%07x\n", __func__, argument);
    instruction_pointer++;
    accu = accu ^ memory[argument];
}

void not(void) {
    //invert accu
    instruction_pointer++;
    accu = ~accu;
}
void halt(void) {
    //halt execution
    printf("Halting execution\n");
    exit(0);
}

void dump(void) {
    //dump memory
    //hexDump("dmp instruction called, dumping memory:", &memory, sizeof(memory));
    hexDump("dmp instruction called, dumping memory:", &memory, 256);
    instruction_pointer++;
}
