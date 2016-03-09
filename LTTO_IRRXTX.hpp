#include "LTTO_IRRX.hpp"
#include "LTTO_IRTX.hpp"

#ifndef LTTO_IRRXTX_H
#define LTTO_IRRXTX_H

class LTTO_IRRXTX : public LTTO_IRRX, public LTTO_IRTX {
	public:
		LTTO_IRRXTX();
		void reset(void);
		void Tick1ms(void);
	private:

};

#endif
