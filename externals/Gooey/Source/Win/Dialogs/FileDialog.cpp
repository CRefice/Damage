#include <algorithm>

#include "Platform.hpp"

#include "Filedialog.hpp"

namespace goo
{
DialogResult OpenFileDialog::show() {
	std::string filterCopy(_filter);
	std::replace(filterCopy.begin(), filterCopy.end(), '|', '\0');
	//WinApi wants two null-chars... don't know why
	filterCopy += '\0';
	filterCopy += '\0';

	OPENFILENAME ofn;
	::ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = _buffer;
	ofn.nMaxFile = sizeof(_buffer);
	ofn.lpstrFilter = filterCopy.c_str();
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrTitle = _text.c_str();
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	return (::GetOpenFileName(&ofn) == TRUE) ? DialogResult::OK : DialogResult::Cancel;
}

std::string OpenFileDialog::fileName() const {
	return std::string(_buffer);
}

//TODO:implement these two... maybe?
void OpenFileDialog::setText(std::string text) {
	_text = std::move(text);
}

void OpenFileDialog::setFilter(std::string filter) {
 	_filter = std::move(filter);
}
}
