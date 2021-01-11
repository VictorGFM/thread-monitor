#include <algorithm>
#include <iostream>
#include <random>
#include <thread>
#include "oven.hpp"
#include "user.hpp"

using std::cout;
using std::default_random_engine;
using std::endl;
using std::thread;
using std::shuffle;


void createQueue(int ovenTimes) {
    Oven *oven = new Oven();

    User *users[8];
    
    users[0] = new User("Sheldon", ovenTimes);
    users[1] = new User("Amy", ovenTimes);
    users[2] = new User("Howard", ovenTimes);
    users[3] = new User("Bernardette", ovenTimes);
    users[4] = new User("Leonard", ovenTimes);
    users[5] = new User("Penny", ovenTimes);
    users[6] = new User("Stuart", ovenTimes);
    users[7] = new User("Kripke", ovenTimes);

    shuffle(users, users + sizeof(users) / sizeof(users[0]), default_random_engine(0));

    for (int i = 0; i < 8; i++) {
        oven->Wait(users[i]);
    }

    for (int i = 0; i < 8; i++) {
        users[i]->t.join();
    }

    oven->raj.join();
}

int main(int paramsLen, char *params[]) {
    if (paramsLen != 2) {
        cout << "\n Numero de parametros invalido" << endl;
        return 0;
    }

    int ovenTimes = atoi(params[1]);
    if (ovenTimes < 1) {
        cout << "\n Parametro invalido" << endl;
        return 0;
    }

    createQueue(ovenTimes);

    return 0;
}