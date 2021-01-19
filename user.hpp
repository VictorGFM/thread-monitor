#ifndef USER_H
#define USER_H
#endif

#include <string>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

class User {

    public:
        User(string name, int ovenTimes) {
            this->name = name;
            this->ovenTimes = ovenTimes;
        };

        string Name();
        int OvenTimes();
        bool CanUse(vector<User*> queue);
        void UseOven();
        void Work();
        pthread_t t;

    private:
        string name;
        int ovenTimes;
};