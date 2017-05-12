#pragma once

#include <string>

#include "Control.hpp"
#include "Font.hpp"
#include "Event.hpp"

namespace goo
{
class ButtonBase : public Control
{
public:
  ButtonBase() : Control() {}
  ButtonBase(std::string text) : _text(std::move(text)), Control() {}
  ButtonBase(std::string text, const Point& pos, const Size& size) : _text(std::move(text)), Control(pos, size) {}

  Event<> onClick;

  void setText(std::string text);
  std::string text() const { return _text; }

protected:
  std::string _text;
};
}
