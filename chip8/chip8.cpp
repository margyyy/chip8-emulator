#include "chip8.hpp"
#include "/home/daniel/Coding/cpp/embedded/utils/logger.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
void chip_8::load_font_set() {
  uint8_t start = 0x50;
  for (auto &a : chip_8::fontset) {
    chip_8::memory.m[start] = a;
    start++;
  }
  LOGGER::getIstance().display("FONTS LOADED", "GREEN");
};

void chip_8::OP_00E0() { memset(video, 0, sizeof(video)); };

void chip_8::OP_00EE(){
  if (chip_8::cpu.sp == 0) return;
  chip_8::cpu.sp = chip_8::cpu.sp - 1;
  chip_8::cpu.pc = chip_8::cpu.stack[chip_8::cpu.sp];
}

void chip_8::OP_0nnn(uint16_t addr){
}

void chip_8::OP_1nnn(uint16_t addr){
  chip_8::cpu.pc = addr;
}

void chip_8::OP_2nnn(uint16_t addr){
  chip_8::cpu.stack[chip_8::cpu.sp] = chip_8::cpu.pc;
  chip_8::cpu.sp+=1;
  chip_8::cpu.pc = addr;
};

void chip_8::OP_3xkk(uint8_t x, uint8_t kk){
  if (chip_8::cpu.registers[x] == kk) chip_8::cpu.pc += 2;
}

void chip_8::OP_4xkk(uint8_t x, uint8_t kk){
  if (chip_8::cpu.registers[x] != kk) chip_8::cpu.pc += 2;
}

void chip_8::OP_5xy0(uint8_t x, uint8_t y){
  if (chip_8::cpu.registers[x] == chip_8::cpu.registers[y]) chip_8::cpu.pc += 2;
}

void chip_8::OP_6xkk(uint8_t x, uint8_t kk){
  chip_8::cpu.registers[x] = kk;
}

void chip_8::OP_7xkk(uint8_t x, uint8_t kk){
  chip_8::cpu.registers[x] += kk;
}

void chip_8::OP_8xy0(uint8_t x, uint8_t y){
  chip_8::cpu.registers[x] = chip_8::cpu.registers[y];
}

void chip_8::OP_8xy1(uint8_t x, uint8_t y){
  chip_8::cpu.registers[x] |= chip_8::cpu.registers[y];
}

void chip_8::OP_8xy2(uint8_t x, uint8_t y){
  chip_8::cpu.registers[x] &= chip_8::cpu.registers[y];
}

void chip_8::OP_8xy3(uint8_t x, uint8_t y){
  chip_8::cpu.registers[x] ^= chip_8::cpu.registers[y];
}

void chip_8::OP_8xy4(uint8_t x, uint8_t y){
  uint16_t sum = chip_8::cpu.registers[x] + chip_8::cpu.registers[y];
  chip_8::cpu.registers[0xF] = sum > 0xFF ? 1 : 0;
  chip_8::cpu.registers[x] = sum & 0xFF;
}

void chip_8::OP_8xy5(uint8_t x, uint8_t y){
  uint8_t vx = chip_8::cpu.registers[x];
  uint8_t vy = chip_8::cpu.registers[y];
  chip_8::cpu.registers[0xF] = vx > vy ? 1 : 0;
  chip_8::cpu.registers[x] = vx - vy;
}

void chip_8::OP_8xy6(uint8_t x, uint8_t y){
  chip_8::cpu.registers[0xF] = chip_8::cpu.registers[x] & 0x1;
  chip_8::cpu.registers[x] >>= 1;
}

void chip_8::OP_8xy7(uint8_t x, uint8_t y){
  uint8_t vx = chip_8::cpu.registers[x];
  uint8_t vy = chip_8::cpu.registers[y];
  chip_8::cpu.registers[0xF] = vy > vx ? 1 : 0;
  chip_8::cpu.registers[x] = vy - vx;
}

void chip_8::OP_8xyE(uint8_t x, uint8_t y){
  chip_8::cpu.registers[0xF] = (chip_8::cpu.registers[x] >> 7) & 0x1;
  chip_8::cpu.registers[x] <<= 1;
}

void chip_8::OP_9xy0(uint8_t x, uint8_t y){
  if (chip_8::cpu.registers[x] != chip_8::cpu.registers[y]) chip_8::cpu.pc += 2;
}

void chip_8::OP_Annn(uint16_t addr){
  chip_8::cpu.index_register = addr;
}

void chip_8::OP_Bnnn(uint16_t addr){
  chip_8::cpu.pc = addr + chip_8::cpu.registers[0];
}

void chip_8::OP_Cxkk(uint8_t x, uint8_t kk){
  chip_8::cpu.registers[x] = (rand() % 256) & kk;
}

