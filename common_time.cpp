#include <ctime>
#include <iostream>
#include <string>
#include "common_time.h"
#define BEGIN 4
#define DATE_LEN 27 //buffer which should have room for at least 26 bytes

Time::Time() {
	this->date = std::chrono::system_clock::now();
}

void Time::validTo() {
    // common_time.cpp:18:  Consider using ctime_r(...) instead of ctime(...)
    // for improved thread safety.  [runtime/threadsafe_fn] [2]
    std::chrono::duration<int,std::ratio<60*60*60*24> > month(1);
    this->date = this->date + month;
}

void Time::toString(std::string& str) {
    std::time_t t = std::chrono::system_clock::to_time_t(this->date);
    char buf[DATE_LEN];
    //ctime return value: string "Www Mmm dd hh:mm:ss yyyy"

    // common_time.cpp:18:  Consider using ctime_r(...) instead of ctime(...)
    // for improved thread safety.  [runtime/threadsafe_fn] [2]

    // The reentrant version ctime_r() does the same, but stores the string in
    // r-supplied buffer which should have room for at least 26 bytes. It need,
    // timezone, and daylight.
    ctime_r(&t,buf);
    str= std::string{ &buf[BEGIN] }; //subbstr -> MMM DD HH:mm:SS YYYY
}

/*
Mar 28 21:33:04 2019
28 21:33:04 2019
*/
