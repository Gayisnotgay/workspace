#pragma once
#include <ctime>
#include "MessageCenter/MessageCenter.h"

class TimeTool {
public:
    TimeTool()
    {
        timespec_get(&m_st, TIME_UTC);
    }

    double GetElapsedMilliSecond()
    {
        timespec tmEnd;
        timespec_get(&tmEnd, TIME_UTC);
        return ((tmEnd.tv_sec - m_st.tv_sec) * 1000 + (tmEnd.tv_nsec - m_st.tv_nsec) / 1000000);
    }

    static void millisleep(double time)
    {
        timespec tm = {0};
        tm.tv_sec = static_cast<time_t>(time / 1000);
        tm.tv_nsec = static_cast<long>((time - static_cast<time_t>(tm.tv_sec * 1000)) * 1000000);
        timespec rm = {0};
        int sleepResult = nanosleep(&tm, &rm);
        if (sleepResult != EXIT_SUCCESS) {
            MessageCenter::Instance() << "nanosleep returned in advance, expSleepTime:" << time << "ms, remainTime:" << rm.tv_sec << "s, " << rm.tv_nsec << "ns" << std::endl;
        }
    }

private:
    timespec m_st = {0, 0};
};