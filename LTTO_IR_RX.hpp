#include "LTTO_IR_RX_RAW.hpp"

#include "LTTO_IR_Storage.hpp"

#ifndef LTTO_IR_RX_H
#define LTTO_IR_RX_H

enum class eLTTO_IR_RX_PACKETREADY {
	//Which type or types of packet have been received and are waiting
	NONE,
	SINGLE,
	MULTIBYTE,
	BOTH
};

class LTTO_IR_RX {
	public:
		//Initialization and low-level hardware interfacing
		LTTO_IR_RX(void);		//Constructor, sets up the library
		void Tick1ms(void);		//Call this once a millisecond for timekeeping within the library
		void reset(void);		//Call this if things go wonky, to get back to a known-good state
		/*
			Whenever a capture cycle is complete, call this function with the time that the IR carrier was present and not-present, in quarter-milliseconds.
			Will return an enum at the end, with info about whether this capture cycle completes a packet or if one had already been waiting from a previous cycle.
		*/
		eLTTO_IR_RX_PACKETREADY newSample(unsigned int qmsOn, unsigned int qmsOff);

		//And now the user-facing functions!
		eLTTO_IR_RX_PACKETREADY signaturesWaiting(void);	//Get an enum about any packets waiting in the RX buffer
		bool getSignature(LTTO_IR_SIGNATURE &output);		//Get a single signature that has been received. Will return true if there was one waiting.
		bool getMultibyte(LTTO_IR_MULTIBYTE &output);		//Get a multibyte message that has been received. Will return true if there was one waiting.
	private:
		LTTO_IR_RX_RAW RX;		//The class that handles the lower-level bits of receiving a signature.

		LTTO_IR_SIGNATURE signatureBuffer;	//Temporary storage for a single signature that has been captured.

		LTTO_IR_MULTIBYTE multibyteBuffer;	//Temporary storage for a multibyte message that has been captured.
		uint8_t multibyteAge;				//How many milliseconds have passed since the last byte of a multibyte message has been received, so we can fail if it's been too long.
		bool multibyteComplete;				//Whether or not the multibyte message has been completed.

		void multibyteReset(void);			//Clears the multibyte buffer and resets the age and completion status.
};

#endif
