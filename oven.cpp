#include "oven.hpp"

set<string> queue;

void printQueue(set<string> queue) {
    cout << "--------FILA: ";
    for(string character : queue) {
        cout << character << " ";
    }
    cout << endl;
}

bool queueContains(string name) {
    return queue.find(name) != queue.end();
}

bool isCoupleInQueue(string name) {
    if(name == SHELDON) {
        return queueContains(AMY);
    } else if(name == AMY) {
        return queueContains(SHELDON);
    } else if(name == HOWARD) {
        return queueContains(BERNARDETTE);
    } else if(name == BERNARDETTE) {
        return queueContains(HOWARD);
    } else if(name == LEONARD) {
        return queueContains(PENNY);
    } else if(name == PENNY) {
        return queueContains(LEONARD);
    } else {
        return false;
    }
}

void Oven::wait(Character* character) {

    pthread_mutex_lock(&this->mutex);

    cout << "‍🙋  ‍" << character->getName() << " quer usar o forno" << endl;
    queue.insert(character->getName());

    bool pairCalled = false;
    if(isCoupleInQueue(character->getName())) {
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

    //printQueue(queue);
    useOven();

    pthread_mutex_unlock(&this->mutex);
}

bool Oven::havePriorityToUseOven(Character* character, bool* pairCalled) {
    if(character->getName() == SHELDON || character->getName() == AMY) {
        if(queueContains(LEONARD) && queueContains(PENNY)) {
            if(*pairCalled) {
                *pairCalled = false;
            }
            return false;
        } else if(!isCoupleInQueue(character->getName()) && !(*pairCalled)) {
            if(*pairCalled) {
                *pairCalled = false;
            }
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
        } else if(!isCoupleInQueue(character->getName()) && !(*pairCalled)) {
            if(*pairCalled) {
                *pairCalled = false;
            }
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
        } else if(!isCoupleInQueue(character->getName()) && !(*pairCalled)) {
            if(*pairCalled) {
                *pairCalled = false;
            }
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
    }
    return false;
}

void Oven::free(Character* character) {
    pthread_mutex_lock(&this->mutex);

    cout << "🍴  " << character->getName() << " vai comer" << endl;
    freeOven();
    queue.erase(character->getName());

    if(character->getName() == SHELDON || character->getName() == AMY) {
        if(isCoupleInQueue(character->getName())) {
            pthread_cond_signal(&this->coupleCallSheldonAmy);
            if(queueContains(LEONARD) && queueContains(PENNY)) {
                pthread_cond_broadcast(&this->priorityToUseOven);
            }
        } else {
            pthread_cond_broadcast(&this->priorityToUseOven);
        }
    } else if(character->getName() == HOWARD || character->getName() == BERNARDETTE) {
        if(isCoupleInQueue(character->getName())) {
            pthread_cond_signal(&this->coupleCallHowardBernardette);
            if(queueContains(SHELDON) && queueContains(AMY)) {
                pthread_cond_broadcast(&this->priorityToUseOven);
            }
        } else {
            pthread_cond_broadcast(&this->priorityToUseOven);
        }
    } else if(character->getName() == LEONARD || character->getName() == PENNY) {
        if(isCoupleInQueue(character->getName())) {
            pthread_cond_signal(&this->coupleCallLeonardPenny);
            if(queueContains(HOWARD) && queueContains(BERNARDETTE)) {
                pthread_cond_broadcast(&this->priorityToUseOven);
            }
        } else {
            pthread_cond_broadcast(&this->priorityToUseOven);
        }
    } else if(character->getName() == STUART || character->getName() == KRIPKE) {
        pthread_cond_broadcast(&this->priorityToUseOven);
    }
    
    pthread_mutex_unlock(&this->mutex);
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
        perror("Error on condition variable signal");
        exit(EXIT_FAILURE); 
    }
}

void* Oven::RajStart(void* args) {
    Oven* object = (Oven*) args;
    while(true) {
    }
    return 0;
}