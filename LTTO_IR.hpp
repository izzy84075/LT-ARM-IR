#ifndef LTTO_IR_H
#define LTTO_IR_H

#include "stdint.h"

#include "LTTO_IR_SIGNATURE.hpp"
#include "LTTO_IR_MULTIBYTE.hpp"

enum class eLTTO_IR_HEADERTYPE {
	NORMAL = 0x00,      // 3/6/3 header
	BEACON = 0x01       // 3/6/6 header
};

enum class eLTTO_IR_BITCOUNT {
    //How many bits of data are associated with this signature
	FIVE = 0x00,
	SEVEN = 0x01,
	EIGHT = 0x02,
	NINE = 0x03
};

enum class eLTTO_IR_SFP {
    //How long of a delay should there be between this and the next thing we send?
    //Most things should use "medium", multibyte packets should use "long"
    //Can't remember what uses "short"...
	SHORT = 0x00,
	MEDIUM = 0x01,
	LONG = 0x02
};

typedef struct LTTO_IR {
    //This is a raw LTTO IR packet, with all the nitty gritty details.
	eLTTO_IR_HEADERTYPE headerType;                         //Which header type is associated with this signature
	eLTTO_IR_BITCOUNT bitCount;                             //How many bits are associated with this signature
	eLTTO_IR_SFP SFP;                                       //How long of a delay there should be between this and the next signature
	uint16_t data;                                          //The data associated with this packet, right-aligned

	void reset(void);                                       //Sets the above to NORMAL, FIVE, SHORT and 0, respectively.

	LTTO_IR& operator= (const LTTO_IR_SIGNATURE& from);     //Used for converting an LTTO_IR_SIGNATURE to an LTTO_IR
	bool convertTo(LTTO_IR_SIGNATURE *output);              //Used for converting an LTTO_IR to an LTTO_IR_SIGNATURE.
} LTTO_IR;

#endif
