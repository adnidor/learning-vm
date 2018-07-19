jmp 0x1000
string:
0x48656c6c6f20576f726c64210a
begin:
ldc reg2 *string
ldc reg0 +string
ldc reg1 0x01
print:
prm reg2 reg0
add
jlt +begin print
hlt
