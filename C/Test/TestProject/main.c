#include "eFSPMsgDecoderTest.h"
#include "eFSPMsgEncoderTest.h"
#include "eFSPMsgReceiverTest.h"
#include "eFSPMsgTransmitterTest.h"
#include "eFSPMsgDeEncoderTest.h"
#include "eFSPMsgRxTransmitterTest.h"

int main(void);


int main(void)
{
    /* Start testing */
    msgEncoderTest();
    msgDecoderTest();
    msgDeEncoderTest();
    msgReceiverTest();
    msgTransmitterTest();
    msgRxTransmitterTest();

    return 0;
}
