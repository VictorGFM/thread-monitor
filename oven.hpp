#ifndef OVEN_H
#define OVEN_H
#endif

#include "character.hpp"

#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <pthread.h>
#include <unistd.h>

using std::list;
using std::find;
using std::string;
using std::vector;

class Oven {
    private:
        list<string> queue;
        bool ovenInUse;
        pthread_mutex_t mutex;
        pthread_cond_t priorityToUseOven;
        pthread_cond_t pairCallSheldonAmy;
        pthread_cond_t pairCallHowardBernardette;
        pthread_cond_t pairCallLeonardPenny;
        string releasedCharacterDeadlock;
        bool coupleDeadlockOccurred;

        bool havePriorityToUseOven(Character* character, bool* pairCalled);
        void waitPairCall(string name);
        void findFirstOfCouples(vector<string>* charactersName);
        bool queueContains(string name);
        string getPairName(string name);
        bool isPairInQueue(string name);
        bool isCharacterInQueueWithoutPair(string name);
        int isQueueInDeadlock();
        void useOven();
        void freeOven();
        bool isOvenInUse();
        void characterWait(pthread_cond_t* condVar);
        void characterSignal(pthread_cond_t* condVar);
        void characterBroadcast(pthread_cond_t* condVar);
        void initMutex(pthread_mutex_t* mutex);
        void initCondVar(pthread_cond_t* condVar);
        void destroyMutex(pthread_mutex_t* mutex);
        void destroyCondVar(pthread_cond_t* condVar);
    public:

        Oven() {
            ovenInUse = false;
            releasedCharacterDeadlock = "";
            coupleDeadlockOccurred = false;

            initMutex(&mutex);
            initCondVar(&priorityToUseOven);
            initCondVar(&pairCallSheldonAmy);
            initCondVar(&pairCallHowardBernardette);
            initCondVar(&pairCallLeonardPenny);
        };

        ~Oven() {
            destroyMutex(&mutex);
            destroyCondVar(&priorityToUseOven);
            destroyCondVar(&pairCallSheldonAmy);
            destroyCondVar(&pairCallHowardBernardette);
            destroyCondVar(&pairCallLeonardPenny);
        }

        void wait(Character* character);
        void free(Character* character);
        void verify();
};