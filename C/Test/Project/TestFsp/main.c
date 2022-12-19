#include "eFSPMsgEncoderTest.h"
#include "eFSPMsgDecoderTest.h"
#include "eFSPMsgDeEncoderTest.h"
#include "eFSPMsgTransmitterTest.h"
#include "eFSPMsgReceiverTest.h"
#include "eFSPMsgRxTransmitterTest.h"


int main(void);


int main(void)
{
    /* Start testing */
    //  eFSP_TEST_msgEncoder();
    //  eFSP_TEST_msgDecoder();
    //  eFSP_TEST_msgDeEncoder();
    //  eFSP_TEST_msgTransmitter();
    //  eFSP_TEST_msgReceiver();
    eFSP_TEST_msgRxTransmitter();

    return 0;
}
