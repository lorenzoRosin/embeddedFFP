/**
 * @file       eFSPMsgEncoderTest.c
 *
 * @brief      Message encoder test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgEncoderTest.h"
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

static bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptEr(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void msgEncoderTestBadPointer(void);
static void msgEncoderTestBadInit(void);
static void msgEncoderTestBadIniMsg(void);
static void msgEncoderTestBadParamEntr(void);
static void msgEncoderTestCorruptContext(void);
static void msgEncoderTestBadClBck(void);
static void msgEncoderTestMsgEnd(void);
static void msgEncoderTestGeneral(void);
static void msgEncoderTestGeneral2(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void msgEncoderTest(void)
{
	(void)printf("\n\nMESSAGE ENCODER TEST START \n\n");

    msgEncoderTestBadPointer();
    msgEncoderTestBadInit();
    msgEncoderTestBadIniMsg();
    msgEncoderTestBadParamEntr();
    msgEncoderTestCorruptContext();
    msgEncoderTestBadClBck();
    msgEncoderTestMsgEnd();
    msgEncoderTestGeneral();
    msgEncoderTestGeneral2();

    (void)printf("\n\nMESSAGE ENCODER TEST END \n\n");
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
void msgEncoderTestBadPointer(void)
{
    /* Local variable */
    s_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[5u];
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;
    bool_t isInit;

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_InitCtx(NULL, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_InitCtx(&ctx, NULL, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 2  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), NULL, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, NULL) )
    {
        (void)printf("msgEncoderTestBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 4  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_StartNewMessage(NULL, sizeof(memArea)) )
    {
        (void)printf("msgEncoderTestBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 5  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_GetPayloadLocation(NULL, &dataP, &var32) )
    {
        (void)printf("msgEncoderTestBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 6  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_GetPayloadLocation(&ctx, NULL, &var32) )
    {
        (void)printf("msgEncoderTestBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 7  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_GetPayloadLocation(&ctx, &dataP, NULL) )
    {
        (void)printf("msgEncoderTestBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 8  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_RestartCurrentMessage(NULL) )
    {
        (void)printf("msgEncoderTestBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 9  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_GetRemToRetrive(NULL, &var32) )
    {
        (void)printf("msgEncoderTestBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 10 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_GetRemToRetrive(&ctx, NULL) )
    {
        (void)printf("msgEncoderTestBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_RetriveEChunk(NULL, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("msgEncoderTestBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_RetriveEChunk(&ctx, NULL, sizeof(memArea), &var32) )
    {
        (void)printf("msgEncoderTestBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == MSGE_RetriveEChunk(&ctx, memArea, sizeof(memArea), NULL) )
    {
        (void)printf("msgEncoderTestBadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 14 -- FAIL \n");
    }

    if( MSGE_RES_BADPOINTER == MSGE_IsInit( NULL, &isInit ) )
    {
        (void)printf("msgEncoderTestBadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 15 -- FAIL \n");
    }

    if( MSGE_RES_BADPOINTER == MSGE_IsInit( &ctx, NULL ) )
    {
        (void)printf("msgEncoderTestBadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 16 -- FAIL \n");
    }
}

void msgEncoderTestBadInit(void)
{
    /* Local variable */
    s_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[5u];
    uint32_t var32;
    uint8_t* dataP;
    bool_t isInit;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;

    /* Set value */
    ctx.byteStufferCtnx.isInit = false;
    ctx.cbCrcCtx = &ctxAdapterCrc;
    ctx.cbCrcPtr = cbCrcPTest;

    /* Function */
    if( MSGE_RES_NOINITLIB == MSGE_StartNewMessage(&ctx, 1u) )
    {
        (void)printf("msgEncoderTestBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadInit 1  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_NOINITLIB == MSGE_GetPayloadLocation(&ctx, &dataP, &var32) )
    {
        (void)printf("msgEncoderTestBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadInit 2  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_NOINITLIB == MSGE_RestartCurrentMessage(&ctx) )
    {
        (void)printf("msgEncoderTestBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadInit 3  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_NOINITLIB == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        (void)printf("msgEncoderTestBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadInit 4  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_NOINITLIB == MSGE_RetriveEChunk(&ctx, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("msgEncoderTestBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadInit 5  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_IsInit( &ctx, &isInit ) )
    {
        if( false == isInit )
        {
            (void)printf("msgEncoderTestBadInit 6  -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestBadInit 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestBadInit 6  -- FAIL \n");
    }
}

void msgEncoderTestBadIniMsg(void)
{
    /* Local variable */
    s_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    bool_t isInit;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestBadIniMsg 1  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadIniMsg 1  -- FAIL \n");
    }

    if( MSGE_RES_NOINITMESSAGE == MSGE_RestartCurrentMessage(&ctx) )
    {
        (void)printf("msgEncoderTestBadIniMsg 2  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadIniMsg 2  -- FAIL \n");
    }

    if( MSGE_RES_NOINITMESSAGE == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        (void)printf("msgEncoderTestBadIniMsg 3  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadIniMsg 3  -- FAIL \n");
    }

    if( MSGE_RES_NOINITMESSAGE == MSGE_RetriveEChunk(&ctx, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("msgEncoderTestBadIniMsg 4  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadIniMsg 4  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_IsInit( &ctx, &isInit ) )
    {
        if( true == isInit )
        {
            (void)printf("msgEncoderTestBadIniMsg 5  -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestBadIniMsg 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestBadIniMsg 5  -- FAIL \n");
    }
}

void msgEncoderTestBadParamEntr(void)
{
    /* Local variable */
    s_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( MSGE_RES_BADPARAM == MSGE_InitCtx(&ctx, memArea, EFSP_MIN_MSGEN_BUFFLEN - 1u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadParamEntr 1  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadParamEntr 2  -- FAIL \n");
    }

    if( MSGE_RES_BADPARAM == MSGE_StartNewMessage(&ctx, 0u) )
    {
        (void)printf("msgEncoderTestBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadParamEntr 3  -- FAIL \n");
    }

    if( MSGE_RES_BADPARAM == MSGE_StartNewMessage(&ctx, (MAX_UINT32VAL - EFSP_MIN_MSGEN_PAYLOAD + 1u) ) )
    {
        (void)printf("msgEncoderTestBadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadParamEntr 4  -- FAIL \n");
    }

    if( MSGE_RES_BADPARAM == MSGE_StartNewMessage(&ctx, (3u) ) )
    {
        (void)printf("msgEncoderTestBadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadParamEntr 5  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_StartNewMessage(&ctx, (2u) ) )
    {
        (void)printf("msgEncoderTestBadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadParamEntr 6  -- FAIL \n");
    }

    if( MSGE_RES_BADPARAM == MSGE_RetriveEChunk(&ctx, memArea, 0u, &var32) )
    {
        (void)printf("msgEncoderTestBadParamEntr 7  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadParamEntr 7  -- FAIL \n");
    }
}

void msgEncoderTestCorruptContext(void)
{
    /* Local variable */
    s_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestCorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 1  -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGE_RES_CORRUPTCTX == MSGE_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgEncoderTestCorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 2  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestCorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 3  -- FAIL \n");
    }

    ctx.cbCrcPtr = NULL;
    if( MSGE_RES_CORRUPTCTX == MSGE_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgEncoderTestCorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 4  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestCorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 5  -- FAIL \n");
    }

    ctx.cbCrcPtr = NULL;
    if( MSGE_RES_CORRUPTCTX == MSGE_GetPayloadLocation(&ctx, &dataP, &var32) )
    {
        (void)printf("msgEncoderTestCorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 6  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestCorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 7  -- FAIL \n");
    }

    ctx.cbCrcPtr = NULL;
    if( MSGE_RES_CORRUPTCTX == MSGE_RestartCurrentMessage(&ctx) )
    {
        (void)printf("msgEncoderTestCorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 8  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestCorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 9  -- FAIL \n");
    }

    ctx.cbCrcPtr = NULL;
    if( MSGE_RES_CORRUPTCTX == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        (void)printf("msgEncoderTestCorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 10 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestCorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 11 -- FAIL \n");
    }

    ctx.cbCrcPtr = NULL;
    if( MSGE_RES_CORRUPTCTX == MSGE_RetriveEChunk(&ctx, memArea, 2u, &var32) )
    {
        (void)printf("msgEncoderTestCorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 12 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestCorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 13 -- FAIL \n");
    }

    ctx.byteStufferCtnx.memArea = NULL;
    if( MSGE_RES_CORRUPTCTX == MSGE_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgEncoderTestCorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 14 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestCorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 15 -- FAIL \n");
    }

    ctx.byteStufferCtnx.memArea = NULL;
    if( MSGE_RES_CORRUPTCTX == MSGE_GetPayloadLocation(&ctx, &dataP, &var32) )
    {
        (void)printf("msgEncoderTestCorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 16 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestCorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 17 -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgEncoderTestCorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 18 -- FAIL \n");
    }

    ctx.byteStufferCtnx.memArea = NULL;
    if( MSGE_RES_CORRUPTCTX == MSGE_RestartCurrentMessage(&ctx) )
    {
        (void)printf("msgEncoderTestCorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 19 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestCorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 20 -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgEncoderTestCorruptContext 21 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 21 -- FAIL \n");
    }

    ctx.byteStufferCtnx.memArea = NULL;
    if( MSGE_RES_CORRUPTCTX == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        (void)printf("msgEncoderTestCorruptContext 22 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 22 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestCorruptContext 23 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 23 -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgEncoderTestCorruptContext 24 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 24 -- FAIL \n");
    }

    ctx.byteStufferCtnx.memArea = NULL;
    if( MSGE_RES_CORRUPTCTX == MSGE_RetriveEChunk(&ctx, memArea, 2u, &var32) )
    {
        (void)printf("msgEncoderTestCorruptContext 25 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestCorruptContext 25 -- FAIL \n");
    }
}

void msgEncoderTestBadClBck(void)
{
    /* Local variable */
    s_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msge cbCrcPTest = &c32SAdaptEr;
    s_eCU_crcAdapterCtx ctxAdapterCrc;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestBadClBck 1  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadClBck 1  -- FAIL \n");
    }

    if( MSGE_RES_CRCCLBKERROR == MSGE_StartNewMessage(&ctx, 2u) )
    {
        if( CRC_RES_BADPOINTER == ctxAdapterCrc.lastError )
        {
            (void)printf("msgEncoderTestBadClBck 2  -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestBadClBck 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestBadClBck 2  -- FAIL \n");
    }
}

void msgEncoderTestMsgEnd(void)
{
    /* Local variable */
    s_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[12u] = {0u};
    uint8_t  msgA[24u] = {0u};
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestMsgEnd 1  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestMsgEnd 1  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_GetPayloadLocation(&ctx, &dataP, &var32) )
    {
        if( 4u == var32 )
        {
            (void)printf("msgEncoderTestMsgEnd 2  -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestMsgEnd 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestMsgEnd 2  -- FAIL \n");
    }

    dataP[0u] = 1u;
    dataP[1u] = 2u;
    dataP[2u] = 3u;
    dataP[3u] = 4u;

    if( MSGE_RES_OK == MSGE_StartNewMessage(&ctx, 4u) )
    {
        (void)printf("msgEncoderTestMsgEnd 3  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestMsgEnd 3  -- FAIL \n");
    }

    /* Parse all the message */
    if( MSGE_RES_MESSAGEENDED == MSGE_RetriveEChunk(&ctx, msgA, sizeof(msgA), &var32) )
    {
        if( 14u == var32 )
        {
            if( (0x70u == msgA[1u]) && (0x3Fu == msgA[2u]) && (0x71u == msgA[3u]) && (0xBDu == msgA[4u]) )
            {
                if( (4u == msgA[5u]) && (0u == msgA[6u]) && (0u == msgA[7u]) && (0u == msgA[8u]) )
                {
                    if( (1u == msgA[9u]) && (2u == msgA[10u]) && (3u == msgA[11u]) && (4u == msgA[12u]) )
                    {
                        (void)printf("msgEncoderTestMsgEnd 4  -- OK \n");
                    }
                    else
                    {
                        (void)printf("msgEncoderTestMsgEnd 4  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("msgEncoderTestMsgEnd 4  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("msgEncoderTestMsgEnd 4  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("msgEncoderTestMsgEnd 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestMsgEnd 4  -- FAIL \n");
    }
}

void msgEncoderTestGeneral(void)
{
    /* Local variable */
    s_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[12u] = {0u};
    uint8_t  msgA[24u] = {0u};
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestGeneral 1  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 1  -- FAIL \n");
    }

    ctx.byteStufferCtnx.memAreaSize = 3u;
    if( MSGE_RES_CORRUPTCTX == MSGE_StartNewMessage(&ctx, sizeof(memArea)) )
    {
        (void)printf("msgEncoderTestGeneral 2  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 2  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestGeneral 3  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 3  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_GetPayloadLocation(&ctx, &dataP, &var32) )
    {
        if( 4u == var32 )
        {
            (void)printf("msgEncoderTestGeneral 4  -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestGeneral 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 4  -- FAIL \n");
    }

    dataP[0u] = 1u;
    dataP[1u] = 2u;
    dataP[2u] = 3u;
    dataP[3u] = ECU_ESC;
    if( MSGE_RES_OK == MSGE_StartNewMessage(&ctx, var32) )
    {
        (void)printf("msgEncoderTestGeneral 5  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 5  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        if( 15u == var32)
        {
            (void)printf("msgEncoderTestGeneral 6  -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestGeneral 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 6  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_RetriveEChunk(&ctx, msgA, 5u, &var32) )
    {
        if( 5u == var32 )
        {
            if( (ECU_SOF == msgA[0u]) && (0x7Bu == msgA[1u]) && (0x39u == msgA[2u]) && (0x19u == msgA[3u]) &&
                (0x52u == msgA[4u]) )
            {
                (void)printf("msgEncoderTestGeneral 7  -- OK \n");
            }
            else
            {
                (void)printf("msgEncoderTestGeneral 7  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("msgEncoderTestGeneral 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 7  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_RestartCurrentMessage(&ctx) )
    {
        (void)printf("msgEncoderTestGeneral 8  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 8  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        if( 15u == var32)
        {
            (void)printf("msgEncoderTestGeneral 9  -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestGeneral 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 9  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_RetriveEChunk(&ctx, msgA, 5u, &var32) )
    {
        if( 5u == var32 )
        {
            if( (ECU_SOF == msgA[0u]) && (0x7Bu == msgA[1u]) && (0x39u == msgA[2u]) && (0x19u == msgA[3u]) &&
                (0x52u == msgA[4u]) )
            {
                (void)printf("msgEncoderTestGeneral 10 -- OK \n");
            }
            else
            {
                (void)printf("msgEncoderTestGeneral 10 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("msgEncoderTestGeneral 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 10 -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        if( 10u == var32)
        {
            (void)printf("msgEncoderTestGeneral 11 -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestGeneral 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 11 -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_RetriveEChunk(&ctx, msgA, 4u, &var32) )
    {
        if( 4u == var32 )
        {
            if( (0x04u == msgA[0u]) && (0x00u == msgA[1u]) && (0x00u == msgA[2u]) && (0x00u == msgA[3u]) )
            {
                (void)printf("msgEncoderTestGeneral 12 -- OK \n");
            }
            else
            {
                (void)printf("msgEncoderTestGeneral 12 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("msgEncoderTestGeneral 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 12 -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        if( 6u == var32)
        {
            (void)printf("msgEncoderTestGeneral 13 -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestGeneral 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 13 -- FAIL \n");
    }

    if( MSGE_RES_MESSAGEENDED == MSGE_RetriveEChunk(&ctx, msgA, 10u, &var32) )
    {
        if( 6u == var32 )
        {
            if( (0x01u == msgA[0u]) && (0x02u == msgA[1u]) && (0x03u == msgA[2u]) && (ECU_ESC == msgA[3u]) &&
                ( ((uint8_t)~ECU_ESC) == msgA[4u]) && (ECU_EOF == msgA[5u]) )
            {
                (void)printf("msgEncoderTestGeneral 14 -- OK \n");
            }
            else
            {
                (void)printf("msgEncoderTestGeneral 14 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("msgEncoderTestGeneral 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 14 -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        if( 0u == var32)
        {
            (void)printf("msgEncoderTestGeneral 15 -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestGeneral 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 15 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestGeneral 16 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 16 -- FAIL \n");
    }

    ctx.byteStufferCtnx.memAreaSize = 3u;
    if( MSGE_RES_CORRUPTCTX == MSGE_GetPayloadLocation(&ctx, &dataP, &var32) )
    {
        (void)printf("msgEncoderTestGeneral 17 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestGeneral 17 -- FAIL \n");
    }
}


void msgEncoderTestGeneral2(void)
{
    /* Local variable */
    s_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[12u] = {0u};
    uint8_t  msgA[24u] = {0u};
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( MSGE_RES_OK == MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestGeneral2 1  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestGeneral2 1  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_GetPayloadLocation(&ctx, &dataP, &var32) )
    {
        if( 4u == var32 )
        {
            (void)printf("msgEncoderTestGeneral2 2  -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestGeneral2 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral2 2  -- FAIL \n");
    }

    dataP[0u] = 1u;
    dataP[1u] = 2u;
    dataP[2u] = 3u;
    dataP[3u] = 0x53u;
    if( MSGE_RES_OK == MSGE_StartNewMessage(&ctx, var32) )
    {
        (void)printf("msgEncoderTestGeneral2 3  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestGeneral2 3  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        if( 15u == var32)
        {
            (void)printf("msgEncoderTestGeneral2 4  -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestGeneral2 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral2 4  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_RetriveEChunk(&ctx, msgA, 6u, &var32) )
    {
        if( 6u == var32 )
        {
            if( (ECU_SOF == msgA[0u]) && (0x72u == msgA[1u]) && (0xC4u == msgA[2u]) && (ECU_ESC == msgA[3u]) &&
                ( ((uint8_t)(~0xA1u)) == msgA[4u] ) && (0xDBu == msgA[5u]) )
            {
                (void)printf("msgEncoderTestGeneral2 5  -- OK \n");
            }
            else
            {
                (void)printf("msgEncoderTestGeneral2 5  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("msgEncoderTestGeneral2 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral2 5  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        if( 9u == var32)
        {
            (void)printf("msgEncoderTestGeneral2 6  -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestGeneral2 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral2 6  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_RetriveEChunk(&ctx, msgA, 4u, &var32) )
    {
        if( 4u == var32 )
        {
            if( (0x04u == msgA[0u]) && (0x00u == msgA[1u]) && (0x00u == msgA[2u]) && (0x00u == msgA[3u]) )
            {
                (void)printf("msgEncoderTestGeneral2 7  -- OK \n");
            }
            else
            {
                (void)printf("msgEncoderTestGeneral2 7  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("msgEncoderTestGeneral2 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral2 7  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        if( 5u == var32)
        {
            (void)printf("msgEncoderTestGeneral2 8  -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestGeneral2 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral2 8  -- FAIL \n");
    }

    if( MSGE_RES_MESSAGEENDED == MSGE_RetriveEChunk(&ctx, msgA, 10u, &var32) )
    {
        if( 5u == var32 )
        {
            if( (0x01u == msgA[0u]) && (0x02u == msgA[1u]) && (0x03u == msgA[2u]) && (0x53u == msgA[3u]) &&
                ( ECU_EOF == msgA[4u])  )
            {
                (void)printf("msgEncoderTestGeneral2 9  -- OK \n");
            }
            else
            {
                (void)printf("msgEncoderTestGeneral2 9  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("msgEncoderTestGeneral2 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral2 9  -- FAIL \n");
    }

    if( MSGE_RES_OK == MSGE_GetRemToRetrive(&ctx, &var32) )
    {
        if( 0u == var32)
        {
            (void)printf("msgEncoderTestGeneral2 10 -- OK \n");
        }
        else
        {
            (void)printf("msgEncoderTestGeneral2 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgEncoderTestGeneral2 10 -- FAIL \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b"
#endif