#pragma once

#include "ButtonBase.hpp"

namespace goo
{
class CheckBox : public ButtonBase
{
public:
  CheckBox() : ButtonBase() {}
  CheckBox(std::string text) : ButtonBase(std::move(text)) {}
  CheckBox(std::string text, const Point& pos, const Size& size) : ButtonBase(std::move(text), pos, size) {}

  void setChecked(bool state);
  bool checked() const;

private:
  void createControl() override;
};
}
