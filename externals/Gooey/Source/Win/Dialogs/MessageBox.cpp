#include "Platform.hpp"

#include "MessageBox.hpp"

namespace goo
{
DialogResult MessageBox::show(const std::string& text, const std::string& title, MessageBoxIcon icon, MessageBoxButtons buttons) {
	unsigned long params = 0;
	switch (buttons) {
	case MessageBoxButtons::AbortRetryIgnore:
		params |= 0x2;
		break;

	case MessageBoxButtons::OKCancel:
		params |= 0x1;
		break;

	case MessageBoxButtons::RetryCancel:
		params |= 0x5;
		break;

	case MessageBoxButtons::YesNo:
		params |= 0x4;
		break;

	case MessageBoxButtons::YesNoCancel:
		params |= 0x3;
		break;
	}

	switch (icon) {
	case MessageBoxIcon::Asterisk:
	case MessageBoxIcon::Information:
		params |= 0x40;
		break;

	case MessageBoxIcon::Error:
	case MessageBoxIcon::Hand:
	case MessageBoxIcon::Stop:
		params |= 0x10;
		break;

	case MessageBoxIcon::Exclamation:
	case MessageBoxIcon::Warning:
		params |= 0x30;
		break;

	case MessageBoxIcon::Question:
		params |= 0x20;
		break;
	}

	switch (MessageBoxA(NULL, text.c_str(), title.c_str(), params)) {
	case IDABORT:
		return DialogResult::Abort;

	case IDCANCEL:
		return DialogResult::Cancel;

	case IDIGNORE:
	case IDCONTINUE:
		return DialogResult::Ignore;

	case IDNO:
		return DialogResult::No;

	case IDOK:
		return DialogResult::OK;

	case IDRETRY:
		return DialogResult::Retry;

	case IDYES:
		return DialogResult::Yes;

	default:
		return DialogResult::Cancel;
	}
}
}
