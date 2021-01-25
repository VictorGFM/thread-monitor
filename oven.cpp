#include "oven.hpp"

void Oven::wait(Character* character) {

    pthread_mutex_lock(&mutex);

    cout << "‍🙋  ‍" << character->getName() << " quer usar o forno" << endl;
    queue.push_back(character->getName());

    bool pairCalled = false;
    if(isPairInQueue(character->getName())) {
        waitPairCall(character->getName());
        pairCalled = true;
    }
    
    while(!havePriorityToUseOven(character, &pairCalled)) {
        characterWait(&priorityToUseOven);
    }

    useOven();

    pthread_mutex_unlock(&mutex);
}

void Oven::free(Character* character) {
    pthread_mutex_lock(&mutex);

    cout << "🍴  " << character->getName() << " vai comer" << endl;
    freeOven();
    queue.remove(character->getName());

    if(isPairInQueue(character->getName())) {
        if(character->getName() == SHELDON || character->getName() == AMY) {
            characterSignal(&pairCallSheldonAmy);
        }  else if(character->getName() == HOWARD || character->getName() == BERNARDETTE) {
            characterSignal(&pairCallHowardBernardette);
        } else if(character->getName() == LEONARD || character->getName() == PENNY) {
            characterSignal(&pairCallLeonardPenny);
        }
    } else {
        characterBroadcast(&priorityToUseOven);
    }
    
    pthread_mutex_unlock(&mutex);
}

void Oven::verify() {
    pthread_mutex_lock(&mutex);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, 2);

    switch(isQueueInDeadlock()) {
        case MENS_DEADLOCK: {
            string charactersName[3] = {SHELDON, HOWARD, LEONARD};
            releasedCharacterDeadlock = charactersName[distribution(gen)];
            characterBroadcast(&priorityToUseOven);
            break;
        } case WOMENS_DEADLOCK: {
            string charactersName[3] = {AMY, BERNARDETTE, PENNY};
            releasedCharacterDeadlock = charactersName[distribution(gen)];
            characterBroadcast(&priorityToUseOven);
            break;
        } case COUPLE_DEADLOCK: {
            vector<string> charactersName;
            findFirstOfCouples(&charactersName);
            releasedCharacterDeadlock = charactersName[distribution(gen)];
            characterBroadcast(&priorityToUseOven);
            break;
        } default:
            break;
    }
    
    pthread_mutex_unlock(&mutex);
}

