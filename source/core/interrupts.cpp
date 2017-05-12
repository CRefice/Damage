#include "interrupts.hpp"

namespace core
{
void InterruptRequester::connect(InterruptHandler* handler)  {
	irqHandler = handler;
}

void InterruptRequester::raise(Interrupt i) {
	irqHandler->handle(i);
}
}
