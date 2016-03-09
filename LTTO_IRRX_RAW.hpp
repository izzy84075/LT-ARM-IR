#include "LTTO_IR.hpp"

#ifndef LTTO_IRRX_RAW_H
#define LTTO_IRRX_RAW_H

class LTTO_IRRX_RAW {
	public:
		LTTO_IRRX_RAW(void);
		bool newSample(unsigned int qmsOn, unsigned int qmsOff);
		bool isSignatureWaiting(void);
		LTTO_IR getSignature(void);
		void reset(void);
	private:
		unsigned int raw;
		unsigned int step;
		LTTO_IR workingBuffer;
		bool signatureWaiting;
};

#endif
