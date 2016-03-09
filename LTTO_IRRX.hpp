#include "LTTO_IRRX_RAW.hpp"

#include "LTTO_IR.hpp"
#include "LTTO_IR_SIGNATURE.hpp"
#include "LTTO_IR_MULTIBYTE.hpp"

#ifndef LTTO_IRRX_H
#define LTTO_IRRX_H

enum class eLTTO_IRRX_PACKETREADY {
    //Which type or types of packet have been received and are waiting
	NONE,
	SINGLE,
	MULTIBYTE,
	BOTH
};

class LTTO_IRRX {
	public:
	    //Initialization and low-level hardware interfacing
		LTTO_IRRX(void);        //Constructor, sets up the library
		void Tick1ms(void);     //Call this once a millisecond for timekeeping within the library
		void reset(void);       //Call this if things go wonky, to get back to a known-good state
        /*
            Whenever a capture cycle is complete, call this function with the time that the IR carrier was present and not-present, in quarter-milliseconds.
            Will return an enum at the end, with info about whether this capture cycle completes a packet or if one had already been waiting from a previous cycle.
        */
		eLTTO_IRRX_PACKETREADY newSample(unsigned int qmsOn, unsigned int qmsOff);

        //And now the user-facing functions!
		eLTTO_IRRX_PACKETREADY signaturesWaiting(void); //Get an enum about any packets waiting in the RX buffer
		bool getSignature(LTTO_IR_SIGNATURE &output);   //Get a single signature that has been received. Will return true if there was one waiting.
		bool getMultibyte(LTTO_IR_MULTIBYTE &output);   //Get a multibyte message that has been received. Will return true if there was one waiting.
	private:
		LTTO_IRRX_RAW RX;       //The class that handles the lower-level bits of receiving a signature.

		LTTO_IR_SIGNATURE signatureBuffer;

		LTTO_IR_MULTIBYTE multibyteBuffer;
		uint8_t multibyteAge;
		bool multibyteComplete;

		void multibyteReset(void);
};

#endif
