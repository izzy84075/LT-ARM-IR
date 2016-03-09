#include "LTTO_IR_RX.hpp"
#include "LTTO_IR_TX.hpp"

#ifndef LTTO_IR_RXTX_H
#define LTTO_IR_RXTX_H

class LTTO_IR_RXTX : public LTTO_IR_RX, public LTTO_IR_TX {
	public:
		LTTO_IR_RXTX();				//Constructor. Initializes everything.
		void reset(void);			//Calls IR_RX and IR_TX's reset()s.
		void Tick1ms(void);			//Calls IR_RX and IR_TX's Tick1ms()s.
	private:

};

#endif
