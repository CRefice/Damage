#pragma once

#include "Control.hpp"

namespace goo
{
class InputBox : public Control
{
public:
  InputBox() : Control() {}
  InputBox(const Point& pos, const Size& size) : Control(pos, size) {}

  void setText(std::string text);
  std::string text() const;

private:
  void createControl() override;

  char _buffer[0x200];
};
}
