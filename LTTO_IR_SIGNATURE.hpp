#include "stdint.h"

#ifndef LTTO_IR_SIGNATURE_H
#define LTTO_IR_SIGNATURE_H

enum class eLTTO_IR_SIGNATURETYPE {
	LTTO_BEACON,
	LTAR_BEACON,
	TAG,
	MULTIBYTE_PTYPE,
	MULTIBYTE_DATA,
	MULTIBYTE_CSUM,
	INVALID
};

typedef struct LTTO_IR_SIGNATURE {
	eLTTO_IR_SIGNATURETYPE sigType;         //What type of signature this is.
	uint16_t data;                          //The data to be sent as part of the signature. Data should be right-aligned, and will be truncated to match the signature type.

	void reset(void);                       //Sets the sigType to INVALID and the data to 0.
} LTTO_IR_SIGNATURE;

#endif
