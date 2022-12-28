/**
 * @file       eFSP_MSGDTST.h
 *
 * @brief      Message dencoder test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_MSGDTST.h"
#include "eFSP_MSGD.h"
#include "eCU_CRC.h"

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
struct t_eFSP_MSGD_CrcCtxUser
{
    e_eCU_CRC_RES eLastEr;
};

static bool_t eFSP_MSGDTST_c32SAdapt(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val);
static bool_t eFSP_MSGDTST_c32SAdaptEr(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val);



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSP_MSGDTST_BadPointer(void);
static void eFSP_MSGDTST_BadInit(void);
static void eFSP_MSGDTST_BadParamEntr(void);
static void eFSP_MSGDTST_CorruptedCtx(void);
static void eFSP_MSGDTST_BadClBck(void);
static void eFSP_MSGDTST_MsgEnd(void);
static void eFSP_MSGDTST_FrameRestart(void);
static void eFSP_MSGDTST_BadFrame(void);
static void eFSP_MSGDTST_OutOfMem();
static void eFSP_MSGDTST_General(void);
static void eFSP_MSGDTST_General2(void);
static void eFSP_MSGDTST_CorernerMulti(void);
static void eFSP_MSGDTST_ErrorAndContinue(void);
static void eFSP_MSGDTST_ErrorAndContinueEx(void);
static void eFSP_MSGDTST_ErrorShortFrame(void);
static void eFSP_MSGDTST_ErrorBadStuff(void);
static void eFSP_MSGDTST_Corner(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_MSGDTST_ExeTest(void)
{
	(void)printf("\n\nMESSAGE DECODER TEST START \n\n");

    eFSP_MSGDTST_BadPointer();
    eFSP_MSGDTST_BadInit();
    eFSP_MSGDTST_BadParamEntr();
    eFSP_MSGDTST_CorruptedCtx();
    eFSP_MSGDTST_BadClBck();
    eFSP_MSGDTST_OutOfMem();
    eFSP_MSGDTST_MsgEnd();
    eFSP_MSGDTST_BadFrame();
    eFSP_MSGDTST_FrameRestart();
    eFSP_MSGDTST_General();
    eFSP_MSGDTST_General2();
    eFSP_MSGDTST_CorernerMulti();
    eFSP_MSGDTST_ErrorAndContinue();
    eFSP_MSGDTST_ErrorAndContinueEx();
    eFSP_MSGDTST_ErrorShortFrame();
    eFSP_MSGDTST_ErrorBadStuff();
    eFSP_MSGDTST_Corner();

    (void)printf("\n\nMESSAGE DECODER TEST END \n\n");
}

/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
bool_t eFSP_MSGDTST_c32SAdapt(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t result;
    t_eFSP_MSGD_CrcCtx* ctxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGD_CrcCtx*)p_ptCtx;

        ctxCur->eLastEr = eCU_CRC_32Seed(p_uS, (const uint8_t*)p_puD, p_uDLen, p_puC32Val);
        if( e_eCU_CRC_RES_OK == ctxCur->eLastEr )
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

bool_t eFSP_MSGDTST_c32SAdaptEr(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t result;
    t_eFSP_MSGD_CrcCtx* ctxCur;

    (void)p_uS;
    (void)p_puD;
    (void)p_uDLen;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGD_CrcCtx*)p_ptCtx;

        ctxCur->eLastEr = e_eCU_CRC_RES_BADPOINTER;
        result = false;
        *p_puC32Val = 0u;
    }

    return result;
}

/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSP_MSGDTST_BadPointer(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;
    bool_t isMsgDec;
    bool_t isInit;

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_InitCtx(NULL, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_InitCtx(&ctx, NULL, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), NULL, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, NULL) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_NewMsg(NULL) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_GetDecodedData(NULL, &dataP, &var32) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_GetDecodedData(&ctx, NULL, &var32) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_GetDecodedData(&ctx, &dataP, NULL) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_GetDecodedLen(NULL, &var32) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 9  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_GetDecodedLen(&ctx, NULL) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 10 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_IsWaitingSof(NULL, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_IsWaitingSof(&ctx, NULL) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_IsAFullMsgDecoded(NULL, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_IsAFullMsgDecoded(&ctx, NULL) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_IsFrameBad(NULL, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_IsFrameBad(&ctx, NULL) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_GetMostEffDatLen(NULL, &var32) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_GetMostEffDatLen(&ctx, NULL) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_InsEncChunk(NULL, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_InsEncChunk(&ctx, NULL, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 20 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_InsEncChunk(&ctx, memArea, sizeof(memArea), NULL) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 21 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_IsInit( NULL, &isInit ) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADPOINTER == eFSP_MSGD_IsInit( &ctx, NULL ) )
    {
        (void)printf("eFSP_MSGDTST_BadPointer 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadPointer 23 -- FAIL \n");
    }
}

void eFSP_MSGDTST_BadInit(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    uint32_t var32;
    uint8_t* dataP;
    bool_t isMsgDec;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    ctxAdapterCrc.eLastEr = e_eCU_CRC_RES_OK;
    bool_t isInit;

    /* Function */
    ctx.tBUNSTFCtx.bIsInit = false;
    ctx.ptCrcCtx = &ctxAdapterCrc;
    ctx.fCrc = cbCrcPTest;

    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_GetDecodedData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_GetDecodedLen(&ctx, &var32) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( e_eCU_CRC_RES_OK == ctxAdapterCrc.eLastEr )
        {
            (void)printf("eFSP_MSGDTST_BadInit 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_BadInit 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_GetMostEffDatLen(&ctx, &var32) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_NOINITLIB == eFSP_MSGD_InsEncChunk(&ctx, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_MSGDTST_BadInit 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 8  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsInit( &ctx, &isInit ) )
    {
        if( false == isInit )
        {
            (void)printf("eFSP_MSGDTST_BadInit 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_BadInit 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadInit 9  -- FAIL \n");
    }
}

void eFSP_MSGDTST_BadParamEntr(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t var32;
    bool_t isInit;

    /* Function */
    if( e_eFSP_MSGD_RES_BADPARAM == eFSP_MSGD_InitCtx(&ctx, memArea, 8u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_BADPARAM == eFSP_MSGD_InsEncChunk(&ctx, memArea, 0u, &var32) )
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsInit( &ctx, &isInit ) )
    {
        if( true == isInit )
        {
            (void)printf("eFSP_MSGDTST_BadParamEntr 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_BadParamEntr 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadParamEntr 4  -- FAIL \n");
    }
}

void eFSP_MSGDTST_CorruptedCtx(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;
    bool_t isMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 1  -- FAIL \n");
    }

    ctx.fCrc = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 3  -- FAIL \n");
    }

    ctx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 5  -- FAIL \n");
    }

    ctx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetDecodedData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 7  -- FAIL \n");
    }

    ctx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetDecodedLen(&ctx,&var32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 9  -- FAIL \n");
    }

    ctx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 10 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 11 -- FAIL \n");
    }

    ctx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 13 -- FAIL \n");
    }

    ctx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 15 -- FAIL \n");
    }

    ctx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetMostEffDatLen(&ctx, &var32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 17 -- FAIL \n");
    }

    ctx.ptCrcCtx = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_InsEncChunk(&ctx, memArea, 10u, &var32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 19 -- FAIL \n");
    }

    ctx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 20 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 21 -- FAIL \n");
    }

    ctx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetDecodedData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 22 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 23 -- FAIL \n");
    }

    ctx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetDecodedLen(&ctx, &var32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 24 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 25 -- FAIL \n");
    }

    ctx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 26 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 27 -- FAIL \n");
    }

    ctx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 28 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 29 -- FAIL \n");
    }

    ctx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 30 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 31 -- FAIL \n");
    }

    ctx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_GetMostEffDatLen(&ctx, &var32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 32 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 33 -- FAIL \n");
    }

    ctx.tBUNSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGD_RES_CORRUPTCTX == eFSP_MSGD_InsEncChunk(&ctx, memArea, 10u, &var32) )
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorruptedCtx 34 -- FAIL \n");
    }
}

