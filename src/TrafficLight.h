#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;


// FP.3 Define a class „MessageQueue“ which has the public methods send and receive (1). 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type (2). 
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase (3). 
// Also, there should be an std::condition_variable as well as an std::mutex as private members (4). 

template <class T>
class MessageQueue
{
public:
    // (1,2) methods declaration
    void send(T &&msg);
    T receive();
private:
    std::deque<T> _queue; // (3)
    std::condition_variable _cond;          // (4)
    std::mutex _mtx;                        // (4)
};

// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. 
// The class shall have the public methods „void waitForGreen()“ and „void simulate()“ 
// as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that 
// can be either „red“ or „green“. Also, add the private method „void cycleThroughPhases()“. 
// Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. 
enum TrafficLightPhase{
    red,
    green,
};

class TrafficLight : public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();
    // getters / setters
    TrafficLightPhase getCurrentPhase();
    // typical behaviour methods
    void waitForGreen();
    void simulate();

private:
    // typical behaviour methods
    void cycleThroughPhases();
    // can take „red“ or „green“ as its value
    TrafficLightPhase _currentPhase;

    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase 
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling 
    // send in conjunction with move semantics.

    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif