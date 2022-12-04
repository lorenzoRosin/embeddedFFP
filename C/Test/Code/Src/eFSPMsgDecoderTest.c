/**
 * @file eFSPMsgDecoderTest.c
 *
 */



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgDecoderTest.h"
#include "eCUCrc.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
typedef struct
{
    e_eCU_CRC_Res lastError;
}s_eCU_crcAdapterCtx;

static bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptEr(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void msgDecoderTestBadPointer(void);
static void msgDecoderTestBadInit(void);
static void msgDecoderTestBadParamEntr(void);
static void msgDecoderTestCorruptedCtx(void);
static void msgDecoderTestBadClBck(void);
static void msgDecoderTestMsgEnd(void);
static void msgDecoderTestOutOfMem();
static void msgDecoderTestGeneral(void);
static void msgDecoderTestCorernerMulti(void);
static void msgDecoderTestErrorAndContinue(void);
static void msgDecoderTestErrorAndContinueEx(void);
static void msgDecoderTestErrorTooLong(void);
static void msgDecoderTestErrorTooShort(void);
static void msgDecoderTestErrorTooShortExt(void);
static void msgDecoderTestCodeCoverage(void);

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void msgDecoderTest(void)
{
	(void)printf("\n\nMESSAGE DECODER TEST START \n\n");

    msgDecoderTestBadPointer();
    msgDecoderTestBadInit();
    msgDecoderTestBadParamEntr();
    msgDecoderTestCorruptedCtx();
    msgDecoderTestBadClBck();
    msgDecoderTestOutOfMem();
    msgDecoderTestMsgEnd();
    msgDecoderTestBadFrame();
    msgDecoderTestFrameRestart();
    msgDecoderTestGeneral();
    msgDecoderTestCorernerMulti();
    msgDecoderTestErrorAndContinue();
    msgDecoderTestErrorAndContinueEx();
    msgDecoderTestErrorTooLong();
    msgDecoderTestErrorTooShort();
    msgDecoderTestErrorTooShortExt();
    msgDecoderTestCodeCoverage();

    (void)printf("\n\nMESSAGE DECODER TEST END \n\n");
}

/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val)
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

        ctxCur->lastError = CRC_32Seed(s, (const uint8_t*)d, dLen, c32Val);
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

