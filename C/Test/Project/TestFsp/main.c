#include "eFSPMsgEncoderTest.h"
#include "eFSPMsgDecoderTest.h"
#include "eFSPMsgDeEncoderTest.h"
// #include "eFSPMsgReceiverTest.h"
// #include "eFSPMsgRxTransmitterTest.h"
// #include "eFSPMsgTransmitterTest.h"

int main(void);


int main(void)
{
    /* Start testing */
    msgEncoderTest();
    msgDecoderTest();
    msgDeEncoderTest();
    // msgTransmitterTest();
    // msgReceiverTest();
    // msgRxTransmitterTest();

    return 0;
}
