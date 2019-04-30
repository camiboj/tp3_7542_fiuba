#ifndef SERVER_THREAD_H_
#define SERVER_THREAD_H_
#include <thread>

class Thread {
    private:
        std::thread thread;
        bool is_dead;
 
    public:
        Thread();
        void start();
        void join();
        virtual void run() = 0; 
        virtual void stop() = 0;
        virtual bool isDead() = 0;
        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;
        virtual ~Thread();
        Thread(Thread&& other);
        Thread& operator=(Thread&& other);
};

#endif
