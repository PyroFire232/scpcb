#include "Timing.h"

Timing::Timing(int tickrate) {
    timeStep = 1.0 / tickrate;
    accumulatedSeconds = 0.0;
    
    initialTime = std::chrono::high_resolution_clock::now();
}

double Timing::getTimeStep() const {
    return timeStep;
}

void Timing::addSecondsToAccumulator(double seconds) {
    if (seconds <= 0.0) { return; }
    accumulatedSeconds += seconds;
}

bool Timing::tickReady() {
    return accumulatedSeconds >= timeStep;
}

void Timing::subtractTick() {
    if (accumulatedSeconds <= 0.0) { return; }
    
    accumulatedSeconds -= timeStep;
}


double Timing::getElapsedSeconds() {
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(now - initialTime);
    
    initialTime = now;
    return timeSpan.count();
}
