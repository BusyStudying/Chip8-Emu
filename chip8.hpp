#ifndef chip8_h
#define chip8_h

const unsigned int MEMORY_SIZE = 4096;
const unsigned int BEGIN_PROGRAM_ADDR = 0x200;
const unsigned int REGISTERS = 16;

class Chip8 {
public:
    Chip8();
    void Load_ROM(const char *filename);
    void print_memory(void);
private:
    unsigned char mem[MEMORY_SIZE]{};
};
#endif