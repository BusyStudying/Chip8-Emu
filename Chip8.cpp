#include "Chip8.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>

Chip8::Chip8() {
    pc = 0x200;
    //instruction = (mem[0x200] << 8) + mem[0x201];
}

void Chip8::Load_ROM(const char *filename) {
    std::cout << "Loading \"" << filename << "\" into memory" << std::endl;
    std::ifstream fin(filename, std::ios::in|std::ios::binary|std::ios::ate);
    if (fin.is_open()) {
        std::streampos size = fin.tellg();
        fin.seekg(0, std::ios::beg);
        char *buffer = new char[size];
        fin.read(buffer, size);
        fin.close();
        
        for (int i = 0; i < size; i++) {
            mem[BEGIN_PROGRAM_ADDR + i] = buffer[i];
        }
        delete[] buffer;
    }
    else{
        std::cout<<"Error: File not found"<< std::endl;
    }
}

void Chip8::print_memory() {
    //print program space in memory
    for (int i = 0x200; i < 0x600; i++) {
        if ((i - 0x200) % 8 == 0) {
            std::cout << std::endl;
        } 
        std::cout << std::setw(3) << std::hex << (int)mem[i] << " ";
        
    }
    std::cout << "\nFINISHED\n";
}

void Chip8::print_registers() {
    std::cout << "   PC: " << pc;
    std::cout << "    SP: " << (unsigned int)sp << std::endl;
    for (int i = 0; i < REGISTERS; i++) {
        if ((i & 1) == 0) {
            std::cout << std::endl;
        }
        std::cout << std::setw(4) << "V" << std::hex << i << ": " 
        << std::hex << (unsigned int)register_file[i];
    }
    std::cout << std::endl << std::endl << "   VF: " << (unsigned int)VF;
    std::cout << "    I: " << registerI << std::endl << std::endl;
}

void Chip8::fetch() {
    instruction = (mem[pc] << 8) + (mem[pc + 1]);
    pc += 2;
}

void Chip8::decode_execute() {
    switch((instruction & 0xF000) >> 12) {
        case 0:
            if (instruction & 0x000F == 0) {
                CLS_00E0();
                break;
            } else {
                RET_00EE();
                break;
            }
        case 1:
            JP_1nnn();
            break;
        case 2:
            CALL_2nnn();
            break;
        case 3:
            SE_3xkk();
            break;
        case 4:
            SNE_4xkk();
            break;
        case 5:
            SE_5xy0();
            break;
        case 6:
            LD_6xkk();
            break;
        case 7:
            ADD_7xkk();
            break;
        case 8:
            switch (instruction & 0x000F) {
                case 0:
                    LD_8xy0();
                    break;
                case 1:
                    OR_8xy1();
                    break;
                case 2:
                    AND_8xy2();
                    break;
                case 3:
                    XOR_8xy3();
                    break;
                case 4:
                    ADD_8xy4();
                    break;
                case 5:
                    SUB_8xy5();
                    break;
                case 6:
                    SHR_8xy6();
                    break;
                case 7:
                    SUBN_8xy7();
                    break;
                case 14:
                    SHL_8xyE();
                    break;
                default:
                    NOP();
                    break;
            }
            break;
        case 9:
            SNE_9xy0();
            break;
        case 10:
            LD_Annn();
            break;
        case 11:
            JP_Bnnn();
            break;
        case 12:
            RND_Cxkk();
            break;
        default:
            NOP();
            break;
    }
}

void Chip8::cycle() {
    fetch();
    decode_execute();
}


