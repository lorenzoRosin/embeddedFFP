/**
 * @file eFSPDataUnPackTest.c
 *
 */
#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPDataUnPackTest.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void dataUnPackTestBadPointer(void);
static void dataUnPackTestBadInit(void);
static void dataUnPackTestBadParamEntr(void);
static void dataUnPackTestBadParamStatus(void);
static void dataUnPackTestOutOfMem(void);
static void dataUnPackTestEndianLe(void);
static void dataUnPackTestEndianBe(void);
static void dataUnPackTestCycle(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void dataUnPackTest(void)
{
	(void)printf("\n\nDATA UNPACK TEST START \n\n");

    dataUnPackTestBadPointer();
    dataUnPackTestBadInit();
    dataUnPackTestBadParamEntr();
    dataUnPackTestBadParamStatus();
    dataUnPackTestOutOfMem();
    dataUnPackTestEndianLe();
    dataUnPackTestEndianBe();
    dataUnPackTestCycle();

    (void)printf("\n\nDATA UNPACK END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void dataUnPackTestBadPointer(void)
{
    /* Local variable */
    s_eFSP_DataUnPackCtx ctx;
    uint8_t  badPointerMempool[5u];
    uint64_t varTemp64;
    uint32_t varTemp32;
    uint16_t varTemp16;
    uint8_t  varTemp8;

    /* Function */
    if( ECU_RES_BADPOINTER == dataUnPackinitCtx(NULL, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataUnPackTestBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadPointer 1  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataUnPackinitCtx(&ctx, NULL, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataUnPackTestBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadPointer 2  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataUnPackReset( NULL ) )
    {
        (void)printf("dataUnPackTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadPointer 3  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataUnPackGetRemToPop( NULL, &varTemp32 ) )
    {
        (void)printf("dataUnPackTestBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadPointer 4  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataUnPackGetRemToPop( &ctx, NULL ) )
    {
        (void)printf("dataUnPackTestBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadPointer 5  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataUnPackPopArray( NULL, badPointerMempool, sizeof(badPointerMempool) ) )
    {
        (void)printf("dataUnPackTestBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadPointer 6  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataUnPackPopArray( &ctx, NULL, sizeof(badPointerMempool) ) )
    {
        (void)printf("dataUnPackTestBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadPointer 7  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataUnPackPopU8( NULL, &varTemp8 ) )
    {
        (void)printf("dataUnPackTestBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadPointer 8  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataUnPackPopU16( NULL, &varTemp16 ) )
    {
        (void)printf("dataUnPackTestBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadPointer 9  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataUnPackPopU32( NULL, &varTemp32 ) )
    {
        (void)printf("dataUnPackTestBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadPointer 10 -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == dataUnPackPopU64( NULL, &varTemp64 ) )
    {
        (void)printf("dataUnPackTestBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadPointer 11 -- FAIL \n");
    }
}

void dataUnPackTestBadInit(void)
{
    /* Local variable */
    s_eFSP_DataUnPackCtx ctx;
    uint8_t  badPointerMempool[5u];
    uint64_t varTemp64;
    uint32_t varTemp32;
    uint16_t varTemp16;
    uint8_t  varTemp8;

    /* Init variable */
    ctx.isInit = true;

    /* Function */
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataUnPackTestBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadInit 1  -- FAIL \n");
    }

    /* Init variable */
    ctx.isInit = false;

    if( ECU_RES_NOINITLIB == dataUnPackReset( &ctx ) )
    {
        (void)printf("dataUnPackTestBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadInit 2  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == dataUnPackGetRemToPop( &ctx, &varTemp32 ) )
    {
        (void)printf("dataUnPackTestBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadInit 3  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == dataUnPackPopArray( &ctx, badPointerMempool, sizeof(badPointerMempool) ) )
    {
        (void)printf("dataUnPackTestBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadInit 4  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == dataUnPackPopU8( &ctx, &varTemp8 ) )
    {
        (void)printf("dataUnPackTestBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadInit 5  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == dataUnPackPopU16( &ctx, &varTemp16 ) )
    {
        (void)printf("dataUnPackTestBadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadInit 6  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == dataUnPackPopU32( &ctx, &varTemp32 ) )
    {
        (void)printf("dataUnPackTestBadInit 7  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadInit 7  -- FAIL \n");
    }

    if( ECU_RES_NOINITLIB == dataUnPackPopU64( &ctx, &varTemp64 ) )
    {
        (void)printf("dataUnPackTestBadInit 8  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadInit 8  -- FAIL \n");
    }
}

void dataUnPackTestBadParamEntr(void)
{
    /* Local variable */
    s_eFSP_DataUnPackCtx ctx;
    uint8_t  badPointerMempool[5u];

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_BADPARAM == dataUnPackinitCtx(&ctx, badPointerMempool, 0u, true) )
    {
        (void)printf("dataUnPackTestBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadParamEntr 1  -- FAIL \n");
    }

    /* Init variable */
    ctx.isInit = true;

    if( ECU_RES_BADPARAM == dataUnPackPopArray( &ctx, badPointerMempool, 0u ) )
    {
        (void)printf("dataUnPackTestBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadParamEntr 2  -- FAIL \n");
    }
}

void dataUnPackTestBadParamStatus(void)
{
    /* Local variable */
    s_eFSP_DataUnPackCtx ctx;
    uint8_t  badPointerMempool[5u];
    uint32_t varTemp;

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataUnPackTestBadParamStatus 1  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadParamStatus 1  -- FAIL \n");
    }

    /* Init variable */
    ctx.memUPKASize = 0u;

    if( ECU_RES_BADPARAM == dataUnPackGetRemToPop(&ctx, &varTemp) )
    {
        (void)printf("dataUnPackTestBadParamStatus 2  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadParamStatus 2  -- FAIL \n");
    }

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataUnPackTestBadParamStatus 3  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadParamStatus 3  -- FAIL \n");
    }

    /* Init variable */
    ctx.memUPKA = NULL;

    if( ECU_RES_BADPARAM == dataUnPackGetRemToPop(&ctx, &varTemp) )
    {
        (void)printf("dataUnPackTestBadParamStatus 4  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadParamStatus 4  -- FAIL \n");
    }

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataUnPackTestBadParamStatus 5  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadParamStatus 5  -- FAIL \n");
    }

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataUnPackTestBadParamStatus 6  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadParamStatus 6  -- FAIL \n");
    }

    /* Init variable */
    ctx.memUPKACntr = ctx.memUPKASize + 1u;

    if( ECU_RES_BADPARAM == dataUnPackGetRemToPop(&ctx, &varTemp) )
    {
        (void)printf("dataUnPackTestBadParamStatus 7  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestBadParamStatus 7  -- FAIL \n");
    }
}

void dataUnPackTestOutOfMem(void)
{
    /* Local variable */
    s_eFSP_DataUnPackCtx ctx;
    uint8_t  badPointerMempool[2u];
    uint8_t  pushPool[2u];
    uint64_t varTemp64;
    uint32_t varTemp32;
    uint16_t varTemp16;
    uint8_t  varTemp8;

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, 1u, true) )
    {
        (void)printf("dataUnPackTestOutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestOutOfMem 1  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == dataUnPackPopArray( &ctx, pushPool, sizeof(pushPool) ) )
    {
        (void)printf("dataUnPackTestOutOfMem 2  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestOutOfMem 2  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU8( &ctx, &varTemp8 ) )
    {
        (void)printf("dataUnPackTestOutOfMem 3  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestOutOfMem 3  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == dataUnPackPopU8( &ctx, &varTemp8 ) )
    {
        (void)printf("dataUnPackTestOutOfMem 4  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestOutOfMem 4  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackReset( &ctx  ) )
    {
        (void)printf("dataUnPackTestOutOfMem 5  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestOutOfMem 5  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == dataUnPackPopU16( &ctx, &varTemp16 ) )
    {
        (void)printf("dataUnPackTestOutOfMem 6  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestOutOfMem 6  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == dataUnPackPopU32( &ctx, &varTemp32 ) )
    {
        (void)printf("dataUnPackTestOutOfMem 7  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestOutOfMem 7  -- FAIL \n");
    }

    if( ECU_RES_OUTOFMEM == dataUnPackPopU64( &ctx, &varTemp64 ) )
    {
        (void)printf("dataUnPackTestOutOfMem 8  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestOutOfMem 8  -- FAIL \n");
    }

}

void dataUnPackTestEndianLe(void)
{
    /* Local variable */
    s_eFSP_DataUnPackCtx ctx;
    uint8_t  badPointerMempool[20u];
    uint8_t  pushPool[20u];
    uint64_t varTemp64;
    uint32_t varTemp32;
    uint16_t varTemp16;
    uint8_t  varTemp8;

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, 1u, true) )
    {
        (void)printf("dataUnPackTestEndianLe 1  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestEndianLe 1  -- FAIL \n");
    }

    badPointerMempool[0u] = 0x12u;
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, 1u, true) )
    {
        (void)printf("dataUnPackTestEndianLe 2  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestEndianLe 2  -- FAIL \n");
    }

    badPointerMempool[0u] = 0xF1u;
    if( ECU_RES_OK == dataUnPackPopArray( &ctx, pushPool, 1u ) )
    {
        if( 0xF1u == pushPool[0u] )
        {
            (void)printf("dataUnPackTestEndianLe 3  -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestEndianLe 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestEndianLe 3  -- FAIL \n");
    }

    badPointerMempool[0u] = 0x12u;
    if( ECU_RES_OK == dataUnPackReset( &ctx  ) )
    {
        (void)printf("dataUnPackTestEndianLe 4  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestEndianLe 4  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU8( &ctx, &varTemp8 ) )
    {
        if( 0x12u == varTemp8 )
        {
            (void)printf("dataUnPackTestEndianLe 5  -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestEndianLe 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestEndianLe 5  -- FAIL \n");
    }

    badPointerMempool[0u] = 0x34u;
    badPointerMempool[1u] = 0x12u;
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, 4u, true) )
    {
        (void)printf("dataUnPackTestEndianLe 6  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestEndianLe 6  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU16( &ctx, &varTemp16 ) )
    {
        if( 0x1234u == varTemp16 )
        {
            (void)printf("dataUnPackTestEndianLe 7  -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestEndianLe 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestEndianLe 7  -- FAIL \n");
    }

    badPointerMempool[0u] = 0x78u;
    badPointerMempool[1u] = 0x56u;
    badPointerMempool[2u] = 0x34u;
    badPointerMempool[3u] = 0x12u;
    if( ECU_RES_OK == dataUnPackReset( &ctx ) )
    {
        (void)printf("dataUnPackTestEndianLe 8  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestEndianLe 8  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU32( &ctx, &varTemp32 ) )
    {
        if( 0x12345678u == varTemp32 )
        {
            (void)printf("dataUnPackTestEndianLe 9  -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestEndianLe 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestEndianLe 9  -- FAIL \n");
    }

    badPointerMempool[0u] = 0xF0u;
    badPointerMempool[1u] = 0xDEu;
    badPointerMempool[2u] = 0xBCu;
    badPointerMempool[3u] = 0x9Au;
    badPointerMempool[4u] = 0x78u;
    badPointerMempool[5u] = 0x56u;
    badPointerMempool[6u] = 0x34u;
    badPointerMempool[7u] = 0x12u;
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, 8u, true) )
    {
        (void)printf("dataUnPackTestEndianLe 10 -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestEndianLe 10 -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU64( &ctx, &varTemp64 ) )
    {
        if( 0x123456789ABCDEF0UL == varTemp64 )
        {
            (void)printf("dataUnPackTestEndianLe 11 -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestEndianLe 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestEndianLe 11 -- FAIL \n");
    }
}

void dataUnPackTestEndianBe(void)
{
    /* Local variable */
    s_eFSP_DataUnPackCtx ctx;
    uint8_t  badPointerMempool[20u];
    uint8_t  pushPool[20u];
    uint64_t varTemp64;
    uint32_t varTemp32;
    uint16_t varTemp16;
    uint8_t  varTemp8;

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), false) )
    {
        (void)printf("dataUnPackTestEndianBe 1  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestEndianBe 1  -- FAIL \n");
    }

    badPointerMempool[0u] = 0x12u;
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), false) )
    {
        (void)printf("dataUnPackTestEndianBe 2  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestEndianBe 2  -- FAIL \n");
    }

    badPointerMempool[0u] = 0xF1u;
    if( ECU_RES_OK == dataUnPackPopArray( &ctx, pushPool, 1u ) )
    {
        if( 0xF1u == pushPool[0u] )
        {
            (void)printf("dataUnPackTestEndianBe 3  -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestEndianBe 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestEndianBe 3  -- FAIL \n");
    }

    badPointerMempool[0u] = 0x12u;
    if( ECU_RES_OK == dataUnPackReset( &ctx ) )
    {
        (void)printf("dataUnPackTestEndianBe 4  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestEndianBe 4  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU8( &ctx, &varTemp8 ) )
    {
        if( 0x12u == varTemp8 )
        {
            (void)printf("dataUnPackTestEndianBe 5  -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestEndianBe 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestEndianBe 5  -- FAIL \n");
    }

    badPointerMempool[0u] = 0x12u;
    badPointerMempool[1u] = 0x34u;
    if( ECU_RES_OK == dataUnPackReset( &ctx ) )
    {
        (void)printf("dataUnPackTestEndianBe 6  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestEndianBe 6  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU16( &ctx, &varTemp16 ) )
    {
        if( 0x1234u == varTemp16 )
        {
            (void)printf("dataUnPackTestEndianBe 7  -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestEndianBe 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestEndianBe 7  -- FAIL \n");
    }

    badPointerMempool[0u] = 0x12u;
    badPointerMempool[1u] = 0x34u;
    badPointerMempool[2u] = 0x56u;
    badPointerMempool[3u] = 0x78u;
    if( ECU_RES_OK == dataUnPackReset( &ctx ) )
    {
        (void)printf("dataUnPackTestEndianBe 8  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestEndianBe 8  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU32( &ctx, &varTemp32 ) )
    {
        if( 0x12345678u == varTemp32 )
        {
            (void)printf("dataUnPackTestEndianBe 9  -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestEndianBe 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestEndianBe 9  -- FAIL \n");
    }

    badPointerMempool[0u] = 0x12u;
    badPointerMempool[1u] = 0x34u;
    badPointerMempool[2u] = 0x56u;
    badPointerMempool[3u] = 0x78u;
    badPointerMempool[4u] = 0x9Au;
    badPointerMempool[5u] = 0xBCu;
    badPointerMempool[6u] = 0xDEu;
    badPointerMempool[7u] = 0xF0u;

    if( ECU_RES_OK == dataUnPackReset( &ctx ) )
    {
        (void)printf("dataUnPackTestEndianBe 10 -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestEndianBe 10 -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU64( &ctx, &varTemp64 ) )
    {
        if( 0x123456789ABCDEF0UL == varTemp64 )
        {
            (void)printf("dataUnPackTestEndianBe 11 -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestEndianBe 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestEndianBe 11 -- FAIL \n");
    }
}

void dataUnPackTestCycle(void)
{
    /* Local variable */
    s_eFSP_DataUnPackCtx ctx;
    uint8_t  badPointerMempool[20u];
    uint16_t varTemp16;

    /* Init variable */
    ctx.isInit = false;

    /* Function */
    if( ECU_RES_OK == dataUnPackinitCtx(&ctx, badPointerMempool, sizeof(badPointerMempool), true) )
    {
        (void)printf("dataUnPackTestCycle 1  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestCycle 1  -- FAIL \n");
    }

    badPointerMempool[0u] = 0x78u;
    badPointerMempool[1u] = 0x56u;
    badPointerMempool[2u] = 0x34u;
    badPointerMempool[3u] = 0x12u;
    if( ECU_RES_OK == dataUnPackReset( &ctx ) )
    {
        (void)printf("dataUnPackTestCycle 2  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestCycle 2  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU16( &ctx, &varTemp16 ) )
    {
        if( 0x5678u == varTemp16 )
        {
            (void)printf("dataUnPackTestCycle 3  -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestCycle 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestCycle 3  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU16( &ctx, &varTemp16 ) )
    {
        if( 0x1234u == varTemp16 )
        {
            (void)printf("dataUnPackTestCycle 4  -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestCycle 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestCycle 4  -- FAIL \n");
    }

    /* Function */
    if( ECU_RES_OK == dataUnPackReset( &ctx ) )
    {
        (void)printf("dataUnPackTestCycle 5  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestCycle 5  -- FAIL \n");
    }

    badPointerMempool[0u] = 0x78u;
    badPointerMempool[1u] = 0x56u;
    badPointerMempool[2u] = 0x34u;
    badPointerMempool[3u] = 0x12u;
    if( ECU_RES_OK == dataUnPackReset( &ctx ) )
    {
        (void)printf("dataUnPackTestCycle 6  -- OK \n");
    }
    else
    {
        (void)printf("dataUnPackTestCycle 6  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU16( &ctx, &varTemp16 ) )
    {
        if( 0x5678u == varTemp16 )
        {
            (void)printf("dataUnPackTestCycle 7  -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestCycle 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestCycle 7  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU16( &ctx, &varTemp16 ) )
    {
        if( 0x1234u == varTemp16 )
        {
            (void)printf("dataUnPackTestCycle 8  -- OK \n");
        }
        else
        {
            (void)printf("dataUnPackTestCycle 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("dataUnPackTestCycle 8  -- FAIL \n");
    }
}


#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
#endif