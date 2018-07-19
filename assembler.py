#!/usr/bin/python3
import sys
import struct
import yaml

class IllegalInstructionError(Exception):
    pass

class IllegalArgumentError(Exception):
    pass

class Instruction():
    def __init__(self, expression):
        try:
            self.expression = expression
            self.ins = instructions[expression]
            self.value = self.ins['instruction']
            self.params = self.ins['parameters']
        except NameError as e:
            raise IllegalInstructionError("Instruction {} not found".format(repr(expression)))

    def __bytes__(self):
        return bytes([self.value]) 

    def __repr__(self):
        return "Instruction({})".format(repr(self.expression))

class Parameter():
    def __init__(self,type,value):
        if type not in ["byte","addr","reg"]:
            raise ValueError("Invalid parameter type: {}".format(type))
        self.type = type
        self.invalue = value

        if self.type == "byte":
            self.length = 1
            if self.invalue.startswith("0x"):
                self.value = bytes.fromhex(self.invalue[2:])
            elif self.invalue.startswith("+"): #low addr byte
                self.value = bytes([struct.pack("<H",labels[self.invalue[1:]])[0]])
            elif self.invalue.startswith("*"): #high addr byte
                self.value = bytes([struct.pack("<H",labels[self.invalue[1:]])[1]])
            else:
                raise IllegalArgumentError("Invalid Argument: {}".format(repr(self.invalue)))

        elif self.type == "addr":
            self.length = 2
            if self.invalue.startswith("0x"):
                self.value = bytes.fromhex(self.invalue[2:])
            else:
                self.value = struct.pack("<H",labels[self.invalue])

        elif self.type == "reg":
            self.length = 1
            if self.invalue.startswith("0x"):
                self.value = bytes.fromhex(self.invalue[2:])
            else:
                self.value = bytes([registers[self.invalue]])

        if len(self.value) != self.length:
            raise IllegalArgumentError("Wrong length, expected {} but got {}".format(self.length,len(self.value)))

    def __bytes__(self):
        return self.value

    def __repr__(self):
        return "Parameter({},{})".format(repr(self.type),repr(self.value))

IS_FILE = "instructionset.yml"
COMMENT_MARKER = "#"

instructions = {}

for element in yaml.load(open(IS_FILE)):
    instructions[element['mnemonic']] = element

registers = { "reg0": 0x00,
              "reg1": 0x01,
              "reg2": 0x02,
              "reg3": 0x03,
              "reg4": 0x04,
              "reg5": 0x05,
              "reg6": 0x06,
              "reg7": 0x07,
              "reg8": 0x08,
              "reg9": 0x09 }

labels = {}

infile = sys.stdin
outfile = open(1, "wb")

machine_code = bytearray()

for linenumber, line in enumerate(infile):
    line = line.strip()

    if len(line) == 0:
        continue

    if line.startswith(COMMENT_MARKER):
        continue

    if line[-1] == ":": #ends with ':'
        labels[line[:-1]] = len(machine_code)
        continue

    elements = line.split()

    if elements[0].startswith("0x"):
        for element in elements:
            b = bytes.fromhex(element[2:])
            machine_code.extend(b)

    else:
        ins = Instruction(elements[0])
        machine_code.extend(bytes(ins))

        for i,element in enumerate(elements[1:]):
            if element.startswith(COMMENT_MARKER):
                break

            if len(ins.params) < i-1:
                raise IllegalArgumentException("Too many arguments")
            
            param = Parameter(ins.params[i],element)
            machine_code.extend(bytes(param))

outfile.write(machine_code)
