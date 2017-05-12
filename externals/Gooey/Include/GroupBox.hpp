#pragma once

#include <string>

#include "Control.hpp"

namespace goo
{
class GroupBox : public Control
{
public:
  GroupBox() : Control() {}
  GroupBox(std::string text) : _text(std::move(text)), Control() {}
  GroupBox(std::string text, const Point& pos, const Size& size) : _text(std::move(text)), Control(pos, size) {}

  void setText(std::string text);
  std::string text() { return _text; }

private:
  void createControl() override;

  std::string _text;
};
}
