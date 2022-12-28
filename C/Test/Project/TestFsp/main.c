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
    eFSP_MSGETST_ExeTest();
    eFSP_MSGDTST_ExeTest();
    eFSP_MSGDMSGETST_ExeTest();
    eFSP_MSGTXTST_ExeTest();
    eFSP_MSGRXTST_ExeTest();
    eFSP_MSGRXMSGTXTST_ExeTest();

    return 0;
}
