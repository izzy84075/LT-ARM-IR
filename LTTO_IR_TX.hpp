#include "LTTO_IR_Storage.hpp"
#include "LTTO_IR_TX_imp.hpp"

#ifndef LTTO_IR_TX_H
#define LTTO_IR_TX_H

class LTTO_IR_TX {
	public:
		//Initialization and low-level hardware interfacing
		LTTO_IR_TX();			//Constructor, sets up the library
		void Tick1ms(void);		//Call this once a millisecond, for timekeeping within the library
		void reset(void);		//Call this if things go wonky, to get back to a known-good state

		//And now the user-facing functions!

		//All forms of the enqueue() function return a bool telling you whether the signature queued up successfully or not.
		//If this is an important signature to go out, make sure to wait until the library isn't busy by calling isBusy() first.
		bool enqueue(eLTTO_IR_SIGNATURETYPE sigType, uint16_t data);	//Use this to queue up a signature without having prepared it ahead of time
		bool enqueue(LTTO_IR_SIGNATURE &signature);						//Queue up a pre-prepared single signature
		bool enqueue(LTTO_IR_MULTIBYTE &structure);						//Queue up a pre-prepared multibyte message
		bool enqueue(LTTO_IR_STORAGE &signature);						//Queue up a raw LTTO_IR signature.

		bool isBusy(void);		//Will return true if the library is busy sending something, or false otherwise.
	private:
		LTTO_IR_STORAGE workingBuffer;
		LTTO_IR_MULTIBYTE multibyteBuffer;

		unsigned int timer;
		unsigned int step;
		unsigned int dataCounter;
		unsigned int multibytePosition;
		bool busy;

		bool tryAdvancingMultibyte(void);
};

#endif
