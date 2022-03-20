#include "Chip8.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char** argv) {

    if (argc != 3) {
        std::cout << "Incorrect number of arguments" << std::endl;
        return 0;
    }

    const char *romFilename = argv[1];
    int set_delay = std::stoi(argv[2]);

    Chip8 chip8;
    chip8.Load_ROM(romFilename);
    chip8.print_memory();
    auto tend = std::chrono::high_resolution_clock::now();
    while(!chip8.quit) {
        auto tbegin = std::chrono::high_resolution_clock::now();
        float curr_tdelay = std::chrono::duration<float, std::chrono::milliseconds::period>(tbegin - tend).count();
        if (curr_tdelay > set_delay) {
            tend = tbegin;
            chip8.cycle();
            chip8.print_registers();
            chip8.end_on_NOP();
        }
    }
    chip8.print_registers();
    return 0;
}
