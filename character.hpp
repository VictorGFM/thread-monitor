#ifndef CHARACTER_H
#define CHARACTER_H
#endif

#include "utils.hpp"

#include <string>
#include <iostream>
#include <random>
#include <pthread.h>
#include <unistd.h>

using std::string;
using std::cout;
using std::endl;
using std::mt19937;
using std::uniform_int_distribution;
using std::random_device;

class Character {
    private:
        string name;

    public:
        pthread_t thread;

        Character(string name) {
            this->name = name;
        };

        void useOven();
        void eat();
        void work();
        string getName();
};