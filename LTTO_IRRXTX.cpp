#include "LTTO_IRRXTX.hpp"

LTTO_IRRXTX::LTTO_IRRXTX() {
	reset();
}

void LTTO_IRRXTX::reset(void) {
	LTTO_IRRX::reset();
	LTTO_IRTX::reset();
}

void LTTO_IRRXTX::Tick1ms(void) {
	LTTO_IRRX::Tick1ms();
	LTTO_IRTX::Tick1ms();
}
