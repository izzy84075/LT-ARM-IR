#ifndef LTTO_IR_TX_IMP_H
#define LTTO_IR_TX_IMP_H

void LTTO_IR_TX_update_IRLED(bool state);		//This function manipulates the data modulation pin for IR data. It does NOT handle carrier generation! Will need tweaked for whatever hardware this is running on.

#endif