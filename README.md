Despite the project name saying ARM, this is actually a pretty much universal library for LTTO-style IR transmission/reception.

*LTTO_IR_Storage* files have to do with storage signatures/messages in various formats, of varying detail and memory usage.
**LTTO_IR_Storage_Raw(Which contains the LTTO_IR_STORAGE class) is the lowest level of abstraction. It stores all the nitty-gritty details, and is used internally by the TX and RX code.
**LTTO_IR_Storage_Signature is a single signature abstracted out to a simple signature type(Tag, beacon, etc.) and the data to go with that type.
**LTTO_IR_Storage_Multibyte is a block of messages, starting with a PacketType, followed by multiple data bytes, and ending with a checksum. The checksum is calculated automatically internally, and handled automatically by the TX and RX code.

*LTTO_IR_RX* files handle the reception of LTTO-style IR transmissions.
**LTTO_IR_RX_RAW handles the low-level reception of a single signature. It only deals in LTTO_IR_STORAGE, it does not convert to the simpler formats.
**LTTO_IR_RX takes the output of LTTO_IR_RX_RAW and handles converting the LTTO_IR_STORAGE results into LTTO_IR_Storage_Signature or LTTO_IR_Storage_Multibyte appropriately. This includes caching the Multibyte data until an entire message is assembled, and throwing it out if it gets corrupted or never actually completes.

*LTTO_IR_TX* files handle the transmission of LTTO-style IR packets.
**LTTO_IR_TX takes any of the three storage formats and converts them to a series of pulses on an IO pin to encode data on a carrier.
**LTTO_IR_TX_imp handles the actual control of the IO pin, and is called from the LTTO_IR_TX class.

*LTTO_IR_RXTX combines the LTTO_IR_RX and LTTO_IR_TX classes into one class that handles both transmission and reception.