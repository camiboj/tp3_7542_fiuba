#ifndef COMMON_TIME_H
#define COMMON_TIME_H
#include <time.h>
#include <chrono>
#include <string>

class Time {
    private:
        std::chrono::system_clock::time_point date;
    public:
        /*
         * Contenedor de tiempo que se inicializa con la fecha de su creación.
        */
        Time();
        //Time(std::string filename, std::string& to, std::string& from);
        
        /*
         * Le suma 60 dias a la fecha actual.
        */
        void validTo();
        /*
         * Almacena en str la fecha en formato MMM DD HH:mm:SS YYYY
        */
        void toString(std::string& str);
};

#endif
