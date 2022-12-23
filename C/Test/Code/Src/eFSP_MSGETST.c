/**
 * @file       eFSP_MSGETST.c
 *
 * @brief      Message encoder test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_MSGETST.h"
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
    #pragma cstat_disable = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "MISRAC2012-Rule-2.2_b"
    /* Suppressed for code clarity in test execution*/
#endif

/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
typedef struct
{
    e_eCU_CRC_RES lastError;
}s_eCU_crcAdapterCtx;

static bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptEr(void* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSP_TEST_msgEncoderBadPointer(void);
static void eFSP_TEST_msgEncoderBadInit(void);
static void eFSP_TEST_msgEncoderBadIniMsg(void);
static void eFSP_TEST_msgEncoderBadParamEntr(void);
static void eFSP_TEST_msgEncoderCorruptContext(void);
static void eFSP_TEST_msgEncoderBadClBck(void);
static void eFSP_TEST_msgEncoderMsgEnd(void);
static void eFSP_TEST_msgEncoderGeneral(void);
static void eFSP_TEST_msgEncoderGeneral2(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_TEST_msgEncoder(void)
{
	(void)printf("\n\nMESSAGE ENCODER TEST START \n\n");

    eFSP_TEST_msgEncoderBadPointer();
    eFSP_TEST_msgEncoderBadInit();
    eFSP_TEST_msgEncoderBadIniMsg();
    eFSP_TEST_msgEncoderBadParamEntr();
    eFSP_TEST_msgEncoderCorruptContext();
    eFSP_TEST_msgEncoderBadClBck();
    eFSP_TEST_msgEncoderMsgEnd();
    eFSP_TEST_msgEncoderGeneral();
    eFSP_TEST_msgEncoderGeneral2();

    (void)printf("\n\nMESSAGE ENCODER TEST END \n\n");
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
        if( e_eCU_CRC_RES_OK == ctxCur->lastError )
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

        ctxCur->lastError = e_eCU_CRC_RES_BADPOINTER;
        result = false;
        *c32Val = 0u;
    }

    return result;
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSP_TEST_msgEncoderBadPointer(void)
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
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_InitCtx(NULL, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_InitCtx(&ctx, NULL, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 2  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), NULL, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 3  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, NULL) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 4  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_NewMessage(NULL, sizeof(memArea)) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 5  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_GetWherePutData(NULL, &dataP, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 6  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_GetWherePutData(&ctx, NULL, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 7  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_GetWherePutData(&ctx, &dataP, NULL) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 8  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_RestartMessage(NULL) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 9  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_GetRemByteToGet(NULL, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 10 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_GetRemByteToGet(&ctx, NULL) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_GetEncChunk(NULL, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_GetEncChunk(&ctx, NULL, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == eFSP_MSGE_GetEncChunk(&ctx, memArea, sizeof(memArea), NULL) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 14 -- FAIL \n");
    }

    if( MSGE_RES_BADPOINTER == eFSP_MSGE_IsInit( NULL, &isInit ) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 15 -- FAIL \n");
    }

    if( MSGE_RES_BADPOINTER == eFSP_MSGE_IsInit( &ctx, NULL ) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadPointer 16 -- FAIL \n");
    }
}

void eFSP_TEST_msgEncoderBadInit(void)
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
    ctx.bstf_Ctx.bIsInit = false;
    ctx.p_crcCtx = &ctxAdapterCrc;
    ctx.f_Crc = cbCrcPTest;

    /* Function */
    if( MSGE_RES_NOINITLIB == eFSP_MSGE_NewMessage(&ctx, 1u) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadInit 1  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_NOINITLIB == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadInit 2  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_NOINITLIB == eFSP_MSGE_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadInit 3  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_NOINITLIB == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadInit 4  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_NOINITLIB == eFSP_MSGE_GetEncChunk(&ctx, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadInit 5  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_IsInit( &ctx, &isInit ) )
    {
        if( false == isInit )
        {
            (void)printf("eFSP_TEST_msgEncoderBadInit 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderBadInit 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadInit 6  -- FAIL \n");
    }
}

void eFSP_TEST_msgEncoderBadIniMsg(void)
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
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadIniMsg 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadIniMsg 1  -- FAIL \n");
    }

    if( MSGE_RES_NOINITMESSAGE == eFSP_MSGE_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadIniMsg 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadIniMsg 2  -- FAIL \n");
    }

    if( MSGE_RES_NOINITMESSAGE == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadIniMsg 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadIniMsg 3  -- FAIL \n");
    }

    if( MSGE_RES_NOINITMESSAGE == eFSP_MSGE_GetEncChunk(&ctx, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadIniMsg 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadIniMsg 4  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_IsInit( &ctx, &isInit ) )
    {
        if( true == isInit )
        {
            (void)printf("eFSP_TEST_msgEncoderBadIniMsg 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderBadIniMsg 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadIniMsg 5  -- FAIL \n");
    }
}

void eFSP_TEST_msgEncoderBadParamEntr(void)
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
    if( MSGE_RES_BADPARAM == eFSP_MSGE_InitCtx(&ctx, memArea, EFSP_MIN_MSGEN_BUFFLEN - 1u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 1  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 2  -- FAIL \n");
    }

    if( MSGE_RES_BADPARAM == eFSP_MSGE_NewMessage(&ctx, 0u) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 3  -- FAIL \n");
    }

    if( MSGE_RES_BADPARAM == eFSP_MSGE_NewMessage(&ctx, (MAX_UINT32VAL - EFSP_MIN_MSGEN_PAYLOAD + 1u) ) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 4  -- FAIL \n");
    }

    if( MSGE_RES_BADPARAM == eFSP_MSGE_NewMessage(&ctx, (3u) ) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 5  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, (2u) ) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 6  -- FAIL \n");
    }

    if( MSGE_RES_BADPARAM == eFSP_MSGE_GetEncChunk(&ctx, memArea, 0u, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadParamEntr 7  -- FAIL \n");
    }
}

void eFSP_TEST_msgEncoderCorruptContext(void)
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
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 1  -- FAIL \n");
    }

    ctx.p_crcCtx = NULL;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 2  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 3  -- FAIL \n");
    }

    ctx.f_Crc = NULL;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 4  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 5  -- FAIL \n");
    }

    ctx.f_Crc = NULL;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 6  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 7  -- FAIL \n");
    }

    ctx.f_Crc = NULL;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 8  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 9  -- FAIL \n");
    }

    ctx.f_Crc = NULL;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 10 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 11 -- FAIL \n");
    }

    ctx.f_Crc = NULL;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetEncChunk(&ctx, memArea, 2u, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 12 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 13 -- FAIL \n");
    }

    ctx.bstf_Ctx.puBuff = NULL;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 14 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 15 -- FAIL \n");
    }

    ctx.bstf_Ctx.puBuff = NULL;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 16 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 17 -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 18 -- FAIL \n");
    }

    ctx.bstf_Ctx.puBuff = NULL;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 19 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 20 -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 21 -- FAIL \n");
    }

    ctx.bstf_Ctx.puBuff = NULL;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 22 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 23 -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 24 -- FAIL \n");
    }

    ctx.bstf_Ctx.puBuff = NULL;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetEncChunk(&ctx, memArea, 2u, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderCorruptContext 25 -- FAIL \n");
    }
}

