#include "character.hpp"

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> distribution(3, 6); // uniform, unbiased

string Character::getName() {
    return this->name;
}

/* bool queueContains(vector<Character*> queue, string name) {
    for (auto u : queue) {
        if (u->Name() == name) {
            return true;
        }
    }
    return false;
} */

/* bool Character::CanUse(vector<Character*> queue) {
    if (this->name == "Sheldon") {
        if (queueContains(queue, "Amy")) {
            if (!queueContains(queue, "Leonard") || !queueContains(queue, "Penny")) {
                return true;
            }
            return false;
        }
        if (queueContains(queue, "Leonard") || queueContains(queue, "Penny")) {
            return false;
        }
        return true;
    } else if (this->name == "Amy") {
        if (queueContains(queue, "Sheldon")) {
            if (!queueContains(queue, "Leonard") || !queueContains(queue, "Penny")) {
                return true;
            }
            return false;
        }
        if (queueContains(queue, "Leonard") || queueContains(queue, "Penny")) {
            return false;
        }
        return true;    
    } else if (this->name == "Howard") {
        if (queueContains(queue, "Bernardette")) {
            if (!queueContains(queue, "Sheldon") || !queueContains(queue, "Amy")) {
                return true;
            }
            return false;
        }
        if (queueContains(queue, "Sheldon") || queueContains(queue, "Amy")) {
            return false;
        }
        return true;
    } else if (this->name == "Bernardette") {
        if (queueContains(queue, "Howard")) {
            if (!queueContains(queue, "Sheldon") || !queueContains(queue, "Amy")) {
                return true;
            }
            return false;
        }
        if (queueContains(queue, "Sheldon") || queueContains(queue, "Amy")) {
            return false;
        }
        return true;
    } else if (this->name == "Leonard") {
        if (queueContains(queue, "Penny")) {
            if (!queueContains(queue, "Howard") || !queueContains(queue, "Bernardette")) {
                return true;
            }
            return false;
        }
        if (queueContains(queue, "Howard") || queueContains(queue, "Bernardette")) {
            return false;
        }
        return true;
    } else if (this->name == "Penny") {
        if (queueContains(queue, "Leonard")) {
            if (!queueContains(queue, "Howard") || !queueContains(queue, "Bernardette")) {
                return true;
            }
            return false;
        }
        if (queueContains(queue, "Howard") || queueContains(queue, "Bernardette")) {
            return false;
        }
        return true;
    } else if (this->name == "Stuart") {
        if (queue.empty()) {
            return true;
        }
        if (queue.size() == 1 && queueContains(queue, "Kripke")) {
            return true;
        }
        return false;
    } else if (this->name == "Kripke") {
        if (queue.empty()) {
            return true;
        }
        return false;
    } else {
        return false;
    }
} */

void Character::useOven() {
    cout << this->name << " comeca a esquentar algo" << endl;
    sleep(1);
}

void Character::eat() {
    cout << this->name << " vai comer" << endl;
    sleep(distribution(gen));
}

void Character::work() {
    cout << this->name << " voltou para o trabalho" << endl;
    sleep(distribution(gen));
}

