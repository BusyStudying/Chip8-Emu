import argparse
def main():
    parser = argparse.ArgumentParser(description='Assembler for CHIP8')
    parser.add_argument('input', help="input in CHIP8 assembly", type=argparse.FileType('r'))
    parser.add_argument('output', help="output file name")
    args = parser.parse_args()
    #f = args.output
    assemble(args.input)
    

def assemble(source):
    for instruction in source.readlines():
        instruction_list = instruction.split(' ')

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
