#pragma once
const unsigned int MEMORY_SIZE = 4096;
const unsigned int BEGIN_PROGRAM_ADDR = 0x200;
const unsigned int REGISTERS = 16;
const unsigned int DISPLAY_HEIGHT = 6;
const unsigned int DISPLAY_WIDTH = 32;

class Chip8 {
public:
    Chip8();
    void Load_ROM(const char *filename);
    void print_memory(void);
private:
    unsigned char mem[MEMORY_SIZE]{};
    unsigned char register_file[REGISTERS - 1]{};
    unsigned char VF{};
    unsigned short pc{};
    unsigned char sp{};
    unsigned short stack[16]{};
    unsigned char DT{};
    unsigned char ST{};
    unsigned int display[DISPLAY_WIDTH][DISPLAY_HEIGHT]{};
    unsigned short instruction{};
    unsigned short registerI{};

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