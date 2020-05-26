#include <iostream>
#include <queue>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <ctime>

using namespace std;

class Computer {

private:

    bool status;
    bool power;
    string brokenPart;
public:
    void setBrokenPart(const string &brokenPart) {
        Computer::brokenPart = brokenPart;
    }

    bool isStatus() const {
        return status;
    }

    bool isPower() const {
        return power;
    }

    string getBrokenPart() {
        return brokenPart;
    }

    void changeStatus() {
        if (status)
            status = false;
        else
            status = true;
    }

    void changePower() {
        if (power)
            power = false;
        else
            power = true;
    }

    Computer() {
        status = true;
        power = true;
    }
};

class User {
private:
    string name;
public:
    void setName(const string &name) {
        User::name = name;
    }

private:
    Computer *pc;
    time_t readyWhen;
public:
    int getWaitingTime() const {
        return readyWhen;
    }

    time_t getDate() {
        return readyWhen;
    }

    void setWaitingTime(int waitingTime) {
        this->readyWhen = waitingTime;
    }


    Computer *getPc() {
        return pc;
    }

    string getName() {
        return name;
    }

    User(string name) : name(name), pc(new Computer()) {}

    User() : pc(new Computer()), readyWhen(100) {}

    ~User() {
        delete pc;
    }
};


class Service {
private:
    double account;
    time_t last;
public:
    double getAccount() const {
        return account;
    }
    Service() : account(0), last(100) {}

    void addUser(User &obj) {
        time_t curr = time(nullptr);
        if (!obj.getPc()->isStatus()) {
            string tmp = obj.getPc()->getBrokenPart();
            time_t curr = time(nullptr);
            if (last <= curr)
                last = curr;
            if (tmp == "Gpu") {
                account = account + 300;
                obj.setWaitingTime((int) last + 20);
                last = last + 20;
            } else if (tmp == "Cpu") {
                account = account + 270;
                obj.setWaitingTime((int) last + 15);
                last = last + 15;
            } else if (tmp == "Disk") {
                account = account + 100;
                obj.setWaitingTime((int) last + 5);
                last = last + 5;
            } else if (tmp == "Psu") {
                account = account + 170;
                obj.setWaitingTime((int) last + 10);
                last = last + 10;
            } else if (!obj.getPc()->isPower()) {
                account = account + 50;
                obj.setWaitingTime((int) last + 2);
                last = last + 2;
            }
        }
    }

    void repair(User &obj) {
        string tmp = obj.getPc()->getBrokenPart();
        if (obj.getPc()->getBrokenPart().empty()) {
            obj.getPc()->changePower();
            obj.getPc()->changeStatus();
            obj.setWaitingTime(100);
        } else {
            obj.getPc()->setBrokenPart("");
            obj.getPc()->changeStatus();
            obj.setWaitingTime(100);
        }
    }
};


int main() {
    User users[40];
    for (int i = 0; i < 40; i++) {
        users[i].setName("USER" + to_string(i + 1));
    }
    char choice;
    Service service;

    while (true) {
        cout << "Menu aplikacji:" << endl;
        cout << "1- Generate random user" << endl;
        cout << "2- Show list with computers waiting to be repaired" << endl;
        cout << "3- Show account status" << endl;
        cout << "4- Exit" << endl;
        cin >> choice;
        if (choice == '1') {
            srand(time(NULL));
            int randomNumber = rand() % 40;
            User *tmp = &users[randomNumber];
            if (tmp->getPc()->isStatus()) {
                randomNumber = rand() % 100 + 1;
                cout << randomNumber << endl;
                if (randomNumber >= 0 && randomNumber <= 15) {  // Gpu is getting broken
                    tmp->getPc()->setBrokenPart("Gpu");
                    tmp->getPc()->changeStatus();
                } else if (randomNumber > 15 && randomNumber <= 45) {   // Disk is getting broken
                    tmp->getPc()->changeStatus();
                    tmp->getPc()->setBrokenPart("Disk");
                } else if (randomNumber > 45 && randomNumber <= 55) {   // Psu is getting broken
                    tmp->getPc()->changeStatus();
                    tmp->getPc()->setBrokenPart("Psu");
                } else if (randomNumber > 55 && randomNumber <= 67) {   // Processor is getting broken
                    tmp->getPc()->changeStatus();
                    tmp->getPc()->setBrokenPart("Cpu");
                } else {
                    tmp->getPc()->changeStatus();
                    tmp->getPc()->changePower();
                }

                if (!tmp->getPc()->getBrokenPart().empty())
                    cout << tmp->getName() << " Has broken: " << tmp->getPc()->getBrokenPart() << endl;
                else if (!tmp->getPc()->isPower())
                    cout << tmp->getName() << "Computer unplugged from electricity" << endl;

                service.addUser(*tmp);
            }
            time_t test = time(nullptr);
            for (int i = 0; i < 40; i++) {
                if (users[i].getWaitingTime() < test && !users[i].getPc()->isStatus()) {
                    service.repair(users[i]);
                }
            }
            continue;

        } else if (choice == '2') {
            time_t test = time(nullptr);
            for (int i = 0; i < 40; i++) {
                if (users[i].getWaitingTime() < test && !users[i].getPc()->isStatus()) {
                    service.repair(users[i]);
                }
            }
            for (int i = 0; i < 40; i++) {
                if (!users[i].getPc()->isStatus()) {
                    time_t ret = users[i].getDate();
                    time_t *time = &ret;
                    if (!users[i].getPc()->getBrokenPart().empty())
                        cout << users[i].getName() << " Broken Part: " << users[i].getPc()->getBrokenPart()
                             << " Repaired at: " << ctime(time) << endl;
                    else if (!users[i].getPc()->isPower())
                        cout << users[i].getName() << " Computer unplugged from electricity  Repaired at: "
                             << ctime(time) << endl;

                }
            }
            time_t ret = time(nullptr);
            time_t *time = &ret;
            cout << "Current time: " << ctime(time)<<endl;
        }else if(choice == '3'){
            cout << "Service account status: " << service.getAccount() << endl;
        }
        else if (choice == '4') {
            return 0;
        }
    }
}
