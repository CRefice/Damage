#pragma once

#include <string>

#include "Control.hpp"
#include "Font.hpp"

namespace goo
{
class Label : public Control
{
public:
  Label() : Control() {}
  Label(std::string text) : _text(std::move(text)), Control() {}
  Label(std::string text, const Point& pos, const Size& size) : _text(std::move(text)), Control(pos, size) {}

  std::string text() { return _text; }
  void setText(std::string text);

private:
  void createControl() override;
  std::string _text;
};
}
