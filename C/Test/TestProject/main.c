#include "eFSPMsgDecoderTest.h"
#include "eFSPMsgDeEncoderTest.h"
#include "eFSPMsgEncoderTest.h"
#include "eFSPMsgReceiverTest.h"
#include "eFSPMsgRxTransmitterTest.h"
#include "eFSPMsgTransmitterTest.h"

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
