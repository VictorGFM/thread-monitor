#include "oven.hpp"

set<string> queue;

void printQueue(set<string> queue) {
    cout << "FILA: ";
    for(string character : queue) {
        cout << character << " ";
    }
    cout << endl;
}

bool queueContains(string name) {
    return queue.find(name) != queue.end();
}

bool queueContainsCouple() {
    return ((queueContains(SHELDON) && queueContains(AMY))
            || (queueContains(HOWARD) && queueContains(BERNARDETTE))
            || (queueContains(LEONARD) && queueContains(PENNY)));
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

void Oven::characterWait(pthread_cond_t* varCond) {
    if(pthread_cond_wait(varCond, &this->mutex) != 0) {
        perror("Error on condition variable signal");
        exit(EXIT_FAILURE); 
    }
}

void Oven::wait(Character* character) {

    pthread_mutex_lock(&this->mutex);

    cout << character->getName() << " quer usar o forno" << endl;
    queue.insert(character->getName());
    printQueue(queue);

    pthread_mutex_unlock(&this->mutex);


    while(true) {
        pthread_mutex_lock(&this->mutex);

        if(character->getName() == SHELDON || character->getName() == AMY) {
            if(isCoupleInQueue(character->getName())) { 
                if(queueContains(LEONARD) && queueContains(PENNY)) {
                    characterWait(&leonardAndPennyInQueue);
                }
                //characterWait(&coupleSheldonAmyInQueue);
            } else {
                if(queueContains(HOWARD) && queueContains(BERNARDETTE)) {
                    characterWait(&howardAndBernardetteInQueue);
                } 
                if(queueContains(LEONARD) || queueContains(PENNY))  {
                    characterWait(&leonardOrPennyInQueue);
                } 
            }
        } else if(character->getName() == HOWARD || character->getName() == BERNARDETTE) {
            if(isCoupleInQueue(character->getName())) {
                if(queueContains(SHELDON) && queueContains(AMY)) {
                    characterWait(&sheldonAndAmyInQueue);
                }
            } else {
                if(queueContains(LEONARD) && queueContains(PENNY)) {
                    characterWait(&leonardAndPennyInQueue);
                }
                if(queueContains(SHELDON) || queueContains(AMY))  {
                    characterWait(&sheldonOrAmyInQueue);
                }
            }
        } else if(character->getName() == LEONARD || character->getName() == PENNY) {
            if(isCoupleInQueue(character->getName())) {
                if(queueContains(HOWARD) && queueContains(BERNARDETTE)) {
                    characterWait(&howardAndBernardetteInQueue);
                }
                //couple rule
            } else {
                if(queueContains(SHELDON) && queueContains(AMY)) {
                    characterWait(&sheldonAndAmyInQueue);
                }
                if(queueContains(HOWARD) || queueContains(BERNARDETTE))  {
                    characterWait(&howardOrBernardetteInQueue);
                }
            }
        } 
        if(!this->ovenInUse) {
            this->ovenInUse = true;
            pthread_mutex_unlock(&this->mutex);
            break;
        }
    
        pthread_mutex_unlock(&this->mutex);
    }
}

void Oven::free(Character* character) {
    pthread_mutex_lock(&this->mutex);

    this->ovenInUse = false;
    
    queue.erase(character->getName());
    cout << character->getName() << " vai comer" << endl;
    
    if(character->getName() == SHELDON || character->getName() == AMY) {
        pthread_cond_signal(&sheldonAndAmyInQueue);
        if(!isCoupleInQueue(character->getName())) {
             pthread_cond_signal(&sheldonOrAmyInQueue);
        }
    } else if(character->getName() == HOWARD || character->getName() == BERNARDETTE) {
        pthread_cond_signal(&howardAndBernardetteInQueue);
        if(!isCoupleInQueue(character->getName())) { // && !queueContainsCouple()
             pthread_cond_signal(&howardOrBernardetteInQueue);
        }
    } else if(character->getName() == LEONARD || character->getName() == PENNY) {
        pthread_cond_signal(&leonardAndPennyInQueue);
        if(!isCoupleInQueue(character->getName())) {
             pthread_cond_signal(&leonardOrPennyInQueue);
        }
    }

    pthread_mutex_unlock(&this->mutex);
}

/* void* Oven::RajStart(void* args) {
    Oven* object = (Oven*) args;
    int noUseTime = 0;
    while (1) {
        if (object->inUse) {
            sleep(5);
            continue;
        }
        if (object->characterQueue.empty()) {
            sleep(5);
            noUseTime+=5;
            if (noUseTime > 5) {
                break;
            }
            continue;
        }
        object->Use(object->characterQueue.front());
    }
    return 0;
} */