void eFSP_MSGDTST_BadClBck(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdaptEr;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t var32;
    bool_t isMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_BadClBck 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadClBck 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_BadClBck 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadClBck 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, 0x00u, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_CRCCLBKERROR == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        if( e_eCU_CRC_RES_BADPOINTER == ctxAdapterCrc.eLastEr )
        {
            (void)printf("eFSP_MSGDTST_BadClBck 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_BadClBck 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadClBck 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_CRCCLBKERROR == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_BadClBck 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadClBck 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_CRCCLBKERROR == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        (void)printf("eFSP_MSGDTST_BadClBck 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadClBck 5  -- FAIL \n");
    }
}

void eFSP_MSGDTST_MsgEnd(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_MsgEnd 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_MsgEnd 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_MsgEnd 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_MsgEnd 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xA6u, 0xC1u, 0xDCu, 0x0Au, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        (void)printf("eFSP_MSGDTST_MsgEnd 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_MsgEnd 3  -- FAIL \n");
    }
}

void eFSP_MSGDTST_OutOfMem(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[9u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_OutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_OutOfMem 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_OutOfMem 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_OutOfMem 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xE9u, 0x7Au, 0xF2u, 0xDAu, 0x02, 0x00u, 0x00u, 0x00u, 0x01, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_OUTOFMEM == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        (void)printf("eFSP_MSGDTST_OutOfMem 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_OutOfMem 3  -- FAIL \n");
    }
}

