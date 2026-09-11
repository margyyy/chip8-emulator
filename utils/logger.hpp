#include <iostream>
#include <ctime>
#ifndef LOGGER_H
#define LOGGER_H
class LOGGER {

    public:
        static LOGGER& getIstance() {
            static LOGGER obj;
            return obj;
        }


        void display(std::string str){
            time_t tt;
            struct tm* ti;

            time(&tt);
            ti = localtime(&tt);

            std::cout << asctime(ti) << ">> " << str << "\n";
        }

    private:
        LOGGER() = default;
        LOGGER(const LOGGER&) = delete;
        LOGGER& operator=(const LOGGER&) = delete;

};
#endif
