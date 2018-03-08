//
// Created by anlijiu on 18-1-25.
//

#ifndef CAN_TIMER_H
#define CAN_TIMER_H

#include <thread>
#include <chrono>
#include <functional>

namespace can {

class Timer {
public:
  typedef std::chrono::milliseconds Interval;
  typedef std::function<void(void)> TimeoutFunc;

  Timer(const TimeoutFunc &timeout);

  Timer(const TimeoutFunc &timeout,
        const Interval &interval,
        bool single_shot = true);

  void Start(bool multiThread = false);

  void Stop();

  bool Running() const;

  void SetSingleShot(bool singleShot);

  bool IsSingleShot() const;

  void SetInterval(const Interval &interval);

  const Interval &GetInterval() const;

  void SetTimeoutFunc(const TimeoutFunc &timeout);

  const TimeoutFunc &GetTimeoutFunc() const;

private:
  std::thread thread_;

  bool running_ = false;
  bool is_single_shot_ = true;

  Interval interval_ = Interval(0);
  TimeoutFunc timeout_func_ = nullptr;

  void Temporize_();

  void SleepThenTimeout_();

};


} //namespace can



#endif //CAN_TIMER_H
