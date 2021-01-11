#include "user.hpp"

string User::Name() {
    return this->name;
}

int User::OvenTimes() {
    return this->ovenTimes;
}

// void User::Wait(void (*f) (User*)) {
//     this->t = thread(f);
//     printf("\n%s quer usar o forno", this->name);
// }

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
    }
}

void User::UseOven() {
    printf("\n%s comeca a esquentar algo", this->name);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    this->ovenTimes--;
    printf("\n%s vai comer", this->name);
}

void User::Work() {
    printf("\n%s voltou para o trabalho", this->name);
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

