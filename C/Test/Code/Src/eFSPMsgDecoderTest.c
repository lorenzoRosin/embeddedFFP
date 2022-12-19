/**
 * @file       eFSPMsgDecoderTest.h
 *
 * @brief      Message dencoder test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgDecoderTest.h"
#include "eCUCrc.h"

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
    /* Suppressed for code clarity in test execution*/
#endif

#include <stdio.h>

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
#endif

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
typedef struct
{
    e_eCU_CRC_Res lastError;
}s_eCU_crcAdapterCtx;

static bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptEr(void* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSP_TEST_msgDecoderBadPointer(void);
static void eFSP_TEST_msgDecoderBadInit(void);
static void eFSP_TEST_msgDecoderBadParamEntr(void);
static void eFSP_TEST_msgDecoderCorruptedCtx(void);
static void eFSP_TEST_msgDecoderBadClBck(void);
static void eFSP_TEST_msgDecoderMsgEnd(void);
static void eFSP_TEST_msgDecoderFrameRestart(void);
static void eFSP_TEST_msgDecoderBadFrame(void);
static void eFSP_TEST_msgDecoderOutOfMem();
static void eFSP_TEST_msgDecoderGeneral(void);
static void eFSP_TEST_msgDecoderGeneral2(void);
static void eFSP_TEST_msgDecoderCorernerMulti(void);
static void eFSP_TEST_msgDecoderErrorAndContinue(void);
static void eFSP_TEST_msgDecoderErrorAndContinueEx(void);
static void eFSP_TEST_msgDecoderErrorShortFrame(void);
static void eFSP_TEST_msgDecoderErrorBadStuff(void);
static void eFSP_TEST_msgDecoderCorner(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_TEST_msgDecoder(void)
{
	(void)printf("\n\nMESSAGE DECODER TEST START \n\n");

    eFSP_TEST_msgDecoderBadPointer();
    eFSP_TEST_msgDecoderBadInit();
    eFSP_TEST_msgDecoderBadParamEntr();
    eFSP_TEST_msgDecoderCorruptedCtx();
    eFSP_TEST_msgDecoderBadClBck();
    eFSP_TEST_msgDecoderOutOfMem();
    eFSP_TEST_msgDecoderMsgEnd();
    eFSP_TEST_msgDecoderBadFrame();
    eFSP_TEST_msgDecoderFrameRestart();
    eFSP_TEST_msgDecoderGeneral();
    eFSP_TEST_msgDecoderGeneral2();
    eFSP_TEST_msgDecoderCorernerMulti();
    eFSP_TEST_msgDecoderErrorAndContinue();
    eFSP_TEST_msgDecoderErrorAndContinueEx();
    eFSP_TEST_msgDecoderErrorShortFrame();
    eFSP_TEST_msgDecoderErrorBadStuff();
    eFSP_TEST_msgDecoderCorner();

    (void)printf("\n\nMESSAGE DECODER TEST END \n\n");
}

/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val)
{
    bool_t result;
    s_eCU_crcAdapterCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == c32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (s_eCU_crcAdapterCtx*)cntx;

        ctxCur->lastError = eCU_CRC_32Seed(s, (const uint8_t*)d, dLen, c32Val);
        if( CRC_RES_OK == ctxCur->lastError )
        {
            result = true;
        }
        else
        {
            result = false;
        }
    }

    return result;
}

