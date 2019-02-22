#include "timer.h"

namespace can {

Timer::Timer(const TimeoutFunc &timeout_func)
        : timeout_func_(timeout_func) {
}

Timer::Timer(const TimeoutFunc &timeout_func,
             const Interval &interval,
             bool single_shot)
        : is_single_shot_(single_shot),
          interval_(interval),
          timeout_func_(timeout_func) {
}

void
Timer::Start(bool multi_thread) {
  if (this->Running() == true)
    return;

  running_ = true;

  if (multi_thread == true) {
    thread_ = std::thread(
            &Timer::Temporize_, this);
  } else {
    this->Temporize_();
  }
}

void
Timer::Stop() {
  running_ = false;
  thread_.join();
}

bool
Timer::Running() const {
  return running_;
}

void
Timer::SetSingleShot(bool single_shot) {
  if (this->Running() == true)
    return;

  is_single_shot_ = single_shot;
}

bool
Timer::IsSingleShot() const {
  return is_single_shot_;
}

void
Timer::SetInterval(const Timer::Interval &interval) {
  if (this->Running() == true)
    return;

  interval_ = interval;
}

const Timer::Interval&
Timer::GetInterval() const {
  return interval_;
}

void
Timer::SetTimeoutFunc(const TimeoutFunc &timeout) {
  if (this->Running() == true)
    return;

  timeout_func_ = timeout;
}

const Timer::TimeoutFunc&
Timer::GetTimeoutFunc() const {
  return timeout_func_;
}

void Timer::Temporize_() {
  if (is_single_shot_ == true) {
    this->SleepThenTimeout_();
  } else {
    while (this->Running() == true) {
      this->SleepThenTimeout_();
    }
  }
}

void Timer::SleepThenTimeout_() {
  // std::chrono::microseconds m1(250);
  // std::this_thread::sleep_for(m1);
  std::this_thread::sleep_for(interval_);

  if (this->Running() == true)
    this->GetTimeoutFunc()();
}

} //namespace can
