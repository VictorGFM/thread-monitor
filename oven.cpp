#include "oven.hpp"

void Oven::wait(Character* character) {

    pthread_mutex_lock(&this->mutex);

    cout << "‍🙋  ‍" << character->getName() << " quer usar o forno" << endl;
    queue.push_back(character->getName());

    bool pairCalled = false;
    if(isPairInQueue(character->getName())) {
        if(character->getName() == SHELDON || character->getName() == AMY) {
            characterWait(&this->coupleCallSheldonAmy);
        } else if(character->getName() == HOWARD || character->getName() == BERNARDETTE) {
            characterWait(&this->coupleCallHowardBernardette);
        } else if(character->getName() == LEONARD || character->getName() == PENNY) {
            characterWait(&this->coupleCallLeonardPenny);
        }
        pairCalled = true;
    }
    while(!havePriorityToUseOven(character, &pairCalled)) {
        characterWait(&this->priorityToUseOven);
    }

    useOven();

    pthread_mutex_unlock(&this->mutex);
}

bool Oven::havePriorityToUseOven(Character* character, bool* pairCalled) {
    if(isQueueInDeadlock()) {
        if(character->getName() == releasedCharacterDeadlock) {
            cout << "💀  Raj detectou um deadlock, liberando " << releasedCharacterDeadlock << endl;
            if(isQueueInDeadlock() == COUPLE_DEADLOCK) {
                coupleDeadlockOccurred = true;
            } else {
                releasedCharacterDeadlock = "";
            }
            return true;
        } else {
            return false;
        }
    } else if(coupleDeadlockOccurred) {
        if(character->getName() == getPairName(releasedCharacterDeadlock)) {
            return true;
        } else {
            return false;
        }
    } else if(character->getName() == SHELDON || character->getName() == AMY) {
        if(queueContains(LEONARD) && queueContains(PENNY)) {
            if(*pairCalled) {
                *pairCalled = false;
            }
            return false;
        } else if(!isPairInQueue(character->getName()) && !(*pairCalled)) {
            if(queueContains(HOWARD) && queueContains(BERNARDETTE)) {
                return false;
            } else if(queueContains(LEONARD) || queueContains(PENNY)) {
                return false;
            } else if(isOvenInUse()) {
                return false;
            }
        }
        return true;
    } else if(character->getName() == HOWARD || character->getName() == BERNARDETTE) {
        if(queueContains(SHELDON) && queueContains(AMY)) {
            if(*pairCalled) {
                *pairCalled = false;
            }
            return false;
        } else if(!isPairInQueue(character->getName()) && !(*pairCalled)) {
            if(queueContains(LEONARD) && queueContains(PENNY)) {
                return false;
            } else if(queueContains(SHELDON) || queueContains(AMY)) {
                return false;
            } else if(isOvenInUse()) {
                return false;
            }
        }
        return true;
    }  else if(character->getName() == LEONARD || character->getName() == PENNY) {
        if(queueContains(HOWARD) && queueContains(BERNARDETTE)) {
            if(*pairCalled) {
                *pairCalled = false;
            }
            return false;
        } else if(!isPairInQueue(character->getName()) && !(*pairCalled)) {
            if(queueContains(SHELDON) && queueContains(PENNY)) {
                return false;
            } else if(queueContains(HOWARD) || queueContains(BERNARDETTE)) {
                return false;
            } else if(isOvenInUse()) {
                return false;
            }
        }
        return true;
    } else if(character->getName() == STUART) {
        if(queueContains(SHELDON) || queueContains(AMY)
            || queueContains(HOWARD) || queueContains(BERNARDETTE)
            || queueContains(LEONARD) || queueContains(PENNY)) {
            return false;
        } else if(isOvenInUse()) {
            return false;
        }
        return true;
    } else  if(character->getName() == KRIPKE) {
        if(queueContains(SHELDON) || queueContains(AMY)
            || queueContains(HOWARD) || queueContains(BERNARDETTE)
            || queueContains(LEONARD) || queueContains(PENNY)
            || queueContains(STUART)) {
            return false;
        } else if(isOvenInUse()) {
            return false;
        }
        return true;
    } else {
        return false;
    }
}