void Chip8::NOP() {
}
void Chip8::CLS_00E0(){
    //Clear Display
    for (int x = 0; x < DISPLAY_WIDTH; x++){
        for (int y = 0; y < DISPLAY_HEIGHT; y++)
            display[x][y] = 0;
    }
}
void Chip8::RET_00EE() {
    //return from subroutine
    pc = stack[sp];
    sp -= 1;
}
void Chip8::JP_1nnn(){
    //jump to location nnn
    pc = instruction & 0x0FFF; 
}
void Chip8::CALL_2nnn(){
    //call subroutine at nnn
    sp +=1;
    stack[sp] = pc;
    pc = instruction & 0x0FFF;

}
void Chip8::SE_3xkk() {
    //Skip next instruction if Vx == kk

    uint8_t reg_num = (instruction & 0x0F00) >> 8;
    uint8_t byte = instruction & 0x00FF;
    if (register_file[reg_num] == byte){
        pc += 2;
    }

}
void Chip8::SNE_4xkk(){
    //Skip next instruction if Vx != Vy
    uint8_t reg_num = (instruction & 0x0F00) >> 8;
    uint8_t byte = instruction & 0x00FF;
    if (register_file[reg_num] != byte){
        pc += 2;
    }
}
void Chip8::SE_5xy0(){
    //Skip next instruction if Vx == Vy
    uint8_t reg_x = (instruction & 0x0F00) >> 8;
    uint8_t reg_y = (instruction & 0x00F0) >> 4;
    if (register_file[reg_x] == register_file[reg_y]){
        pc += 2;
    }
}
void Chip8::LD_6xkk() {
    //Set Vx = kk
    register_file[(instruction & 0x0F00) >> 8] = instruction & 0xFF;
}
void Chip8::ADD_7xkk() {
    //Set Vx = Vx + kk
    register_file[(instruction & 0x0F00) >> 8] += instruction & 0xFF;
}
void Chip8::LD_8xy0() {
    //Set Vx = Vy
    register_file[(instruction & 0x0F00) >> 8] = register_file[(instruction & 0x00F0) >> 4];
}
void Chip8::OR_8xy1() {
    //Set Vx = Vx bitwise OR Vy
    register_file[(instruction & 0x0F00) >> 8] =  register_file[(instruction & 0x0F00) >> 8] | register_file[(instruction & 0x00F0) >> 4];
}
void Chip8::AND_8xy2() {
    //Set Vx = Vx bitwise AND Vy
    register_file[(instruction & 0x0F00) >> 8] =  register_file[(instruction & 0x0F00) >> 8] & register_file[(instruction & 0x00F0) >> 4];
}
void Chip8::XOR_8xy3() {
    //Set Vx = Vx bitwise XOR Vy
    register_file[(instruction & 0x0F00) >> 8] ^= register_file[(instruction & 0x00F0) >> 4];
}
void Chip8::ADD_8xy4() {
    //Set Vx = Vx + Vy, set VF = carry
    uint16_t Vx = register_file[(instruction & 0x0F00) >> 8];
    uint16_t Vy = register_file[(instruction & 0x00F0) >> 4];
    if (Vx + Vy > 255) {
        VF = 1;
    } else {
        VF = 0;
    }
    register_file[(instruction & 0x0F00) >> 8] += register_file[(instruction & 0x00F0) >> 4];
}
void Chip8::SUB_8xy5() {
    //Set Vx = Vx - Vy, set VF = NOT borrow
    if (register_file[(instruction & 0x0F00) >> 8] > register_file[(instruction & 0x00F0) >> 4]) {
        VF = 1;
    } else {
        VF = 0;
    }
    register_file[(instruction & 0x0F00) >> 8] -= register_file[(instruction & 0x00F0) >> 4];

}
void Chip8::SHR_8xy6(){
    //Set Vx = Vx SHR 1
    if (register_file[(instruction & 0x0F00) >> 8] & 1) {
        VF = 0;

    } else {
        VF = 1;
    }
    register_file[(instruction & 0x0F00) >> 8] = register_file[(instruction & 0x0F00) >> 8] >> 1;
}
void Chip8::SUBN_8xy7(){
    //Set Vx = Vy - Vx, set VF = NOT borrow
    if (register_file[(instruction & 0x0F00) >> 8] < register_file[(instruction & 0x00F0) >> 4]) {
        VF = 1;
    } else {
        VF = 0;
    }
    register_file[(instruction & 0x0F00) >> 8] = register_file[(instruction & 0x00F0) >> 4] - register_file[(instruction & 0x0F00) >> 8];
}

void Chip8::SHL_8xyE(){
    //Set Vx = Vx SHL 1
    if (register_file[(instruction & 0x0F00) >> 8] & 1) {
        VF = 0;

    } else {
        VF = 1;
    }
    register_file[(instruction & 0x0F00) >> 8] = register_file[(instruction & 0x0F00) >> 8] << 1;
}
void Chip8::SNE_9xy0() {
    //Skip next instruction if Vx != Vy 
    if(register_file[(instruction & 0x0F00) >> 8] != register_file[(instruction & 0x0F00) >> 4]) {
        pc += 2;
    } 
}
void Chip8::LD_Annn() {
    //Set I = nnn
    registerI = instruction & 0x0FFF;

}
void Chip8::JP_Bnnn() {
    //Jump to location nnn + V0
    pc = register_file[0] + (instruction & 0x0FFF);
}
void Chip8::RND_Cxkk() { 
    //Set Vx = random byte AND kk
    register_file[(instruction & 0x0F00) >> 8] = (rand() % 256) & (instruction & 0x0FF);
}
void Chip8::DRW_Dxyn(){
    //skip for now
}