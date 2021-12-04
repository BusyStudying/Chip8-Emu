#include "chip8.hpp"

#include <iostream>
#include <fstream>
//using namespace std;
Chip8::Chip8(){}

void Chip8::Load_ROM(const char *filename) {
    std::cout << filename << std::endl;
    std::ifstream fin(filename, std::ios::in|std::ios::binary|std::ios::ate);
    if (fin.is_open()) {
        std::streampos size = fin.tellg();
        fin.seekg(0, std::ios::beg);
        char *buffer = new char[size];
        fin.read(buffer, size);
        fin.close();
        
        for (int i = 0; buffer[i] != '\0'; i++) {
            mem[BEGIN_PROGRAM_ADDR + i] = buffer[i];
        }
        delete[] buffer;
        
    }
    else{
        std::cout<<"Error: File not found"<< std::endl;
    }
}

void Chip8::print_memory() {

    for (int i = 512; i < 600; i++) {
        std::cout << (int)mem[i] << " ";
        
    }
}
