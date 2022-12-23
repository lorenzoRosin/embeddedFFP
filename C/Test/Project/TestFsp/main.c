#include "eFSP_MSGETST.h"
#include "eFSP_MSGDTST.h"
#include "eFSP_MSGDMSGETST.h"
#include "eFSP_MSGTXTST.h"
#include "eFSP_MSGRXTST.h"
#include "eFSP_MSGRXMSGTXTST.h"


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
