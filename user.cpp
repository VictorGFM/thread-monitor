#include "user.hpp"

string User::Name() {
    return this->name;
}

int User::OvenTimes() {
    return this->ovenTimes;
}

bool queueContains(vector<User*> queue, string name) {
    for (auto u : queue) {
        if (u->Name() == name) {
            return true;
        }
    }
    return false;
}

bool User::CanUse(vector<User*> queue) {
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
}

void User::UseOven() {
    cout << endl << this->name << " comeca a esquentar algo";
    sleep(1);
    this->ovenTimes--;
    cout << endl << this->name << " vai comer";
}

void User::Work() {
     cout << endl << this->name << " voltou para o trabalho";
    sleep(5);
}

