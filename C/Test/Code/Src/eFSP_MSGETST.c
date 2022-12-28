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
#include <stdio.h>

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "MISRAC2012-Rule-2.2_b"
    /* Suppressed for code clarity in test execution*/
#endif

/***********************************************************************************************************************
 *   PRIVATE TEST STRUCT DECLARATION
 **********************************************************************************************************************/
struct t_eFSP_MSGE_CrcCtxUser
{
    e_eCU_CRC_RES eLastEr;
};



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
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
    bool_t l_bResult;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        l_bResult = false;
    }
    else
    {
        p_ptCtx->eLastEr = eCU_CRC_32Seed(p_uS, (const uint8_t*)p_puD, p_uDLen, p_puC32Val);
        if( e_eCU_CRC_RES_OK == p_ptCtx->eLastEr )
        {
            l_bResult = true;
        }
        else
        {
            l_bResult = false;
        }
    }

    return l_bResult;
}

bool_t eFSP_MSGETST_c32SAdaptEr(t_eFSP_MSGE_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t l_bResult;

    (void)p_uS;
    (void)p_puD;
    (void)p_uDLen;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        l_bResult = false;
    }
    else
    {
        p_ptCtx->eLastEr = e_eCU_CRC_RES_BADPOINTER;
        l_bResult = false;
        *p_puC32Val = 0u;
    }

    return l_bResult;
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSP_MSGETST_BadPointer(void)
{
    /* Local variable */
    t_eFSP_MSGE_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    f_eFSP_MSGE_CrcCb l_fCrcTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;
    uint8_t* l_puData;
    bool_t l_bIsInit;

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_InitCtx(NULL, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_InitCtx(&l_tCtx, NULL, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), NULL, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, NULL) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_NewMessage(NULL, sizeof(l_auMemArea)) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetWherePutData(NULL, &l_puData, &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetWherePutData(&l_tCtx, NULL, &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetWherePutData(&l_tCtx, &l_puData, NULL) )
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
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetRemByteToGet(NULL, &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 10 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetRemByteToGet(&l_tCtx, NULL) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetEncChunk(NULL, l_auMemArea, sizeof(l_auMemArea), &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetEncChunk(&l_tCtx, NULL, sizeof(l_auMemArea), &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), NULL) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 14 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_IsInit( NULL, &l_bIsInit ) )
    {
        (void)printf("eFSP_MSGETST_BadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadPointer 15 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_BADPOINTER == eFSP_MSGE_IsInit( &l_tCtx, NULL ) )
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
    t_eFSP_MSGE_Ctx l_tCtx;
    uint8_t  l_auMemArea[5u];
    uint32_t l_uVar32;
    uint8_t* l_puData;
    bool_t l_bIsInit;
    f_eFSP_MSGE_CrcCb l_fCrcTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx l_tCtxAdapterCrc;

    /* Set value */
    l_tCtx.tBSTFCtx.bIsInit = false;
    l_tCtx.ptCrcCtx = &l_tCtxAdapterCrc;
    l_tCtx.fCrc = l_fCrcTest;

    /* Function */
    if( e_eFSP_MSGE_RES_NOINITLIB == eFSP_MSGE_NewMessage(&l_tCtx, 1u) )
    {
        (void)printf("eFSP_MSGETST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInit 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_NOINITLIB == eFSP_MSGE_GetWherePutData(&l_tCtx, &l_puData, &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_BadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInit 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_NOINITLIB == eFSP_MSGE_RestartMessage(&l_tCtx) )
    {
        (void)printf("eFSP_MSGETST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInit 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_NOINITLIB == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInit 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_NOINITLIB == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInit 5  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( false == l_bIsInit )
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
    t_eFSP_MSGE_Ctx l_tCtx;
    uint8_t  l_auMemArea[10u];
    f_eFSP_MSGE_CrcCb l_fCrcTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;
    bool_t l_bIsInit;

    /* Set value */
    (void)memset(l_auMemArea, 0, sizeof(l_auMemArea));

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 1  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_NOINITMESSAGE == eFSP_MSGE_RestartMessage(&l_tCtx) )
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 2  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_NOINITMESSAGE == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 3  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_NOINITMESSAGE == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadInitMsg 4  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_IsInit( &l_tCtx, &l_bIsInit ) )
    {
        if( true == l_bIsInit )
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
    t_eFSP_MSGE_Ctx l_tCtx;
    uint8_t  l_auMemArea[10u];
    f_eFSP_MSGE_CrcCb l_fCrcTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;

    /* Set value */
    (void)memset(l_auMemArea, 0, sizeof(l_auMemArea));

    /* Function */
    if( e_eFSP_MSGE_RES_BADPARAM == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, EFSP_MIN_MSGEN_BUFFLEN - 1u, l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 1  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 2  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_BADPARAM == eFSP_MSGE_NewMessage(&l_tCtx, 0u) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 3  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_BADPARAM == eFSP_MSGE_NewMessage(&l_tCtx, (MAX_UINT32VAL - EFSP_MIN_MSGEN_PAYLOAD + 1u) ) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 4  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_BADPARAM == eFSP_MSGE_NewMessage(&l_tCtx, (3u) ) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 5  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&l_tCtx, (2u) ) )
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadParamEntr 6  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_BADPARAM == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMemArea, 0u, &l_uVar32) )
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
    t_eFSP_MSGE_Ctx l_tCtx;
    uint8_t  l_auMemArea[10u];
    f_eFSP_MSGE_CrcCb l_fCrcTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;
    uint8_t* l_puData;

    /* Set value */
    (void)memset(l_auMemArea, 0, sizeof(l_auMemArea));

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 1  -- FAIL \n");
    }

    l_tCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_NewMessage(&l_tCtx, 2u) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 3  -- FAIL \n");
    }

    l_tCtx.fCrc = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_NewMessage(&l_tCtx, 2u) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 5  -- FAIL \n");
    }

    l_tCtx.fCrc = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetWherePutData(&l_tCtx, &l_puData, &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 7  -- FAIL \n");
    }

    l_tCtx.fCrc = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_RestartMessage(&l_tCtx) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 9  -- FAIL \n");
    }

    l_tCtx.fCrc = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 10 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 11 -- FAIL \n");
    }

    l_tCtx.fCrc = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMemArea, 2u, &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 13 -- FAIL \n");
    }

    l_tCtx.tBSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_NewMessage(&l_tCtx, 2u) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 15 -- FAIL \n");
    }

    l_tCtx.tBSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetWherePutData(&l_tCtx, &l_puData, &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 17 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&l_tCtx, 2u) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 18 -- FAIL \n");
    }

    l_tCtx.tBSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_RestartMessage(&l_tCtx) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 20 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&l_tCtx, 2u) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 21 -- FAIL \n");
    }

    l_tCtx.tBSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 22 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 23 -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&l_tCtx, 2u) )
    {
        (void)printf("eFSP_MSGETST_CorruptContext 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_CorruptContext 24 -- FAIL \n");
    }

    l_tCtx.tBSTFCtx.puBuff = NULL;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMemArea, 2u, &l_uVar32) )
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
    t_eFSP_MSGE_Ctx l_tCtx;
    uint8_t  l_auMemArea[10u];
    f_eFSP_MSGE_CrcCb l_fCrcTest = &eFSP_MSGETST_c32SAdaptEr;
    t_eFSP_MSGE_CrcCtx l_tCtxAdapterCrc;

    /* Set value */
    (void)memset(l_auMemArea, 0, sizeof(l_auMemArea));

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_BadClBck 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_BadClBck 1  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_CRCCLBKERROR == eFSP_MSGE_NewMessage(&l_tCtx, 2u) )
    {
        if( e_eCU_CRC_RES_BADPOINTER == l_tCtxAdapterCrc.eLastEr )
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
    t_eFSP_MSGE_Ctx l_tCtx;
    uint8_t  l_auMemArea[12u] = {0u};
    uint8_t  l_auMsgA[24u] = {0u};
    f_eFSP_MSGE_CrcCb l_fCrcTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;
    uint8_t* l_puData;

    /* Set value */
    (void)memset(l_auMemArea, 0, sizeof(l_auMemArea));

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_MsgEnd 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_MsgEnd 1  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetWherePutData(&l_tCtx, &l_puData, &l_uVar32) )
    {
        if( 4u == l_uVar32 )
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

    l_puData[0u] = 1u;
    l_puData[1u] = 2u;
    l_puData[2u] = 3u;
    l_puData[3u] = 4u;

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&l_tCtx, 4u) )
    {
        (void)printf("eFSP_MSGETST_MsgEnd 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_MsgEnd 3  -- FAIL \n");
    }

    /* Parse all the message */
    if( e_eFSP_MSGE_RES_MESSAGEENDED == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMsgA, sizeof(l_auMsgA), &l_uVar32) )
    {
        if( 14u == l_uVar32 )
        {
            if( (0x70u == l_auMsgA[1u]) && (0x3Fu == l_auMsgA[2u]) && (0x71u == l_auMsgA[3u]) && (0xBDu == l_auMsgA[4u]) )
            {
                if( (4u == l_auMsgA[5u]) && (0u == l_auMsgA[6u]) && (0u == l_auMsgA[7u]) && (0u == l_auMsgA[8u]) )
                {
                    if( (1u == l_auMsgA[9u]) && (2u == l_auMsgA[10u]) && (3u == l_auMsgA[11u]) && (4u == l_auMsgA[12u]) )
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
    t_eFSP_MSGE_Ctx l_tCtx;
    uint8_t  l_auMemArea[12u] = {0u};
    uint8_t  l_auMsgA[24u] = {0u};
    f_eFSP_MSGE_CrcCb l_fCrcTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;
    uint8_t* l_puData;

    /* Set value */
    (void)memset(l_auMemArea, 0, sizeof(l_auMemArea));

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_General 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 1  -- FAIL \n");
    }

    l_tCtx.tBSTFCtx.uBuffL = 3u;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_NewMessage(&l_tCtx, sizeof(l_auMemArea)) )
    {
        (void)printf("eFSP_MSGETST_General 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_General 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 3  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetWherePutData(&l_tCtx, &l_puData, &l_uVar32) )
    {
        if( 4u == l_uVar32 )
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

    l_puData[0u] = 1u;
    l_puData[1u] = 2u;
    l_puData[2u] = 3u;
    l_puData[3u] = ECU_ESC;
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&l_tCtx, l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_General 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 5  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        if( 15u == l_uVar32)
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

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMsgA, 5u, &l_uVar32) )
    {
        if( 5u == l_uVar32 )
        {
            if( (ECU_SOF == l_auMsgA[0u]) && (0x7Bu == l_auMsgA[1u]) && (0x39u == l_auMsgA[2u]) && (0x19u == l_auMsgA[3u]) &&
                (0x52u == l_auMsgA[4u]) )
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

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_RestartMessage(&l_tCtx) )
    {
        (void)printf("eFSP_MSGETST_General 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 8  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        if( 15u == l_uVar32)
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

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMsgA, 5u, &l_uVar32) )
    {
        if( 5u == l_uVar32 )
        {
            if( (ECU_SOF == l_auMsgA[0u]) && (0x7Bu == l_auMsgA[1u]) && (0x39u == l_auMsgA[2u]) && (0x19u == l_auMsgA[3u]) &&
                (0x52u == l_auMsgA[4u]) )
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

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        if( 10u == l_uVar32)
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

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMsgA, 4u, &l_uVar32) )
    {
        if( 4u == l_uVar32 )
        {
            if( (0x04u == l_auMsgA[0u]) && (0x00u == l_auMsgA[1u]) && (0x00u == l_auMsgA[2u]) && (0x00u == l_auMsgA[3u]) )
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

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        if( 6u == l_uVar32)
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

    if( e_eFSP_MSGE_RES_MESSAGEENDED == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMsgA, 10u, &l_uVar32) )
    {
        if( 6u == l_uVar32 )
        {
            if( (0x01u == l_auMsgA[0u]) && (0x02u == l_auMsgA[1u]) && (0x03u == l_auMsgA[2u]) && (ECU_ESC == l_auMsgA[3u]) &&
                ( ((uint8_t)~ECU_ESC) == l_auMsgA[4u]) && (ECU_EOF == l_auMsgA[5u]) )
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

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        if( 0u == l_uVar32)
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
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_General 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General 16 -- FAIL \n");
    }

    l_tCtx.tBSTFCtx.uBuffL = 3u;
    if( e_eFSP_MSGE_RES_CORRUPTCTX == eFSP_MSGE_GetWherePutData(&l_tCtx, &l_puData, &l_uVar32) )
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
    t_eFSP_MSGE_Ctx l_tCtx;
    uint8_t  l_auMemArea[12u] = {0u};
    uint8_t  l_auMsgA[24u] = {0u};
    f_eFSP_MSGE_CrcCb l_fCrcTest = &eFSP_MSGETST_c32SAdapt;
    t_eFSP_MSGE_CrcCtx l_tCtxAdapterCrc;
    uint32_t l_uVar32;
    uint8_t* l_puData;

    /* Set value */
    (void)memset(l_auMemArea, 0, sizeof(l_auMemArea));

    /* Function */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtx, l_auMemArea, sizeof(l_auMemArea), l_fCrcTest, &l_tCtxAdapterCrc) )
    {
        (void)printf("eFSP_MSGETST_General2 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General2 1  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetWherePutData(&l_tCtx, &l_puData, &l_uVar32) )
    {
        if( 4u == l_uVar32 )
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

    l_puData[0u] = 1u;
    l_puData[1u] = 2u;
    l_puData[2u] = 3u;
    l_puData[3u] = 0x53u;
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&l_tCtx, l_uVar32) )
    {
        (void)printf("eFSP_MSGETST_General2 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGETST_General2 3  -- FAIL \n");
    }

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        if( 15u == l_uVar32)
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

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMsgA, 6u, &l_uVar32) )
    {
        if( 6u == l_uVar32 )
        {
            if( (ECU_SOF == l_auMsgA[0u]) && (0x72u == l_auMsgA[1u]) && (0xC4u == l_auMsgA[2u]) && (ECU_ESC == l_auMsgA[3u]) &&
                ( ((uint8_t)(~0xA1u)) == l_auMsgA[4u] ) && (0xDBu == l_auMsgA[5u]) )
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

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        if( 9u == l_uVar32)
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

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMsgA, 4u, &l_uVar32) )
    {
        if( 4u == l_uVar32 )
        {
            if( (0x04u == l_auMsgA[0u]) && (0x00u == l_auMsgA[1u]) && (0x00u == l_auMsgA[2u]) && (0x00u == l_auMsgA[3u]) )
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

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        if( 5u == l_uVar32)
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

    if( e_eFSP_MSGE_RES_MESSAGEENDED == eFSP_MSGE_GetEncChunk(&l_tCtx, l_auMsgA, 10u, &l_uVar32) )
    {
        if( 5u == l_uVar32 )
        {
            if( (0x01u == l_auMsgA[0u]) && (0x02u == l_auMsgA[1u]) && (0x03u == l_auMsgA[2u]) && (0x53u == l_auMsgA[3u]) &&
                ( ECU_EOF == l_auMsgA[4u])  )
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

    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetRemByteToGet(&l_tCtx, &l_uVar32) )
    {
        if( 0u == l_uVar32)
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