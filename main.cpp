#include "/home/daniel/Coding/cpp/embedded/chip8/chip8.hpp"
int main(int argc, char* argv[]){


    // INIT CPU
    // CPU c = cpu(argv[1]);

    chip_8 test = chip_8();
    // test.memory.print_memory();
    // test.OP_00E0();
    // test.print_display();
    test.load_rom("/home/daniel/Coding/cpp/embedded/ROMS/Airplane.ch8");
    // test.memory.print_memory();
    return 0;
}





