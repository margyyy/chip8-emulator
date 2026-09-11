#include "/home/daniel/Coding/cpp/embedded/utils/toHextoNum.hpp"
#include <sstream>
#include <string>
std::string toHex(const uint16_t num){
    std::stringstream stream;
    stream << std::hex << num;
    return "0x" + stream.str();
};
