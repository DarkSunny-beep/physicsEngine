#pragma once

#ifndef CYCLONE_DEMO_TIMING_H // if not defined.
#define CYCLONE_DEMO_TIMING_H

struct TimingData
{
    // frame을 총 몇번 그렸는가 -> 초당 프레임 수 계산 가능(fps: frames per second)
    unsigned frameNumber;
    // unsigned~ (대개) unsigned int 의 줄임말

    // 마지막 frame까지의 소요 시간. 게임 시작 후 끝까지 걸린 시간.
    unsigned lastFrameTimestamp;

    // 마지막 frame이 지속된 시간
    unsigned lastFrameDuration;

    // 마지막 frame까지 걸린 clockstamp. 밀리초보다 훨씬 정말한 시계이며 수가 빠르게 올라가므로 unsigned long으로 설정
    unsigned long lastFrameClockstamp;

    // 마지막 frame이 지속된 ClockTicks.
    unsigned long lastFrameClockTicks;

    // 렌더링을 멈췄는지
    bool isPaused;

    // Calculated data

    // 만일 일시적인 렉으로 lastFrameDuration이 갑자기 팍 튀게 되었을 때 이를 그대로 사용하지 않도록 적절히 평균 시간을 내주는 것
    double averageFrameDuration;

    float fps;

    /**
     * Gets the global timing data object.
     */
    static TimingData& get();

    /**
     * Updates the timing system, should be called once per frame.
     */
    static void update();

    /**
     * Initialises the frame information system. Use the overall
     * init function to set up all modules.
     */
    static void init();

    /**
     * Deinitialises the frame information system.
     */
    static void deinit();

    /**
     * Gets the global system time, in the best resolution possible.
     * Timing is in milliseconds.
     */
    static unsigned getTime();

    /**
     * Gets the clock ticks since process start.
     */
    static unsigned long getClock();


private:
    // 싱글톤 패턴. TimingData가 여러개 만들어지는 것을 방지함
    TimingData() {}
    TimingData(const TimingData &) {}
    TimingData& operator=(const TimingData &);
    // 기본 생성자, 덮어씌우기 등을 모두 private에 넣어서 방지
};


#endif // CYCLONE_DEMO_TIMING_H

