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
        pthread_mutex_t mutex;
        pthread_cond_t sheldonOrAmyInQueue;
        pthread_cond_t sheldonAndAmyInQueue;
        pthread_cond_t howardOrBernardetteInQueue;
        pthread_cond_t howardAndBernardetteInQueue;
        pthread_cond_t leonardOrPennyInQueue;
        pthread_cond_t leonardAndPennyInQueue;
        pthread_cond_t coupleSheldonAmyInQueue;
        
        bool ovenInUse;

        void characterWait(pthread_cond_t* varCond);
        
        /* static void* RajStart(void* args); */
    public:
        pthread_t raj;

        Oven() {
            ovenInUse = false;
            if(pthread_mutex_init(&mutex, NULL) != 0) {
                perror("Error on initializing mutex");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_init(&sheldonOrAmyInQueue, NULL) != 0) {
                perror("Error on initializing condition variable");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_init(&sheldonAndAmyInQueue, NULL) != 0) {
                perror("Error on initializing condition variable");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_init(&howardOrBernardetteInQueue, NULL) != 0) {
                perror("Error on initializing condition variable");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_init(&howardAndBernardetteInQueue, NULL) != 0) {
                perror("Error on initializing condition variable");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_init(&leonardOrPennyInQueue, NULL) != 0) {
                perror("Error on initializing condition variable");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_init(&leonardAndPennyInQueue, NULL) != 0) {
                perror("Error on initializing condition variable");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_init(&coupleSheldonAmyInQueue, NULL) != 0) {
                perror("Error on initializing condition variable");
                exit(EXIT_FAILURE); 
            }

            /* if(pthread_create(&raj, NULL, RajStart, (void*) this) < 0) {
                cout << "Error on creating thread!" << endl;
                exit(EXIT_FAILURE);
            } */
        };

        ~Oven() {
            if(pthread_mutex_destroy(&mutex) != 0) {
                perror("Error on destroying mutex");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_destroy(&sheldonOrAmyInQueue) != 0) {
                perror("Error on destroying condition variable");
                exit(EXIT_FAILURE); 
            }
        }

        void wait(Character* character);
        void free(Character* character);
};