bool_t c32SAdaptEr(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val)
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
void msgDecoderTestBadPointer(void)
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
    if( MSGD_RES_BADPOINTER == MSGD_InitCtx(NULL, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_InitCtx(&ctx, NULL, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), NULL, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 4  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_StartNewMsg(NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 5  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_GetDecodedData(NULL, &dataP, &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 6  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_GetDecodedData(&ctx, NULL, &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 7  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_GetDecodedData(&ctx, &dataP, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 8  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_GetDecodedLen(NULL, &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 9  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_GetDecodedLen(&ctx, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 10 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_IsWaitingSof(NULL, &isMsgDec) )
    {
        (void)printf("msgDecoderTestBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_IsWaitingSof(&ctx, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_IsAFullMsgDecoded(NULL, &isMsgDec) )
    {
        (void)printf("msgDecoderTestBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_IsAFullMsgDecoded(&ctx, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_IsCurrentFrameBad(NULL, &isMsgDec) )
    {
        (void)printf("msgDecoderTestBadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_IsCurrentFrameBad(&ctx, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_GetMostEffDatLen(NULL, &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_GetMostEffDatLen(&ctx, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 18 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_InsEncChunk(NULL, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 19 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_InsEncChunk(&ctx, NULL, sizeof(memArea), &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 20 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 20 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == MSGD_InsEncChunk(&ctx, memArea, sizeof(memArea), NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 21 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 21 -- FAIL \n");
    }
}

void msgDecoderTestBadInit(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    uint32_t var32;
    uint8_t* dataP;
    bool_t isMsgDec;
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;

    /* Function */
    ctx.byteUStufferCtnx.isInit = false;
    ctx.cbCrcCtx = &ctxAdapterCrc;
    ctx.cbCrcPtr = cbCrcPTest;

    if( MSGD_RES_NOINITLIB == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 1  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == MSGD_GetDecodedData(&ctx, &dataP, &var32) )
    {
        (void)printf("msgDecoderTestBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == MSGD_GetDecodedLen(&ctx, &var32) )
    {
        (void)printf("msgDecoderTestBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 3  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 4  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 5  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == MSGD_IsCurrentFrameBad(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestBadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 6  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == MSGD_GetMostEffDatLen(&ctx, &var32) )
    {
        (void)printf("msgDecoderTestBadInit 7  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 7  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == MSGD_InsEncChunk(&ctx, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("msgDecoderTestBadInit 8  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 8  -- FAIL \n");
    }
}

void msgDecoderTestBadParamEntr(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_BADPARAM == MSGD_InitCtx(&ctx, memArea, 8u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamEntr 1  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamEntr 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPARAM == MSGD_InsEncChunk(&ctx, memArea, 0u, &var32) )
    {
        (void)printf("msgDecoderTestBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamEntr 3  -- FAIL \n");
    }
}

void msgDecoderTestCorruptedCtx(void)
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
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 1  -- FAIL \n");
    }

    ctx.cbCrcPtr = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 3  -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 4  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 4  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 5  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 5  -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_GetDecodedData(&ctx, &dataP, &var32) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 6  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 6  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 7  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 7  -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_GetDecodedLen(&ctx,&var32) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 8  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 8  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 9  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 9  -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 10 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 10 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 11 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 11 -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 12 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 12 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 13 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 13 -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_IsCurrentFrameBad(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 14 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 14 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 15 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 15 -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_GetMostEffDatLen(&ctx, &var32) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 16 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 16 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 17 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 17 -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_InsEncChunk(&ctx, memArea, 10u, &var32) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 18 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 18 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 19 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 19 -- FAIL \n");
    }

    ctx.byteUStufferCtnx.memArea = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 20 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 20 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 21 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 21 -- FAIL \n");
    }

    ctx.byteUStufferCtnx.memArea = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_GetDecodedData(&ctx, &dataP, &var32) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 22 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 22 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 23 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 23 -- FAIL \n");
    }

    ctx.byteUStufferCtnx.memArea = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_GetDecodedLen(&ctx, &var32) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 24 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 24 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 25 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 25 -- FAIL \n");
    }

    ctx.byteUStufferCtnx.memArea = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_IsWaitingSof(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 26 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 26 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 27 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 27 -- FAIL \n");
    }

    ctx.byteUStufferCtnx.memArea = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 28 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 28 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 29 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 29 -- FAIL \n");
    }

    ctx.byteUStufferCtnx.memArea = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_IsCurrentFrameBad(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 30 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 30 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 31 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 31 -- FAIL \n");
    }

    ctx.byteUStufferCtnx.memArea = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_GetMostEffDatLen(&ctx, &var32) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 32 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 32 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 33 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 33 -- FAIL \n");
    }

    ctx.byteUStufferCtnx.memArea = NULL;
    if( MSGD_RES_CORRUPTCTX == MSGD_InsEncChunk(&ctx, memArea, 10u, &var32) )
    {
        (void)printf("msgDecoderTestCorruptedCtx 34 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorruptedCtx 34 -- FAIL \n");
    }
}

void msgDecoderTestBadClBck(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdaptEr;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadClBck 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadClBck 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestBadClBck 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadClBck 2  -- FAIL \n");
    }

    if( MSGD_RES_CRCCLBKERROR == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestBadClBck 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadClBck 3  -- FAIL \n");
    }

    if( MSGD_RES_CRCCLBKERROR == MSGD_IsCurrentFrameBad(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestBadClBck 4  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadClBck 4  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, 0x00u, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( MSGD_RES_CRCCLBKERROR == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        if( CRC_RES_BADPOINTER == ctxAdapterCrc.lastError )
        {
            (void)printf("msgDecoderTestBadClBck 5  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestBadClBck 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestBadClBck 5  -- FAIL \n");
    }
}

void msgDecoderTestMsgEnd(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestMsgEnd 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestMsgEnd 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestMsgEnd 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestMsgEnd 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xA6u, 0xC1u, 0xDCu, 0x0Au, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( MSGD_RES_MESSAGEENDED == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        (void)printf("msgDecoderTestMsgEnd 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestMsgEnd 3  -- FAIL \n");
    }
}

void msgDecoderTestOutOfMem(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[9u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestOutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestOutOfMem 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestOutOfMem 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestOutOfMem 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xE9u, 0x7Au, 0xF2u, 0xDAu, 0x02, 0x00u, 0x00u, 0x00u, 0x01, 0x01, ECU_EOF};

    if( MSGD_RES_OUTOFMEM == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        (void)printf("msgDecoderTestOutOfMem 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestOutOfMem 3  -- FAIL \n");
    }
}

void msgDecoderTestBadFrame(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadFrame 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadFrame 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestBadFrame 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadFrame 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, 0x00u, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( MSGD_RES_BADFRAME == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        (void)printf("msgDecoderTestBadFrame 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadFrame 3  -- FAIL \n");
    }
}

void msgDecoderTestFrameRestart(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestFrameRestart 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestFrameRestart 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestFrameRestart 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestFrameRestart 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, ECU_SOF, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( MSGD_RES_FRAMERESTART == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &var32) )
    {
        (void)printf("msgDecoderTestFrameRestart 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestFrameRestart 3  -- FAIL \n");
    }
}

void msgDecoderTestGeneral(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t errorFound;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestGeneral 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestGeneral 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestGeneral 3  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestGeneral 4  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestGeneral 5  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestGeneral 6  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( MSGD_RES_MESSAGEENDED == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( ( 14u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestGeneral 7  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("msgDecoderTestGeneral 8  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("msgDecoderTestGeneral 9  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("msgDecoderTestGeneral 10 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("msgDecoderTestGeneral 11 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 11 -- FAIL \n");
    }

    if( ( ECU_SOF == payLoadLoc[0u] ) && ( ECU_EOF == payLoadLoc[1u] ) )
    {
        (void)printf("msgDecoderTestGeneral 12 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 12 -- FAIL \n");
    }

    if( MSGD_RES_MESSAGEENDED == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( 0u == consumed )
        {
            (void)printf("msgDecoderTestGeneral 13 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 13 -- FAIL \n");
    }
}

void msgDecoderTestCorernerMulti(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t errorFound;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorernerMulti 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestCorernerMulti 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 3  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 4  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestCorernerMulti 5  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestCorernerMulti 6  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x73u, 0x9Fu, 0x52u, 0xD9u, 0x07, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), 0x01, 0x02, 0x03, 0x04, 0x05, ECU_EOF};

    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, testData, 4u, &consumed, &errorFound) )
    {
        if( ( 4u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestCorernerMulti 7  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 5u == mostEfficient )
        {
            (void)printf("msgDecoderTestCorernerMulti 8  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestCorernerMulti 9  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 10 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 11 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 11 -- FAIL \n");
    }

    consumed = 0u;
    errorFound = 0u;
    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, &testData[4u], 4u, &consumed, &errorFound) )
    {
        if( ( 4u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestCorernerMulti 12 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 12 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 1u == mostEfficient )
        {
            (void)printf("msgDecoderTestCorernerMulti 13 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 13 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestCorernerMulti 14 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 14 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 15 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 15 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 16 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 16 -- FAIL \n");
    }

    consumed = 0u;
    errorFound = 0u;
    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, &testData[8u], 4u, &consumed, &errorFound) )
    {
        if( ( 4u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestCorernerMulti 17 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 17 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 7u == mostEfficient )
        {
            (void)printf("msgDecoderTestCorernerMulti 18 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 18 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestCorernerMulti 19 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 19 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 20 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 20 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 21 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 21 -- FAIL \n");
    }

    consumed = 0u;
    errorFound = 0u;
    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, &testData[12u], 4u, &consumed, &errorFound) )
    {
        if( ( 4u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestCorernerMulti 22 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 22 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 3u == mostEfficient )
        {
            (void)printf("msgDecoderTestCorernerMulti 23 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 23 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestCorernerMulti 24 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 24 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x05u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 25 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 25 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x05u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 26 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 26 -- FAIL \n");
    }

    consumed = 0u;
    errorFound = 0u;
    if( MSGD_RES_MESSAGEENDED == MSGD_InsEncChunk(&ctx, &testData[16u], 4u, &consumed, &errorFound) )
    {
        if( ( 3u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestCorernerMulti 27 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 27 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("msgDecoderTestCorernerMulti 28 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 28 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("msgDecoderTestCorernerMulti 29 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 29 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x07u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 30 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 30 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 30 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x07u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 31 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 31 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 31 -- FAIL \n");
    }

    if( ( ECU_SOF == payLoadLoc[0u] ) && ( ECU_EOF == payLoadLoc[1u] ) && ( 0x01 == payLoadLoc[2u] ) &&
        ( 0x02 == payLoadLoc[3u] ) && ( 0x03 == payLoadLoc[4u] ) && ( 0x04 == payLoadLoc[5u] ) &&
        ( 0x05 == payLoadLoc[6u] ) )
    {
        (void)printf("msgDecoderTestCorernerMulti 32 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 32 -- FAIL \n");
    }
}

void msgDecoderTestErrorAndContinue(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t errorFound;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestErrorAndContinue 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestErrorAndContinue 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorAndContinue 3  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorAndContinue 4  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestErrorAndContinue 5  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestErrorAndContinue 6  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC,
                           (uint8_t)(~ECU_SOF), ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( MSGD_RES_MESSAGEENDED == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( ( 15u == consumed ) && ( 1u == errorFound) )
        {
            (void)printf("msgDecoderTestErrorAndContinue 7  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("msgDecoderTestErrorAndContinue 8  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("msgDecoderTestErrorAndContinue 9  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorAndContinue 10 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorAndContinue 11 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 11 -- FAIL \n");
    }

    if( ( ECU_SOF == payLoadLoc[0u] ) && ( ECU_EOF == payLoadLoc[1u] ) )
    {
        (void)printf("msgDecoderTestErrorAndContinue 12 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 12 -- FAIL \n");
    }

    if( MSGD_RES_MESSAGEENDED == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( 0u == consumed )
        {
            (void)printf("msgDecoderTestErrorAndContinue 13 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 13 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestErrorAndContinue 14 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 14 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestErrorAndContinue 15 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 15 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorAndContinue 16 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 16 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorAndContinue 17 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 17 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestErrorAndContinue 18 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 18 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestErrorAndContinue 19 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 19 -- FAIL \n");
    }

    /* Remove */
    uint8_t testData2[] = { ECU_SOF, ECU_SOF, 0x00u, 0x00u, 0x00u, 0x00u, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC,
                           (uint8_t)(~ECU_SOF), ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, testData2, sizeof(testData2), &consumed, &errorFound) )
    {
        if( ( 15u == consumed ) && ( 2u == errorFound) )
        {
            (void)printf("msgDecoderTestErrorAndContinue 20 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 20 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestErrorAndContinue 21 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 21 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestErrorAndContinue 22 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 22 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorAndContinue 23 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 23 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorAndContinue 24 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 24 -- FAIL \n");
    }

    if( ( ECU_SOF == payLoadLoc[0u] ) && ( ECU_EOF == payLoadLoc[1u] ) )
    {
        (void)printf("msgDecoderTestErrorAndContinue 25 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 25 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, testData2, sizeof(testData2), &consumed, &errorFound) )
    {
        if( 15u == consumed )
        {
            (void)printf("msgDecoderTestErrorAndContinue 26 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinue 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinue 26 -- FAIL \n");
    }
}

void msgDecoderTestErrorAndContinueEx(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t errorFound;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 3  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 4  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 5  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 6  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, 0x00u, 0x00u, 0x00u, 0x00u, 0x02, 0x00u, 0x00u, 0x00u, 0x01, 0x02, ECU_EOF,
                           ECU_SOF, 0x30u, 0x5Cu, 0xB1u, 0xD7u, 0x02, 0x00u, 0x00u, 0x00u, 0x01, 0x02, ECU_EOF};

    if( MSGD_RES_MESSAGEENDED == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( ( 24u == consumed ) && ( 1u == errorFound) )
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 7  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 8  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 9  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 10 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 11 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 11 -- FAIL \n");
    }

    if( ( 0x01 == payLoadLoc[0u] ) && ( 0x02 == payLoadLoc[1u] ) )
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 12 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 12 -- FAIL \n");
    }

    if( MSGD_RES_MESSAGEENDED == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( 0u == consumed )
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 13 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorAndContinueEx 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorAndContinueEx 13 -- FAIL \n");
    }
}

void msgDecoderTestErrorTooLong(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t errorFound;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestErrorTooLong 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooLong 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestErrorTooLong 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooLong 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorTooLong 3  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooLong 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooLong 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorTooLong 4  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooLong 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooLong 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestErrorTooLong 5  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooLong 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooLong 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestErrorTooLong 6  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooLong 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooLong 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x03, 0x00u, 0x00u, 0x00u,
                           0x01, 0x02, 0x03, 0x04, 0x05, ECU_EOF};

    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, testData, 13u, &consumed, &errorFound) )
    {
        if( ( 13u == consumed ) && ( 1u == errorFound) )
        {
            (void)printf("msgDecoderTestErrorTooLong 7  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooLong 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooLong 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestErrorTooLong 8  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooLong 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooLong 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestErrorTooLong 9  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooLong 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooLong 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorTooLong 10 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooLong 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooLong 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorTooLong 11 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooLong 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooLong 11 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( 15u == consumed )
        {
            (void)printf("msgDecoderTestErrorTooLong 12 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooLong 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooLong 12 -- FAIL \n");
    }
}

void msgDecoderTestErrorTooShort(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t errorFound;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestErrorTooShort 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShort 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestErrorTooShort 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShort 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorTooShort 3  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShort 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShort 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorTooShort 4  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShort 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShort 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestErrorTooShort 5  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShort 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShort 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestErrorTooShort 6  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShort 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShort 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x03, 0x00u, 0x00u, 0x00u,
                           0x01, 0x02, ECU_EOF};

    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( ( 12u == consumed ) && ( 1u == errorFound) )
        {
            (void)printf("msgDecoderTestErrorTooShort 7  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShort 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShort 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestErrorTooShort 8  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShort 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShort 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestErrorTooShort 9  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShort 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShort 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorTooShort 10 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShort 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShort 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorTooShort 11 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShort 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShort 11 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( 12u == consumed )
        {
            (void)printf("msgDecoderTestErrorTooShort 12 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShort 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShort 12 -- FAIL \n");
    }
}

void msgDecoderTestErrorTooShortExt(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t errorFound;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestErrorTooShortExt 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShortExt 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestErrorTooShortExt 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShortExt 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorTooShortExt 3  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShortExt 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShortExt 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorTooShortExt 4  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShortExt 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShortExt 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestErrorTooShortExt 5  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShortExt 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShortExt 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestErrorTooShortExt 6  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShortExt 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShortExt 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = { ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x03, 0x00u, 0x00u, ECU_EOF};

    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( ( 9u == consumed ) && ( 1u == errorFound) )
        {
            (void)printf("msgDecoderTestErrorTooShortExt 7  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShortExt 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShortExt 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestErrorTooShortExt 8  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShortExt 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShortExt 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_IsAFullMsgDecoded(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestErrorTooShortExt 9  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShortExt 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShortExt 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorTooShortExt 10 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShortExt 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShortExt 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_GetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestErrorTooShortExt 11 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShortExt 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShortExt 11 -- FAIL \n");
    }

    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( 9u == consumed )
        {
            (void)printf("msgDecoderTestErrorTooShortExt 12 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestErrorTooShortExt 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestErrorTooShortExt 12 -- FAIL \n");
    }
}

void msgDecoderTestCodeCoverage(void)
{
    /* Local variable */
    s_eFSP_MSGD_Ctx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t errorFound;
    uint32_t mostEfficient;

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCodeCoverage 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCodeCoverage 1  -- FAIL \n");
    }

    ctx.byteUStufferCtnx.memAreaSize = 0u;
    if( MSGD_RES_CORRUPTCTX == MSGD_StartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestCodeCoverage 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCodeCoverage 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == MSGD_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCodeCoverage 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCodeCoverage 3  -- FAIL \n");
    }

    uint8_t testData[] = { ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC,
                           (uint8_t)(~ECU_SOF), ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( MSGD_RES_OK == MSGD_InsEncChunk(&ctx, testData, 9u, &consumed, &errorFound) )
    {
        if( ( 9u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestCodeCoverage 4  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCodeCoverage 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCodeCoverage 4  -- FAIL \n");
    }

    ctx.byteUStufferCtnx.memAreaCntr = 15u;
    if( MSGD_RES_CORRUPTCTX == MSGD_GetMostEffDatLen(&ctx, &mostEfficient) )
    {
        (void)printf("msgDecoderTestCodeCoverage 5  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCodeCoverage 5  -- FAIL \n");
    }
}