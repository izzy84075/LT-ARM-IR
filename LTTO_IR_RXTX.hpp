#include "LTTO_IR_RX.hpp"
#include "LTTO_IR_TX.hpp"

#ifndef LTTO_IR_RXTX_H
#define LTTO_IR_RXTX_H

class LTTO_IR_RXTX : public LTTO_IR_RX, public LTTO_IR_TX {
	public:
		LTTO_IR_RXTX();
		void reset(void);
		void Tick1ms(void);
	private:

};

#endif
