#pragma once

#include <vector>

#include "Control.hpp"
#include "Text.hpp"

namespace goo
{
class ColumnHeader
{
public:
  ColumnHeader(std::string text, int width) : _text(std::move(text)), _width(width) {}

  std::string text() const { return _text; }
  int width() const { return _width; }

  void setTextAlignment(TextAlignment alignment);
  TextAlignment textAlignment() const { return _alignment; }

private:
  std::string _text;
  int _width;
  TextAlignment _alignment = TextAlignment::Center;
};

class ListViewItem
{
public:
	//The string acts as the first subitem.
	ListViewItem(std::string text) { _subItems.push_back(std::move(text)); }
	ListViewItem(std::vector<std::string> subItems) : _subItems(std::move(subItems)) {}

	std::vector<std::string> subItems() const { return _subItems; }
	void setSubItem(int index, std::string text);

private:
	void doSetSubitem(int index, const std::string& text);

	std::vector<std::string> _subItems;
};

class ListView : public Control
{
public:
  ListView() : Control() {}
  ListView(const Point& pos, const Size& size) : Control(pos, size) {}

  void addColumn(ColumnHeader column);
  void addItem(ListViewItem item);

  std::vector<ColumnHeader> columns() const { return _columns; }
  std::vector<ListViewItem> items() const { return _items; }

private:
  void createControl() override;

  void doAddColumn(const ColumnHeader& column, int index);
  void doAddItem(const ListViewItem& item, int index);

  std::vector<ColumnHeader> _columns;
  std::vector<ListViewItem> _items;

};
}
