//
// Created by Alon on 15/06/2019.
//

#ifndef PART1_SUBJECT_H
#define PART1_SUBJECT_H

#include "OOP5EventException.h"
#include "Observer.h"
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
class Subject {
    // init vars here
    vector<Observer<T> *> observers;
public:
    void notify(T const &t) {
        for (Observer<T> *&o : observers) {
            o->handleEvent(t);
        }
    }

    void addObserver(Observer<T> &o) {
        typename std::vector<Observer<T> *>::iterator found;
        found = find(observers.begin(), observers.end(), &o);
        if (found != observers.end())
            throw ObserverAlreadyKnownToSubject();
        observers.push_back(&o);
    }

    void removeObserver(Observer<T> &o) {
        typename std::vector<Observer<T> *>::iterator found;
        found = find(observers.begin(), observers.end(), &o);
        if (found == observers.end())
            throw ObserverUnknownToSubject();
        observers.erase(found);
    }

    Subject<T> &operator+=(Observer<T> &o) {
        this->addObserver(o);
        return *this;
    }

    Subject<T> &operator-=(Observer<T> &o) {
        this->removeObserver(o);
        return *this;
    }

    Subject<T> &operator()(T const &t) {
        this->notify(t);
        return *this;
    }
};


#endif //PART1_SUBJECT_H
