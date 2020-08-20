#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <chrono>

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}


/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
}

using std::chrono::system_clock;
using std::chrono::seconds;
using std::chrono::milliseconds;
// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop (0) that measures the time between two loop cycles (1)
    // and toggles the current phase of the traffic light between red and green (2) and sends an update method 
    // to the message queue using move semantics (3). 
    // The cycle duration should be a random value between 4 and 6 seconds (4). 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles (5). 
    srand(time(NULL)); // seed 
    int cycleDuration = rand() % 6 + 4;
    std::chrono::time_point<system_clock> lastUpdate;
    long previousLastUpdate; 

    // initialize clock moment
    lastUpdate = system_clock::now();
    // (0) Start infinite loop
    while(true) {

        // (1) time difference between loops
        previousLastUpdate = std::chrono::duration_cast<seconds>(system_clock::now() - lastUpdate).count();

        // (5) Sleep to reduce CPU load
        std::this_thread::sleep_for(milliseconds(1));

        if (previousLastUpdate >= cycleDuration) {
            // (2) toggles the current phase of the traffic light between red and green
            _currentPhase = (_currentPhase != red) ? red : green;

            // (3) sends an update method
            // TODO need the queue in FP.3 to implement this step

            // reinit vars
            lastUpdate = system_clock::now();
            cycleDuration = rand() % 6 + 4; // duration (4)
        }
    }
}

