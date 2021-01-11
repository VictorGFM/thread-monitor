#include "oven.hpp"

void Oven::Use(User* user) {
    mtx.lock();
    this->inUse = true;
    // sleep(1);
    user->UseOven();
    this->Free(user);
    this->inUse = false;
    mtx.unlock();
}

void Oven::Wait(User* user) {
    this->userQueue.push_back(user);
    printf("\n%s quer usar o forno", user->Name());
    // user->Wait(UserWait);
    user->t = thread(UserWait);
}

void Oven::UserWait(User* user) {
    while(1) {
        if (this->inUse) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        if (!user->CanUse(this->userQueue)) {
            continue;
        }

        this->Use(user);
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

void Oven::RajStart() {
    int noUseTime = 0;
    while (1) {
        if (this->inUse) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }
        if (this->userQueue.empty()) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            noUseTime+=5;
            if (noUseTime > 5) {
                return;
            }
            continue;
        }
        this->Use(this->userQueue.front());
    }
}