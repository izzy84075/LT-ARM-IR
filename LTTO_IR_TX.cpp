#include "LTTO_IR_TX.hpp"

LTTO_IR_TX::LTTO_IR_TX() {
	reset();
}

void LTTO_IR_TX::reset(void) {
	timer = 0;
	workingBuffer.reset();
	multibyteBuffer.reset();
	busy = false;
	step = 0;
	dataCounter = 0;
	multibytePosition = 0;
	LTTO_IR_TX_update_IRLED(false);
}

void LTTO_IR_TX::Tick1ms(void) {
	if(timer) {
		timer--;
	}

	if(busy && !timer) {
		switch(step) {
			case 0:
				//PRE-SYNC
				//3ms on
				LTTO_IR_TX_update_IRLED(true);
				timer = 3;

				step++;
				break;
			case 1:
				//PRE-SYNC PAUSE
				//6ms off
				LTTO_IR_TX_update_IRLED(false);
				timer = 6;

				step++;
				break;
			case 2:
				//SYNC
				//3ms or 6ms on
				LTTO_IR_TX_update_IRLED(true);
				switch(workingBuffer.headerType) {
					case eLTTO_IR_HEADERTYPE::NORMAL:
						timer = 3;
						break;
					case eLTTO_IR_HEADERTYPE::BEACON:
						timer = 6;
						break;
				}

				//Set up the counter for how many data bits we have to send
				switch(workingBuffer.bitCount) {
					case eLTTO_IR_BITCOUNT::FIVE:
						dataCounter = 5;
						break;
					case eLTTO_IR_BITCOUNT::SEVEN:
						dataCounter = 7;
						break;
					case eLTTO_IR_BITCOUNT::EIGHT:
						dataCounter = 8;
						break;
					case eLTTO_IR_BITCOUNT::NINE:
						dataCounter = 9;
						break;
				}

				step++;
				break;
			case 3:
				//Data bit pause
				//2ms off
				LTTO_IR_TX_update_IRLED(false);
				timer = 2;

				step++;
				break;
			case 4:
				//Data bit
				//1ms or 2ms on
				LTTO_IR_TX_update_IRLED(true);

				if(workingBuffer.data & 0x01) {
					//This is a "One" bit
					timer = 2;
				} else {
					//This is a "Zero" bit
					timer = 1;
				}

				//Figure out whether we're done, shift the data right one if not
				dataCounter--;
				if(dataCounter != 0) {
					//Not done yet
					//Shift the data right one and go back to the data bit pause
					workingBuffer.data >>= 1;
					step = 3;
				} else {
					//Done. On to the SFP!
					step++;
				}

				break;
			case 5:
				//Special Format Pause
				//25ms, 56ms or 80ms off

				switch(workingBuffer.SFP) {
					case eLTTO_IR_SFP::SHORT:
						timer = 25;
						break;
					case eLTTO_IR_SFP::MEDIUM:
						timer = 56;
						break;
					case eLTTO_IR_SFP::LONG:
						timer = 80;
						break;
				}

				//Fall through into default: so that we can queue up another packet while waiting for the SFP to finish.
			default:
				//Either we just finished a packet or something went wrong. Turn the LED off(Just to be safe) and clean up.
				LTTO_IR_TX_update_IRLED(false);
				busy = false;
				workingBuffer.reset();
				step = 0;
				break;
		}
	}

	tryAdvancingMultibyte();
}

bool LTTO_IR_TX::enqueue(eLTTO_IR_SIGNATURETYPE sigType, uint16_t data) {
	LTTO_IR_SIGNATURE temp;

	temp.sigType = sigType;
	temp.data = data;

	return enqueue(temp);
}

bool LTTO_IR_TX::enqueue(LTTO_IR_SIGNATURE &signature) {
	if(!isBusy()) {
		workingBuffer = signature;
		step = 0;
		busy = true;
		return true;
	}
	return false;
}

bool LTTO_IR_TX::enqueue(LTTO_IR_MULTIBYTE &structure) {
	if(!multibyteBuffer.size && structure.size) {
		multibyteBuffer = structure;
		tryAdvancingMultibyte();
		return true;
	}
	return false;
}

bool LTTO_IR_TX::enqueue(LTTO_IR_STORAGE &signature) {
	if(!isBusy()) {
		workingBuffer = signature;
		step = 0;
		busy = true;
		return true;
	}
	return false;
}

bool LTTO_IR_TX::isBusy(void) {
	if(busy || multibyteBuffer.size) {
		return true;
	}
	return false;
}

bool LTTO_IR_TX::tryAdvancingMultibyte(void) {
	if(!busy && multibyteBuffer.size) {
		if(multibytePosition == 0) {
			enqueue(eLTTO_IR_SIGNATURETYPE::MULTIBYTE_PTYPE, multibyteBuffer.data[multibytePosition++]);
		} else if(multibytePosition < multibyteBuffer.size) {
			enqueue(eLTTO_IR_SIGNATURETYPE::MULTIBYTE_DATA, multibyteBuffer.data[multibytePosition++]);
		} else {
			enqueue(eLTTO_IR_SIGNATURETYPE::MULTIBYTE_CSUM, multibyteBuffer.runningChecksum);
			multibyteBuffer.size = 0;
			multibytePosition = 0;
		}
		return true;
	}
	return false;
}
