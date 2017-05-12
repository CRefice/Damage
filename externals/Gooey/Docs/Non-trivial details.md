#Data structures
Most of the classes in this library only serve as a wrapper around system-specific handles.
For example, in windows, the Bitmap class defined in Drawing.hpp only contains an hbitmap handle. It doesn't contain the actual bitmap data.
#Default Control Visibility
By default, a control is visible on creation.
If you want it to be hidden, instead, just call hide()
before its creation.
#Adding items to a menu
You cannot call Menu::appendItem() with an rvalue as an argument, or you'll get undefined behavior.
This makes sense: why would you want to add a temporary menu item to a menu bar? You wouldn't be able to set any behavior for it, for example.