void eFSP_TEST_msgEncoderBadClBck(void)
{
    /* Local variable */
    s_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msge cbCrcPTest = &c32SAdaptEr;
    s_eCU_crcAdapterCtx ctxAdapterCrc;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderBadClBck 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadClBck 1  -- FAIL \n");
    }

    if( MSGE_RES_CRCCLBKERROR == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        if( e_eCU_CRC_RES_BADPOINTER == ctxAdapterCrc.lastError )
        {
            (void)printf("eFSP_TEST_msgEncoderBadClBck 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderBadClBck 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderBadClBck 2  -- FAIL \n");
    }
}

void eFSP_TEST_msgEncoderMsgEnd(void)
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
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderMsgEnd 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderMsgEnd 1  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        if( 4u == var32 )
        {
            (void)printf("eFSP_TEST_msgEncoderMsgEnd 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderMsgEnd 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderMsgEnd 2  -- FAIL \n");
    }

    dataP[0u] = 1u;
    dataP[1u] = 2u;
    dataP[2u] = 3u;
    dataP[3u] = 4u;

    if( MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, 4u) )
    {
        (void)printf("eFSP_TEST_msgEncoderMsgEnd 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderMsgEnd 3  -- FAIL \n");
    }

    /* Parse all the message */
    if( MSGE_RES_MESSAGEENDED == eFSP_MSGE_GetEncChunk(&ctx, msgA, sizeof(msgA), &var32) )
    {
        if( 14u == var32 )
        {
            if( (0x70u == msgA[1u]) && (0x3Fu == msgA[2u]) && (0x71u == msgA[3u]) && (0xBDu == msgA[4u]) )
            {
                if( (4u == msgA[5u]) && (0u == msgA[6u]) && (0u == msgA[7u]) && (0u == msgA[8u]) )
                {
                    if( (1u == msgA[9u]) && (2u == msgA[10u]) && (3u == msgA[11u]) && (4u == msgA[12u]) )
                    {
                        (void)printf("eFSP_TEST_msgEncoderMsgEnd 4  -- OK \n");
                    }
                    else
                    {
                        (void)printf("eFSP_TEST_msgEncoderMsgEnd 4  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eFSP_TEST_msgEncoderMsgEnd 4  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSP_TEST_msgEncoderMsgEnd 4  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderMsgEnd 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderMsgEnd 4  -- FAIL \n");
    }
}

void eFSP_TEST_msgEncoderGeneral(void)
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
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 1  -- FAIL \n");
    }

    ctx.bstf_Ctx.uBuffL = 3u;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_NewMessage(&ctx, sizeof(memArea)) )
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 2  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 3  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        if( 4u == var32 )
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 4  -- FAIL \n");
    }

    dataP[0u] = 1u;
    dataP[1u] = 2u;
    dataP[2u] = 3u;
    dataP[3u] = ECU_ESC;
    if( MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 5  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 15u == var32)
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 6  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&ctx, msgA, 5u, &var32) )
    {
        if( 5u == var32 )
        {
            if( (ECU_SOF == msgA[0u]) && (0x7Bu == msgA[1u]) && (0x39u == msgA[2u]) && (0x19u == msgA[3u]) &&
                (0x52u == msgA[4u]) )
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral 7  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral 7  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 7  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 8  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 15u == var32)
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 9  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&ctx, msgA, 5u, &var32) )
    {
        if( 5u == var32 )
        {
            if( (ECU_SOF == msgA[0u]) && (0x7Bu == msgA[1u]) && (0x39u == msgA[2u]) && (0x19u == msgA[3u]) &&
                (0x52u == msgA[4u]) )
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral 10 -- OK \n");
            }
            else
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral 10 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 10 -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 10u == var32)
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 11 -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&ctx, msgA, 4u, &var32) )
    {
        if( 4u == var32 )
        {
            if( (0x04u == msgA[0u]) && (0x00u == msgA[1u]) && (0x00u == msgA[2u]) && (0x00u == msgA[3u]) )
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral 12 -- OK \n");
            }
            else
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral 12 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 12 -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 6u == var32)
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 13 -- FAIL \n");
    }

    if( MSGE_RES_MESSAGEENDED == eFSP_MSGE_GetEncChunk(&ctx, msgA, 10u, &var32) )
    {
        if( 6u == var32 )
        {
            if( (0x01u == msgA[0u]) && (0x02u == msgA[1u]) && (0x03u == msgA[2u]) && (ECU_ESC == msgA[3u]) &&
                ( ((uint8_t)~ECU_ESC) == msgA[4u]) && (ECU_EOF == msgA[5u]) )
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral 14 -- OK \n");
            }
            else
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral 14 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 14 -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 0u == var32)
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 15 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 16 -- FAIL \n");
    }

    ctx.bstf_Ctx.uBuffL = 3u;
    if( MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral 17 -- FAIL \n");
    }
}


