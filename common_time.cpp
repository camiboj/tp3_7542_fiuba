#include <ctime>
#include "common_time.h"
#define BEGIN 4
#define END 23

Time::Time() {
	this->date = std::chrono::system_clock::now();
}


void Time::plusMonth() {
    std::chrono::duration<int,std::ratio<60*60*60*24> > month (1);
    this->date = this->date + month;
}

void Time::toString(std::string str) {
    std::time_t t = std::chrono::system_clock::to_time_t(this->date);
    str = ctime(&t); //ctime return value: string "Www Mmm dd hh:mm:ss yyyy"
    str = str.substr(BEGIN, END); //substr -> MMM DD HH:mm:SS YYYY
}

