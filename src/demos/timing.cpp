#include "timing.h"


static bool qpcFlag; // 윈도우에만 있는 cpu시계. query performance counter

#if (__APPLE__ || __unix)
	#define TIMING_UNIX	1

	#include <stdlib.h>
	#include <sys/time.h> // 맥/리눅스 전용 시계

	// assume unix based OS
	typedef unsigned long long	LONGLONG;
    // 엄청나게 큰 숫자를 위한 자료형 unsigned long long. 이를 윈도우에선 LONGLONG으로 부름
#else
	#define TIMING_WINDOWS	1
	// assume windows

	// Import the high performance timer (c. 4ms).
	#include <windows.h>
	#include <mmsystem.h>

	static double qpcFrequency;
#endif



// Internal time and clock access functions
unsigned systemTime()
{
#if TIMING_UNIX
	struct timeval tv; //sys에 있는 구조체
	gettimeofday(&tv, 0);

	return tv.tv_sec * 1000 + tv.tv_usec/1000;
    //sec: 초라서 밀리초로 변환하기 위해 *1000, usec: 마이크로 초라서 1000으로 나눠 줌

#else
    if(qpcFlag)
    {
        static LONGLONG qpcMillisPerTick;
        QueryPerformanceCounter((LARGE_INTEGER*)&qpcMillisPerTick);
        return (unsigned)(qpcMillisPerTick * qpcFrequency);
    }
    else
    {
        return unsigned(timeGetTime());
    }
#endif

}

unsigned TimingData::getTime()
{
    return systemTime();
}

#if TIMING_WINDOWS
unsigned long systemClock()
{
    __asm {
    	rdtsc;
    }
}
#endif

unsigned long TimingData::getClock()
{

#if TIMING_UNIX
	struct timeval tv;
	gettimeofday(&tv, 0);

	return tv.tv_sec * 1000 + tv.tv_usec/1000;
#else
    return systemClock();
#endif
}

// Sets up the timing system and registers the performance timer.
void initTime()
{
#if TIMING_UNIX
    qpcFlag = false;
#else
    LONGLONG time;

    qpcFlag = (QueryPerformanceFrequency((LARGE_INTEGER*)&time) > 0);

    // Check if we have access to the performance counter at this
    // resolution.
    if (qpcFlag) qpcFrequency = 1000.0 / time;
#endif
}


// Holds the global frame time that is passed around
// 하지만 시계 자체를 건들 수는 없도록 static으로 설정
static TimingData *timingData = NULL;

// Retrieves the global frame info instance
TimingData& TimingData::get()
{
    return (TimingData&)*timingData;
}

// Updates the global frame information. Should be called once per frame.
void TimingData::update()
{
    if (!timingData) return;

    // Advance the frame number.
    if (!timingData->isPaused)
    {
        timingData->frameNumber++;
    }

    // Update the timing information.
    unsigned thisTime = systemTime();
    timingData->lastFrameDuration = thisTime -
        timingData->lastFrameTimestamp;
    timingData->lastFrameTimestamp = thisTime;

    // Update the tick information.
    unsigned long thisClock = getClock();
    timingData->lastFrameClockTicks =
    thisClock - timingData->lastFrameClockstamp;
    timingData->lastFrameClockstamp = thisClock;

    // Update the RWA frame rate if we are able to.
    if (timingData->frameNumber > 1) {
        if (timingData->averageFrameDuration <= 0)
        {
            timingData->averageFrameDuration =
                (double)timingData->lastFrameDuration;
        }
        else
        {
            // RWA over 100 frames.
            timingData->averageFrameDuration *= 0.99;
            timingData->averageFrameDuration +=
                0.01 * (double)timingData->lastFrameDuration;

            // Invert to get FPS
            timingData->fps =
                (float)(1000.0/timingData->averageFrameDuration);
        }
    }
}

void TimingData::init()
{
    // Set up the timing system.
    initTime();

    // Create the frame info object
    if (!timingData) timingData = new TimingData();

    // Set up the frame info structure.
    timingData->frameNumber = 0;

    timingData->lastFrameTimestamp = systemTime();
    timingData->lastFrameDuration = 0;

    timingData->lastFrameClockstamp = getClock();
    timingData->lastFrameClockTicks = 0;

    timingData->isPaused = false;

    timingData->averageFrameDuration = 0;
    timingData->fps = 0;
}

void TimingData::deinit()
{
        delete timingData;
        timingData = NULL;
}