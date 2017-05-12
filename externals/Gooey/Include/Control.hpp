#pragma once

#include <string>

#include "Drawing.hpp"
#include "Font.hpp"
#include "Handle.hpp"

namespace goo
{
//Forward declaration for parent container
class Container;

class Control
{
public:
	Control() : _pos(defaultPosition()), _size(defaultSize()) {}
	Control(const Point& pos, const Size& size) : _pos(pos), _size(size) {}
	
	void create() {
		createControl();
		_created = true;
	}

	bool created() const { return _created; }
	
	void setVisible(bool state);
	void show() { setVisible(true); }
	void hide() { setVisible(false); }
	bool visible() const;
	
	void setEnabled(bool state);
	void enable() { setEnabled(true); }
	void disable() { setEnabled(false); }
	bool enabled() const;
	
	void setParent(const Container& parent);
	void unparent();
	const Container* parent() const { return _parent; }
	
	void setPosition(const Point& pos);
	void setSize(const Size& size);
	void setBounds(const Point& pos, const Size& size) {
		setPosition(pos);
		setSize(size);
	}

	Point position() const { return _pos; }
	Size size() const { return _size; }
	
	Font font() { return _font; }
	void setFont(Font font);

	ControlHandle& handle() { return _handle; }
	const ControlHandle& handle() const { return _handle; }

protected:
	virtual void createControl() = 0;
	void createHandle(const char* name, const std::string& text, long style, long exStyle = 0);

private:
	Point _pos;
	Size _size;
	Font _font = defaultFont();

	const Container* _parent = nullptr;
	bool _created = false;
	
	ControlHandle _handle;
};
}
