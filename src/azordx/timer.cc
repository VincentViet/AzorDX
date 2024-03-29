#include <AzorDx/timer.h>
#include <windows.h>

float azordx::Timer::delta_time = 0;

static bool is_pause = false;
static float sec_per_count = 0;
static __int64 count_per_sec = 0;
static __int64 current_time = 0;
static __int64 prev_time = 0;

namespace azordx {
void tick() {
  if (!is_pause) {
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_time));
    Timer::delta_time = (current_time - prev_time) * sec_per_count;
    prev_time = current_time;
  } else {
    Timer::delta_time = 0;
  }
}

void initialize_timer() {
  QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&count_per_sec));
  sec_per_count = 1.0f / count_per_sec;
}
}  // namespace azordx

void azordx::Timer::pause() {
  if (!is_pause) {
    is_pause = true;
  }
}

void azordx::Timer::resume() {
  if (is_pause) {
    is_pause = false;
  }
}