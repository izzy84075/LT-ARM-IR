#include "stdint.h"

#ifndef LTTO_IR_MULTIBYTE_H
#define LTTO_IR_MULTIBYTE_H

struct LTTO_IR_MULTIBYTE {
	uint8_t data[22];               //A buffer for the data within the packet, including the packet type.
	uint8_t runningChecksum;        //The checksum for all the data currently in the buffer.
	uint8_t size;                   //How much data is in the buffer.

	void reset(void);               //Call this to clear the buffer and reset the checksum/size.

	bool appendByte(uint8_t what);  //Add a byte to the buffer. The first byte after a reset is the packet type, every subsequent byte is data. Checksums should be left out, as they're handled automatically.
};

#endif
