#include "LTTO_IRRX.hpp"

LTTO_IRRX_RAW::LTTO_IRRX_RAW(void) {
	reset();
}

bool LTTO_IRRX_RAW::newSample(unsigned int qmsOn, unsigned int qmsOff) {

	if(!signatureWaiting) {
		switch(step) {
			default:
			case 0:
				//PRE-SYNC/PRE-SYNC PAUSE.

				if(qmsOn > 10 && qmsOn < 14) {
					//3ms on, PRE-SYNC
					if(qmsOff > 22 && qmsOff < 26) {
						//6ms off, PRE-SYNC PAUSE
						step = 1;
					}
				}
				break;
			case 1:
				//SYNC pulse. Will either be 3ms for most data or 6ms for a beacon.

				if(qmsOn > 22 && qmsOn < 26) {
					//6ms on, beacon SYNC.
					workingBuffer.headerType = eLTTO_IR_HEADERTYPE::BEACON;
				} else if(qmsOn > 10 && qmsOn < 14) {
					//3ms on, data SYNC.
					workingBuffer.headerType = eLTTO_IR_HEADERTYPE::NORMAL;
				} else {
					//Something went wrong. Abort!
					workingBuffer.reset();
					break;
				}

				if(qmsOff > 6 && qmsOff < 10) {
					//2ms off, pause between data bits
					step++;
				} else {
					//Invalid pause. Something happened. Abort!
					workingBuffer.reset();
				}

				break;
			case 2:	//First data bit
			case 3: //Second data bit
			case 4: //Third data bit
			case 5: //Fourth data bit
			case 7: //Sixth data bit
				//Data bits!

				if(qmsOn > 2 && qmsOn < 6) {
					//"Zero" bit, 1ms on
					//Shift the data left by one
					workingBuffer.data <<= 1;
				} else if(qmsOn > 6 && qmsOn < 10) {
					//"One" bit, 2ms on
					//Shift the data left by one and stuff a 1 bit in.
					workingBuffer.data <<= 1;
					workingBuffer.data |= 0x01;
				} else {
					//Not a valid data bit. Abort!
					workingBuffer.reset();
					break;
				}

				if(qmsOff > 6 && qmsOff < 10) {
					//2ms off, pause between data bits
					step++;
				} else {
					//Invalid pause. Something happened. Abort!
					workingBuffer.reset();
				}
				break;
			case 6: //Fifth data bit
			case 8: //Seventh data bit
			case 9: //Eighth data bit
			case 10: //Ninth data bit
				//Might be the last bit of a packet, so handle that correctly.

				if(qmsOn > 2 && qmsOn < 6) {
					//"Zero" bit, 1ms on
					//Shift the data left by one
					workingBuffer.data <<= 1;
				} else if(qmsOn > 6 && qmsOn < 10) {
					//"One" bit, 2ms on
					//Shift the data left by one and stuff a 1 bit in.
					workingBuffer.data <<= 1;
					workingBuffer.data |= 0x01;
				} else {
					//Not a valid data bit. Abort!
					workingBuffer.reset();
					break;
				}

				if(qmsOff > 6 && qmsOff < 10) {
					//2ms off, pause between data bits
					step++;
				} else if(qmsOff > 14) {
					//End of the packet. Figure out how many bits we captured and store that.
					switch(step) {
						case 6:
							workingBuffer.bitCount = eLTTO_IR_BITCOUNT::FIVE;
							break;
						case 8:
							workingBuffer.bitCount = eLTTO_IR_BITCOUNT::SEVEN;
							break;
						case 9:
							workingBuffer.bitCount = eLTTO_IR_BITCOUNT::EIGHT;
							break;
						case 10:
							workingBuffer.bitCount = eLTTO_IR_BITCOUNT::NINE;
							break;
					}
					step = 0;
					signatureWaiting = true;
				} else {
					//Invalid pause. Something happened. Abort!
					workingBuffer.reset();
				}
				break;
		}
	}

	return signatureWaiting;
}

bool LTTO_IRRX_RAW::isSignatureWaiting(void) {
	return signatureWaiting;
}

LTTO_IR LTTO_IRRX_RAW::getSignature(void) {
	LTTO_IR tempBuffer = workingBuffer;
	workingBuffer.reset();
	signatureWaiting = false;
	return tempBuffer;
}

void LTTO_IRRX_RAW::reset(void) {
	raw = 0;
	step = 0;
	signatureWaiting = false;
	workingBuffer.reset();
}
