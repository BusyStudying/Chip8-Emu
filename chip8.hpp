#ifndef chip8_h
#define chip8_h

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
    unsigned char register_file[REGISTERS - 1];
    unsigned char VF;
    uint16_t pc;
    unsigned char sp;
    uint16_t stack[16];
    unsigned char DT;
    unsigned char ST;
    uint32_t display[DISPLAY_WIDTH][DISPLAY_HEIGHT];
    uint16_t instruction;
    uint16_t registerI;

    void Chip8::NOP();
    void Chip8::CLS_00E0();
    void Chip8::RET_00EE();
    void Chip8::JP_1nnn();
    void Chip8::CALL_2nnn();
    void Chip8::SE_3xkk();
    void Chip8::SNE_4xkk();
    void Chip8::SE_5xy0();
    void Chip8::LD_6xkk();
    void Chip8::ADD_7xkk();
    void Chip8::LD_8xy0();
    void Chip8::OR_8xy1();
    void Chip8::AND_8xy2();
    void Chip8::XOR_8xy3();
    void Chip8::ADD_8xy4();
    void Chip8::SUB_8xy5();
    void Chip8::SHR_8xy6();
    void Chip8::SUBN_8xy7();
    void Chip8::SHL_8xyE();
    void Chip8::SNE_9xy0();
    void Chip8::LD_Annn();
    void Chip8::JP_Bnnn();
    void Chip8::RND_Cxkk();
    void Chip8::DRW_Dxyn();
    void Chip8::fetch();
    void Chip8::decode_execute();
    
};
#endif