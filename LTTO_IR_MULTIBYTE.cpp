#include "LTTO_IR_MULTIBYTE.hpp"

void LTTO_IR_MULTIBYTE::reset(void) {
    size = 0;
}

bool LTTO_IR_MULTIBYTE::appendByte(uint8_t what) {
    if(size < 23) {
        data[size] = what;
        runningChecksum += what;
        size++;
        return true;
    }
    return false;
}