void Oven::free(Character* character) {
    pthread_mutex_lock(&this->mutex);

    cout << "🍴  " << character->getName() << " vai comer" << endl;
    freeOven();
    queue.remove(character->getName());

    if(coupleDeadlockOccurred) {
        if(character->getName() == getPairName(releasedCharacterDeadlock)) {
            coupleDeadlockOccurred = false;
            releasedCharacterDeadlock = "";
        }
    }

    if(character->getName() == SHELDON || character->getName() == AMY) {
        if(isPairInQueue(character->getName())) {
            characterSignal(&this->coupleCallSheldonAmy);
            if(queueContains(LEONARD) && queueContains(PENNY)) {
                characterBroadcast(&this->priorityToUseOven);
            }
        } else {
            characterBroadcast(&this->priorityToUseOven);
        }
    } else if(character->getName() == HOWARD || character->getName() == BERNARDETTE) {
        if(isPairInQueue(character->getName())) {
            characterSignal(&this->coupleCallHowardBernardette);
            if(queueContains(SHELDON) && queueContains(AMY)) {
                characterBroadcast(&this->priorityToUseOven);
            }
        } else {
            characterBroadcast(&this->priorityToUseOven);
        }
    } else if(character->getName() == LEONARD || character->getName() == PENNY) {
        if(isPairInQueue(character->getName())) {
            characterSignal(&this->coupleCallLeonardPenny);
            if(queueContains(HOWARD) && queueContains(BERNARDETTE)) {
                characterBroadcast(&this->priorityToUseOven);
            }
        } else {
            characterBroadcast(&this->priorityToUseOven);
        }
    } else if(character->getName() == STUART || character->getName() == KRIPKE) {
        characterBroadcast(&this->priorityToUseOven);
    }
    
    pthread_mutex_unlock(&this->mutex);
}

void Oven::verify() {
    pthread_mutex_lock(&this->mutex);

    random_device rd;
    mt19937 gen(rd());

    switch(isQueueInDeadlock()) {
        case MENS_DEADLOCK: {
            string charactersName[3] = {SHELDON, HOWARD, LEONARD};
            uniform_int_distribution<int> distribution(0, 2);
            releasedCharacterDeadlock = charactersName[distribution(gen)];
            characterBroadcast(&this->priorityToUseOven);
            break;
        } case WOMENS_DEADLOCK: {
            string charactersName[3] = {AMY, BERNARDETTE, PENNY};
            uniform_int_distribution<int> distribution(0, 2);
            releasedCharacterDeadlock = charactersName[distribution(gen)];
            characterBroadcast(&this->priorityToUseOven);
            break;
        } case COUPLE_DEADLOCK: {
            vector<string> charactersName;
            findFirstOfCouples(&charactersName);
            uniform_int_distribution<int> distribution(0, 2);
            releasedCharacterDeadlock = charactersName[distribution(gen)];
            characterBroadcast(&this->priorityToUseOven);
            break;
        } default:
            break;
    }
    
    pthread_mutex_unlock(&this->mutex);
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
    this->ovenInUse = true;
}

void Oven::freeOven() {
    this->ovenInUse = false;
}

bool Oven::isOvenInUse() {
    return this->ovenInUse;
}

void Oven::characterWait(pthread_cond_t* varCond) {
    if(pthread_cond_wait(varCond, &this->mutex) != 0) {
        perror("Error on condition variable wait");
        exit(EXIT_FAILURE); 
    }
}

void Oven::characterSignal(pthread_cond_t* varCond) {
    if(pthread_cond_signal(varCond) != 0) {
        perror("Error on condition variable signal");
        exit(EXIT_FAILURE); 
    }
}

void Oven::characterBroadcast(pthread_cond_t* varCond) {
    if(pthread_cond_broadcast(varCond) != 0) {
        perror("Error on condition variable broadcast");
        exit(EXIT_FAILURE); 
    }
}