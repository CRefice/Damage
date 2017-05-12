#include <algorithm>

#include "Platform.hpp"

#include "ListView.hpp"

namespace goo
{
void ListView::createControl() {
	createHandle(WC_LISTVIEW, "", LVS_EDITLABELS | LVS_REPORT, WS_EX_CLIENTEDGE);
	for (int n = 0; n < _columns.size(); ++n) {
		doAddColumn(_columns[n], n);
	}
	for (int n = 0; n < _items.size(); ++n) {
		doAddItem(_items[n], n);
	}
}

void ListView::addColumn(ColumnHeader column) {
	doAddColumn(column, _columns.size());
	_columns.push_back(std::move(column));
}
void ListView::addItem(ListViewItem item) {
	doAddItem(item, _items.size());
	_items.push_back(std::move(item));
}

void ListView::doAddColumn(const ColumnHeader& column, int index) {
	int fmt;
	switch (column.textAlignment()) {
	case TextAlignment::Center:
		fmt = LVCFMT_CENTER;
		break;

	case TextAlignment::Left:
		fmt = LVCFMT_LEFT;
		break;

	case TextAlignment::Right:
		fmt = LVCFMT_RIGHT;
		break;
	}

	const std::string& text = column.text();
	char* temp = new char[text.size() + 1];
	std::copy(text.cbegin(), text.cend(), temp);
	temp[text.size()] = '\0';

	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = index;
	lvc.iOrder = index + 1;
	lvc.pszText = temp;
	lvc.cx = column.width();
	lvc.fmt = fmt;
	ListView_InsertColumn(handle(), _columns.size(), &lvc);

	delete[] temp;
}

void ListView::doAddItem(const ListViewItem& item, int index) {
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
	lvi.iSubItem = 0;
	lvi.pszText = "";
	lvi.state = 0;
	lvi.stateMask = 0;
	lvi.iItem = index;
	ListView_InsertItem(handle(), &lvi);

	auto subIt = item.subItems();
	for (int n = 0; n < subIt.size(); ++n) {
		char* text = const_cast<char *>(subIt[n].c_str());
		ListView_SetItemText(handle(), index, n, text);
	}
}

void ColumnHeader::setTextAlignment(TextAlignment alignment) {
	_alignment = alignment;
	// TODO: actual handling
}
}
