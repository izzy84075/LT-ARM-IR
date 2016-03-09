#include "LTTO_IR_Storage.hpp"

#ifndef LTTO_IR_RX_RAW_H
#define LTTO_IR_RX_RAW_H

class LTTO_IR_RX_RAW {
	public:
		LTTO_IR_RX_RAW(void);					//Constructor. Initializes everything.
		
		/*
			Whenever a capture cycle is complete, call this function with the time that the IR carrier was present and not-present, in quarter-milliseconds.
			Will return a boolean, true if a signature was completed with this capture cycle, false otherwise.
		*/
		bool newSample(unsigned int qmsOn, unsigned int qmsOff);
		bool isSignatureWaiting(void);			//Returns true if a signature is waiting to be processed.
		LTTO_IR_STORAGE getSignature(void);		//Returns the signature that is waiting in the buffer.
		void reset(void);						//Resets everything to the default state.
	private:
		unsigned int step;						//Where we're at in the capture/decoding process.
		LTTO_IR_STORAGE workingBuffer;			//Storage for the currently-being-captured signature.
		bool signatureWaiting;					//Whether or not the signature we're working on has been completed.
};

#endif
