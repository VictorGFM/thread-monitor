#include "oven.hpp"

struct parameters_struct {
    Oven* object;
    User* user;
} parameters;

void Oven::Use(User* user) {
    pthread_mutex_lock(&mtx);
    this->inUse = true;
    user->UseOven();
    this->Free(user);
    this->inUse = false;
    pthread_mutex_unlock(&mtx);
}

void Oven::Wait(User* user) {
    this->userQueue.push_back(user);
    cout << endl << user->Name() << " quer usar o forno";
    struct parameters_struct* parameters = (struct parameters_struct*) malloc(sizeof(struct parameters_struct*));
    parameters->object = this;
    parameters->user = user;
    
    if(pthread_create(&user->t, NULL, UserWait, (void*) parameters) < 0) {
        cout << "Error on creating thread!" << endl;
        exit(EXIT_FAILURE);
    }
}

void* Oven::UserWait(void* args) {
    struct parameters_struct* parameters = (struct parameters_struct*)args;
    Oven* object = parameters->object;
    User* user = parameters->user;
    while(1) {
        if (object->inUse) {
            sleep(1);
            continue;
        }

        if (!user->CanUse(object->userQueue)) {
            continue;
        }

        object->Use(user);
    }
}

void Oven::Free(User* user) {
    for (auto it = this->userQueue.begin(); it != this->userQueue.end(); ++it) {
        if ((*it)->Name() == user->Name()) {
            this->userQueue.erase(it);
            user->Work();
            if (user->OvenTimes() > 0) {
                this->Wait(user);
            }
        }
    }
}

void* Oven::RajStart(void* args) {
    Oven* object = (Oven*) args;
    int noUseTime = 0;
    while (1) {
        if (object->inUse) {
            sleep(5);
            continue;
        }
        if (object->userQueue.empty()) {
            sleep(5);
            noUseTime+=5;
            if (noUseTime > 5) {
                break;
            }
            continue;
        }
        object->Use(object->userQueue.front());
    }
    return 0;
}