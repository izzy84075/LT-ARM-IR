#include "LTTO_IR.hpp"

void LTTO_IR::reset(void) {
    headerType = eLTTO_IR_HEADERTYPE::NORMAL;
    bitCount = eLTTO_IR_BITCOUNT::FIVE;
    SFP = eLTTO_IR_SFP::SHORT;
    data = 0;
}

LTTO_IR& LTTO_IR::operator=(const LTTO_IR_SIGNATURE& from) {
    data = 0;
    switch(from.sigType) {
        case eLTTO_IR_SIGNATURETYPE::LTTO_BEACON:
            headerType = eLTTO_IR_HEADERTYPE::BEACON;
            bitCount = eLTTO_IR_BITCOUNT::FIVE;
            SFP = eLTTO_IR_SFP::SHORT;
            break;
        case eLTTO_IR_SIGNATURETYPE::LTAR_BEACON:
            headerType = eLTTO_IR_HEADERTYPE::BEACON;
            bitCount = eLTTO_IR_BITCOUNT::NINE;
            SFP = eLTTO_IR_SFP::SHORT;
            break;
        case eLTTO_IR_SIGNATURETYPE::TAG:
            headerType = eLTTO_IR_HEADERTYPE::NORMAL;
            bitCount = eLTTO_IR_BITCOUNT::SEVEN;
            SFP = eLTTO_IR_SFP::MEDIUM;
            break;
        case eLTTO_IR_SIGNATURETYPE::MULTIBYTE_PTYPE:
            headerType = eLTTO_IR_HEADERTYPE::NORMAL;
            bitCount = eLTTO_IR_BITCOUNT::NINE;
            SFP = eLTTO_IR_SFP::SHORT;
            break;
        case eLTTO_IR_SIGNATURETYPE::MULTIBYTE_DATA:
            headerType = eLTTO_IR_HEADERTYPE::NORMAL;
            bitCount = eLTTO_IR_BITCOUNT::EIGHT;
            SFP = eLTTO_IR_SFP::SHORT;
            break;
        case eLTTO_IR_SIGNATURETYPE::MULTIBYTE_CSUM:
            headerType = eLTTO_IR_HEADERTYPE::NORMAL;
            bitCount = eLTTO_IR_BITCOUNT::NINE;
            SFP = eLTTO_IR_SFP::LONG;
            data |= 0x0100;
            break;
        default:
            break;
    }

    data |= from.data;

    return *this;
}

bool LTTO_IR::convertTo(LTTO_IR_SIGNATURE *output) {
    switch(headerType) {
        case eLTTO_IR_HEADERTYPE::NORMAL:
            //Tag, multibyte PType, multibyte data, or multibyte CSum
            switch(bitCount) {
                case eLTTO_IR_BITCOUNT::SEVEN:
                    //Tag
                    output->sigType = eLTTO_IR_SIGNATURETYPE::TAG;
                    break;
                case eLTTO_IR_BITCOUNT::EIGHT:
                    //Multibyte data
                    output->sigType = eLTTO_IR_SIGNATURETYPE::MULTIBYTE_DATA;
                    break;
                case eLTTO_IR_BITCOUNT::NINE:
                    //Multibyte PType or multibyte CSum
                    if(data & 0x0100) {
                        //Multibyte CSum
                        output->sigType = eLTTO_IR_SIGNATURETYPE::MULTIBYTE_CSUM;
                    } else {
                        //Multibyte PType
                        output->sigType = eLTTO_IR_SIGNATURETYPE::MULTIBYTE_PTYPE;
                    }
                    break;
                default:
                    //Invalid
                    output->sigType = eLTTO_IR_SIGNATURETYPE::INVALID;
                    return false;
            }
            break;
        case eLTTO_IR_HEADERTYPE::BEACON:
            //LTTO beacon or LTAR beacon
            switch(bitCount) {
                case eLTTO_IR_BITCOUNT::FIVE:
                    //LTTO beacon
                    output->sigType = eLTTO_IR_SIGNATURETYPE::LTTO_BEACON;
                    break;
                case eLTTO_IR_BITCOUNT::NINE:
                    //LTAR beacon
                    output->sigType = eLTTO_IR_SIGNATURETYPE::LTAR_BEACON;
                    break;
                default:
                    output->sigType = eLTTO_IR_SIGNATURETYPE::INVALID;
                    return false;
            }
            break;
    }

    output->data = data;

    return true;
}

