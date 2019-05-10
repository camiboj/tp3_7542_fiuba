#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "client_time.h"
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
    
    struct tm* time = 0; //‘time’ is used uninitialized 
    localtime_r(&t, time);
    std::stringstream ssTp;
	ssTp << std::put_time(time,"%b %d %T %Y");
    str = ssTp.str();
}


/************************************ PUT_TIME ********************************
 * %b	Abbreviated month name 
 * %d	Day of the month, zero-padded (01-31)
 * %T	ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S
 * %Y	Year complete number
 * str << std::put_time(this->date,"%b %d %T %Y");
 * 
 * => STR = MMM DD HH:mm:SS YYYY
*/
