#ifndef CPU_H
#define CPU_H
#include <cstdint>
#include <cstring>
#include <string>
#include "/home/daniel/Coding/cpp/embedded/utils/logger.hpp"
#include "/home/daniel/Coding/cpp/embedded/utils/toHextoNum.hpp"

class CPU {
    public:

        std::string name;
        uint8_t registers[16];
        uint16_t index_register{};

        uint16_t pc{};

        uint16_t stack[16]{};
        uint8_t sp{};

        CPU(uint16_t program_count){
            LOGGER::getIstance().display("CPU INIT", "GREEN");
            pc = program_count;
            LOGGER::getIstance().display("Program count set to " + toHex(pc), "GREEN");
            memset(registers, 0, sizeof(registers));
            LOGGER::getIstance().display("All registers set to 0", "GREEN");
            memset(stack, 0, sizeof(stack));
            LOGGER::getIstance().display("Stack set to  0", "GREEN");
            index_register = 0;
            sp = 0;
        }
};

#endif
