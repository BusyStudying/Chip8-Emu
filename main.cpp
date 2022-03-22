#include "Chip8.hpp"
#include "SDL_Display.hpp"
#include <iostream>
#include <chrono>

#define print_usage "Usage: ./Chip8 <ROM_Filename> <Frequency (Hz)> <Window scale (64x32 * scale)>"

int main(int argc, char** argv) {

    if (argc != 4) {
        std::cout << "Incorrect number of arguments" << std::endl;
        std::cout << print_usage << std::endl;
        return 0;
    }

    const char *romFilename = argv[1];
    int set_frequency;
    int scale;
    try {
        set_frequency = std::stoi(argv[2]);
        scale = std::stoi(argv[3]);
    }
    catch(...) {
        std::cout << print_usage << std::endl;
        exit(0);
    }
    Chip8 chip8;
    if(!chip8.Load_ROM(romFilename)) {
        exit(0);
    }

    SDL_Display SDL_Display("CHIP-8", scale * DISPLAY_WIDTH, scale * DISPLAY_HEIGHT, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    int pitch = sizeof(uint32_t) * DISPLAY_WIDTH;
    bool quit = false;


    double set_frequency_ms = (1 / (double)set_frequency) * 1000;
    int cycle_update_counter = (((1.0 / 60.0) * 1000) / set_frequency_ms) + 1;
    int cycle_counter = 0;

    auto tend = std::chrono::high_resolution_clock::now();

    while(!quit) {
        quit = SDL_Display.process_input(chip8.keyboard);
        auto tbegin = std::chrono::high_resolution_clock::now();
        float curr_tdelay = std::chrono::duration<float, std::chrono::milliseconds::period>(tbegin - tend).count();
        if (curr_tdelay > set_frequency_ms) {
            tend = tbegin;
            chip8.cycle();
            cycle_counter++;
            if (cycle_counter >= cycle_update_counter) {
                chip8.update_timers();
                cycle_counter = 0;
            }
            SDL_Display.update_display(chip8.display, pitch);
        }
    }
    return 0;
}
