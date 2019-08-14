//
// Created by Alon on 15/06/2019.
//

#ifndef PART1_OBSERVER_H
#define PART1_OBSERVER_H

template <typename T>
class Observer {
public:
    virtual void handleEvent(const T&) = 0;
};


#endif //PART1_OBSERVER_H
