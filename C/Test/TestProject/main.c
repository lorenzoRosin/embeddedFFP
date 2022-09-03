#include "eCUCrcTest.h"
#include "eCUCrcDigestTest.h"
#include "eCUCircularQueueTest.h"
#include "eCUDataPackTest.h"
#include "eCUDataUnPackTest.h"
#include "eCUDataPackUnPackTest.h"
#include "eCUByteStufferTest.h"
#include "eCUByteUnStufferTest.h"
#include "eCUDataStuffUnStuffTest.h"
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
