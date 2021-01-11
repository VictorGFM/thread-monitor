#ifndef USER_H
#define USER_H
#endif

#include <string>
#include <thread>
#include <vector>

using std::string;
using std::thread;
using std::vector;

class User {

    public:
        User(string name, int ovenTimes) {
            this->name = name;
            this->ovenTimes = ovenTimes;
            // this->t = t;
        };

        string Name();
        int OvenTimes();
        // void Wait(void (*f) (User*));
        bool CanUse(vector<User*> queue);
        void UseOven();
        void Work();
        thread t;

    private:
        string name;
        int ovenTimes;
};