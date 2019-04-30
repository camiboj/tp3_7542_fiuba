#include "server_thread.h"
Thread::Thread() {}

void Thread::start() {
   this->thread = std::thread(&Thread::run, this);
}
            

void Thread::join() {
   this->thread.join();
}

Thread::~Thread() {} 

Thread::Thread(Thread&& other) {
this->thread = std::move(other.thread);
}

