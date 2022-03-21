#include "Chip8.hpp"
#include "SDL_Display.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char** argv) {

    if (argc != 4) {
        std::cout << "Incorrect number of arguments" << std::endl;
        return 0;
    }

    const char *romFilename = argv[1];
    int set_delay = std::stoi(argv[2]);
    int scale = std::stoi(argv[3]);
    SDL_Display SDL_Display("CHIP-8", scale * DISPLAY_WIDTH, scale * DISPLAY_HEIGHT, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    bool quit = false;
    Chip8 chip8;
    int pitch = sizeof(uint32_t) * DISPLAY_WIDTH;
    chip8.Load_ROM(romFilename);
    //chip8.print_memory();
    auto tend = std::chrono::high_resolution_clock::now();
    //int count = 0;
    while(!quit) {
        quit = SDL_Display.process_input(chip8.keyboard);
        auto tbegin = std::chrono::high_resolution_clock::now();
        float curr_tdelay = std::chrono::duration<float, std::chrono::milliseconds::period>(tbegin - tend).count();
        if (curr_tdelay > set_delay) {
            tend = tbegin;
            chip8.cycle();
            SDL_Display.update_display(chip8.display, pitch);
            //std::cout << "Cycle: " << count++ << std::endl;
            //chip8.print_registers();
            //chip8.end_on_NOP();
        }
    }
    return 0;
}
