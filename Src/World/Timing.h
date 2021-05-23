#ifndef TIMING_H_INCLUDED
#define TIMING_H_INCLUDED

#include <chrono>

class Timing {
    private:
        std::chrono::high_resolution_clock::time_point initialTime;
        std::chrono::high_resolution_clock::time_point prevTime;

        // Tick rate
        int tickRate; // Tick Rate - goal n Ticks Per Second (TPS), averages between n and n+1.
        int tickStep; // Time between ticks (in nanoseconds)
        float sinceLastTick; // True time since the last tick (in fractions of a second)
        float avgTickRate; // Estimated true tickrate (in TPS)
        std::chrono::high_resolution_clock::duration tickStepDuration; // The time between ticks (in high resolution nanoseconds)
        std::chrono::high_resolution_clock::time_point compareTick; // the high-precision moment when the next tick should execute
        std::chrono::high_resolution_clock::time_point lastTick; // the high-precision moment when the previous tick executed.

        // Frame timers (Milliseconds)
        int frameRate;
        int frameStep;
        float sinceLastFrame;
        float avgFrameRate;
        std::chrono::high_resolution_clock::duration frameStepDuration;
        std::chrono::high_resolution_clock::time_point compareFrame;
        std::chrono::high_resolution_clock::time_point lastFrame;

    public:
        Timing(int tickrate, int framerate);

        // Tick rates
        int getTickRate() const;
        float getSinceTick() const;
        float getAvgTickRate() const;
        void setTickRate(int rate);
        bool tickReady();
        void tickFinished();

        // Frame rates
        int getFrameRate() const;
        float getSinceFrame() const;
        float getAvgFrameRate() const;
        void setFrameRate(int rate);
        bool frameReady();
        void frameFinished();

};

#endif // TIMING_H_INCLUDED
