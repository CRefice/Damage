#pragma once

#include <functional>

namespace goo
{
template<typename ...Args>
class Event
{
public:
  using FuncType = std::function<void(Args...)>;

  Event() : _handler([](Args...) {}) {}
  Event(FuncType handler) : _handler(std::move(handler)) {}

	void setHandler(FuncType handler) { _handler = handler; }

  void operator()(Args... args) { _handler(args...); }

private:
  FuncType _handler;
};

//The following are some utility event arguments
struct CancelToken
{
  bool cancel = false;
};
}