void eFSP_MSGDTST_BadFrame(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_BadFrame 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadFrame 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_BadFrame 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadFrame 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, 0x00u, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        (void)printf("eFSP_MSGDTST_BadFrame 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_BadFrame 3  -- FAIL \n");
    }
}

void eFSP_MSGDTST_FrameRestart(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_FrameRestart 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_FrameRestart 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_FrameRestart 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_FrameRestart 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, ECU_SOF, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        (void)printf("eFSP_MSGDTST_FrameRestart 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_FrameRestart 3  -- FAIL \n");
    }
}

void eFSP_MSGDTST_General(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_General 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_General 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 6  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 8  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed) )
    {
        if( 14u == consumed )
        {
            (void)printf("eFSP_MSGDTST_General 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 15 -- FAIL \n");
    }

    if( ( ECU_SOF == payLoadLoc[0u] ) && ( ECU_EOF == payLoadLoc[1u] ) )
    {
        (void)printf("eFSP_MSGDTST_General 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 16 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed) )
    {
        if( 0u == consumed )
        {
            (void)printf("eFSP_MSGDTST_General 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General 17 -- FAIL \n");
    }
}

void eFSP_MSGDTST_General2(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_General2 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_General2 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 6  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General2 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 8  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, testData, 1u, &consumed) )
    {
        if( 1u == consumed )
        {
            (void)printf("eFSP_MSGDTST_General2 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General2 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 15 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[1], 4u, &consumed) )
    {
        if( 4u == consumed )
        {
            (void)printf("eFSP_MSGDTST_General2 16 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 16 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 4u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General2 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 17 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 18 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 19 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 20 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 21 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[5], 4u, &consumed) )
    {
        if( 4u == consumed )
        {
            (void)printf("eFSP_MSGDTST_General2 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 23 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 3u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General2 24 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 24 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 25 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 26 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 26 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 27 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 27 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 28 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 28 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 29 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 29 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[9], 3u, &consumed) )
    {
        if( 3u == consumed )
        {
            (void)printf("eFSP_MSGDTST_General2 30 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 30 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 30 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 2u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General2 31 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 31 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 31 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 32 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 32 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 32 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 33 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 33 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 33 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 34 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 34 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 34 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 35 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 35 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 35 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 36 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 36 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 36 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, &testData[12], 2u, &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_MSGDTST_General2 37 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 37 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 37 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_General2 38 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 38 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 38 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 39 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 39 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 39 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 40 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 40 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 40 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_General2 41 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 41 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 41 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 42 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 42 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 42 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_General2 43 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_General2 43 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_General2 43 -- FAIL \n");
    }
}

void eFSP_MSGDTST_CorernerMulti(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x73u, 0x9Fu, 0x52u, 0xD9u, 0x07, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), 0x01, 0x02, 0x03, 0x04, 0x05, ECU_EOF};

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, testData, 4u, &consumed) )
    {
        if( 4u == consumed )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 5u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 8  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 11 -- FAIL \n");
    }

    consumed = 0u;
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[4u], 4u, &consumed) )
    {
        if( 4u == consumed)
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 1u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 15 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 16 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 16 -- FAIL \n");
    }

    consumed = 0u;
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[8u], 2u, &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 17 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 18 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 19 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 20 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 21 -- FAIL \n");
    }

    consumed = 0u;
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[10u], 4u, &consumed) )
    {
        if( 4u == consumed )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 5u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 23 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 24 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 24 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x03u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 25 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x03u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 26 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 26 -- FAIL \n");
    }

    consumed = 0u;
    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, &testData[14u], 10u, &consumed) )
    {
        if( 5u == consumed )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 27 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 27 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 28 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 28 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 29 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 29 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x07u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 30 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 30 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 30 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x07u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 31 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_CorernerMulti 31 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 31 -- FAIL \n");
    }

    if( ( ECU_SOF == payLoadLoc[0u] ) && ( ECU_EOF == payLoadLoc[1u] ) && ( 0x01u == payLoadLoc[2u] ) &&
        ( 0x02u == payLoadLoc[3u] ) && ( 0x03u == payLoadLoc[4u] ) && ( 0x04u == payLoadLoc[5u] ) &&
        ( 0x05u == payLoadLoc[6u] ) )
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_CorernerMulti 32 -- FAIL \n");
    }
}