bool Oven::havePriorityToUseOven(Character* character, bool* pairCalled) {
    if(isQueueInDeadlock()) {
        if(character->getName() == releasedCharacterDeadlock) {
            cout << "💀  Raj detectou um deadlock, liberando " << releasedCharacterDeadlock << endl;
            releasedCharacterDeadlock = "";
            return true;
        } else {
            return false;
        }
    } else if(character->getName() == SHELDON || character->getName() == AMY) {
        if(isPairInQueue(character->getName())) {
            if(queueContains(LEONARD) && queueContains(PENNY)) {
                return false;
            }
        } else if(*pairCalled) {
            *pairCalled = false;
            if((queueContains(LEONARD) && queueContains(PENNY)) 
                && !(queueContains(HOWARD) && queueContains(BERNARDETTE))) {
                characterBroadcast(&priorityToUseOven);
                return false;
            }
        } else {
            if((queueContains(LEONARD) && queueContains(PENNY)) 
                || (queueContains(HOWARD) && queueContains(BERNARDETTE))) {
                return false;
            } else if(queueContains(LEONARD) || queueContains(PENNY)) {
                return false;
            }
        }
        if(isOvenInUse()) {
            return false;
        } else {
            return true;
        }
    
    } else if(character->getName() == HOWARD || character->getName() == BERNARDETTE) {
        if(isPairInQueue(character->getName())) {
            if(queueContains(SHELDON) && queueContains(AMY)) {
                return false;
            }
        } else if(*pairCalled) {
            *pairCalled = false;
            if((queueContains(SHELDON) && queueContains(AMY)) 
                && !(queueContains(LEONARD) && queueContains(PENNY))) {
                characterBroadcast(&priorityToUseOven);
                return false;
            }
        } else {
            if((queueContains(SHELDON) && queueContains(AMY)) 
                || (queueContains(LEONARD) && queueContains(PENNY))) {
                return false;
            } else if(queueContains(SHELDON) || queueContains(AMY)) {
                return false;
            }
        }
        if(isOvenInUse()) {
            return false;
        } else {
            return true;
        }
    }  else if(character->getName() == LEONARD || character->getName() == PENNY) {
        if(isPairInQueue(character->getName())) {
            if(queueContains(HOWARD) && queueContains(BERNARDETTE)) {
                return false;
            }
        } else if(*pairCalled) {
            *pairCalled = false;
            if((queueContains(HOWARD) && queueContains(BERNARDETTE))
                && !(queueContains(SHELDON) && queueContains(AMY))) {
                characterBroadcast(&priorityToUseOven);
                return false;
            }
        } else {
            if((queueContains(HOWARD) && queueContains(BERNARDETTE))
                || (queueContains(SHELDON) && queueContains(AMY))) {
                return false;
            } else if(queueContains(HOWARD) || queueContains(BERNARDETTE)) {
                return false;
            }
        }
        if(isOvenInUse()) {
            return false;
        } else {
            return true;
        }
    } else if(character->getName() == STUART) {
        if(queueContains(SHELDON) || queueContains(AMY)
            || queueContains(HOWARD) || queueContains(BERNARDETTE)
            || queueContains(LEONARD) || queueContains(PENNY)) {
            return false;
        }
        if(isOvenInUse()) {
            return false;
        } else {
            return true;
        }
    } else  if(character->getName() == KRIPKE) {
        if(queueContains(SHELDON) || queueContains(AMY)
            || queueContains(HOWARD) || queueContains(BERNARDETTE)
            || queueContains(LEONARD) || queueContains(PENNY)
            || queueContains(STUART)) {
            return false;
        }
        if(isOvenInUse()) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

void Oven::waitPairCall(string name) {
    if(name == SHELDON || name == AMY) {
        characterWait(&pairCallSheldonAmy);
    } else if(name == HOWARD || name == BERNARDETTE) {
        characterWait(&pairCallHowardBernardette);
    } else if(name == LEONARD || name == PENNY) {
        characterWait(&pairCallLeonardPenny);
    }
}

void Oven::findFirstOfCouples(vector<string>* charactersName) {
    bool firstOfSheldonAndAmyFound = false; 
    bool firstOfHowardAndBernardette = false;
    bool firstOfLeonardAndPenny = false;
    for(const auto& element : queue) {
        if(!firstOfSheldonAndAmyFound && (element == SHELDON || element == AMY)) {
            firstOfSheldonAndAmyFound = true;
            charactersName->push_back(element);
        } else if(!firstOfHowardAndBernardette && (element == HOWARD || element == BERNARDETTE)) {
            firstOfHowardAndBernardette = true;
            charactersName->push_back(element);
        } else if(!firstOfLeonardAndPenny && (element == LEONARD || element == PENNY)) {
            firstOfLeonardAndPenny = true;
            charactersName->push_back(element);
        }
    }
}

bool Oven::queueContains(string name) {
    return (find(queue.begin(), queue.end(), name) != queue.end());
}

string Oven::getPairName(string name) {
    if(name == SHELDON) {
        return AMY;
    } else if(name == AMY) {
        return SHELDON;
    } else if(name == HOWARD) {
        return BERNARDETTE;
    } else if(name == BERNARDETTE) {
        return HOWARD;
    } else if(name == LEONARD) {
        return PENNY;
    } else if(name == PENNY) {
        return LEONARD;
    } else {
        return "";
    }
}

bool Oven::isPairInQueue(string name) {
    return queueContains(getPairName(name));
}

bool Oven::isCharacterInQueueWithoutPair(string name) {
    return (queueContains(name) && !isPairInQueue(name));
}

int Oven::isQueueInDeadlock() {
    bool mensDeadlock = isCharacterInQueueWithoutPair(SHELDON)
                        && isCharacterInQueueWithoutPair(HOWARD)
                        && isCharacterInQueueWithoutPair(LEONARD);

    bool womensDeadlock = isCharacterInQueueWithoutPair(AMY)
                          && isCharacterInQueueWithoutPair(BERNARDETTE)
                          && isCharacterInQueueWithoutPair(PENNY);

    bool coupleDeadlock = queueContains(SHELDON) && queueContains(AMY)
                          && queueContains(HOWARD) && queueContains(BERNARDETTE)
                          && queueContains(LEONARD) && queueContains(PENNY);

    if(!isOvenInUse()) {
        if(mensDeadlock) {
            return MENS_DEADLOCK;
        } else if(womensDeadlock) {
            return WOMENS_DEADLOCK;
        } else if(coupleDeadlock) {
            return COUPLE_DEADLOCK;
        }
    }

    return  0;
}

void Oven::useOven() {
    ovenInUse = true;
}

void Oven::freeOven() {
    ovenInUse = false;
}

bool Oven::isOvenInUse() {
    return ovenInUse;
}

void Oven::characterWait(pthread_cond_t* condVar) {
    if(pthread_cond_wait(condVar, &mutex) != 0) {
        perror("Error on condition variable wait");
        exit(EXIT_FAILURE); 
    }
}

void Oven::characterSignal(pthread_cond_t* condVar) {
    if(pthread_cond_signal(condVar) != 0) {
        perror("Error on condition variable signal");
        exit(EXIT_FAILURE); 
    }
}

void Oven::characterBroadcast(pthread_cond_t* condVar) {
    if(pthread_cond_broadcast(condVar) != 0) {
        perror("Error on condition variable broadcast");
        exit(EXIT_FAILURE); 
    }
}

void Oven::initMutex(pthread_mutex_t* mutex) {
    if(pthread_mutex_init(mutex, NULL) != 0) {
        perror("Error on initializing mutex");
        exit(EXIT_FAILURE); 
    }
}

void Oven::initCondVar(pthread_cond_t* condVar) {
    if(pthread_cond_init(condVar, NULL) != 0) {
        perror("Error on initializing condition variable");
        exit(EXIT_FAILURE); 
    }
}

void Oven::destroyMutex(pthread_mutex_t* mutex) {
    if(pthread_mutex_destroy(mutex) != 0) {
        perror("Error on destroying mutex");
        exit(EXIT_FAILURE); 
    }
}

void Oven::destroyCondVar(pthread_cond_t* condVar) {
    if(pthread_cond_destroy(condVar) != 0) {
        perror("Error on destroying condition variable");
        exit(EXIT_FAILURE); 
    }
}