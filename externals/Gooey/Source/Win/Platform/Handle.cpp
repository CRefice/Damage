#include <utility>

#include "Handle.hpp"

namespace goo
{
template<typename T>
Handle<T>::Handle(const Handle<T>& handle) {
	::DuplicateHandle(GetCurrentProcess(), handle,
			GetCurrentProcess(), (LPHANDLE)(&_handle), 0, FALSE,
			DUPLICATE_SAME_ACCESS);
}
template<>
Handle<NativeImage>::Handle(const Handle<NativeImage>& handle) {
	_handle = (HBITMAP)(::CopyImage(handle, IMAGE_BITMAP, 0, 0, 0));
}

template<typename T>
Handle<T>& Handle<T>::operator=(const Handle<T>& handle)
{
	if (_handle == handle) return *this;
	Handle<T> copy(handle);
	std::swap(*this, std::move(copy));
	return *this;
}
template<typename T>
Handle<T>& Handle<T>::operator=(T handle)
{
	_handle = handle;
	return *this;
}

template<>
Handle<NativeControl>::~Handle()
{
	::PostMessage(_handle, WM_CLOSE, 0, 0);
}
template<>
Handle<NativeFont>::~Handle()
{
	::DeleteObject((HGDIOBJ)_handle);
}
template<>
Handle<NativeMenu>::~Handle()
{
	::DestroyMenu(_handle);
}
template<>
Handle<NativeImage>::~Handle()
{
	::DeleteObject((HGDIOBJ)_handle);
}
template<>
Handle<NativeLayout>::~Handle() {}

template struct Handle<NativeControl>;
template struct Handle<NativeFont>;
template struct Handle<NativeMenu>;
template struct Handle<NativeImage>;
template struct Handle<NativeLayout>;
}
