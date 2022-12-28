/**
 * @file       eFSP_MSGDMSGETST.c
 *
 * @brief      Message encoder dencoder test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_MSGDMSGETST.h"
#include "eFSP_MSGE.h"
#include "eFSP_MSGD.h"
#include "eCU_CRC.h"
#include <stdio.h>

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "CERT-INT30-C_b"
    /* Suppressed for code clarity in test execution*/
#endif





/***********************************************************************************************************************
 *   PRIVATE TEST STRUCT DECLARATION
 **********************************************************************************************************************/
struct t_eFSP_MSGD_CrcCtxUser
{
    e_eCU_CRC_RES eLastEr;
};

struct t_eFSP_MSGE_CrcCtxUser
{
    e_eCU_CRC_RES eLastEr;
};



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSP_MSGDMSGETST_c32SAdapt(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val);
static bool_t eFSP_MSGDMSGETST_c32SAdaptE(t_eFSP_MSGE_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val);



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSP_MSGDMSGETST_Common(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_MSGDMSGETST_ExeTest(void)
{
	(void)printf("\n\nMESSAGE DECODER ENCODER TEST START \n\n");

    eFSP_MSGDMSGETST_Common();

    (void)printf("\n\nMESSAGE DECODER ENCODER TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION IMPLEMENTATION
 **********************************************************************************************************************/
bool_t eFSP_MSGDMSGETST_c32SAdapt(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t l_bRes;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->eLastEr = eCU_CRC_32Seed(p_uS, (const uint8_t*)p_puD, p_uDLen, p_puC32Val);
        if( e_eCU_CRC_RES_OK == p_ptCtx->eLastEr )
        {
            l_bRes = true;
        }
        else
        {
            l_bRes = false;
        }
    }

    return l_bRes;
}

bool_t eFSP_MSGDMSGETST_c32SAdaptE(t_eFSP_MSGE_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t l_bRes;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->eLastEr = eCU_CRC_32Seed(p_uS, (const uint8_t*)p_puD, p_uDLen, p_puC32Val);
        if( e_eCU_CRC_RES_OK == p_ptCtx->eLastEr )
        {
            l_bRes = true;
        }
        else
        {
            l_bRes = false;
        }
    }

    return l_bRes;
}

/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSP_MSGDMSGETST_Common(void)
{
    /* Local variable for message ENCODER */
    t_eFSP_MSGE_Ctx l_tCtxEnc;
    t_eFSP_MSGE_CrcCtx  l_tCtxCrcEnc;
    f_eFSP_MSGE_CrcCb l_fCrcEnc = &eFSP_MSGDMSGETST_c32SAdaptE;
    uint8_t  l_auEemEncoderArea[100u] = {0u};
    uint32_t l_uEncMaxPaySize;
    uint8_t* l_puEncPayLoc;
    uint8_t l_auEncDataSend[200u];
    uint32_t l_uEncToSendSize;
    e_eFSP_MSGE_RES l_eRncoderRet;
    uint32_t l_uEncCounter;

    /* Local variable for message ENCODER */
    t_eFSP_MSGD_Ctx l_tCtxDec;
    t_eFSP_MSGD_CrcCtx  l_tCtxCrcDec;
    f_eFSP_MSGD_CrcCb l_fCrcPDec = &eFSP_MSGDMSGETST_c32SAdapt;
    uint8_t  l_auMemDecoderArea[100u] = {0u};
    uint32_t l_uDecTotCounter;
    uint32_t l_uDecCurCounter;
    uint32_t l_uMostEfficientDataExtr;

    /* INIT MESSAGE ENCODER */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&l_tCtxEnc, l_auEemEncoderArea, sizeof(l_auEemEncoderArea), l_fCrcEnc, &l_tCtxCrcEnc) )
    {
        (void)printf("eFSP_MSGDMSGETST_Common 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 1  -- FAIL \n");
    }

    /* INIT MESSAGE DECODER */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&l_tCtxDec, l_auMemDecoderArea, sizeof(l_auMemDecoderArea), l_fCrcPDec, &l_tCtxCrcDec) )
    {
        (void)printf("eFSP_MSGDMSGETST_Common 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 2  -- FAIL \n");
    }

    /* LOAD A MESSAGE IN THE ENCODER */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetWherePutData(&l_tCtxEnc, &l_puEncPayLoc, &l_uEncMaxPaySize) )
    {
        if( ( sizeof(l_auEemEncoderArea) - EFSP_MSGEN_HEADERSIZE ) == l_uEncMaxPaySize )
        {
            (void)printf("eFSP_MSGDMSGETST_Common 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDMSGETST_Common 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 3  -- FAIL \n");
    }

    /* Inserte data to encode */
    l_puEncPayLoc[0u] = ECU_SOF;
    l_puEncPayLoc[1u] = ECU_ESC;
    l_puEncPayLoc[2u] = ECU_ESC;
    l_puEncPayLoc[3u] = ECU_EOF;
    l_puEncPayLoc[4u] = 0xFFu;
    l_puEncPayLoc[5u] = 0xCCu;
    l_puEncPayLoc[6u] = ECU_SOF;

    /* Start encoding a new message */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&l_tCtxEnc, 0x07u) )
    {
        (void)printf("eFSP_MSGDMSGETST_Common 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 4  -- FAIL \n");
    }

    /* Start deconding a new message */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&l_tCtxDec) )
    {
        (void)printf("eFSP_MSGDMSGETST_Common 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 5  -- FAIL \n");
    }

    /* RETRIVE ENCODED DATA */
    l_eRncoderRet = e_eFSP_MSGE_RES_OK;
    l_uEncCounter = 0u;

    while( e_eFSP_MSGE_RES_OK == l_eRncoderRet )
    {
        l_eRncoderRet = eFSP_MSGE_GetEncChunk(&l_tCtxEnc, &l_auEncDataSend[l_uEncCounter], 1u, &l_uEncToSendSize);

        if( ( e_eFSP_MSGE_RES_OK == l_eRncoderRet ) || ( e_eFSP_MSGE_RES_MESSAGEENDED == l_eRncoderRet ) )
        {
            l_uEncCounter += l_uEncToSendSize;
        }
    }

    if( e_eFSP_MSGE_RES_MESSAGEENDED == l_eRncoderRet )
    {
        if( 22u == l_uEncCounter )
        {
            (void)printf("eFSP_MSGDMSGETST_Common 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDMSGETST_Common 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 6  -- FAIL \n");
    }

    /* DECODER DATA */
    l_uDecTotCounter = 0u;
    l_uDecCurCounter = 0u;

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtxDec, &l_uMostEfficientDataExtr) )
    {
        if( 9u == l_uMostEfficientDataExtr )
        {
            (void)printf("eFSP_MSGDMSGETST_Common 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDMSGETST_Common 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 7  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtxDec, l_auEncDataSend, 8u, &l_uDecCurCounter) )
    {
        (void)printf("eFSP_MSGDMSGETST_Common 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 8  -- FAIL \n");
    }
    l_uDecTotCounter += l_uDecCurCounter;

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtxDec, &l_uMostEfficientDataExtr) )
    {
        if( 1u == l_uMostEfficientDataExtr )
        {
            (void)printf("eFSP_MSGDMSGETST_Common 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDMSGETST_Common 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 9  -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&l_tCtxDec, &l_auEncDataSend[l_uDecTotCounter], l_uMostEfficientDataExtr, &l_uDecCurCounter) )
    {
        (void)printf("eFSP_MSGDMSGETST_Common 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 10 -- FAIL \n");
    }
    l_uDecTotCounter += l_uDecCurCounter;

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&l_tCtxDec, &l_uMostEfficientDataExtr) )
    {
        if( 8u == l_uMostEfficientDataExtr )
        {
            (void)printf("eFSP_MSGDMSGETST_Common 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDMSGETST_Common 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 11 -- FAIL \n");
    }

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&l_tCtxDec, &l_auEncDataSend[l_uDecTotCounter], 30u, &l_uDecCurCounter) )
    {
        if( 13u == l_uDecCurCounter )
        {
            (void)printf("eFSP_MSGDMSGETST_Common 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGDMSGETST_Common 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 12 -- FAIL \n");
    }

}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "CERT-INT30-C_b"
#endif