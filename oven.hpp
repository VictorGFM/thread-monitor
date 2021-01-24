#ifndef OVEN_H
#define OVEN_H
#endif

#include "character.hpp"

#include <set>
#include <string>
#include <pthread.h>
#include <unistd.h>

using std::set;
using std::string;

class Oven {
    private:
        bool ovenInUse;
        pthread_mutex_t mutex;
        pthread_cond_t priorityToUseOven;
        pthread_cond_t coupleCallSheldonAmy;
        pthread_cond_t coupleCallHowardBernardette;
        pthread_cond_t coupleCallLeonardPenny;
        
        void characterWait(pthread_cond_t* varCond);
        bool havePriorityToUseOven(Character* character, bool* pairCalled);
        static void* RajStart(void* args);

    public:
        pthread_t raj;

        Oven() {
            ovenInUse = false;
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
            if(pthread_create(&raj, NULL, RajStart, (void*) this) < 0) {
                cout << "Error on creating thread!" << endl;
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
        void useOven();
        void freeOven();
        bool isOvenInUse();
};