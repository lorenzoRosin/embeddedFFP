#include "eFSPByteStufferTest.h"
#include "eFSPByteUnStufferTest.h"
#include "eFSPDataStuffUnStuffTest.h"
int main(void);


int main(void)
{
    /* Start testing */
    byteStufferTest();
    byteUnStufferTest();
    dataStuffUnStuffTest();

    return 0;
}
