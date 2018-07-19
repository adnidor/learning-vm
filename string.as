jmp 0x1000 #jump to beginning of program
#cannot use label because the assembler soesn't know about that yet

string:
#data ("Hello World!\n")
0x48656c6c6f20576f726c64210a 

begin:

#load addr of string in reg0 and reg2
ldc reg2 *string 
ldc reg0 +string

ldc reg1 0x01 #load addend in reg1

print:
prm reg2 reg0 #print character at addr
add #increase address (reg0=reg0+reg1)

jlt +begin print #loop until end of string

hlt #halt execution