void chip_8::OP_Dxyn(uint8_t x, uint8_t y, uint8_t n){
  uint8_t vx = chip_8::cpu.registers[x] % 64;
  uint8_t vy = chip_8::cpu.registers[y] % 32;
  chip_8::cpu.registers[0xF] = 0;
  for (uint8_t row = 0; row < n; row++) {
    uint8_t sprite = chip_8::memory.m[chip_8::cpu.index_register + row];
    for (uint8_t col = 0; col < 8; col++) {
      if (sprite & (0x80 >> col)) {
        uint16_t px = (vx + col) % 64;
        uint16_t py = (vy + row) % 32;
        uint16_t index = py * 64 + px;
        if (video[index] == 1) chip_8::cpu.registers[0xF] = 1;
        video[index] ^= 1;
      }
    }
  }
}

void chip_8::OP_Ex9E(uint8_t x){
  if (chip_8::keypad[chip_8::cpu.registers[x]]) chip_8::cpu.pc += 2;
}

void chip_8::OP_ExA1(uint8_t x){
  if (!chip_8::keypad[chip_8::cpu.registers[x]]) chip_8::cpu.pc += 2;
}

void chip_8::OP_Fx07(uint8_t x){
  chip_8::cpu.registers[x] = chip_8::delay_timer;
}

void chip_8::OP_Fx0A(uint8_t x){
  for (uint8_t i = 0; i < 16; i++) {
    if (chip_8::keypad[i]) {
      chip_8::cpu.registers[x] = i;
      return;
    }
  }
  chip_8::cpu.pc -= 2;
}

void chip_8::OP_Fx15(uint8_t x){
  chip_8::delay_timer = chip_8::cpu.registers[x];
}

void chip_8::OP_Fx18(uint8_t x){
  chip_8::sound_timer = chip_8::cpu.registers[x];
}

void chip_8::OP_Fx1E(uint8_t x){
  chip_8::cpu.index_register += chip_8::cpu.registers[x];
}

void chip_8::OP_Fx29(uint8_t x){
  chip_8::cpu.index_register = 0x50 + (chip_8::cpu.registers[x] * 5);
}

void chip_8::OP_Fx33(uint8_t x){
  uint8_t value = chip_8::cpu.registers[x];
  chip_8::memory.m[chip_8::cpu.index_register] = value / 100;
  chip_8::memory.m[chip_8::cpu.index_register + 1] = (value / 10) % 10;
  chip_8::memory.m[chip_8::cpu.index_register + 2] = value % 10;
}

void chip_8::OP_Fx55(uint8_t x){
  for (uint8_t i = 0; i <= x; i++) {
    chip_8::memory.m[chip_8::cpu.index_register + i] = chip_8::cpu.registers[i];
  }
}

void chip_8::OP_Fx65(uint8_t x){
  for (uint8_t i = 0; i <= x; i++) {
    chip_8::cpu.registers[i] = chip_8::memory.m[chip_8::cpu.index_register + i];
  }
}

void chip_8::print_display() {
  for (auto &a : video) {
    std::cout << a << " ";
  }
}

void chip_8::print_chip8(){
  std::cout << "===== CHIP-8 STATE =====\n";

  // Registers V0..V15
  for (int i = 0; i < 16; ++i) {
    std::cout << "V" << std::dec << i << " = 0x"
              << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
              << static_cast<int>(cpu.registers[i]) << "\n";
  }

  // Index register
  std::cout << std::dec << "I  = 0x" << std::hex << std::uppercase
            << std::setw(4) << std::setfill('0') << cpu.index_register << "\n";

  // Program counter
  std::cout << std::dec << "PC = 0x" << std::hex << std::uppercase
            << std::setw(4) << std::setfill('0') << cpu.pc << "\n";

  // Stack pointer
  std::cout << std::dec << "SP = 0x" << std::hex << std::uppercase
            << std::setw(2) << std::setfill('0') << static_cast<int>(cpu.sp) << "\n";

  // Stack
  std::cout << std::dec << "Stack:\n";
  for (int i = 0; i < 16; ++i) {
    std::cout << "  [" << std::dec << i << "] = 0x"
              << std::hex << std::uppercase << std::setw(4) << std::setfill('0')
              << cpu.stack[i] << "\n";
  }

  std::cout << std::dec;

  std::this_thread::sleep_for(std::chrono::seconds(2));

  // Clear the screen
  std::cout << "\033[2J\033[1;1H" << std::flush;
}

void chip_8::load_rom(char const *filename) {
  std::ifstream file(filename, std::ios::binary | std::ios::ate);

  if (file.is_open()) {
    std::streampos size = file.tellg();
    char *buffer = new char[size];

    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();

    for (long i = 0; i < size; ++i) {
      memory.m[DEFAULT_PROGRAM_COUNT + i] = buffer[i];
    }

    delete[] buffer;
  }
}

void chip_8::decode() {
  uint8_t opcode[2] = {0, 0}; // opcode used to decode instructions
}
