#include "character.hpp"

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> distribution(3, 6);

void Character::useOven() {
    cout << "🔥  " << name << " comeca a esquentar algo" << endl;
    sleep(1);
}

void Character::eat() {
    sleep(distribution(gen));
}

void Character::work() {
    cout << "💼  " << name << " voltou para o trabalho" << endl;
    sleep(distribution(gen));
}

string Character::getName() {
    return name;
}