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
        pthread_cond_t coupleCallSheldonAmy;
        pthread_cond_t coupleCallHowardBernardette;
        pthread_cond_t coupleCallLeonardPenny;
        string releasedCharacterDeadlock;
        bool coupleDeadlockOccurred;

        void findFirstOfCouples(vector<string>* charactersName);
        bool queueContains(string name);
        string getPairName(string name);
        bool isPairInQueue(string name);
        bool isCharacterInQueueWithoutPair(string name);
        int isQueueInDeadlock();
        void useOven();
        void freeOven();
        bool isOvenInUse();
        void characterWait(pthread_cond_t* varCond);
        void characterSignal(pthread_cond_t* varCond);
        void characterBroadcast(pthread_cond_t* varCond);
        bool havePriorityToUseOven(Character* character, bool* pairCalled);
    public:

        Oven() {
            ovenInUse = false;
            releasedCharacterDeadlock = "";
            coupleDeadlockOccurred = false;
            if(pthread_mutex_init(&mutex, NULL) != 0) {
                perror("Error on initializing mutex");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_init(&priorityToUseOven, NULL) != 0) {
                perror("Error on initializing condition variable");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_init(&coupleCallSheldonAmy, NULL) != 0) {
                perror("Error on initializing condition variable");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_init(&coupleCallHowardBernardette, NULL) != 0) {
                perror("Error on initializing condition variable");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_init(&coupleCallLeonardPenny, NULL) != 0) {
                perror("Error on initializing condition variable");
                exit(EXIT_FAILURE); 
            }
        };

        ~Oven() {
            if(pthread_mutex_destroy(&mutex) != 0) {
                perror("Error on destroying mutex");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_destroy(&priorityToUseOven) != 0) {
                perror("Error on destroying condition variable");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_destroy(&coupleCallSheldonAmy) != 0) {
                perror("Error on destroying condition variable");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_destroy(&coupleCallHowardBernardette) != 0) {
                perror("Error on destroying condition variable");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_destroy(&coupleCallLeonardPenny) != 0) {
                perror("Error on destroying condition variable");
                exit(EXIT_FAILURE); 
            }
        }

        void wait(Character* character);
        void free(Character* character);
        void verify();
};