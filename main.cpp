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
string charactersName[NUMBER_CHARACTERS] = {SHELDON, AMY, LEONARD, PENNY};

Oven oven;
int amountOvenUse;

void validateParameters(int argc);
void initializeCharacters(Character* characters[]);
void shuffleCharacters(Character* characters[]);
void startCharacterThread(Character* character);
void* characterRoutine(void* parameters);
void joinCharacterThread(Character* character);
void joinRajThread(pthread_t* raj);


int main(int argc, char *argv[]) {

    validateParameters(argc);
    amountOvenUse = atoi(argv[1]);

    Character* characters[NUMBER_CHARACTERS];
    initializeCharacters(characters);

    shuffleCharacters(characters);

    for(Character* c : characters) {
        startCharacterThread(c);
    }

    for (Character* c : characters) {
        joinCharacterThread(c);
    }

    joinRajThread(&oven.raj);

    return 0;
}

void validateParameters(int argc) {
    if (argc != 2) {
        cout << "\n Inform exclusively the number of times the oven will be used!" << endl;
        exit(EXIT_FAILURE);
    }
}

void initializeCharacters(Character* characters[]) {
    for(int i=0; i<NUMBER_CHARACTERS; i++) {
        characters[i] = new Character(charactersName[i]);
    }
}

void shuffleCharacters(Character* characters[]) {
    random_device rd;
    shuffle(characters, characters+NUMBER_CHARACTERS, default_random_engine(rd()));
}



void startCharacterThread(Character* character) {
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

void joinCharacterThread(Character* character) {
    if(pthread_join(character->thread, NULL) !=0) {
        perror("Error on joining thread");
        exit(EXIT_FAILURE);
    }
}

void joinRajThread(pthread_t* raj) {
    if(pthread_join(*raj, NULL) != 0) {
        perror("Error on joining thread");
        exit(EXIT_FAILURE);
    }
}