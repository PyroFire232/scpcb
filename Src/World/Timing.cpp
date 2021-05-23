#include "Timing.h"

Timing::Timing(int tickrate, int framerate) {
    setTickRate(tickrate+1);
    setFrameRate(framerate+1);
    sinceLastTick = 0.f;
    sinceLastFrame = 0.f;
    avgTickRate = 0.f;
    avgFrameRate = 0.f;

    initialTime = std::chrono::high_resolution_clock::now();
    compareTick = initialTime - tickStepDuration; // runFirst
    compareFrame = initialTime - frameStepDuration; // runFirst
    lastTick = compareTick;
    lastFrame = compareFrame;
}

void Timing::setFrameRate(int rate) {
    frameRate = rate;
    frameStep = ceil(1000000000.f / float(rate));
    frameStepDuration = std::chrono::nanoseconds(frameStep);
}
void Timing::setTickRate(int rate) {
    tickRate = rate;
    tickStep = ceil(1000000000.f / float(rate));
    tickStepDuration = std::chrono::nanoseconds(tickStep);
}

// TimeRate/FrameRate globals
int Timing::getTickRate() const { return tickRate; }
int Timing::getFrameRate() const { return frameRate; }
float Timing::getSinceTick() const { return sinceLastTick; }
float Timing::getSinceFrame() const { return sinceLastFrame; }
float Timing::getAvgTickRate() const { return avgTickRate; }
float Timing::getAvgFrameRate() const { return avgFrameRate; }

// Clock Ticking
bool Timing::tickReady() {
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    if (now >= compareTick) {
        std::chrono::high_resolution_clock::duration sinceLast = (now - lastTick);
        sinceLastTick = float(std::chrono::duration_cast<std::chrono::nanoseconds>(sinceLast).count()) / 1000000000.f;
        compareTick = (lastTick + tickStepDuration + tickStepDuration) - sinceLast;
        lastTick = now;
        return true;
    }
    return false;
}
void Timing::tickFinished() {
    //Clock::time_point now = Clock::now();
    //Clock::duration sinceLast = (now - compareTick);
    // Unfinished : Used to track how long a frame or tick takes.
}


// Frame Ticking
bool Timing::frameReady() {
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    if (now >= compareFrame) {
        std::chrono::high_resolution_clock::duration sinceLast = (now - lastFrame);
        sinceLastFrame = float(std::chrono::duration_cast<std::chrono::nanoseconds>(sinceLast).count()) / 1000000000.f;
        compareFrame = (lastFrame + frameStepDuration+frameStepDuration)-sinceLast;
        lastFrame = now;
        return true;
    }
    return false;
}
void Timing::frameFinished() {
    //Clock::time_point now = Clock::now();
    //Clock::duration sinceLast = (now - compareFrame);
    // Unfinished : Used to track how long a frame or tick takes.
}
