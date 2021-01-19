#ifndef OVEN_H
#define OVEN_H
#endif

#include "user.hpp"
#include <vector>
#include <pthread.h>
#include <unistd.h>

using std::vector;

class Oven {

    public:
        Oven() {
            if(pthread_create(&raj, NULL, RajStart, (void*) this) < 0) {
                cout << "Error on creating thread!" << endl;
                exit(EXIT_FAILURE);
            }
        };

        void Wait(User* user);
        void Use(User* user);
        void Free(User* user);
        bool IsInUse();
        bool HasQueue();
        pthread_t raj;

    private:
        static void* RajStart(void* args);
        static void* UserWait(void* args);

        bool inUse;
        pthread_mutex_t mtx;
        vector<User*> userQueue;
};