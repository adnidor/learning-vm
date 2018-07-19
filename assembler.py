#!/usr/bin/python3
import sys
import struct

instructions = { "dbg": { "value":0x01, "params":[] },
                 "dmp": { "value":0x02, "params":[] },
                 "hlt": { "value":0x03, "params":[] },
                 "prm": { "value":0x04, "params":["reg","reg"] },
                 "pr0": { "value":0x05, "params":[] },
                 "ldc": { "value":0x06, "params":["reg", "byte"] },
                 "ldy": { "value":0x07, "params":["reg", "reg"] },
                 "add": { "value":0x08, "params":[] },
                 "sub": { "value":0x09, "params":[] },
                 "jmp": { "value":0x0a, "params":["addr"] },
                 "jlt": { "value":0x0b, "params":["byte","addr"] },
                 "jgt": { "value":0x0c, "params":["byte","addr"] },
                 "jeq": { "value":0x0d, "params":["byte","addr"] },
                 "sto": { "value":0x0e, "params":["reg","addr"] } }

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

markers = {}

infile = sys.stdin
outfile = open(1, "wb")

machine_code = bytearray()

no_output = False

for line in infile:
    line = line.strip()
    elements = line.split()
    if elements[0].startswith("0x"):
        for element in elements:
            b = bytes.fromhex(element[2:])
            if not no_output:
                machine_code.extend(b)
    elif line[-1] == ":":
        markers[line[:-1]] = len(machine_code)
    else:
        ins = instructions[elements[0]]
        machine_code.extend(bytes([ins["value"]]))
        if len(ins["params"]) > 0:
            for i in range(1,len(ins["params"])+1):
                length = 1
                if ins["params"][i-1] == "addr":
                    length = 2
                    if not elements[i].startswith("0x"):
                        b = struct.pack("<H",markers[elements[i]])
                        machine_code.extend(b)
                        continue
                elif ins["params"][i-1] == "reg":
                    if not elements[i].startswith("0x"):
                        b = bytes([registers[elements[i]]])
                        machine_code.extend(b)
                        continue
                if elements[i].startswith("#"):
                    b = bytes([struct.pack("<H",markers[elements[i][1:]])[0]])
                    machine_code.extend(b)
                    continue
                if elements[i].startswith("*"):
                    b = bytes([struct.pack("<H",markers[elements[i][1:]])[1]])
                    machine_code.extend(b)
                    continue

                if (len(elements[i])-2)/2 != length:
                    print("Error: Argument has wrong length")
                    exit()
                b = bytes.fromhex(elements[i][2:])[0:length]
                machine_code.extend(b)

outfile.write(machine_code)
