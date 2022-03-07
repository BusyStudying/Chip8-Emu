#pragma once
#include <cstdint>
const uint32_t MEMORY_SIZE = 4096;
const uint32_t BEGIN_PROGRAM_ADDR = 0x200;
const uint32_t REGISTERS = 16;
const uint32_t DISPLAY_HEIGHT = 6;
const uint32_t DISPLAY_WIDTH = 32;

class Chip8 {
public:
    Chip8();
    void Load_ROM(const char *filename);
    void print_memory(void);
    void print_registers(void);
    void cycle();
private:
    uint8_t mem[MEMORY_SIZE]{};
    uint8_t register_file[REGISTERS - 1]{};
    uint8_t VF{};
    uint16_t pc{};
    uint8_t sp{};
    uint16_t stack[16]{};
    uint8_t DT{};
    uint8_t ST{};
    uint32_t display[DISPLAY_WIDTH][DISPLAY_HEIGHT]{};
    uint16_t instruction{};
    uint16_t registerI{};

    void NOP();
    void CLS_00E0();
    void RET_00EE();
    void JP_1nnn();
    void CALL_2nnn();
    void SE_3xkk();
    void SNE_4xkk();
    void SE_5xy0();
    void LD_6xkk();
    void ADD_7xkk();
    void LD_8xy0();
    void OR_8xy1();
    void AND_8xy2();
    void XOR_8xy3();
    void ADD_8xy4();
    void SUB_8xy5();
    void SHR_8xy6();
    void SUBN_8xy7();
    void SHL_8xyE();
    void SNE_9xy0();
    void LD_Annn();
    void JP_Bnnn();
    void RND_Cxkk();
    void DRW_Dxyn();
    void fetch();
    void decode_execute();
    
    
};