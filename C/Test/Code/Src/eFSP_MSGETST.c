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
#include "eFSP_MSGE.h"
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
struct t_eFSP_MSGE_CrcCtxUser
{
    e_eCU_CRC_RES eLastEr;
};

static bool_t eFSP_MSGETST_c32SAdapt(t_eFSP_MSGE_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val);
static bool_t eFSP_MSGETST_c32SAdaptEr(t_eFSP_MSGE_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val);



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSP_MSGETST_BadPointer(void);
static void eFSP_MSGETST_BadInit(void);
static void eFSP_MSGETST_BadInitMsg(void);
static void eFSP_MSGETST_BadParamEntr(void);
static void eFSP_MSGETST_CorruptContext(void);
static void eFSP_MSGETST_BadClBck(void);
static void eFSP_MSGETST_MsgEnd(void);
static void eFSP_MSGETST_General(void);
static void eFSP_MSGETST_General2(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_MSGETST_ExeTest(void)
{
	(void)printf("\n\nMESSAGE ENCODER TEST START \n\n");

    eFSP_MSGETST_BadPointer();
    eFSP_MSGETST_BadInit();
    eFSP_MSGETST_BadInitMsg();
    eFSP_MSGETST_BadParamEntr();
    eFSP_MSGETST_CorruptContext();
    eFSP_MSGETST_BadClBck();
    eFSP_MSGETST_MsgEnd();
    eFSP_MSGETST_General();
    eFSP_MSGETST_General2();

    (void)printf("\n\nMESSAGE ENCODER TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
bool_t eFSP_MSGETST_c32SAdapt(t_eFSP_MSGE_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t result;
    t_eFSP_MSGE_CrcCtx* ctxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGE_CrcCtx*)p_ptCtx;

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

bool_t eFSP_MSGETST_c32SAdaptEr(t_eFSP_MSGE_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t result;
    t_eFSP_MSGE_CrcCtx* ctxCur;

    (void)p_uS;
    (void)p_puD;
    (void)p_uDLen;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGE_CrcCtx*)p_ptCtx;

        ctxCur->eLastEr = e_eCU_CRC_RES_BADPOINTER;
        result = false;
        *p_puC32Val = 0u;
    }

    return result;
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSP_MSGETST_BadPointer(void)
{
    /* Local variable */
    t_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[5u];
    f_eFSP_MSGE_CrcCb cbCrcPTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;
    bool_t isInit;

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_InitCtx(NULL, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_InitCtx(&ctx, NULL, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), NULL, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, NULL) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_NewMessage(NULL, sizeof(memArea)) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetWherePutData(NULL, &dataP, &var32) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetWherePutData(&ctx, NULL, &var32) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetWherePutData(&ctx, &dataP, NULL) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_RestartMessage(NULL) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 9  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetRemByteToGet(NULL, &var32) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 10 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetRemByteToGet(&ctx, NULL) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetEncChunk(NULL, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetEncChunk(&ctx, NULL, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetEncChunk(&ctx, memArea, sizeof(memArea), NULL) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 14 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_IsInit( NULL, &isInit ) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 15 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_IsInit( &ctx, NULL ) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 16 -- FAIL \n");
    }
}

void eFSP_MSGETST_BadInit(void)
{
    /* Local variable */
    t_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[5u];
    uint32_t var32;
    uint8_t* dataP;
    bool_t isInit;
    f_eFSP_MSGE_CrcCb cbCrcPTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;

    /* Set value */
    ctx.tBSTFCtx.bIsInit = false;
    ctx.ptCrcCtx = &ctxAdapterCrc;
    ctx.fCrc = cbCrcPTest;

    /* Function */
    if( e_eFSP_MSGE_RES_NOINITLIB == eFSP_MSGE_NewMessage(&ctx, 1u) )
    {
        (void)printf("eFSP_MSGETST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInit 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_NOINITLIB == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_MSGETST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInit 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_NOINITLIB == eFSP_MSGE_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_MSGETST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInit 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_NOINITLIB == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        (void)printf("eFSP_MSGETST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInit 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_NOINITLIB == eFSP_MSGE_GetEncChunk(&ctx, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_MSGETST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInit 5  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_IsInit( &ctx, &isInit ) )
    {
        if( false == isInit )
        {
            (void)printf("eFSP_MSGETST_BadInit 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_BadInit 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInit 6  -- FAIL \n");
    }
}

void eFSP_MSGETST_BadInitMsg(void)
{
    /* Local variable */
    t_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[10u];
    f_eFSP_MSGE_CrcCb cbCrcPTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    uint32_t var32;
    bool_t isInit;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 1  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_NOINITMESSAGE == eFSP_MSGE_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 2  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_NOINITMESSAGE == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 3  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_NOINITMESSAGE == eFSP_MSGE_GetEncChunk(&ctx, memArea, sizeof(memArea), &var32) )
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 4  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_IsInit( &ctx, &isInit ) )
    {
        if( true == isInit )
        {
            (void)printf("eFSP_MSGETST_BadInitMsg 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_BadInitMsg 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 5  -- FAIL \n");
    }
}

void eFSP_MSGETST_BadParamEntr(void)
{
    /* Local variable */
    t_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[10u];
    f_eFSP_MSGE_CrcCb cbCrcPTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    uint32_t var32;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( e_eFSP_MSGE_RES_BADPARAM == eFSP_MSGE_InitCtx(&ctx, memArea, EFSP_MIN_MSGEN_BUFFLEN - 1u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 1  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 2  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_BADPARAM == eFSP_MSGE_NewMessage(&ctx, 0u) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 3  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_BADPARAM == eFSP_MSGE_NewMessage(&ctx, (MAX_UINT32VAL - EFSP_MIN_MSGEN_PAYLOAD + 1u) ) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 4  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_BADPARAM == eFSP_MSGE_NewMessage(&ctx, (3u) ) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 5  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, (2u) ) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 6  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_BADPARAM == eFSP_MSGE_GetEncChunk(&ctx, memArea, 0u, &var32) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 7  -- FAIL \n");
    }
}

void eFSP_MSGETST_CorruptContext(void)
{
    /* Local variable */
    t_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[10u];
    f_eFSP_MSGE_CrcCb cbCrcPTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 1  -- FAIL \n");
    }

    ctx.ptCrcCtx = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 3  -- FAIL \n");
    }

    ctx.fCrc = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 5  -- FAIL \n");
    }

    ctx.fCrc = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 7  -- FAIL \n");
    }

    ctx.fCrc = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 9  -- FAIL \n");
    }

    ctx.fCrc = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 10 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 11 -- FAIL \n");
    }

    ctx.fCrc = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetEncChunk(&ctx, memArea, 2u, &var32) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 13 -- FAIL \n");
    }

    ctx.tBSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 15 -- FAIL \n");
    }

    ctx.tBSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 17 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 18 -- FAIL \n");
    }

    ctx.tBSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 20 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 21 -- FAIL \n");
    }

    ctx.tBSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 22 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 23 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 24 -- FAIL \n");
    }

    ctx.tBSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetEncChunk(&ctx, memArea, 2u, &var32) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 25 -- FAIL \n");
    }
}