bool_t c32SAdaptEr(void* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val)
{
    bool_t result;
    s_eCU_crcAdapterCtx* ctxCur;

    (void)s;
    (void)d;
    (void)dLen;

    if( ( NULL == cntx ) || ( NULL == c32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (s_eCU_crcAdapterCtx*)cntx;

        ctxCur->lastError = CRC_RES_BADPOINTER;
        result = false;
        *c32Val = 0u;
    }

    return result;
}

/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSP_TEST_msgDecoderBadPointer(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;
    bool_t isMsgDec;
    bool_t isInit;

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_InitCtx(NULL, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_InitCtx(&ctx, NULL, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), NULL, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 3  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, NULL) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 4  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_NewMsg(NULL) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 5  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_GetDecodedData(NULL, &dataP, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 6  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_GetDecodedData(&ctx, NULL, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 7  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_GetDecodedData(&ctx, &dataP, NULL) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 8  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_GetDecodedLen(NULL, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 9  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_GetDecodedLen(&ctx, NULL) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 10 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_IsWaitingSof(NULL, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_IsWaitingSof(&ctx, NULL) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_IsAFullMsgDecoded(NULL, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_IsAFullMsgDecoded(&ctx, NULL) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_IsFrameBad(NULL, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_IsFrameBad(&ctx, NULL) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_GetMostEffDatLen(NULL, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_GetMostEffDatLen(&ctx, NULL) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 18 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_InsEncChunk(NULL, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 19 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_InsEncChunk(&ctx, NULL, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 20 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == eFSP_MSGD_InsEncChunk(&ctx, memArea, sizeof(memArea), NULL) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 21 -- FAIL \n");
    }

    if( MSGD_RES_BADPOINTER == eFSP_MSGD_IsInit( NULL, &isInit ) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 22 -- FAIL \n");
    }

    if( MSGD_RES_BADPOINTER == eFSP_MSGD_IsInit( &ctx, NULL ) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadPointer 23 -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderBadInit(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    uint32_t var32;
    uint8_t* dataP;
    bool_t isMsgDec;
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    ctxAdapterCrc.lastError = CRC_RES_OK;
    bool_t isInit;

    /* Function */
    ctx.bunstf_Ctx.isInit = false;
    ctx.p_crcCtx = &ctxAdapterCrc;
    ctx.f_Crc = cbCrcPTest;

    if( MSGD_RES_NOINITLIB == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 1  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == eFSP_MSGD_GetDecodedData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == eFSP_MSGD_GetDecodedLen(&ctx, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 3  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 4  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 5  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( CRC_RES_OK == ctxAdapterCrc.lastError )
        {
            (void)printf("eFSP_TEST_msgDecoderBadInit 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderBadInit 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 6  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == eFSP_MSGD_GetMostEffDatLen(&ctx, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 7  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == eFSP_MSGD_InsEncChunk(&ctx, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsInit( &ctx, &isInit ) )
    {
        if( false == isInit )
        {
            (void)printf("eFSP_TEST_msgDecoderBadInit 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderBadInit 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadInit 9  -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderBadParamEntr(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    bool_t isInit;

    /* Function */
    if( MSGD_RES_BADPARAM == eFSP_MSGD_InitCtx(&ctx, memArea, 8u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadParamEntr 1  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadParamEntr 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPARAM == eFSP_MSGD_InsEncChunk(&ctx, memArea, 0u, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadParamEntr 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsInit( &ctx, &isInit ) )
    {
        if( true == isInit )
        {
            (void)printf("eFSP_TEST_msgDecoderBadParamEntr 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderBadParamEntr 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadParamEntr 4  -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderCorruptedCtx(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 1  -- FAIL \n");
    }

    ctx.f_Crc = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 3  -- FAIL \n");
    }

    ctx.p_crcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 4  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 5  -- FAIL \n");
    }

    ctx.p_crcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetDecodedData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 6  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 7  -- FAIL \n");
    }

    ctx.p_crcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetDecodedLen(&ctx,&var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 8  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 9  -- FAIL \n");
    }

    ctx.p_crcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 10 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 11 -- FAIL \n");
    }

    ctx.p_crcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 12 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 13 -- FAIL \n");
    }

    ctx.p_crcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 14 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 15 -- FAIL \n");
    }

    ctx.p_crcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetMostEffDatLen(&ctx, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 16 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 17 -- FAIL \n");
    }

    ctx.p_crcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_InsEncChunk(&ctx, memArea, 10u, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 18 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 19 -- FAIL \n");
    }

    ctx.bunstf_Ctx.p_memA = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 20 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 21 -- FAIL \n");
    }

    ctx.bunstf_Ctx.p_memA = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetDecodedData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 22 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 23 -- FAIL \n");
    }

    ctx.bunstf_Ctx.p_memA = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetDecodedLen(&ctx, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 24 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 25 -- FAIL \n");
    }

    ctx.bunstf_Ctx.p_memA = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 26 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 27 -- FAIL \n");
    }

    ctx.bunstf_Ctx.p_memA = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 28 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 29 -- FAIL \n");
    }

    ctx.bunstf_Ctx.p_memA = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 30 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 31 -- FAIL \n");
    }

    ctx.bunstf_Ctx.p_memA = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetMostEffDatLen(&ctx, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 32 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 33 -- FAIL \n");
    }

    ctx.bunstf_Ctx.p_memA = NULL;
    if( MSGD_RES_CORRUPTCTX == eFSP_MSGD_InsEncChunk(&ctx, memArea, 10u, &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorruptedCtx 34 -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderBadClBck(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdaptEr;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadClBck 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadClBck 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadClBck 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadClBck 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, 0x00u, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( MSGD_RES_CRCCLBKERROR == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        if( CRC_RES_BADPOINTER == ctxAdapterCrc.lastError )
        {
            (void)printf("eFSP_TEST_msgDecoderBadClBck 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderBadClBck 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadClBck 3  -- FAIL \n");
    }

    if( MSGD_RES_CRCCLBKERROR == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadClBck 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadClBck 4  -- FAIL \n");
    }

    if( MSGD_RES_CRCCLBKERROR == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadClBck 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadClBck 5  -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderMsgEnd(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderMsgEnd 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderMsgEnd 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderMsgEnd 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderMsgEnd 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xA6u, 0xC1u, 0xDCu, 0x0Au, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderMsgEnd 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderMsgEnd 3  -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderOutOfMem(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[9u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderOutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderOutOfMem 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderOutOfMem 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderOutOfMem 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xE9u, 0x7Au, 0xF2u, 0xDAu, 0x02, 0x00u, 0x00u, 0x00u, 0x01, 0x01, ECU_EOF};

    if( MSGD_RES_OUTOFMEM == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderOutOfMem 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderOutOfMem 3  -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderBadFrame(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadFrame 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadFrame 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadFrame 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadFrame 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, 0x00u, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderBadFrame 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderBadFrame 3  -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderFrameRestart(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderFrameRestart 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderFrameRestart 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderFrameRestart 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderFrameRestart 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, ECU_SOF, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        (void)printf("eFSP_TEST_msgDecoderFrameRestart 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderFrameRestart 3  -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderGeneral(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 6  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 8  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed) )
    {
        if( 14u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 11 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 12 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 13 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 14 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 15 -- FAIL \n");
    }

    if( ( ECU_SOF == payLoadLoc[0u] ) && ( ECU_EOF == payLoadLoc[1u] ) )
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 16 -- FAIL \n");
    }

    if( MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed) )
    {
        if( 0u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral 17 -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderGeneral2(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 6  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 8  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, testData, 1u, &consumed) )
    {
        if( 1u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 11 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 12 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 13 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 14 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 15 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[1], 4u, &consumed) )
    {
        if( 4u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 16 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 16 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 4u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 17 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 18 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 19 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 20 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 21 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 22 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[5], 4u, &consumed) )
    {
        if( 4u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 23 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 3u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 24 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 24 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 25 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 26 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 26 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 27 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 27 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 28 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 28 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 29 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 29 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[9], 3u, &consumed) )
    {
        if( 3u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 30 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 30 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 30 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 2u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 31 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 31 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 31 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 32 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 32 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 32 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 33 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 33 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 33 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 34 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 34 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 34 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 35 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 35 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 35 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 36 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 36 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 36 -- FAIL \n");
    }

    if( MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, &testData[12], 2u, &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 37 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 37 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 37 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 38 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 38 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 38 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 39 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 39 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 39 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 40 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 40 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 40 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 41 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 41 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 41 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 42 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 42 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 42 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 43 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderGeneral2 43 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderGeneral2 43 -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderCorernerMulti(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x73u, 0x9Fu, 0x52u, 0xD9u, 0x07, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), 0x01, 0x02, 0x03, 0x04, 0x05, ECU_EOF};

    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, testData, 4u, &consumed) )
    {
        if( 4u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 5u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 11 -- FAIL \n");
    }

    consumed = 0u;
    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[4u], 4u, &consumed) )
    {
        if( 4u == consumed)
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 12 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 1u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 13 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 14 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 15 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 16 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 16 -- FAIL \n");
    }

    consumed = 0u;
    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[8u], 2u, &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 17 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 18 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 19 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 20 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 21 -- FAIL \n");
    }

    consumed = 0u;
    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[10u], 4u, &consumed) )
    {
        if( 4u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 22 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 5u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 23 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 24 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 24 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x03u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 25 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x03u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 26 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 26 -- FAIL \n");
    }

    consumed = 0u;
    if( MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, &testData[14u], 10u, &consumed) )
    {
        if( 5u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 27 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 27 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 28 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 28 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 29 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 29 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x07u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 30 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 30 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 30 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x07u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 31 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorernerMulti 31 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 31 -- FAIL \n");
    }

    if( ( ECU_SOF == payLoadLoc[0u] ) && ( ECU_EOF == payLoadLoc[1u] ) && ( 0x01u == payLoadLoc[2u] ) &&
        ( 0x02u == payLoadLoc[3u] ) && ( 0x03u == payLoadLoc[4u] ) && ( 0x04u == payLoadLoc[5u] ) &&
        ( 0x05u == payLoadLoc[6u] ) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorernerMulti 32 -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderErrorAndContinue(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0xA0u, 0xE6u, 0xDCu, 0x0Du, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC,
                           (uint8_t)(~ECU_SOF), ECU_ESC, (uint8_t)(~ECU_EOF), 0x01, ECU_EOF};

    if( MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[2u], 12u, &consumed) )
    {
        if( 12u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 1u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 11 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 12 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 13 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 14 -- FAIL \n");
    }

    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[14u], 1u, &consumed) )
    {
        if( 1u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 15 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 17 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 18 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 19 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 20 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x03u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 21 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x03u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 22 -- FAIL \n");
    }

    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[14u], 1u, &consumed) )
    {
        if( 0u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 23 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 24 -- FAIL \n");
    }

    /* Remove */
    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[1u], sizeof(testData) - 1u, &consumed) )
    {
        if( (sizeof(testData) - 1u) == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinue 25 -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderErrorAndContinueEx(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0xAAu, 0xAAu, 0xAAu, 0xAAu, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC,
                           (uint8_t)(~ECU_SOF), ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[2u], 12u, &consumed) )
    {
        if( 12u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 1u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 11 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 12 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 13 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 14 -- FAIL \n");
    }

    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[14u], 1u, &consumed) )
    {
        if( 1u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 15 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 17 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 18 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 19 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 20 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 21 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 22 -- FAIL \n");
    }

    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[14u], 1u, &consumed) )
    {
        if( 0u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 23 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 24 -- FAIL \n");
    }

    /* Remove */
    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[1u], sizeof(testData) - 1u, &consumed) )
    {
        if( (sizeof(testData) - 1u) == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorAndContinueEx 25 -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderErrorShortFrame(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0xAAu, 0xAAu, 0xAAu, 0xAAu, 0x04, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[2u], 9u, &consumed) )
    {
        if( 9u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 4u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 11 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 12 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 13 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 14 -- FAIL \n");
    }

    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[11u], 1u, &consumed) )
    {
        if( 1u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 15 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 17 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 18 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 19 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 20 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 21 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 22 -- FAIL \n");
    }

    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[11u], 1u, &consumed) )
    {
        if( 0u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 23 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 24 -- FAIL \n");
    }

    /* Remove */
    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[1u], sizeof(testData) - 1u, &consumed) )
    {
        if( (sizeof(testData) - 1u) == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 25 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 26 -- FAIL \n");
    }

    uint8_t testData2[] = { ECU_SOF, 0xAAu, 0xAAu, ECU_EOF};

    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, testData2, sizeof(testData2), &consumed) )
    {
        if( 4u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 27 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 27 -- FAIL \n");
    }

    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, testData2, sizeof(testData2), &consumed) )
    {
        if( 0u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 28 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 28 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 29 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 29 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 30 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 30 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 30 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 31 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 31 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 31 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 32 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 32 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 32 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 33 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 33 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 33 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 34 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 34 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorShortFrame 34 -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderErrorBadStuff(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0xAAu, 0xAAu, 0xAAu, 0xAAu, 0x04, 0x00u, 0x00u, 0x00u, 0x01, ECU_ESC,
                           0x02, 0x03, ECU_EOF};

    if( MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[2u], 9u, &consumed) )
    {
        if( 9u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 4u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 11 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 12 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 13 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 14 -- FAIL \n");
    }

    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[11u], 2u, &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 15 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 17 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 18 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 19 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 20 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 21 -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 22 -- FAIL \n");
    }

    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[11u], 2u, &consumed) )
    {
        if( 0u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 23 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 24 -- FAIL \n");
    }

    /* Remove */
    if( MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[1u], sizeof(testData) - 1u, &consumed) )
    {
        if( 12u == consumed )
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderErrorBadStuff 25 -- FAIL \n");
    }
}

void eFSP_TEST_msgDecoderCorner(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorner 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorner 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgDecoderCorner 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorner 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xA6u, 0xC1u, 0xDCu, 0x0Au, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData) + 10u, &var32) )
    {
        if( sizeof(testData) == var32 )
        {
            (void)printf("eFSP_TEST_msgDecoderCorner 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorner 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorner 3  -- FAIL \n");
    }

    if( MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData) + 10u, &var32) )
    {
        if( 0u == var32 )
        {
            (void)printf("eFSP_TEST_msgDecoderCorner 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDecoderCorner 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDecoderCorner 4  -- FAIL \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b"
#endif