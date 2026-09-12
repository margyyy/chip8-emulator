#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>
#include "/home/daniel/Coding/cpp/embedded/utils/logger.hpp"
class MEMORY {
    public:
        uint8_t m[4096]{};
        MEMORY(){
            LOGGER::getIstance().display("MEMORY INIT","GREEN");
        }
        void print_memory();
};



#endif
