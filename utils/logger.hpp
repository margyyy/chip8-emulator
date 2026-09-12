#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "/home/daniel/Coding/cpp/embedded/utils/termcolor.hpp"
#ifndef LOGGER_H
#define LOGGER_H
class LOGGER {

    public:

        static LOGGER& getIstance() {
            static LOGGER obj;
            return obj;
        }


        void display(std::string str, std::string color){

            time_t tt;
            struct tm* ti;

            time(&tt);
            ti = localtime(&tt);

            if(color == "GREEN"){
                std::cout << asctime(ti) << ">> ";
                std::cout << termcolor::green << str << termcolor::reset << "\n\n";

            };

            if(color == "RED"){
                std::cout << asctime(ti) << ">> ";
                std::cout << termcolor::red << str << termcolor::reset << "\n\n";
            }

        }

    private:
        LOGGER() = default;
        LOGGER(const LOGGER&) = delete;
        LOGGER& operator=(const LOGGER&) = delete;

};
#endif
