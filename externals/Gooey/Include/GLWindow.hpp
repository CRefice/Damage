#pragma once

#include "Window.hpp"

namespace goo
{
class GLWindow : public Window
{
public:
  void swapBuffers();

  void setViewport(const Point& pos, const Size& size);

  void setFullscreen(bool enable);
  void setVsync(bool enable);

private:
  void createControl() override;

  GLContext _context;
};
}
