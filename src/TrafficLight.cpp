#include <iostream>
#include <random>
#include <chrono>
#include <future>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> 
    std::unique_lock<std::mutex> locker(_mtx);
    //and _condition.wait() to wait for and receive new messages 
    _cond.wait(locker, [this] { return !_queue.empty(); });
    // and pull them from the queue using move semantics. 
    T msg = std::move(_queue.back());
    _queue.pop_back();
    // The received object should then be returned by the receive function. 
    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    std::lock_guard<std::mutex> locker(_mtx);
    // as well as _condition.notify_one() to add a new message to the queue 
    _queue.push_back(std::move(msg));
    //and afterwards send a notification.
    _cond.notify_one();
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
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started 
    // in a thread when the public method „simulate“ is called. 
    // To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

using std::chrono::system_clock;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::time_point;
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
    time_point<system_clock> lastUpdate;
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
            std::async(std::launch::async, 
                &MessageQueue<TrafficLightPhase>::send, &_messagesQueue, std::move(_currentPhase)).wait();

            // reinit vars
            lastUpdate = system_clock::now();
            cycleDuration = rand() % 6 + 4; // duration (4)
        }
    }
}

