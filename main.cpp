#include "oven.hpp"

#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <random>
#include <map>

using std::cout;
using std::endl;
using std::default_random_engine;
using std::shuffle;
using std::map;

string charactersName[NUMBER_CHARACTERS] = {SHELDON, AMY, HOWARD, BERNARDETTE, LEONARD, PENNY, STUART, KRIPKE};

Oven oven;
int amountOvenUse;
bool allCharactersFinishedUsingOven;

void validateParameters(int argc);
void initializeCharacters(Character* characters[]);
void shuffleCharacters(Character* characters[]);
void startRajThread(pthread_t* raj);
void* RajRoutine(void* args);
void startCharacterThread(Character* character);
void* characterRoutine(void* parameters);
void joinCharacterThread(pthread_t thread);


int main(int argc, char *argv[]) {
    pthread_t raj;

    validateParameters(argc);
    amountOvenUse = atoi(argv[1]);

    Character* characters[NUMBER_CHARACTERS];
    initializeCharacters(characters);

    shuffleCharacters(characters);

    allCharactersFinishedUsingOven = false;

    startRajThread(&raj);

    for(Character* c : characters) {
        startCharacterThread(c);
        sleep(1);
    }

    for (Character* c : characters) {
        joinCharacterThread(c->thread);
    }

    allCharactersFinishedUsingOven = true;

    joinCharacterThread(raj);

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

void startRajThread(pthread_t* raj) {
    if(pthread_create(raj, NULL, RajRoutine, NULL) < 0) {
        cout << "Error on creating thread!" << endl;
        exit(EXIT_FAILURE);
    }
}

void* RajRoutine(void* args) {
    while(!allCharactersFinishedUsingOven) {
        sleep(5);
        oven.verify();
    }
    return 0;
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

void joinCharacterThread(pthread_t thread) {
    if(pthread_join(thread, NULL) !=0) {
        perror("Error on joining thread");
        exit(EXIT_FAILURE);
    }
}