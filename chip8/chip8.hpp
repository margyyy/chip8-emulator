#ifndef CHIP8_H
#define CHIP8_H
#include "/home/daniel/Coding/cpp/embedded/cpu/cpu.hpp"
#include "/home/daniel/Coding/cpp/embedded/memory/memory.hpp"
#include <cstdint>
#define FONTSET_SIZE 80
#define DEFAULT_PROGRAM_COUNT 0x200
class chip_8 {

    public:
        CPU cpu = CPU(DEFAULT_PROGRAM_COUNT);
        MEMORY memory;
        uint32_t video[64*32];
        uint8_t delay_timer{};
        uint8_t sound_timer{};
        bool keypad[16]{};
        const unsigned char fontset[FONTSET_SIZE] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
            0x20, 0x60, 0x20, 0x20, 0x70,		// 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
            0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
            0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
            0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
            0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
            0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
            0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
            0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
            0xF0, 0x80, 0xF0, 0x80, 0x80		// F
        };

        chip_8(){
            load_font_set();
        };

        void load_font_set();
        void load_rom(char const* filename);
        
        void decode();


        //instructions
        void OP_00E0(); // CLS : clear the display
        void OP_00EE(); // RET : return from a subroutine 
        void OP_0nnn(uint16_t addr); // SYS addr : ignored by modern interpreters
        void OP_1nnn(uint16_t addr); // JP addr : set PC to nnn
        void OP_2nnn(uint16_t addr); // CALL addr : increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
        void OP_3xkk(uint8_t x, uint8_t kk); // SE Vx, byte : skip next instruction if Vx = kk
        void OP_4xkk(uint8_t x, uint8_t kk); // SNE Vx, byte : skip next instruction if Vx != kk
        void OP_5xy0(uint8_t x, uint8_t y); // SE Vx, Vy : skip next instruction if Vx = Vy
        void OP_6xkk(uint8_t x, uint8_t kk); // LD Vx, byte : set Vx = kk
        void OP_7xkk(uint8_t x, uint8_t kk); // ADD Vx, byte : set Vx = Vx + kk
        void OP_8xy0(uint8_t x, uint8_t y); // LD Vx, Vy : set Vx = Vy
        void OP_8xy1(uint8_t x, uint8_t y); // OR Vx, Vy : set Vx = Vx OR Vy
        void OP_8xy2(uint8_t x, uint8_t y); // AND Vx, Vy : set Vx = Vx AND Vy
        void OP_8xy3(uint8_t x, uint8_t y); // XOR Vx, Vy : set Vx = Vx XOR Vy
        void OP_8xy4(uint8_t x, uint8_t y); // ADD Vx, Vy : set Vx = Vx + Vy, set VF = carry
        void OP_8xy5(uint8_t x, uint8_t y); // SUB Vx, Vy : set Vx = Vx - Vy, set VF = NOT borrow
        void OP_8xy6(uint8_t x, uint8_t y); // SHR Vx : set Vx = Vx SHR 1
        void OP_8xy7(uint8_t x, uint8_t y); // SUBN Vx, Vy : set Vx = Vy - Vx, set VF = NOT borrow
        void OP_8xyE(uint8_t x, uint8_t y); // SHL Vx : set Vx = Vx SHL 1
        void OP_9xy0(uint8_t x, uint8_t y); // SNE Vx, Vy : skip next instruction if Vx != Vy
        void OP_Annn(uint16_t addr); // LD I, addr : set I = nnn
        void OP_Bnnn(uint16_t addr); // JP V0, addr : jump to location nnn + V0
        void OP_Cxkk(uint8_t x, uint8_t kk); // RND Vx, byte : set Vx = random byte AND kk
        void OP_Dxyn(uint8_t x, uint8_t y, uint8_t n); // DRW Vx, Vy, nibble : display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
        void OP_Ex9E(uint8_t x); // SKP Vx : skip next instruction if key with the value of Vx is pressed
        void OP_ExA1(uint8_t x); // SKNP Vx : skip next instruction if key with the value of Vx is not pressed
        void OP_Fx07(uint8_t x); // LD Vx, DT : set Vx = delay timer value
        void OP_Fx0A(uint8_t x); // LD Vx, K : wait for a key press, store the value of the key in Vx
        void OP_Fx15(uint8_t x); // LD DT, Vx : set delay timer = Vx
        void OP_Fx18(uint8_t x); // LD ST, Vx : set sound timer = Vx
        void OP_Fx1E(uint8_t x); // ADD I, Vx : set I = I + Vx
        void OP_Fx29(uint8_t x); // LD F, Vx : set I = location of sprite for digit Vx
        void OP_Fx33(uint8_t x); // LD B, Vx : store BCD representation of Vx in memory locations I, I+1, and I+2
        void OP_Fx55(uint8_t x); // LD [I], Vx : store registers V0 through Vx in memory starting at location I
        void OP_Fx65(uint8_t x); // LD Vx, [I] : read registers V0 through Vx from memory starting at location I


        // some stuff
        void print_display();
        void print_chip8();

};




#endif
