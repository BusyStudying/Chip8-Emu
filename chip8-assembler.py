import argparse
import sys
def main():
    parser = argparse.ArgumentParser(description='Assembler for CHIP8')
    parser.add_argument('input', help="input in CHIP8 assembly", type=argparse.FileType('r'))
    parser.add_argument('output', help="output file name")
    args = parser.parse_args()
    #f = args.output
    bytes = assemble(args.input)
    bytes_to_write = bytearray(bytes)
    with open('out', "wb") as f:
        f.write(bytes_to_write)
    f.close()

def assemble(source):
    bytes = []
    line_number = 1
    for instruction in source.readlines():
        if instruction == None:
            continue
        if ';' in instruction:
            instruction = instruction[0:instruction.index(';')]
        instruction_list = instruction.split(', ')
        try:
            word = instruction_table[instruction_list[0]]
            operand_list = instruction_operands[instruction_list[0]]
        except KeyError:
            sys.exit("Invalid instruction " + instruction_list[0] + "\nOn line: " + str(line_number))

        if operand_list[0] < (len(instruction_list) - 1):
            sys.exit("invalid number of operands" + "\nOn line: " + str(line_number))


        if operand_list[0] > 0 and operand_list[1] == True:
            try:
                word += int(register_file[instruction_list[1]] << 8)
            except:
                sys.exit("Invalid register " + str(instruction_list[1]) + "\nOn line: " + str(line_number))
        if operand_list[0] > 1 and operand_list[2] == True:
            try:
                word += int((register_file[instruction_list[2]] << 4))
            except:
                sys.exit("Invalid register " + str(instruction_list[2]) + "\nOn line: " + str(line_number))        
        if operand_list[0] > 1 and operand_list[3] == True:
            try:
                value = int(instruction_list[2])
                word += value
            except:
                sys.exit("Invalid immediate value " + str(instruction_list[2]) + "\nOn line: " + str(line_number))
        if operand_list[0] > 1 and operand_list[4] == True:
            try:
                value = int(instruction_list[2])
                word += value
            except:
                sys.exit("Invalid immediate value " + str(instruction_list[2]) + "\nOn line: " + str(line_number))

                
                
        bytes.append((word >> 8) & 0x00FF)
        bytes.append(word & 0x00FF)
        print(hex(word))
        print(bytes)
        line_number += 1

    return bytes

instruction_table = {
    "CLS" : 0x00E0,
    "RTS" : 0x00EE,
    "JUMP"  : 0x1000,
    "CALL": 0x2000,
    "SKE"  : 0x3000,
    "SKNE" : 0x4000,
    "SKRE" : 0x5000,
    "LOAD" : 0x6000,
    "ADD" : 0x7000,
    "MOVE" : 0x8000,
    "OR" : 0x8001,
    "AND": 0x8002,
    "XOR" : 0x8003,
    "ADDR" : 0x8004,
    "SUB" : 0x8005,
    "SHR" : 0x8006,
    "SUBN" : 0x8007,
    "SHL" : 0x800E,
    "SKRNE": 0x9000,
    "LOADI": 0xA000,
    "JUMPI" : 0xB000,
    "RAND" : 0xC000,
    "DRAW" : 0xD000,
    "SKPR" : 0xE09E,
    "SKUP" : 0xE0A1,
    "MOVED" : 0xF007,
    "KEYD" : 0xF00A,
    "LOADD" : 0xF015,
    "LOADS" : 0xF018,
    "ADDI" : 0xF01E,
    "LDSPR": 0xF029,
    "BCD": 0xF033,
    "STOR": 0xF0FF,
    "READ": 0xF065
}
register_file = {
    "V0" : 0x0,
    "V1" : 0x1,
    "V2" : 0x2,
    "V3" : 0x3,
    "V4" : 0x4,
    "V5" : 0x5,
    "V6" : 0x6,
    "V7" : 0x7,
    "V8" : 0x8,
    "V9" : 0x9,
    "VA" : 0xA,
    "VB" : 0xB,
    "VC" : 0xC,
    "VD" : 0xD,
    "VE" : 0xE,
    "VF" : 0xF
}
#[number of operands, reg, reg, imm (byte), address]
instruction_operands = {
    #     numop  reg    reg   imm    addr 
    "CLS" : [0, False, False, False, False],
    "RTS" : [0, False, False, False, False],
    "JUMP"  : [1, False, False, False, True],
    "CALL": [1, False, False, False, True],
    "SKE"  : [2, True, False, True, False],
    "SKNE" : [2, True, False, True, False],
    "SKRE" : [2, True, True, False, False],
    "LOAD" : [2, True, False, True, False,],
    "ADD" : [2, True, False, True, False],
    "MOVE" : [2, True, True, False, False],
    "OR" : [2, True, True, False, False],
    "AND": [2, True, True, False, False],
    "XOR" : [2, True, True, False, False],
    "ADDR" : [2, True, True, False, False],
    "SUB" : [2, True, True, False, False],
    "SHR" : [2, True, True, False, False],
    "SUBN" : [2, True, True, False, False],
    "SHL" : [2, True, True, False, False],
    "SKRNE": [2, True, True, False, False],
    "LOADI": [1, False, False, False, True],
    "JUMPI" : [1, False, False, False, True],
    "RAND" : [2, True, False, True, False],
    "DRAW" : [3, True, True, True, False],
    "SKPR" : [1, True, False, False, False],
    "SKUP" : [1, True, False, False, False],
    "MOVED" : [1, True, False, False, False],
    "KEYD" : [1, True, False, False, False],
    "LOADD" : [1, True, False, False, False],
    "LOADS" : [1, True, False, False, False],
    "ADDI" : [1, True, False, False, False],
    "LDSPR": [1, True, False, False, False],
    "BCD": [1, True, False, False, False],
    "STOR": [1, True, True, False, False],
    "READ": [1, True, False, False, False]
}
#[number of operands, dest_reg, source_reg, imm (byte), address]
main()