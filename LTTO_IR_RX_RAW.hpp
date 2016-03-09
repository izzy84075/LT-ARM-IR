#include "LTTO_IR_Storage.hpp"

#ifndef LTTO_IR_RX_RAW_H
#define LTTO_IR_RX_RAW_H

class LTTO_IR_RX_RAW {
	public:
		LTTO_IR_RX_RAW(void);
		bool newSample(unsigned int qmsOn, unsigned int qmsOff);
		bool isSignatureWaiting(void);
		LTTO_IR_STORAGE getSignature(void);
		void reset(void);
	private:
		unsigned int raw;
		unsigned int step;
		LTTO_IR_STORAGE workingBuffer;
		bool signatureWaiting;
};

#endif