void eFSP_MSGDTST_ErrorAndContinue(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0xA0u, 0xE6u, 0xDCu, 0x0Du, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC,
                           (uint8_t)(~ECU_SOF), ECU_ESC, (uint8_t)(~ECU_EOF), 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[2u], 12u, &consumed) )
    {
        if( 12u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 8  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 1u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[14u], 1u, &consumed) )
    {
        if( 1u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 15 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 17 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 18 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 19 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 20 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x03u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 21 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x03u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[14u], 1u, &consumed) )
    {
        if( 0u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 24 -- FAIL \n");
    }

    /* Remove */
    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[1u], sizeof(testData) - 1u, &consumed) )
    {
        if( (sizeof(testData) - 1u) == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinue 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinue 25 -- FAIL \n");
    }
}

void eFSP_MSGDTST_ErrorAndContinueEx(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0xAAu, 0xAAu, 0xAAu, 0xAAu, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC,
                           (uint8_t)(~ECU_SOF), ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( e_eFSP_MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[2u], 12u, &consumed) )
    {
        if( 12u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 8  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 1u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[14u], 1u, &consumed) )
    {
        if( 1u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 15 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 17 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 18 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 19 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 20 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 21 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[14u], 1u, &consumed) )
    {
        if( 0u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 24 -- FAIL \n");
    }

    /* Remove */
    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[1u], sizeof(testData) - 1u, &consumed) )
    {
        if( (sizeof(testData) - 1u) == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorAndContinueEx 25 -- FAIL \n");
    }
}

void eFSP_MSGDTST_ErrorShortFrame(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0xAAu, 0xAAu, 0xAAu, 0xAAu, 0x04, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[2u], 9u, &consumed) )
    {
        if( 9u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 8  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 4u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[11u], 1u, &consumed) )
    {
        if( 1u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 15 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 17 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 18 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 19 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 20 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 21 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[11u], 1u, &consumed) )
    {
        if( 0u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 24 -- FAIL \n");
    }

    /* Remove */
    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[1u], sizeof(testData) - 1u, &consumed) )
    {
        if( (sizeof(testData) - 1u) == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 25 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 26 -- FAIL \n");
    }

    uint8_t testData2[] = { ECU_SOF, 0xAAu, 0xAAu, ECU_EOF};

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, testData2, sizeof(testData2), &consumed) )
    {
        if( 4u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 27 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 27 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, testData2, sizeof(testData2), &consumed) )
    {
        if( 0u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 28 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 28 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 29 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 29 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 30 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 30 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 30 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 31 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 31 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 31 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 32 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 32 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 32 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 33 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 33 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 33 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 34 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorShortFrame 34 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorShortFrame 34 -- FAIL \n");
    }
}

void eFSP_MSGDTST_ErrorBadStuff(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 2  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 4  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 5  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 9u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0xAAu, 0xAAu, 0xAAu, 0xAAu, 0x04, 0x00u, 0x00u, 0x00u, 0x01, ECU_ESC,
                           0x02, 0x03, ECU_EOF};

    if( e_eFSP_MSGD_RES_FRAMERESTART == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctx, &testData[2u], 9u, &consumed) )
    {
        if( 9u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 8  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 4u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 10 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 12 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 13 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 14 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 14 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[11u], 2u, &consumed) )
    {
        if( 2u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 15 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 17 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 17 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 18 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 18 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 19 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 19 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_IsFrameBad(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 20 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 21 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 21 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 22 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[11u], 2u, &consumed) )
    {
        if( 0u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 23 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 24 -- FAIL \n");
    }

    /* Remove */
    if( e_eFSP_MSGD_RES_BADFRAME == eFSP_MSGD_InsEncChunk(&ctx, &testData[1u], sizeof(testData) - 1u, &consumed) )
    {
        if( 12u == consumed )
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_ErrorBadStuff 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_ErrorBadStuff 25 -- FAIL \n");
    }
}

void eFSP_MSGDTST_Corner(void)
{
    /* Local variable */
    t_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    f_eFSP_MSGD_CrcCb cbCrcPTest = &eFSP_MSGDTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGDTST_Corner 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_Corner 1  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGDTST_Corner 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDTST_Corner 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xA6u, 0xC1u, 0xDCu, 0x0Au, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData) + 10u, &var32) )
    {
        if( sizeof(testData) == var32 )
        {
            (void)printf("eFSP_MSGDTST_Corner 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_Corner 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_Corner 3  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctx, testData, sizeof(testData) + 10u, &var32) )
    {
        if( 0u == var32 )
        {
            (void)printf("eFSP_MSGDTST_Corner 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDTST_Corner 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDTST_Corner 4  -- FAIL \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b"
#endif