void eFSP_TEST_msgEncoderGeneral2(void)
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
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral2 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral2 1  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        if( 4u == var32 )
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral2 2  -- FAIL \n");
    }

    dataP[0u] = 1u;
    dataP[1u] = 2u;
    dataP[2u] = 3u;
    dataP[3u] = 0x53u;
    if( MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, var32) )
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral2 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral2 3  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 15u == var32)
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral2 4  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&ctx, msgA, 6u, &var32) )
    {
        if( 6u == var32 )
        {
            if( (ECU_SOF == msgA[0u]) && (0x72u == msgA[1u]) && (0xC4u == msgA[2u]) && (ECU_ESC == msgA[3u]) &&
                ( ((uint8_t)(~0xA1u)) == msgA[4u] ) && (0xDBu == msgA[5u]) )
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral2 5  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral2 5  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral2 5  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 9u == var32)
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral2 6  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&ctx, msgA, 4u, &var32) )
    {
        if( 4u == var32 )
        {
            if( (0x04u == msgA[0u]) && (0x00u == msgA[1u]) && (0x00u == msgA[2u]) && (0x00u == msgA[3u]) )
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral2 7  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral2 7  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral2 7  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 5u == var32)
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral2 8  -- FAIL \n");
    }

    if( MSGE_RES_MESSAGEENDED == eFSP_MSGE_GetEncChunk(&ctx, msgA, 10u, &var32) )
    {
        if( 5u == var32 )
        {
            if( (0x01u == msgA[0u]) && (0x02u == msgA[1u]) && (0x03u == msgA[2u]) && (0x53u == msgA[3u]) &&
                ( ECU_EOF == msgA[4u])  )
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral2 9  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_TEST_msgEncoderGeneral2 9  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral2 9  -- FAIL \n");
    }

    if( MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 0u == var32)
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgEncoderGeneral2 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgEncoderGeneral2 10 -- FAIL \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "MISRAC2012-Rule-2.2_b"
#endif