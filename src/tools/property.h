#ifndef PROPERTY_H
#define PROPERTY_H

#include "signal.h"

// A Property encapsulates a value and may inform
// you on any changes applied to this value.

template <typename T>
class Property {

 public:
  typedef T value_type;

  Property(T const& val) : value_(val) {}

  Property(T&& val)
      : value_(std::move(val)) {}

  Property(Property<T> const& to_copy)
      : value_(to_copy.value_) {}

  Property(Property<T>&& to_copy)
      : value_(std::move(to_copy.value_)) {}

  // returns a Signal which is fired when the internal value
  // has been changed. The new value is passed as parameter.
  virtual Signal<T> const& on_change() const {
    return on_change_;
  }

  // sets the Property to a new value.
  // on_change() will be emitted.
  virtual void set(T const& value) {
    if (value_ != value) {
      value_ = value;
      on_change_.emit(value_);
    }
  }

  // returns the internal value
  virtual T const& get() const { return value_; }

  // if there are any Properties connected to this Property,
  // they won't be notified of any further changes
  virtual void disconnect_auditors() {
    on_change_.disconnect_all();
  }

  // assigns the value of another Property
  virtual Property<T>& operator=(Property<T> const& rhs) {
    set(rhs.value_);
    return *this;
  }

  // assigns a new value to this Property
  virtual Property<T>& operator=(T const& rhs) {
    set(rhs);
    return *this;
  }

  // returns the value of this Property
  T const& operator()() const {
    return Property<T>::get();
  }

 private:
  Signal<T> on_change_;

  T value_;
};


// stream operators
template<typename T>
std::ostream& operator<<(std::ostream& str, Property<T> const& val) {
  str << val.get();
  return str;
}

template<typename T>
std::istream& operator>>(std::istream& str, Property<T>& val) {
  T tmp;
  str >> tmp;
  val.set(tmp);
  return str;
}

#endif /* PROPERTY_H */
