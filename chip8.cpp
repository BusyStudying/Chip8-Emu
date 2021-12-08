#include "chip8.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>

//using namespace std;
Chip8::Chip8(){}

void Chip8::Load_ROM(const char *filename) {
    std::cout << filename << std::endl;
    std::ifstream fin(filename, std::ios::in|std::ios::binary|std::ios::ate);
    if (fin.is_open()) {
        std::streampos size = fin.tellg();
        fin.seekg(0, std::ios::beg);
        char *buffer = new char[size];
        fin.read(buffer, size);
        fin.close();
        
        for (int i = 0; buffer[i] != '\0'; i++) {
            mem[BEGIN_PROGRAM_ADDR + i] = buffer[i];
        }
        delete[] buffer;
    }
    else{
        std::cout<<"Error: File not found"<< std::endl;
    }
}

void Chip8::print_memory() {

    for (int i = 512; i < 600; i++) {
        std::cout << (int)mem[i] << " ";
        
    }
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
    pc = stack[sp];
    sp -= 1;
}
void Chip8::JP_1nnn(){
    pc = instruction & 0x0FFF; 
}
void Chip8::CALL_2nnn(){
    sp +=1;
    stack[sp] = pc;
    pc = instruction & 0x0FFF;

}
void Chip8::SE_3xkk() {

    uint8_t reg_num = (instruction & 0x0F00) >> 8;
    uint8_t byte = instruction & 0x00FF;
    if (register_file[reg_num] == byte){
        pc += 2;
    }

}
void Chip8::SNE_4xkk(){
    uint8_t reg_num = (instruction & 0x0F00) >> 8;
    uint8_t byte = instruction & 0x00FF;
    if (register_file[reg_num] != byte){
        pc += 2;
    }
}
void Chip8::SE_5xy0(){
    uint8_t reg_x = (instruction & 0x0F00) >> 8;
    uint8_t reg_y = (instruction & 0x00F0) >> 4;
    if (register_file[reg_x] == register_file[reg_y]){
        pc += 2;
    }
}
void Chip8::LD_6xkk() {
    register_file[(instruction & 0x0F00) >> 8] = instruction & 0xFF;
}
void Chip8::ADD_7xkk() {
    register_file[(instruction & 0x0F00) >> 8] += instruction & 0xFF;
}
void Chip8::LD_8xy0() {
    register_file[(instruction & 0x0F00) >> 8] = register_file[(instruction & 0x00F0) >> 4];
}
void Chip8::OR_8xy1() {
    register_file[(instruction & 0x0F00) >> 8] =  register_file[(instruction & 0x0F00) >> 8] | register_file[(instruction & 0x00F0) >> 4];
}
void Chip8::AND_8xy2() {
    register_file[(instruction & 0x0F00) >> 8] =  register_file[(instruction & 0x0F00) >> 8] & register_file[(instruction & 0x00F0) >> 4];
}
void Chip8::XOR_8xy3() {
    register_file[(instruction & 0x0F00) >> 8] ^= register_file[(instruction & 0x00F0) >> 4];
}
void Chip8::ADD_8xy4() {
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
    if (register_file[(instruction & 0x0F00) >> 8] > register_file[(instruction & 0x00F0) >> 4]) {
        VF = 1;
    } else {
        VF = 0;
    }
    register_file[(instruction & 0x0F00) >> 8] -= register_file[(instruction & 0x00F0) >> 4];

}
void Chip8::SHR_8xy6(){
    if (register_file[(instruction & 0x0F00) >> 8] & 1) {
        VF = 0;

    } else {
        VF = 1;
    }
    register_file[(instruction & 0x0F00) >> 8] = register_file[(instruction & 0x0F00) >> 8] >> 1;
}
void Chip8::SUBN_8xy7(){
    if (register_file[(instruction & 0x0F00) >> 8] < register_file[(instruction & 0x00F0) >> 4]) {
        VF = 1;
    } else {
        VF = 0;
    }
    register_file[(instruction & 0x0F00) >> 8] = register_file[(instruction & 0x00F0) >> 4] - register_file[(instruction & 0x0F00) >> 8];
}

void Chip8::SHL_8xyE(){
    if (register_file[(instruction & 0x0F00) >> 8] & 1) {
        VF = 0;

    } else {
        VF = 1;
    }
    register_file[(instruction & 0x0F00) >> 8] = register_file[(instruction & 0x0F00) >> 8] << 1;
}
void Chip8::SNE_9xy0() { 
    if(register_file[(instruction & 0x0F00) >> 8] != register_file[(instruction & 0x0F00) >> 4]) {
        pc += 2;
    } 
}
void Chip8::LD_Annn() {
    registerI = instruction & 0x0FFF;

}
void Chip8::JP_Bnnn() {
    pc = register_file[0] + (instruction & 0x0FFF);
}
void Chip8::RND_Cxkk() { 
    register_file[(instruction & 0x0F00) >> 8] = (rand() % 256) & (instruction & 0x0FF);
}
void Chip8::DRW_Dxyn(){
    //skip for now
}