void eFSP_MSGETST_BadClBck(void)
{
    /* Local variable */
    t_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[10u];
    f_eFSP_MSGE_CrcCb cbCrcPTest = &eFSP_MSGETST_c32SAdaptEr;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadClBck 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadClBck 1  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_CRCCLBKERROR == eFSP_MSGE_NewMessage(&ctx, 2u) )
    {
        if( e_eCU_CRC_RES_BADPOINTER == ctxAdapterCrc.eLastEr )
        {
            (void)printf("eFSP_MSGETST_BadClBck 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_BadClBck 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadClBck 2  -- FAIL \n");
    }
}

void eFSP_MSGETST_MsgEnd(void)
{
    /* Local variable */
    t_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[12u] = {0u};
    uint8_t  msgA[24u] = {0u};
    f_eFSP_MSGE_CrcCb cbCrcPTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_MsgEnd 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_MsgEnd 1  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        if( 4u == var32 )
        {
            (void)printf("eFSP_MSGETST_MsgEnd 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_MsgEnd 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_MsgEnd 2  -- FAIL \n");
    }

    dataP[0u] = 1u;
    dataP[1u] = 2u;
    dataP[2u] = 3u;
    dataP[3u] = 4u;

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, 4u) )
    {
        (void)printf("eFSP_MSGETST_MsgEnd 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_MsgEnd 3  -- FAIL \n");
    }

    /* Parse all the message */
    if( e_eFSP_MSGE_RES_MESSAGEENDED == eFSP_MSGE_GetEncChunk(&ctx, msgA, sizeof(msgA), &var32) )
    {
        if( 14u == var32 )
        {
            if( (0x70u == msgA[1u]) && (0x3Fu == msgA[2u]) && (0x71u == msgA[3u]) && (0xBDu == msgA[4u]) )
            {
                if( (4u == msgA[5u]) && (0u == msgA[6u]) && (0u == msgA[7u]) && (0u == msgA[8u]) )
                {
                    if( (1u == msgA[9u]) && (2u == msgA[10u]) && (3u == msgA[11u]) && (4u == msgA[12u]) )
                    {
                        (void)printf("eFSP_MSGETST_MsgEnd 4  -- OK \n");
                    }
                    else
                    {
                        (void)printf("eFSP_MSGETST_MsgEnd 4  -- FAIL \n");
                    }
                }
                else
                {
                    (void)printf("eFSP_MSGETST_MsgEnd 4  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("eFSP_MSGETST_MsgEnd 4  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_MSGETST_MsgEnd 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_MsgEnd 4  -- FAIL \n");
    }
}

void eFSP_MSGETST_General(void)
{
    /* Local variable */
    t_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[12u] = {0u};
    uint8_t  msgA[24u] = {0u};
    f_eFSP_MSGE_CrcCb cbCrcPTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_General 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 1  -- FAIL \n");
    }

    ctx.tBSTFCtx.uBuffL = 3u;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_NewMessage(&ctx, sizeof(memArea)) )
    {
        (void)printf("eFSP_MSGETST_General 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_General 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 3  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        if( 4u == var32 )
        {
            (void)printf("eFSP_MSGETST_General 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_General 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 4  -- FAIL \n");
    }

    dataP[0u] = 1u;
    dataP[1u] = 2u;
    dataP[2u] = 3u;
    dataP[3u] = ECU_ESC;
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, var32) )
    {
        (void)printf("eFSP_MSGETST_General 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 5  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 15u == var32)
        {
            (void)printf("eFSP_MSGETST_General 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_General 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 6  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&ctx, msgA, 5u, &var32) )
    {
        if( 5u == var32 )
        {
            if( (ECU_SOF == msgA[0u]) && (0x7Bu == msgA[1u]) && (0x39u == msgA[2u]) && (0x19u == msgA[3u]) &&
                (0x52u == msgA[4u]) )
            {
                (void)printf("eFSP_MSGETST_General 7  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_MSGETST_General 7  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_MSGETST_General 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 7  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_MSGETST_General 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 8  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 15u == var32)
        {
            (void)printf("eFSP_MSGETST_General 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_General 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 9  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&ctx, msgA, 5u, &var32) )
    {
        if( 5u == var32 )
        {
            if( (ECU_SOF == msgA[0u]) && (0x7Bu == msgA[1u]) && (0x39u == msgA[2u]) && (0x19u == msgA[3u]) &&
                (0x52u == msgA[4u]) )
            {
                (void)printf("eFSP_MSGETST_General 10 -- OK \n");
            }
            else
            {
                (void)printf("eFSP_MSGETST_General 10 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_MSGETST_General 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 10 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 10u == var32)
        {
            (void)printf("eFSP_MSGETST_General 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_General 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 11 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&ctx, msgA, 4u, &var32) )
    {
        if( 4u == var32 )
        {
            if( (0x04u == msgA[0u]) && (0x00u == msgA[1u]) && (0x00u == msgA[2u]) && (0x00u == msgA[3u]) )
            {
                (void)printf("eFSP_MSGETST_General 12 -- OK \n");
            }
            else
            {
                (void)printf("eFSP_MSGETST_General 12 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_MSGETST_General 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 12 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 6u == var32)
        {
            (void)printf("eFSP_MSGETST_General 13 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_General 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 13 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_MESSAGEENDED == eFSP_MSGE_GetEncChunk(&ctx, msgA, 10u, &var32) )
    {
        if( 6u == var32 )
        {
            if( (0x01u == msgA[0u]) && (0x02u == msgA[1u]) && (0x03u == msgA[2u]) && (ECU_ESC == msgA[3u]) &&
                ( ((uint8_t)~ECU_ESC) == msgA[4u]) && (ECU_EOF == msgA[5u]) )
            {
                (void)printf("eFSP_MSGETST_General 14 -- OK \n");
            }
            else
            {
                (void)printf("eFSP_MSGETST_General 14 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_MSGETST_General 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 14 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 0u == var32)
        {
            (void)printf("eFSP_MSGETST_General 15 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_General 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 15 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_General 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 16 -- FAIL \n");
    }

    ctx.tBSTFCtx.uBuffL = 3u;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        (void)printf("eFSP_MSGETST_General 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 17 -- FAIL \n");
    }
}


void eFSP_MSGETST_General2(void)
{
    /* Local variable */
    t_eFSP_MSGE_Ctx ctx;
    uint8_t  memArea[12u] = {0u};
    uint8_t  msgA[24u] = {0u};
    f_eFSP_MSGE_CrcCb cbCrcPTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;

    /* Set value */
    (void)memset(memArea, 0, sizeof(memArea));

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_General2 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General2 1  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetWherePutData(&ctx, &dataP, &var32) )
    {
        if( 4u == var32 )
        {
            (void)printf("eFSP_MSGETST_General2 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_General2 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General2 2  -- FAIL \n");
    }

    dataP[0u] = 1u;
    dataP[1u] = 2u;
    dataP[2u] = 3u;
    dataP[3u] = 0x53u;
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctx, var32) )
    {
        (void)printf("eFSP_MSGETST_General2 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General2 3  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 15u == var32)
        {
            (void)printf("eFSP_MSGETST_General2 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_General2 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General2 4  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&ctx, msgA, 6u, &var32) )
    {
        if( 6u == var32 )
        {
            if( (ECU_SOF == msgA[0u]) && (0x72u == msgA[1u]) && (0xC4u == msgA[2u]) && (ECU_ESC == msgA[3u]) &&
                ( ((uint8_t)(~0xA1u)) == msgA[4u] ) && (0xDBu == msgA[5u]) )
            {
                (void)printf("eFSP_MSGETST_General2 5  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_MSGETST_General2 5  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_MSGETST_General2 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General2 5  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 9u == var32)
        {
            (void)printf("eFSP_MSGETST_General2 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_General2 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General2 6  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&ctx, msgA, 4u, &var32) )
    {
        if( 4u == var32 )
        {
            if( (0x04u == msgA[0u]) && (0x00u == msgA[1u]) && (0x00u == msgA[2u]) && (0x00u == msgA[3u]) )
            {
                (void)printf("eFSP_MSGETST_General2 7  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_MSGETST_General2 7  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_MSGETST_General2 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General2 7  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 5u == var32)
        {
            (void)printf("eFSP_MSGETST_General2 8  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_General2 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General2 8  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_MESSAGEENDED == eFSP_MSGE_GetEncChunk(&ctx, msgA, 10u, &var32) )
    {
        if( 5u == var32 )
        {
            if( (0x01u == msgA[0u]) && (0x02u == msgA[1u]) && (0x03u == msgA[2u]) && (0x53u == msgA[3u]) &&
                ( ECU_EOF == msgA[4u])  )
            {
                (void)printf("eFSP_MSGETST_General2 9  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_MSGETST_General2 9  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_MSGETST_General2 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General2 9  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&ctx, &var32) )
    {
        if( 0u == var32)
        {
            (void)printf("eFSP_MSGETST_General2 10 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGETST_General2 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGETST_General2 10 -- FAIL \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "MISRAC2012-Rule-2.2_b"
#endif