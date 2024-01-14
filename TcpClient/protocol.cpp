#include "protocol.h"

// Dynamically malloc memory to data
PDU *mkPDU(uint dataLen)
{
    uint len = sizeof(PDU) + dataLen;
    // Continuous memeory
    PDU *pdu = (PDU*)malloc(len);
    memset(pdu,0,len);

    pdu->len = len;
    pdu->dataLen = dataLen;

    return pdu;
}
