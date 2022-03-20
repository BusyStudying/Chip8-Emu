#include "Chip8.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

#define PIXEL_ON 0xFFFFFFFF
#define FONT_SIZE 80
#define FONT_ADDR 0x50
#define PC_START 0x200
#define Vx register_file[(instruction & 0x0F00) >> 8]
#define Vy register_file[(instruction & 0x00F0) >> 4]

uint8_t font_sprites[FONT_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() {
    pc = PC_START;

    for (int i = 0; i < FONT_SIZE; i++) {
        mem[FONT_ADDR + i] = font_sprites[i];
    }

    mem[0] = 0xDE;
    mem[1] = 0xAD;
    mem[2] = 0xBE;
    mem[3] = 0xEF;

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
    } else {
        std::cout<<"Error: File not found"<< std::endl;
    }
}

void Chip8::print_memory() {
    //print program space in memory
    for (int i = 0x0; i < 0xFFF; i++) {
        if (i % 16 == 0) {
            std::cout << std::endl;
        } 
        std::cout << std::setw(3) << std::hex << (uint)mem[i] << " ";
        
    }
    std::cout << "\nFINISHED\n";
}

void Chip8::print_registers() {
    std::cout << "   PC: " << std::hex << pc;
    std::cout << "    SP: " << std::hex << (uint)sp << std::endl;
    std::cout << "   Instruction Register: " << std::hex << instruction << std::endl;
    for (int i = 0; i < REGISTERS; i++) {
        if ((i & 1) == 0) {
            std::cout << std::endl;
        }
        std::cout << std::setw(4) << "V" << std::hex << i << ": " 
        << std::hex << (uint)register_file[i];
    }
    std::cout << std::endl << std::endl << "   VF: " << std::hex << (uint)VF;
    std::cout << "    I: " << std::hex << I << std::endl << std::endl;
}

void Chip8::fetch() {
    instruction = (mem[pc] << 8) + (mem[pc + 1]);
    pc += 2;
}

