#include "chip8.hpp"
#include <cstdint>
#include <cstring>
#include "/home/daniel/Coding/cpp/embedded/utils/logger.hpp"
#include <fstream>
void chip_8::load_font_set(){
    uint8_t start = 0x50;
    for (auto &a : chip_8::fontset) {
        chip_8::memory.m[start] = a;
        start++;
    }
    LOGGER::getIstance().display("FONTS LOADED");
};

void chip_8::OP_00E0(){
    memset(video, 0, sizeof(video));
};

void chip_8::print_display(){
    for(auto &a : video){
        std::cout << a << " ";
    }
}

void chip_8::load_rom(char const* filename){
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open())
    {
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for (long i = 0; i < size; ++i)
        {
            memory.m[DEFAULT_PROGRAM_COUNT + i] = buffer[i];
        }

        delete[] buffer;
    }

}
