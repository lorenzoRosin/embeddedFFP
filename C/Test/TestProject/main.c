#include "eFSPCrcTest.h"
#include "eFSPCrcDigestTest.h"
#include "eFSPCircularQueueTest.h"
#include "eFSPDataPackTest.h"
#include "eFSPDataUnPackTest.h"
#include "eFSPDataPackUnPackTest.h"
#include "eFSPByteStufferTest.h"
#include "eFSPByteUnStufferTest.h"
#include "eFSPDataStuffUnStuffTest.h"
int main(void);


int main(void)
{
    /* Start testing */
    crc32Test();
    cUCrcDigestTest();
    circularQueueTest();
    dataPackTest();
    dataUnPackTest();
	dataPackUnPackTest();
    byteStufferTest();
    byteUnStufferTest();
    dataStuffUnStuffTest();

    return 0;
}