void Chip8::decode_execute() {
    switch((instruction & 0xF000) >> 12) {
        case 0:
            if (instruction & 0x00EE == 0x00EE) {
                RET_00EE();
            } else if (instruction & 0x00EE == 0x00E0) {
                CLS_00E0();
            } else {
                NOP();
            }
            break;
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
        case 13:
            DRW_Dxyn();
            break;
        case 14:
            switch(instruction & 0xFF) {
                case 0x9E:
                    SKP_Ex9E();
                    break;
                case 0xA1:
                    SKNP_ExA1();
                    break;
            }
            break;
        case 15:
            switch(instruction & 0xFF) {
                case 0x07:
                    LD_Fx07();
                    break;
                case 0x0A:
                    LD_Fx0A();
                    break;
                case 0x15:
                    LD_Fx15();
                    break;
                case 0x18:
                    LD_Fx18();
                    break;
                case 0x1E:
                    ADD_Fx1E();
                    break;
                case 0x29:
                    LD_Fx29();
                    break;
                case 0x33:
                    LD_Fx33();
                    break;
                case 0x55:
                    LD_Fx55();
                    break;
                case 0x65:
                    LD_Fx65();
                    break;
            }
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
    memset(display, 0, sizeof(display)); 
}
void Chip8::RET_00EE() {
    //return from subroutine
    pc = stack[sp];
    sp -= 1;
}
void Chip8::JP_1nnn() {
    //jump to location nnn
    pc = instruction & 0x0FFF; 
}
void Chip8::CALL_2nnn() {
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
void Chip8::SNE_4xkk() {
    //Skip next instruction if Vx != Vy
    uint8_t reg_num = (instruction & 0x0F00) >> 8;
    uint8_t byte = instruction & 0x00FF;
    if (register_file[reg_num] != byte){
        pc += 2;
    }
}
void Chip8::SE_5xy0() {
    //Skip next instruction if Vx == Vy
    uint8_t reg_x = (instruction & 0x0F00) >> 8;
    uint8_t reg_y = (instruction & 0x00F0) >> 4;
    if (register_file[reg_x] == register_file[reg_y]){
        pc += 2;
    }
}
void Chip8::LD_6xkk() {
    //Set Vx = kk
    Vx = instruction & 0xFF;
}
void Chip8::ADD_7xkk() {
    //Set Vx = Vx + kk
    Vx += instruction & 0xFF;
}
void Chip8::LD_8xy0() {
    //Set Vx = Vy
    Vx = register_file[(instruction & 0x00F0) >> 4];
}
void Chip8::OR_8xy1() {
    //Set Vx = Vx bitwise OR Vy
    Vx =  Vx | Vy;
}
void Chip8::AND_8xy2() {
    //Set Vx = Vx bitwise AND Vy
    Vx =  Vx & Vy;
}
void Chip8::XOR_8xy3() {
    //Set Vx = Vx bitwise XOR Vy
    Vx ^= Vy;
}
void Chip8::ADD_8xy4() {
    //Set Vx = Vx + Vy, set VF = carry
    if ((Vx + Vy) > 255) {
        VF = 1;
    } else {
        VF = 0;
    }
    Vx += Vy;
}
void Chip8::SUB_8xy5() {
    //Set Vx = Vx - Vy, set VF = NOT borrow
    if (Vx > Vy) {
        VF = 1;
    } else {
        VF = 0;
    }
    Vx -= Vy;

}
void Chip8::SHR_8xy6() {
    //Set Vx = Vx SHR 1
    if (Vx & 1) {
        VF = 0;

    } else {
        VF = 1;
    }
    Vx = Vx >> 1;
}
void Chip8::SUBN_8xy7() {
    //Set Vx = Vy - Vx, set VF = NOT borrow
    if (Vx < Vy) {
        VF = 1;
    } else {
        VF = 0;
    }
    Vx = Vy - Vx;
}

void Chip8::SHL_8xyE() {
    //Set Vx = Vx SHL 1
    if (Vx & 1) {
        VF = 0;

    } else {
        VF = 1;
    }
    Vx = Vx << 1;
}
void Chip8::SNE_9xy0() {
    //Skip next instruction if Vx != Vy 
    if(Vx != register_file[(instruction & 0x0F00) >> 4]) {
        pc += 2;
    } 
}
void Chip8::LD_Annn() {
    //Set I = nnn
    I = instruction & 0x0FFF;

}
void Chip8::JP_Bnnn() {
    //Jump to location nnn + V0
    pc = register_file[0] + (instruction & 0x0FFF);
}
void Chip8::RND_Cxkk() { 
    //Set Vx = random byte AND kk
    Vx = (rand() % 256) & (instruction & 0x0FF);
}
void Chip8::DRW_Dxyn() {
    uint32_t nibble = instruction & 0x0F;
    for (int i = 0; i < nibble; i++) {
        uint32_t sprite_row = mem[I + i];
        for (int j = 0; j < 8; j++) {
            uint32_t pixel = sprite_row & (0x80 >> j);
            uint display_row = ((Vy % DISPLAY_HEIGHT) + i) * DISPLAY_WIDTH; 
            uint display_col = ((Vx % DISPLAY_WIDTH) + j);
            if (pixel) {
                pixel = PIXEL_ON;
                if (display[display_row + display_col] == PIXEL_ON) {
                    VF = 1;
                }
                display[display_row + display_col] ^= pixel;
            }
        }
    } 
}

void Chip8::SKP_Ex9E() {
    if (keyboard[Vx]) {
        pc += 2;
    }
}
void Chip8::SKNP_ExA1() {
    if (Vx < 15) {
        if (!keyboard[Vx]) {
            pc += 2;
        }
    }
}
void Chip8::LD_Fx07() { 
    Vx = DT;
}
void Chip8::LD_Fx0A() { 
    for (int i = 0; i < 15; i++) {
        if (keyboard[i]) {
           Vx = i; 
           return;
        }
    }
    pc -= 2;
}
void Chip8::LD_Fx15() {
   DT = Vx;
}
void Chip8::LD_Fx18() {
   ST = Vx;
}
void Chip8::ADD_Fx1E() {
    I += Vx;
}
void Chip8::LD_Fx29() {
    I = (Vx * 5) + FONT_ADDR;
}
void Chip8::LD_Fx33() {
    uint BCD = Vx;
    int i = 0;
    while (BCD > 0 && i <= 2) {
        mem[I + i] = BCD % 10;
        BCD /= 10;
        i++;
    }
}
void Chip8::LD_Fx55() {
    for (int i = 0; i < 15; i++) {
        mem[I + i] = register_file[i];
    }
}
void Chip8::LD_Fx65() {
    for (int i = 0; i < 15; i++) {
        register_file[i] = mem[I + i];
    }
}

void Chip8::end_on_NOP() {
    if (!instruction) {
        quit = true;
    }
}
