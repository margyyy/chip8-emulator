#include "memory.hpp"

void MEMORY::print_memory(){
    for(auto &a : MEMORY::m){
        std::cout << (int)a << "\n";
    }
}

