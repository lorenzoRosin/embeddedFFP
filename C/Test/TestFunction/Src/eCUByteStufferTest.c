/**
 * @file eCUByteStufferTest.c
 *
 */
#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6", "MISRAC2004-17.4_b", "CERT-STR32-C"
    /* Suppressed for code clarity in test execution*/
#endif


/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCUByteStufferTest.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void byteStuffTestBadPointer(void);
static void byteStuffTestBadInit(void);
static void byteStuffTestBadParamEntr(void);
static void byteStuffTestBadParamStatus(void);
static void byteStuffTestOutOfMem(void);
static void byteStuffTestGeneral(void);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void byteStufferTest(void)
{
	(void)printf("\n\nBYTE STUFFER TEST START \n\n");

    byteStuffTestBadPointer();
    byteStuffTestBadInit();
    byteStuffTestBadParamEntr();
    byteStuffTestBadParamStatus();
    byteStuffTestOutOfMem();
    byteStuffTestGeneral();

    (void)printf("\n\nBYTE STUFFER TEST END \n\n");
}





/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void byteStuffTestBadPointer(void)
{
    /* Local variable */
    e_eCU_BStuffCtx ctx;
    uint8_t  memArea[5u];
    uint32_t varTemp32;


    /* Function */
    if( ECU_RES_BADPOINTER == bStufferInitCtx(NULL, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadPointer 1  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == bStufferInitCtx(&ctx, NULL, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadPointer 2  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == bStufferReset(NULL) )
    {
        (void)printf("byteStuffTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadPointer 3  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == bStufferGetRemToStuf(NULL, &varTemp32) )
    {
        (void)printf("byteStuffTestBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadPointer 4  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == bStufferGetRemToStuf(&ctx, NULL) )
    {
        (void)printf("byteStuffTestBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadPointer 5  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == bStufferRetriStufChunk(NULL, memArea, sizeof(memArea), &varTemp32 ) )
    {
        (void)printf("byteStuffTestBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadPointer 6  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == bStufferRetriStufChunk(&ctx, NULL, sizeof(memArea), &varTemp32) )
    {
        (void)printf("byteStuffTestBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadPointer 7  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == bStufferRetriStufChunk(&ctx, memArea, sizeof(memArea), NULL) )
    {
        (void)printf("byteStuffTestBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadPointer 8  -- FAIL \n");
    }
}

void byteStuffTestBadInit(void)
{
    /* Local variable */
    e_eCU_BStuffCtx ctx;
    uint8_t  memArea[5u];
    uint32_t varTemp32;

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_NOINITLIB == bStufferReset(&ctx) )
    {
        (void)printf("byteStuffTestBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadInit 1  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == bStufferGetRemToStuf(&ctx, &varTemp32) )
    {
        (void)printf("byteStuffTestBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadInit 2  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == bStufferRetriStufChunk(&ctx, memArea, sizeof(memArea), &varTemp32) )
    {
        (void)printf("byteStuffTestBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadInit 3  -- FAIL \n");
    }
}

void byteStuffTestBadParamEntr(void)
{
    /* Local variable */
    e_eCU_BStuffCtx ctx;
    uint8_t  memArea[5u];
    uint32_t varTemp32;

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_BADPARAM == bStufferInitCtx(&ctx, memArea, 0u) )
    {
        (void)printf("byteStuffTestBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamEntr 1  -- FAIL \n");
    }

    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamEntr 2  -- FAIL \n");
    }


    if( ECU_RES_BADPARAM == bStufferRetriStufChunk(&ctx, memArea, 0u, &varTemp32) )
    {
        (void)printf("byteStuffTestBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamEntr 3  -- FAIL \n");
    }
}

void byteStuffTestBadParamStatus(void)
{
    /* Local variable */
    e_eCU_BStuffCtx ctx;
    uint8_t  memArea[5u];
    uint32_t varTemp32;

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadParamStatus 1  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 1  -- FAIL \n");
    }

    ctx.memAreaSize = 0u;
    if( ECU_RES_BADPARAM == bStufferGetRemToStuf(&ctx, &varTemp32) )
    {
        (void)printf("byteStuffTestBadParamStatus 2  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 2  -- FAIL \n");
    }


    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadParamStatus 3  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 3  -- FAIL \n");
    }

    ctx.memArea = NULL;
    if( ECU_RES_BADPARAM == bStufferGetRemToStuf(&ctx, &varTemp32) )
    {
        (void)printf("byteStuffTestBadParamStatus 4  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 4  -- FAIL \n");
    }

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadParamStatus 5  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 5  -- FAIL \n");
    }

    ctx.memAreaCntr = ctx.memAreaSize + 1u;
    if( ECU_RES_BADPARAM == bStufferGetRemToStuf(&ctx, &varTemp32) )
    {
        (void)printf("byteStuffTestBadParamStatus 6  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 6  -- FAIL \n");
    }

     /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadParamStatus 7  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 7  -- FAIL \n");
    }

    ctx.needEof = false;
    if( ECU_RES_BADPARAM == bStufferGetRemToStuf(&ctx, &varTemp32) )
    {
        (void)printf("byteStuffTestBadParamStatus 8  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 8  -- FAIL \n");
    }


     /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadParamStatus 9  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 9  -- FAIL \n");
    }

    ctx.needSof = false;
    ctx.needEof = false;
    ctx.memAreaCntr = ctx.memAreaSize - 1u;
    if( ECU_RES_BADPARAM == bStufferGetRemToStuf(&ctx, &varTemp32) )
    {
        (void)printf("byteStuffTestBadParamStatus 10 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 10 -- FAIL \n");
    }


     /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadParamStatus 11 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 11 -- FAIL \n");
    }

    ctx.needSof = true;
    ctx.precedentToCheck = true;
    if( ECU_RES_BADPARAM == bStufferGetRemToStuf(&ctx, &varTemp32) )
    {
        (void)printf("byteStuffTestBadParamStatus 12 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 12 -- FAIL \n");
    }

     /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadParamStatus 13 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 13 -- FAIL \n");
    }

    ctx.needEof = false;
    ctx.precedentToCheck = true;
    if( ECU_RES_BADPARAM == bStufferGetRemToStuf(&ctx, &varTemp32) )
    {
        (void)printf("byteStuffTestBadParamStatus 14 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 14 -- FAIL \n");
    }

     /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadParamStatus 15 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 15 -- FAIL \n");
    }

    ctx.needSof = true;
    ctx.memAreaCntr = 1u;
    if( ECU_RES_BADPARAM == bStufferGetRemToStuf(&ctx, &varTemp32) )
    {
        (void)printf("byteStuffTestBadParamStatus 16 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 16 -- FAIL \n");
    }

     /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadParamStatus 17 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 17 -- FAIL \n");
    }

    ctx.memAreaCntr = 0u;
    ctx.precedentToCheck = true;
    if( ECU_RES_BADPARAM == bStufferGetRemToStuf(&ctx, &varTemp32) )
    {
        (void)printf("byteStuffTestBadParamStatus 18 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 18 -- FAIL \n");
    }


     /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestBadParamStatus 19 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 19 -- FAIL \n");
    }

    ctx.precedentToCheck = true;
    ctx.memAreaCntr = 1u;
    ctx.memArea[ctx.memAreaCntr - 1u] = 0xFFu;
    if( ECU_RES_BADPARAM == bStufferGetRemToStuf(&ctx, &varTemp32) )
    {
        (void)printf("byteStuffTestBadParamStatus 20 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestBadParamStatus 20 -- FAIL \n");
    }
}

void byteStuffTestOutOfMem(void)
{
    /* Local variable */
    e_eCU_BStuffCtx ctx;
    uint8_t  memArea[5u];
    uint8_t  memAreaFinalChunk[50u];
    uint32_t varTemp32;

    /* Init variable */
    memArea[0u] = ECU_SOF;
    memArea[1u] = ECU_EOF;
    memArea[2u] = ECU_ESC;
    memArea[3u] = 0x12u;
    memArea[4u] = 0xFFu;

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestOutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestOutOfMem 1  -- FAIL \n");
    }

    if( ECU_RES_OK == bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[0], 1u, &varTemp32) )
    {
        (void)printf("byteStuffTestOutOfMem 2  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestOutOfMem 2  -- FAIL \n");
    }

    if( ECU_RES_OK == bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[1], 1u, &varTemp32) )
    {
        (void)printf("byteStuffTestOutOfMem 3  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestOutOfMem 3  -- FAIL \n");
    }

    if( ECU_RES_OK == bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[2], 1u, &varTemp32) )
    {
        (void)printf("byteStuffTestOutOfMem 4  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestOutOfMem 4  -- FAIL \n");
    }

    if( ECU_RES_OK == bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[3], 7u, &varTemp32) )
    {
        (void)printf("byteStuffTestOutOfMem 5  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestOutOfMem 5  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[10], 1u, &varTemp32) )
    {
        (void)printf("byteStuffTestOutOfMem 6  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestOutOfMem 6  -- FAIL \n");
    }
}

void byteStuffTestGeneral(void)
{
    /* Local variable */
    e_eCU_BStuffCtx ctx;
    uint8_t  memArea[4u];
    uint8_t  memAreaFinalChunk[50u];
    uint8_t  memAreaExpected[9u];
    uint32_t varTemp32;
    uint32_t counter;
    uint32_t remaining;

    /* Init variable */
    memAreaExpected[0u] = ECU_SOF;
    memAreaExpected[1u] = ECU_ESC;
    memAreaExpected[2u] = (uint8_t)~ECU_SOF;
    memAreaExpected[3u] = ECU_ESC;
    memAreaExpected[4u] = (uint8_t)~ECU_EOF;
    memAreaExpected[5u] = 0x12u;
    memAreaExpected[6u] = ECU_ESC;
    memAreaExpected[7u] = (uint8_t)~ECU_ESC;
    memAreaExpected[8u] = ECU_EOF;

    /* Init variable */
    (void)memset(memAreaFinalChunk, 0, sizeof(memAreaFinalChunk));
    memArea[0u] = ECU_SOF;
    memArea[1u] = ECU_EOF;
    memArea[2u] = 0x12u;
    memArea[3u] = ECU_ESC;

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestGeneral 1  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 1  -- FAIL \n");
    }

    e_eCU_Res result = ECU_RES_OK;
    varTemp32 = 0u;
    counter = 0u;

    while( ECU_RES_OK == result )
    {
        result = bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[counter], 1u, &varTemp32);
        if( ( ECU_RES_OUTOFMEM == result) || ( ECU_RES_OK == result) )
        {
            if(varTemp32 <= sizeof(memAreaFinalChunk))
            {
                counter += varTemp32;
            }
            else
            {
                result = ECU_RES_BADPARAM;
            }
        }
    }

    if( ECU_RES_OUTOFMEM == result )
    {
        (void)printf("byteStuffTestGeneral 2  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 2  -- FAIL \n");
    }

    if( 0 == memcmp(memAreaExpected, memAreaFinalChunk, counter) )
    {
        if( sizeof(memAreaExpected) == counter )
        {
            (void)printf("byteStuffTestGeneral 3  -- OK \n");
        }
        else
        {
            (void)printf("byteStuffTestGeneral 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("byteStuffTestGeneral 3  -- FAIL \n");
    }

    /* Init variable */
    (void)memset(memAreaFinalChunk, 0, sizeof(memAreaFinalChunk));
    memArea[0u] = ECU_SOF;
    memArea[1u] = ECU_EOF;
    memArea[2u] = 0x12u;
    memArea[3u] = ECU_ESC;

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestGeneral 4  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 4  -- FAIL \n");
    }

    result = ECU_RES_OK;
    varTemp32 = 0u;
    counter = 0u;

    while( ECU_RES_OK == result )
    {
        result = bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[counter], 2u, &varTemp32);
        if( ( ECU_RES_OUTOFMEM == result) || ( ECU_RES_OK == result) )
        {
            counter += varTemp32;
        }
    }

    if( ECU_RES_OUTOFMEM == result )
    {
        (void)printf("byteStuffTestGeneral 5  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 5  -- FAIL \n");
    }

    if( 0 == memcmp(memAreaExpected, memAreaFinalChunk, counter) )
    {
        if( sizeof(memAreaExpected) == counter )
        {
            (void)printf("byteStuffTestGeneral 6  -- OK \n");
        }
        else
        {
            (void)printf("byteStuffTestGeneral 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("byteStuffTestGeneral 6  -- FAIL \n");
    }

    /* Init variable */
    (void)memset(memAreaFinalChunk, 0, sizeof(memAreaFinalChunk));
    memArea[0u] = ECU_SOF;
    memArea[1u] = ECU_EOF;
    memArea[2u] = 0x12u;
    memArea[3u] = ECU_ESC;

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestGeneral 7  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 7  -- FAIL \n");
    }

    result = ECU_RES_OK;
    varTemp32 = 0u;
    counter = 0u;

    while( ECU_RES_OK == result )
    {
        result = bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[counter], 3u, &varTemp32);
        if( ( ECU_RES_OUTOFMEM == result) || ( ECU_RES_OK == result) )
        {
            counter += varTemp32;
        }
    }

    if( ECU_RES_OUTOFMEM == result )
    {
        (void)printf("byteStuffTestGeneral 8  -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 8  -- FAIL \n");
    }

    if( 0 == memcmp(memAreaExpected, memAreaFinalChunk, counter) )
    {
        if( sizeof(memAreaExpected) == counter )
        {
            (void)printf("byteStuffTestGeneral 9  -- OK \n");
        }
        else
        {
            (void)printf("byteStuffTestGeneral 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("byteStuffTestGeneral 9  -- FAIL \n");
    }


    /* Init variable */
    (void)memset(memAreaFinalChunk, 0, sizeof(memAreaFinalChunk));
    memArea[0u] = ECU_SOF;
    memArea[1u] = ECU_EOF;
    memArea[2u] = 0x12u;
    memArea[3u] = ECU_ESC;

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestGeneral 10 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 10 -- FAIL \n");
    }

    result = ECU_RES_OK;
    varTemp32 = 0u;
    counter = 0u;

    while( ECU_RES_OK == result )
    {
        result = bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[counter], 4u, &varTemp32);
        if( ( ECU_RES_OUTOFMEM == result) || ( ECU_RES_OK == result) )
        {
            counter += varTemp32;
        }
    }

    if( ECU_RES_OUTOFMEM == result )
    {
        (void)printf("byteStuffTestGeneral 11 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 11 -- FAIL \n");
    }

    if( 0 == memcmp(memAreaExpected, memAreaFinalChunk, counter) )
    {
        if( sizeof(memAreaExpected) == counter )
        {
            (void)printf("byteStuffTestGeneral 12 -- OK \n");
        }
        else
        {
            (void)printf("byteStuffTestGeneral 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("byteStuffTestGeneral 12 -- FAIL \n");
    }



    /* Init variable */
    (void)memset(memAreaFinalChunk, 0, sizeof(memAreaFinalChunk));
    memArea[0u] = ECU_SOF;
    memArea[1u] = ECU_EOF;
    memArea[2u] = 0x12u;
    memArea[3u] = ECU_ESC;

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestGeneral 13 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 13 -- FAIL \n");
    }

    result = ECU_RES_OK;
    varTemp32 = 0u;
    counter = 0u;

    while( ECU_RES_OK == result )
    {
        result = bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[counter], 5u, &varTemp32);
        if( ( ECU_RES_OUTOFMEM == result) || ( ECU_RES_OK == result) )
        {
            counter += varTemp32;
        }
    }

    if( ECU_RES_OUTOFMEM == result )
    {
        (void)printf("byteStuffTestGeneral 14 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 14 -- FAIL \n");
    }

    if( 0 == memcmp(memAreaExpected, memAreaFinalChunk, counter) )
    {
        if( sizeof(memAreaExpected) == counter )
        {
            (void)printf("byteStuffTestGeneral 15 -- OK \n");
        }
        else
        {
            (void)printf("byteStuffTestGeneral 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("byteStuffTestGeneral 15 -- FAIL \n");
    }


    /* Init variable */
    (void)memset(memAreaFinalChunk, 0, sizeof(memAreaFinalChunk));
    memArea[0u] = ECU_SOF;
    memArea[1u] = ECU_EOF;
    memArea[2u] = 0x12u;
    memArea[3u] = ECU_ESC;

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestGeneral 16 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 16 -- FAIL \n");
    }

    result = ECU_RES_OK;
    varTemp32 = 0u;
    counter = 0u;

    while( ECU_RES_OK == result )
    {
        result = bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[counter], 6u, &varTemp32);
        if( ( ECU_RES_OUTOFMEM == result) || ( ECU_RES_OK == result) )
        {
            counter += varTemp32;
        }
    }

    if( ECU_RES_OUTOFMEM == result )
    {
        (void)printf("byteStuffTestGeneral 17 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 17 -- FAIL \n");
    }

    if( 0 == memcmp(memAreaExpected, memAreaFinalChunk, counter) )
    {
        if( sizeof(memAreaExpected) == counter )
        {
            (void)printf("byteStuffTestGeneral 18 -- OK \n");
        }
        else
        {
            (void)printf("byteStuffTestGeneral 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("byteStuffTestGeneral 18 -- FAIL \n");
    }



    /* Init variable */
    (void)memset(memAreaFinalChunk, 0, sizeof(memAreaFinalChunk));
    memArea[0u] = ECU_SOF;
    memArea[1u] = ECU_EOF;
    memArea[2u] = 0x12u;
    memArea[3u] = ECU_ESC;

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestGeneral 19 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 19 -- FAIL \n");
    }

    result = ECU_RES_OK;
    varTemp32 = 0u;
    counter = 0u;

    while( ECU_RES_OK == result )
    {
        result = bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[counter], 7u, &varTemp32);
        if( ( ECU_RES_OUTOFMEM == result) || ( ECU_RES_OK == result) )
        {
            counter += varTemp32;
        }
    }

    if( ECU_RES_OUTOFMEM == result )
    {
        (void)printf("byteStuffTestGeneral 20 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 20 -- FAIL \n");
    }

    if( 0 == memcmp(memAreaExpected, memAreaFinalChunk, counter) )
    {
        if( sizeof(memAreaExpected) == counter )
        {
            (void)printf("byteStuffTestGeneral 21 -- OK \n");
        }
        else
        {
            (void)printf("byteStuffTestGeneral 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("byteStuffTestGeneral 21 -- FAIL \n");
    }




    /* Init variable */
    (void)memset(memAreaFinalChunk, 0, sizeof(memAreaFinalChunk));
    memArea[0u] = ECU_SOF;
    memArea[1u] = ECU_EOF;
    memArea[2u] = 0x12u;
    memArea[3u] = ECU_ESC;

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestGeneral 22 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 22 -- FAIL \n");
    }

    result = ECU_RES_OK;
    varTemp32 = 0u;
    counter = 0u;

    while( ECU_RES_OK == result )
    {
        result = bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[counter], 8u, &varTemp32);
        if( ( ECU_RES_OUTOFMEM == result) || ( ECU_RES_OK == result) )
        {
            counter += varTemp32;
        }
    }

    if( ECU_RES_OUTOFMEM == result )
    {
        (void)printf("byteStuffTestGeneral 23 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 23 -- FAIL \n");
    }

    if( 0 == memcmp(memAreaExpected, memAreaFinalChunk, counter) )
    {
        if( sizeof(memAreaExpected) == counter )
        {
            (void)printf("byteStuffTestGeneral 24 -- OK \n");
        }
        else
        {
            (void)printf("byteStuffTestGeneral 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("byteStuffTestGeneral 24 -- FAIL \n");
    }




    /* Init variable */
    (void)memset(memAreaFinalChunk, 0, sizeof(memAreaFinalChunk));
    memArea[0u] = ECU_SOF;
    memArea[1u] = ECU_EOF;
    memArea[2u] = 0x12u;
    memArea[3u] = ECU_ESC;

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestGeneral 25 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 25 -- FAIL \n");
    }

    result = ECU_RES_OK;
    varTemp32 = 0u;
    counter = 0u;

    while( ECU_RES_OK == result )
    {
        result = bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[counter], 9u, &varTemp32);
        if( ( ECU_RES_OUTOFMEM == result) || ( ECU_RES_OK == result) )
        {
            counter += varTemp32;
        }
    }

    if( ECU_RES_OUTOFMEM == result )
    {
        (void)printf("byteStuffTestGeneral 26 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 26 -- FAIL \n");
    }

    if( 0 == memcmp(memAreaExpected, memAreaFinalChunk, counter) )
    {
        if( sizeof(memAreaExpected) == counter )
        {
            (void)printf("byteStuffTestGeneral 27 -- OK \n");
        }
        else
        {
            (void)printf("byteStuffTestGeneral 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("byteStuffTestGeneral 27 -- FAIL \n");
    }



    /* Init variable */
    (void)memset(memAreaFinalChunk, 0, sizeof(memAreaFinalChunk));
    memArea[0u] = ECU_SOF;
    memArea[1u] = ECU_EOF;
    memArea[2u] = 0x12u;
    memArea[3u] = ECU_ESC;

    /* Function */
    if( ECU_RES_OK == bStufferInitCtx(&ctx, memArea, sizeof(memArea)) )
    {
        (void)printf("byteStuffTestGeneral 28 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 28 -- FAIL \n");
    }

    varTemp32 = 0u;
    counter = 0u;
    remaining = 0u;

    result = bStufferGetRemToStuf(&ctx, &remaining);
    if( ECU_RES_OK == result )
    {
        (void)printf("byteStuffTestGeneral 29 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 29 -- FAIL \n");
    }



    while( ( ECU_RES_OK == result ) && ( 0u != remaining ) )
    {
        result = bStufferRetriStufChunk(&ctx, &memAreaFinalChunk[counter], 10u, &varTemp32);
        if( ( ECU_RES_OUTOFMEM == result) || ( ECU_RES_OK == result) )
        {
            counter += varTemp32;
        }

        result = bStufferGetRemToStuf(&ctx, &remaining);
    }

    if( ECU_RES_OK == result )
    {
        (void)printf("byteStuffTestGeneral 30 -- OK \n");
    }
    else
    {
        (void)printf("byteStuffTestGeneral 30 -- FAIL \n");
    }

    if( 0 == memcmp(memAreaExpected, memAreaFinalChunk, counter) )
    {
        if( sizeof(memAreaExpected) == counter )
        {
            (void)printf("byteStuffTestGeneral 31 -- OK \n");
        }
        else
        {
            (void)printf("byteStuffTestGeneral 31 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("byteStuffTestGeneral 31 -- FAIL \n");
    }
}


#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6", "MISRAC2004-17.4_b", "CERT-STR32-C"
#endif