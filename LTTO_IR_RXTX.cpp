#include "LTTO_IR_RXTX.hpp"

LTTO_IR_RXTX::LTTO_IR_RXTX() {
	reset();
}

void LTTO_IR_RXTX::reset(void) {
	LTTO_IR_RX::reset();
	LTTO_IR_TX::reset();
}

void LTTO_IR_RXTX::Tick1ms(void) {
	LTTO_IR_RX::Tick1ms();
	LTTO_IR_TX::Tick1ms();
}
