#ifndef OVEN_H
#define OVEN_H
#endif

#include "character.hpp"
#include "constants.hpp"
#include <set>
#include <pthread.h>
#include <unistd.h>

using std::set;

class Oven {
    private:
        pthread_mutex_t mutex;
        pthread_cond_t sheldonOrAmyInQueue;
        set<string> queue;
        
        /* static void* RajStart(void* args); */
    public:
        pthread_t raj;

        Oven() {
            if(pthread_mutex_init(&mutex, NULL) != 0) {
                perror("Error on initializing mutex");
                exit(EXIT_FAILURE); 
            }
            if(pthread_cond_init(&sheldonOrAmyInQueue, NULL) != 0) {
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