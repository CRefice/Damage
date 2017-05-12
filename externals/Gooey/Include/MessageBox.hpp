#pragma once

#include "Dialog.hpp"

namespace goo
{
enum class MessageBoxButtons
{
	AbortRetryIgnore, OK, OKCancel, RetryCancel, YesNo, YesNoCancel
};

enum class MessageBoxIcon
{
	Asterisk, Error, Exclamation, Hand, Information, None, Question, Stop, Warning
};

namespace MessageBox
{
	DialogResult show(const std::string& text, const std::string& title,
		MessageBoxIcon icon = MessageBoxIcon::None, MessageBoxButtons buttons = MessageBoxButtons::OK);
};
}
