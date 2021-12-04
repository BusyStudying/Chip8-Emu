#include "chip8.hpp"

int main(void) {
    const char romFilename[] = "test";
    Chip8 chip8;
    chip8.Load_ROM(romFilename);
}