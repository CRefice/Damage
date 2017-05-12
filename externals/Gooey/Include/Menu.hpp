#pragma once

#include <vector>

#include "Event.hpp"
#include "Handle.hpp"

namespace goo
{
class MenuItem
{
public:
  Event<> onClick;

protected:
  MenuItem(std::string text, long style);
  MenuItem(std::string text, long style, uintptr_t id);

  MenuHandle _container;
  uintptr_t _id;
  std::string _text;

private:
  long _style;

  friend class Menu;
};

class TextItem : public MenuItem
{
public:
  TextItem(std::string text);

  void setChecked(bool state);
  bool checked() const { return _checked; }

  std::string text() const { return _text; }

private:
  bool _checked;
};

class SeparatorItem : public MenuItem
{
public:
  SeparatorItem();
};

class Menu
{
public:
  Menu();

  void appendItem(MenuItem& item);
  MenuItem* itemFromId(int id);

  MenuHandle& handle() { return _handle; }
  const MenuHandle& handle() const { return _handle; }

private:
  MenuHandle _handle;
};

class MenuBar : public Menu
{};

class PopupMenuItem : public Menu, public MenuItem
{
public:
  PopupMenuItem(std::string text);

  std::string text() const { return _text; }
};
}
