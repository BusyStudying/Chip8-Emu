#include "chip8.hpp"
#include <iostream>

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cout << "Incorrect number of arguments" << std::endl;
        return 0;
    }

    const char *romFilename = argv[1];
    Chip8 chip8;
    chip8.Load_ROM(romFilename);
    chip8.print_memory();
}
