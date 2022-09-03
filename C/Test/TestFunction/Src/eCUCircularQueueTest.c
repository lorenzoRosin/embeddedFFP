/**
 * @file eCUCircularQueueTest.c
 *
 */
#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6", "CERT-STR32-C"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCUCircularQueueTest.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void circularqueueTestBadPointer(void);
static void circularqueueTestBadInit(void);
static void circularqueueTestBadParamEntr(void);
static void circularqueueTestBadParamStatus(void);
static void circularqueueTestOutOfMem(void);
static void circularqueueTestGeneric(void);
static void circularqueueTestCycle(void);

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void circularQueueTest(void)
{
	(void)printf("\n\nCIRCULAR QUEUE TEST START \n\n");

    circularqueueTestBadPointer();
    circularqueueTestBadInit();
    circularqueueTestBadParamEntr();
    circularqueueTestBadParamStatus();
    circularqueueTestOutOfMem();
    circularqueueTestGeneric();
    circularqueueTestCycle();

    (void)printf("\n\nCIRCULAR QUEUE TEST END \n\n");
}





/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void circularqueueTestBadPointer(void)
{
    /* Local variable */
    s_eCU_circQCtx ctx;
    uint8_t  badPointerMempool[5u];
    uint32_t varTemp;

    /* Function */
    if( ECU_RES_BADPOINTER == circQInitCtx(NULL, badPointerMempool, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 1  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == circQInitCtx(&ctx, NULL, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 2  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == circQReset(NULL) )
    {
        (void)printf("circularqueueTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 3  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == circQGetFreeSapce(NULL, &varTemp) )
    {
        (void)printf("circularqueueTestBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 4  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == circQGetFreeSapce(&ctx, NULL) )
    {
        (void)printf("circularqueueTestBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 5  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == circQGetOccupiedSapce(NULL, &varTemp) )
    {
        (void)printf("circularqueueTestBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 6  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == circQGetOccupiedSapce(&ctx, NULL) )
    {
        (void)printf("circularqueueTestBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 7  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == circQInsertData(NULL, badPointerMempool, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 8  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == circQInsertData(&ctx, NULL, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 9  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == circQRetriveData(NULL, badPointerMempool, varTemp) )
    {
        (void)printf("circularqueueTestBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 10 -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == circQRetriveData(&ctx, NULL, varTemp) )
    {
        (void)printf("circularqueueTestBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 11 -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == circQPeekData(NULL, badPointerMempool, varTemp) )
    {
        (void)printf("circularqueueTestBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 12 -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == circQPeekData(&ctx, NULL, varTemp) )
    {
        (void)printf("circularqueueTestBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadPointer 13 -- FAIL \n");
    }
}

void circularqueueTestBadInit(void)
{
    /* Local variable */
    s_eCU_circQCtx ctx;
    uint8_t  badPointerMempool[5u];
    uint32_t varTemp;

    /* Init variable */
    ctx.isInit = true;

    /* Function */
    if( ECU_RES_OK == circQInitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadInit 1  -- FAIL \n");
    }

    /* Init variable */
    ctx.isInit = false;

    if( ECU_RES_NOINITLIB == circQReset(&ctx) )
    {
        (void)printf("circularqueueTestBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadInit 2  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == circQGetFreeSapce(&ctx, &varTemp) )
    {
        (void)printf("circularqueueTestBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadInit 3  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == circQGetOccupiedSapce(&ctx, &varTemp) )
    {
        (void)printf("circularqueueTestBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadInit 4  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == circQInsertData(&ctx, badPointerMempool, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadInit 5  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == circQRetriveData(&ctx, badPointerMempool, varTemp) )
    {
        (void)printf("circularqueueTestBadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadInit 6  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == circQPeekData(&ctx, badPointerMempool, varTemp) )
    {
        (void)printf("circularqueueTestBadInit 7  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadInit 7  -- FAIL \n");
    }
}

void circularqueueTestBadParamEntr(void)
{
    /* Local variable */
    s_eCU_circQCtx ctx;
    uint8_t  badPointerMempool[5u];

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_BADPARAM == circQInitCtx(&ctx, badPointerMempool, 0u) )
    {
        (void)printf("circularqueueTestBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamEntr 1  -- FAIL \n");
    }

    /* Init variable */
    ctx.isInit = true;

    if( ECU_RES_BADPARAM == circQInsertData(&ctx, badPointerMempool, 0u) )
    {
        (void)printf("circularqueueTestBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamEntr 2  -- FAIL \n");
    }

    if( ECU_RES_BADPARAM == circQRetriveData(&ctx, badPointerMempool, 0u) )
    {
        (void)printf("circularqueueTestBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamEntr 3  -- FAIL \n");
    }

    if( ECU_RES_BADPARAM == circQPeekData(&ctx, badPointerMempool, 0u) )
    {
        (void)printf("circularqueueTestBadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamEntr 4  -- FAIL \n");
    }
}

static void circularqueueTestBadParamStatus(void)
{
    /* Local variable */
    s_eCU_circQCtx ctx;
    uint8_t  badPointerMempool[5u];

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == circQInitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestBadParamStatus 1  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamStatus 1  -- FAIL \n");
    }

    /* Init variable */
    ctx.memPoolSize = 0u;

    if( ECU_RES_BADPARAM == circQReset(&ctx) )
    {
        (void)printf("circularqueueTestBadParamStatus 2  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamStatus 2  -- FAIL \n");
    }




    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == circQInitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestBadParamStatus 3  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamStatus 3  -- FAIL \n");
    }

    ctx.memPool = NULL;

    if( ECU_RES_BADPARAM == circQReset(&ctx) )
    {
        (void)printf("circularqueueTestBadParamStatus 4  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamStatus 4  -- FAIL \n");
    }




    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == circQInitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestBadParamStatus 5  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamStatus 5  -- FAIL \n");
    }

    ctx.memPoolFrstFreeIdx = sizeof(badPointerMempool) +1u;

    if( ECU_RES_BADPARAM == circQReset(&ctx) )
    {
        (void)printf("circularqueueTestBadParamStatus 6  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamStatus 6  -- FAIL \n");
    }




    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == circQInitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestBadParamStatus 5  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamStatus 5  -- FAIL \n");
    }

    ctx.memPoolFrstOccIdx = sizeof(badPointerMempool) +1u;

    if( ECU_RES_BADPARAM == circQReset(&ctx) )
    {
        (void)printf("circularqueueTestBadParamStatus 6  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamStatus 6  -- FAIL \n");
    }




    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == circQInitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestBadParamStatus 5  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamStatus 5  -- FAIL \n");
    }

    ctx.memPoolUsedSize = sizeof(badPointerMempool) +1u;

    if( ECU_RES_BADPARAM == circQReset(&ctx) )
    {
        (void)printf("circularqueueTestBadParamStatus 6  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestBadParamStatus 6  -- FAIL \n");
    }
}

void circularqueueTestOutOfMem(void)
{
    /* Local variable */
    s_eCU_circQCtx ctx;
    uint8_t  badPointerMempool[5u];
    uint8_t  retriveData[10u];

    /* Init variable */
    ctx.isInit = false;

    /* Easy one  */
    if( ECU_RES_OK == circQInitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestOutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 1  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == circQPeekData(&ctx, retriveData, sizeof(retriveData)) )
    {
        (void)printf("circularqueueTestOutOfMem 2  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 2  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == circQRetriveData(&ctx, retriveData, sizeof(retriveData)) )
    {
        (void)printf("circularqueueTestOutOfMem 3  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 3  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == circQInsertData(&ctx, retriveData, sizeof(retriveData)) )
    {
        (void)printf("circularqueueTestOutOfMem 4  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 4  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == circQInsertData(&ctx, retriveData, sizeof(badPointerMempool) +1u ) )
    {
        (void)printf("circularqueueTestOutOfMem 5  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 5  -- FAIL \n");
    }

    /* most complete one */
    if( ECU_RES_OK == circQInsertData(&ctx, retriveData, sizeof(badPointerMempool) ) )
    {
        (void)printf("circularqueueTestOutOfMem 6  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 6  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == circQRetriveData(&ctx, retriveData, sizeof(retriveData)) )
    {
        (void)printf("circularqueueTestOutOfMem 7  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 7  -- FAIL \n");
    }

    if( ECU_RES_OK == circQRetriveData(&ctx, retriveData, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestOutOfMem 8  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 8  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == circQRetriveData(&ctx, retriveData, 1u) )
    {
        (void)printf("circularqueueTestOutOfMem 7  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 7  -- FAIL \n");
    }

    if( ECU_RES_OK == circQInsertData(&ctx, retriveData, 2u ) )
    {
        (void)printf("circularqueueTestOutOfMem 8  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 8  -- FAIL \n");
    }

    if( ECU_RES_OK == circQInsertData(&ctx, retriveData, 3u ) )
    {
        (void)printf("circularqueueTestOutOfMem 9  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 9  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == circQInsertData(&ctx, retriveData, 1u ) )
    {
        (void)printf("circularqueueTestOutOfMem 10 -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 10 -- FAIL \n");
    }

    if( ECU_RES_OK == circQRetriveData(&ctx, retriveData, sizeof(badPointerMempool)) )
    {
        (void)printf("circularqueueTestOutOfMem 11 -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestOutOfMem 11 -- FAIL \n");
    }
}

static void circularqueueTestGeneric(void)
{
    /* Local variable */
    s_eCU_circQCtx ctx;
    uint8_t  pointerMempool[10u];
    uint8_t  retriveData[10u] = {0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u};
    uint8_t  insertData[10u]  = {0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u};

    /* Init variable */
    ctx.isInit = false;

    /* Easy one  */
    if( ECU_RES_OK == circQInitCtx(&ctx, pointerMempool, sizeof(pointerMempool) ) )
    {
        (void)printf("circularqueueTestGeneric 1  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestGeneric 1  -- FAIL \n");
    }

    if( ECU_RES_OK == circQInsertData(&ctx, insertData, sizeof(insertData) ) )
    {
        (void)printf("circularqueueTestGeneric 2  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestGeneric 2  -- FAIL \n");
    }

    if( ECU_RES_OK == circQPeekData(&ctx, retriveData, sizeof(retriveData)) )
    {
        if( 0 == memcmp(insertData, retriveData, sizeof(retriveData) ) )
        {
            (void)printf("circularqueueTestGeneric 3  -- OK \n");
        }
        else
        {
            (void)printf("circularqueueTestGeneric 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("circularqueueTestGeneric 3  -- FAIL \n");
    }

    if( ECU_RES_OK == circQRetriveData(&ctx, retriveData, 7u ) )
    {
        if( 0 == memcmp(insertData, retriveData, 7u ) )
        {
            (void)printf("circularqueueTestGeneric 4  -- OK \n");
        }
        else
        {
            (void)printf("circularqueueTestGeneric 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("circularqueueTestGeneric 4  -- FAIL \n");
    }

    if( ECU_RES_OK == circQInsertData(&ctx, insertData, 2u ) )
    {
        (void)printf("circularqueueTestGeneric 5  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestGeneric 5  -- FAIL \n");
    }

    if( ECU_RES_OK == circQRetriveData(&ctx, retriveData, 5u ) )
    {
        if( 0 == memcmp(&insertData[7], retriveData, 3u ) )
        {
            if( 0 == memcmp(&insertData[0], &retriveData[3u], 2u ) )
            {
                (void)printf("circularqueueTestGeneric 6  -- OK \n");
            }
            else
            {
                (void)printf("circularqueueTestGeneric 6  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("circularqueueTestGeneric 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("circularqueueTestGeneric 6  -- FAIL \n");
    }
}

static void circularqueueTestCycle(void)
{
    /* Local variable */
    s_eCU_circQCtx ctx;
    uint8_t  pointerMempool[10u];
    uint8_t  retriveData3[3u] = {0u};
    uint8_t  retriveData4[4u] = {0u};
    uint8_t  insertData[7u]  = {0u, 1u, 2u, 3u, 4u, 5u, 6u};
    bool_t   testOk = true;

    /* Init variable */
    ctx.isInit = false;

    /* Easy one  */
    if( ECU_RES_OK == circQInitCtx(&ctx, pointerMempool, sizeof(pointerMempool) ) )
    {
        (void)printf("circularqueueTestCycle 1  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestCycle 1  -- FAIL \n");
    }

    for(uint32_t i = 0u; ( ( i <= 1000u ) && ( true == testOk ) ); i++)
    {
        if( ECU_RES_OK != circQInsertData(&ctx, insertData, sizeof(insertData) ) )
        {
            testOk = false;
        }

        if( ECU_RES_OK == circQRetriveData(&ctx, retriveData3, sizeof(retriveData3) ) )
        {
            if( 0 != memcmp(insertData, retriveData3, sizeof(retriveData3) ) )
            {
                testOk = false;
            }
        }
        else
        {
            testOk = false;
        }

        if( ECU_RES_OK == circQRetriveData(&ctx, retriveData4, sizeof(retriveData4) ) )
        {
            if( 0 != memcmp(&insertData[3], retriveData4, sizeof(retriveData4) ) )
            {
                testOk = false;
            }
        }
        else
        {
            testOk = false;
        }
    }

    if( true == testOk )
    {
        (void)printf("circularqueueTestCycle 2  -- OK \n");
    }
    else
    {
        (void)printf("circularqueueTestCycle 2  -- FAIL \n");
    }


}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6", "CERT-STR32-C"
#endif