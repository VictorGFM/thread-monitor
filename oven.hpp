#ifndef OVEN_H
#define OVEN_H
#endif

#include "user.hpp"
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

using std::mutex;
using std::vector;
using std::thread;

class Oven {

    public:
        Oven() {
            raj = thread(RajStart);
        };

        void Wait(User* u);
        void Use(User* u);
        void Free(User* u);
        bool IsInUse();
        bool HasQueue();
        thread raj;

    private:
        void RajStart();
        void UserWait(User* u);

        bool inUse;
        mutex mtx;
        vector<User*> userQueue;
};