/**
 * @file eFSPDataPackTest.c
 *
 */
#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPDataPackTest.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void dataPackTestBadPointer(void);
static void dataPackTestBadInit(void);
static void dataPackTestBadParamEntr(void);
static void dataPackTestBadParamStatus(void);
static void dataPackTestOutOfMem(void);
static void dataPackTestEndianLe(void);
static void dataPackTestEndianBe(void);
static void dataPackTestCycle(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void dataPackTest(void)
{
	(void)printf("\n\nDATA PACK TEST START \n\n");

    dataPackTestBadPointer();
    dataPackTestBadInit();
    dataPackTestBadParamEntr();
    dataPackTestBadParamStatus();
    dataPackTestOutOfMem();
    dataPackTestEndianLe();
    dataPackTestEndianBe();
    dataPackTestCycle();

    (void)printf("\n\nDATA PACK END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void dataPackTestBadPointer(void)
{
    /* Local variable */
    s_eFSP_DataPackCtx ctx;
    uint8_t  badPointerMempool[5u];
    uint32_t varTemp;

    /* Function */
    if( ECU_RES_BADPOINTER == dataPackinitCtx(NULL, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataPackTestBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadPointer 1  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataPackinitCtx(&ctx, NULL, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataPackTestBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadPointer 2  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataPackReset( NULL ) )
    {
        (void)printf("dataPackTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadPointer 3  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataPackGetNPushed( NULL, &varTemp ) )
    {
        (void)printf("dataPackTestBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadPointer 4  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataPackGetNPushed( &ctx, NULL ) )
    {
        (void)printf("dataPackTestBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadPointer 5  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataPackPushArray( NULL, badPointerMempool, sizeof(badPointerMempool) ) )
    {
        (void)printf("dataPackTestBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadPointer 6  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataPackPushArray( &ctx, NULL, sizeof(badPointerMempool) ) )
    {
        (void)printf("dataPackTestBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadPointer 7  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataPackPushU8( NULL, 10u ) )
    {
        (void)printf("dataPackTestBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadPointer 8  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataPackPushU16( NULL, 10u ) )
    {
        (void)printf("dataPackTestBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadPointer 9  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataPackPushU32( NULL, 10u ) )
    {
        (void)printf("dataPackTestBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadPointer 10 -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataPackPushU64( NULL, 10u ) )
    {
        (void)printf("dataPackTestBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadPointer 11 -- FAIL \n");
    }
}

void dataPackTestBadInit(void)
{
    /* Local variable */
    s_eFSP_DataPackCtx ctx;
    uint8_t  badPointerMempool[5u];
    uint32_t varTemp;

    /* Init variable */
    ctx.isInit = true;

    /* Function */
    if( ECU_RES_OK == dataPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataPackTestBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadInit 1  -- FAIL \n");
    }

    /* Init variable */
    ctx.isInit = false;

    if( ECU_RES_NOINITLIB == dataPackReset( &ctx ) )
    {
        (void)printf("dataPackTestBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadInit 2  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == dataPackGetNPushed( &ctx, &varTemp ) )
    {
        (void)printf("dataPackTestBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadInit 3  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == dataPackPushArray( &ctx, badPointerMempool, sizeof(badPointerMempool) ) )
    {
        (void)printf("dataPackTestBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadInit 4  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == dataPackPushU8( &ctx, 10u ) )
    {
        (void)printf("dataPackTestBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadInit 5  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == dataPackPushU16( &ctx, 10u ) )
    {
        (void)printf("dataPackTestBadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadInit 6  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == dataPackPushU32( &ctx, 10u ) )
    {
        (void)printf("dataPackTestBadInit 7  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadInit 7  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == dataPackPushU64( &ctx, 10u ) )
    {
        (void)printf("dataPackTestBadInit 8  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadInit 8  -- FAIL \n");
    }
}

void dataPackTestBadParamEntr(void)
{
    /* Local variable */
    s_eFSP_DataPackCtx ctx;
    uint8_t  badPointerMempool[5u];

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_BADPARAM == dataPackinitCtx(&ctx, badPointerMempool, 0u, true) )
    {
        (void)printf("dataPackTestBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadParamEntr 1  -- FAIL \n");
    }

    /* Init variable */
    ctx.isInit = true;

    if( ECU_RES_BADPARAM == dataPackPushArray( &ctx, badPointerMempool, 0u ) )
    {
        (void)printf("dataPackTestBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadParamEntr 2  -- FAIL \n");
    }
}

void dataPackTestBadParamStatus(void)
{
    /* Local variable */
    s_eFSP_DataPackCtx ctx;
    uint8_t  badPointerMempool[5u];
    uint32_t varTemp;



    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataPackTestBadParamStatus 1  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadParamStatus 1  -- FAIL \n");
    }

    /* Init variable */
    ctx.memPKASize = 0u;

    if( ECU_RES_BADPARAM == dataPackGetNPushed(&ctx, &varTemp) )
    {
        (void)printf("dataPackTestBadParamStatus 2  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadParamStatus 2  -- FAIL \n");
    }



    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataPackTestBadParamStatus 3  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadParamStatus 3  -- FAIL \n");
    }

    /* Init variable */
    ctx.memPKA = NULL;

    if( ECU_RES_BADPARAM == dataPackGetNPushed(&ctx, &varTemp) )
    {
        (void)printf("dataPackTestBadParamStatus 4  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadParamStatus 4  -- FAIL \n");
    }



    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataPackTestBadParamStatus 5  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadParamStatus 5  -- FAIL \n");
    }

    /* Init variable */
    ctx.memPKACntr = ctx.memPKASize + 1u;

    if( ECU_RES_BADPARAM == dataPackGetNPushed(&ctx, &varTemp) )
    {
        (void)printf("dataPackTestBadParamStatus 6  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestBadParamStatus 6  -- FAIL \n");
    }
}

void dataPackTestOutOfMem(void)
{
    /* Local variable */
    s_eFSP_DataPackCtx ctx;
    uint8_t  badPointerMempool[2u];

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataPackTestOutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestOutOfMem 1  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == dataPackPushU64( &ctx, 10u ) )
    {
        (void)printf("dataPackTestOutOfMem 2  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestOutOfMem 2  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == dataPackPushU32( &ctx, 10u ) )
    {
        (void)printf("dataPackTestOutOfMem 3  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestOutOfMem 3  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU16( &ctx, 10u ) )
    {
        (void)printf("dataPackTestOutOfMem 4  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestOutOfMem 4  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == dataPackPushU16( &ctx, 10u ) )
    {
        (void)printf("dataPackTestOutOfMem 5  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestOutOfMem 5  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == dataPackPushU8( &ctx, 10u ) )
    {
        (void)printf("dataPackTestOutOfMem 6  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestOutOfMem 6  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackReset( &ctx ) )
    {
        (void)printf("dataPackTestOutOfMem 7  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestOutOfMem 7  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == dataPackPushArray( &ctx, badPointerMempool, 4u ) )
    {
        (void)printf("dataPackTestOutOfMem 8  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestOutOfMem 8  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU16( &ctx, 10u ) )
    {
        (void)printf("dataPackTestOutOfMem 9  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestOutOfMem 9  -- FAIL \n");
    }
}

void dataPackTestEndianLe(void)
{
    /* Local variable */
    s_eFSP_DataPackCtx ctx;
    uint8_t  badPointerMempool[20u];
    uint32_t varTemp;

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataPackTestEndianLe 1  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianLe 1  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU16( &ctx, 0x1234u ) )
    {
        (void)printf("dataPackTestEndianLe 2  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianLe 2  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackGetNPushed( &ctx, &varTemp ) )
    {
        (void)printf("dataPackTestEndianLe 3  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianLe 3  -- FAIL \n");
    }

    if( 0x02u == varTemp)
    {
        if( ( 0x34u == badPointerMempool[0u] ) && ( 0x12u == badPointerMempool[1u] ) )
        {
            (void)printf("dataPackTestEndianLe 4  -- OK \n");
        }
        else
        {
            (void)printf("dataPackTestEndianLe 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataPackTestEndianLe 4  -- FAIL \n");
    }

    /* Function */
    if( ECU_RES_OK == dataPackReset( &ctx ) )
    {
        (void)printf("dataPackTestEndianLe 5  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianLe 5  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU32( &ctx, 0x12345678u ) )
    {
        (void)printf("dataPackTestEndianLe 6  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianLe 6  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackGetNPushed( &ctx, &varTemp ) )
    {
        (void)printf("dataPackTestEndianLe 7  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianLe 7  -- FAIL \n");
    }

    if( 0x04u == varTemp)
    {
        if( ( 0x78u == badPointerMempool[0u] ) && ( 0x56u == badPointerMempool[1u] ) &&
            ( 0x34u == badPointerMempool[2u] ) && ( 0x12u == badPointerMempool[3u] ) )
        {
            (void)printf("dataPackTestEndianLe 8  -- OK \n");
        }
        else
        {
            (void)printf("dataPackTestEndianLe 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataPackTestEndianLe 8  -- FAIL \n");
    }

    /* Function */
    if( ECU_RES_OK == dataPackReset( &ctx ) )
    {
        (void)printf("dataPackTestEndianLe 9  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianLe 9  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU64( &ctx, 0x123456789ABCDEF0u ) )
    {
        (void)printf("dataPackTestEndianLe 10 -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianLe 10 -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackGetNPushed( &ctx, &varTemp ) )
    {
        (void)printf("dataPackTestEndianLe 11 -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianLe 11 -- FAIL \n");
    }

    if( 0x08u == varTemp)
    {
        if( ( 0xF0u == badPointerMempool[0u] ) && ( 0xDEu == badPointerMempool[1u] ) &&
            ( 0xBCu == badPointerMempool[2u] ) && ( 0x9Au == badPointerMempool[3u] ) &&
            ( 0x78u == badPointerMempool[4u] ) && ( 0x56u == badPointerMempool[5u] ) &&
            ( 0x34u == badPointerMempool[6u] ) && ( 0x12u == badPointerMempool[7u] ))
        {
            (void)printf("dataPackTestEndianLe 12 -- OK \n");
        }
        else
        {
            (void)printf("dataPackTestEndianLe 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataPackTestEndianLe 12 -- FAIL \n");
    }
}

void dataPackTestEndianBe(void)
{
    /* Local variable */
    s_eFSP_DataPackCtx ctx;
    uint8_t  badPointerMempool[20u];
    uint32_t varTemp;

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), false) )
    {
        (void)printf("dataPackTestEndianBe 1  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianBe 1  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU16( &ctx, 0x1234u ) )
    {
        (void)printf("dataPackTestEndianBe 2  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianBe 2  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackGetNPushed( &ctx, &varTemp ) )
    {
        (void)printf("dataPackTestEndianBe 3  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianBe 3  -- FAIL \n");
    }

    if( 0x02u == varTemp)
    {
        if( ( 0x12u == badPointerMempool[0u] ) && ( 0x34u == badPointerMempool[1u] ) )
        {
            (void)printf("dataPackTestEndianBe 4  -- OK \n");
        }
        else
        {
            (void)printf("dataPackTestEndianBe 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataPackTestEndianBe 4  -- FAIL \n");
    }

    /* Function */
    if( ECU_RES_OK == dataPackReset( &ctx ) )
    {
        (void)printf("dataPackTestEndianBe 5  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianBe 5  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU32( &ctx, 0x12345678u ) )
    {
        (void)printf("dataPackTestEndianBe 6  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianBe 6  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackGetNPushed( &ctx, &varTemp ) )
    {
        (void)printf("dataPackTestEndianBe 7  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianBe 7  -- FAIL \n");
    }

    if( 0x04u == varTemp)
    {
        if( ( 0x12u == badPointerMempool[0u] ) && ( 0x34u == badPointerMempool[1u] ) &&
            ( 0x56u == badPointerMempool[2u] ) && ( 0x78u == badPointerMempool[3u] ) )
        {
            (void)printf("dataPackTestEndianBe 8  -- OK \n");
        }
        else
        {
            (void)printf("dataPackTestEndianBe 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataPackTestEndianBe 8  -- FAIL \n");
    }

    /* Function */
    if( ECU_RES_OK == dataPackReset( &ctx ) )
    {
        (void)printf("dataPackTestEndianBe 9  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianBe 9  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU64( &ctx, 0x123456789ABCDEF0u ) )
    {
        (void)printf("dataPackTestEndianBe 10 -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianBe 10 -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackGetNPushed( &ctx, &varTemp ) )
    {
        (void)printf("dataPackTestEndianBe 11 -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestEndianBe 11 -- FAIL \n");
    }

    if( 0x08u == varTemp)
    {
        if( ( 0x12u == badPointerMempool[0u] ) && ( 0x34u == badPointerMempool[1u] ) &&
            ( 0x56u == badPointerMempool[2u] ) && ( 0x78u == badPointerMempool[3u] ) &&
            ( 0x9Au == badPointerMempool[4u] ) && ( 0xBCu == badPointerMempool[5u] ) &&
            ( 0xDEu == badPointerMempool[6u] ) && ( 0xF0u == badPointerMempool[7u] ))
        {
            (void)printf("dataPackTestEndianBe 12 -- OK \n");
        }
        else
        {
            (void)printf("dataPackTestEndianBe 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataPackTestEndianBe 12 -- FAIL \n");
    }
}

void dataPackTestCycle(void)
{
    /* Local variable */
    s_eFSP_DataPackCtx ctx;
    uint8_t  badPointerMempool[20u];
    uint32_t varTemp;

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataPackTestCycle 1  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestCycle 1  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU16( &ctx, 0x1234u ) )
    {
        (void)printf("dataPackTestCycle 2  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestCycle 2  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU16( &ctx, 0x5678u ) )
    {
        (void)printf("dataPackTestCycle 3  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestCycle 3  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackGetNPushed( &ctx, &varTemp ) )
    {
        (void)printf("dataPackTestCycle 4  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestCycle 4  -- FAIL \n");
    }

    if( 0x04u == varTemp)
    {
        if( ( 0x34u == badPointerMempool[0u] ) && ( 0x12u == badPointerMempool[1u] ) &&
            ( 0x78u == badPointerMempool[2u] ) && ( 0x56u == badPointerMempool[3u] )  )
        {
            (void)printf("dataPackTestCycle 5  -- OK \n");
        }
        else
        {
            (void)printf("dataPackTestCycle 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataPackTestCycle 5  -- FAIL \n");
    }

    /* Function */
    if( ECU_RES_OK == dataPackReset( &ctx ) )
    {
        (void)printf("dataPackTestCycle 6  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestCycle 6  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU16( &ctx, 0x1234u ) )
    {
        (void)printf("dataPackTestCycle 7  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestCycle 7  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU16( &ctx, 0x5678u ) )
    {
        (void)printf("dataPackTestCycle 8  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestCycle 8  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackGetNPushed( &ctx, &varTemp) )
    {
        (void)printf("dataPackTestCycle 9  -- OK \n");
    }
    else
    {
        (void)printf("dataPackTestCycle 9  -- FAIL \n");
    }

    if( 0x04u == varTemp)
    {
        if( ( 0x34u == badPointerMempool[0u] ) && ( 0x12u == badPointerMempool[1u] ) &&
            ( 0x78u == badPointerMempool[2u] ) && ( 0x56u == badPointerMempool[3u] )  )
        {
            (void)printf("dataPackTestCycle 10 -- OK \n");
        }
        else
        {
            (void)printf("dataPackTestCycle 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataPackTestCycle 10 -- FAIL \n");
    }
}



#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
#endif