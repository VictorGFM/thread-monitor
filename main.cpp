#include "constants.hpp"
#include "oven.hpp"

#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <random>


using std::cout;
using std::endl;
using std::default_random_engine;
using std::shuffle;

//string charactersName[NUMBER_CHARACTERS] = {SHELDON, AMY, HOWARD, BERNARDETTE, LEONARD, PENNY, STUART, KRIPKE};
//string charactersName[NUMBER_CHARACTERS] = {SHELDON, AMY, HOWARD, BERNARDETTE};
string charactersName[NUMBER_CHARACTERS] = {SHELDON};
Oven oven;
int amountOvenUse;

void startThread(Character* character);
void* characterRoutine(void* parameters);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "\n Inform exclusively the number of times the oven will be used!" << endl;
        return 0;
    }

    amountOvenUse = atoi(argv[1]);

    Character *characters[NUMBER_CHARACTERS];
    for(int i=0; i<NUMBER_CHARACTERS; i++) {
        characters[i] = new Character(charactersName[i]);
    }

    random_device rd;
    shuffle(characters, characters+NUMBER_CHARACTERS, default_random_engine(rd()));

    for(Character* c : characters) {
        startThread(c);
    }

    for (Character* c : characters) {
       if(pthread_join(c->thread, NULL) !=0) {
           perror("Error on joining thread");
           exit(EXIT_FAILURE);
       }
    }
    /* if(pthread_join(oven.raj, NULL) != 0) {
        perror("Error on joining thread");
        exit(EXIT_FAILURE);
    } */

    return 0;
}

void startThread(Character* character) {
    if(pthread_create(&(character->thread), NULL, characterRoutine, (void*) character) != 0) {
        perror("Error on creating thread");
        exit(EXIT_FAILURE);
    }
}

void* characterRoutine(void* parameters) {
    Character* character = (Character*) parameters;
    for(int i=0; i<amountOvenUse; i++) {
        oven.wait(character);
        character->useOven();
        oven.free(character);
        character->eat();
        character->work();
    }
    return 0;
}