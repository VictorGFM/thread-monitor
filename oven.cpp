#include "oven.hpp"


void Oven::wait(Character* character) {
    cout << character->getName() << " quer usar o forno" << endl;
    this->queue.insert(character->getName());

    pthread_mutex_lock(&mutex);

    if(character->getName() == SHELDON || character->getName() == AMY) {
        cout << character->getName() << " Pronto para usar o forno" << endl;
        if(queue.find(SHELDON) != queue.end()) { //modificar condicional
            if(queue.find(LEONARD) != queue.end() && queue.find(PENNY) != queue.end()) {
                //dorme
            }
        } else if(existe algum casal na fila?) {
            //dorme
        } else if(queue.find(LEONARD) != queue.end() || queue.find(PENNY) != queue.end())  {
            //dorme
        }
    } else if(character->getName() == HOWARD || character->getName() == BERNARDETTE) {
        if(queue.find(BERNARDETTE) != queue.end())  { //modificar condicional
            if(queue.find(SHELDON) != queue.end() && queue.find(AMY) != queue.end()) {
                cout << character->getName() << " Esperando SHELDON ou AMY terminarem de usar o forno" << endl;
                if(pthread_cond_wait(&sheldonOrAmyInQueue, &mutex) != 0) {
                    perror("Error on condition variable wait");
                    exit(EXIT_FAILURE); 
                }
                cout << character->getName() << " Pronto para usar o forno" << endl;
            } else {
                if(pthread_cond_signal(&sheldonOrAmyInQueue) != 0) {
                    perror("Error on condition variable signal");
                    exit(EXIT_FAILURE); 
                }
                if(pthread_cond_wait(&howardAndBernardetteInQueue, &mutex) != 0) {
                    perror("Error on condition variable wait");
                    exit(EXIT_FAILURE); 
                }
            }
        } else if(queue.find(SHELDON) != queue.end() || queue.find(AMY) != queue.end()) {
            cout << character->getName() << " Esperando SHELDON ou AMY terminarem de usar o forno" << endl;
            if(pthread_cond_wait(&sheldonOrAmyInQueue, &mutex) != 0) {
                perror("Error on condition variable wait");
                exit(EXIT_FAILURE); 
            }
            cout << character->getName() << " Pronto para usar o forno" << endl;
        }
    }

    
}

void Oven::free(Character* character) {
    queue.erase(character->getName());
    cout << character->getName() << " Terminou de usar o forno" << endl;
    pthread_mutex_unlock(&mutex);

    if(character->getName() == SHELDON || character->getName() == AMY)  {
        if(queue.find(SHELDON) == queue.end() && queue.find(AMY) == queue.end())  {
            if(pthread_cond_signal(&sheldonOrAmyInQueue) != 0) {
                perror("Error on condition variable signal");
                exit(EXIT_FAILURE); 
            }
            cout<<"Liberou a variavel condicional"<<endl;
        }
    } else if(character->getName() == HOWARD || character->getName() == BERNARDETTE) {
        if(pthread_cond_signal(&howardAndBernardetteInQueue) != 0) {
            perror("Error on condition variable signal");
            exit(EXIT_FAILURE); 
        }
        if(queue.find(LEONARD) == queue.end() && queue.find(PENNY) == queue.end())  {
            if(pthread_cond_signal(&sheldonOrAmyInQueue) != 0) {
                perror("Error on condition variable signal");
                exit(EXIT_FAILURE); 
            }
            cout<<"Liberou a variavel condicional"<<endl;
        }
    }
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