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
                            "CERT-INT30-C_b"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
struct t_eFSP_MSGD_CrcCtxUser
{
    e_eCU_CRC_RES lastError;
};

struct t_eFSP_MSGE_CrcCtxUser
{
    e_eCU_CRC_RES lastError;
};

static bool_t c32SAdapt(t_eFSP_MSGD_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptE(t_eFSP_MSGE_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);


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
bool_t c32SAdapt(t_eFSP_MSGD_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val)
{
    bool_t result;
    t_eFSP_MSGD_CrcCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == c32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGD_CrcCtx*)cntx;

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

bool_t c32SAdaptE(t_eFSP_MSGE_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val)
{
    bool_t result;
    t_eFSP_MSGE_CrcCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == c32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGE_CrcCtx*)cntx;

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

/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSP_MSGDMSGETST_Common(void)
{
    /* Local variable for message ENCODER */
    t_eFSP_MSGE_Ctx ctxEnc;
    t_eFSP_MSGE_CrcCtx  ctxCrcEnc;
    f_eFSP_MSGE_CrcCb cbCrcPEnc = &c32SAdaptE;
    uint8_t  memEncoderArea[100u] = {0u};
    uint32_t encMaxPaySize;
    uint8_t* encPayLoc;
    uint8_t encDataSend[200u];
    uint32_t encToSendSize;
    e_eFSP_MSGE_RES encoderRet;
    uint32_t encCounter;

    /* Local variable for message ENCODER */
    t_eFSP_MSGD_Ctx ctxDec;
    t_eFSP_MSGD_CrcCtx  ctxCrcDec;
    f_eFSP_MSGD_CrcCb cbCrcPDec = &c32SAdapt;
    uint8_t  memDecoderArea[100u] = {0u};
    uint32_t decTotCounter;
    uint32_t decCurCounter;
    uint32_t mostEfficientDataExtr;

    /* INIT MESSAGE ENCODER */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctxEnc, memEncoderArea, sizeof(memEncoderArea), cbCrcPEnc, &ctxCrcEnc) )
    {
        (void)printf("eFSP_MSGDMSGETST_Common 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 1  -- FAIL \n");
    }

    /* INIT MESSAGE DECODER */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctxDec, memDecoderArea, sizeof(memDecoderArea), cbCrcPDec, &ctxCrcDec) )
    {
        (void)printf("eFSP_MSGDMSGETST_Common 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 2  -- FAIL \n");
    }

    /* LOAD A MESSAGE IN THE ENCODER */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_GetWherePutData(&ctxEnc, &encPayLoc, &encMaxPaySize) )
    {
        if( ( sizeof(memEncoderArea) - EFSP_MSGEN_HEADERSIZE ) == encMaxPaySize )
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
    encPayLoc[0u] = ECU_SOF;
    encPayLoc[1u] = ECU_ESC;
    encPayLoc[2u] = ECU_ESC;
    encPayLoc[3u] = ECU_EOF;
    encPayLoc[4u] = 0xFFu;
    encPayLoc[5u] = 0xCCu;
    encPayLoc[6u] = ECU_SOF;

    /* Start encoding a new message */
    if( e_eFSP_MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctxEnc, 0x07u) )
    {
        (void)printf("eFSP_MSGDMSGETST_Common 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 4  -- FAIL \n");
    }

    /* Start deconding a new message */
    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctxDec) )
    {
        (void)printf("eFSP_MSGDMSGETST_Common 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 5  -- FAIL \n");
    }

    /* RETRIVE ENCODED DATA */
    encoderRet = e_eFSP_MSGE_RES_OK;
    encCounter = 0u;

    while( e_eFSP_MSGE_RES_OK == encoderRet )
    {
        encoderRet = eFSP_MSGE_GetEncChunk(&ctxEnc, &encDataSend[encCounter], 1u, &encToSendSize);

        if( ( e_eFSP_MSGE_RES_OK == encoderRet ) || ( e_eFSP_MSGE_RES_MESSAGEENDED == encoderRet ) )
        {
            encCounter += encToSendSize;
        }
    }

    if( e_eFSP_MSGE_RES_MESSAGEENDED == encoderRet )
    {
        if( 22u == encCounter )
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
    decTotCounter = 0u;
    decCurCounter = 0u;

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctxDec, &mostEfficientDataExtr) )
    {
        if( 9u == mostEfficientDataExtr )
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

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctxDec, encDataSend, 8u, &decCurCounter) )
    {
        (void)printf("eFSP_MSGDMSGETST_Common 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 8  -- FAIL \n");
    }
    decTotCounter += decCurCounter;

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctxDec, &mostEfficientDataExtr) )
    {
        if( 1u == mostEfficientDataExtr )
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

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctxDec, &encDataSend[decTotCounter], mostEfficientDataExtr, &decCurCounter) )
    {
        (void)printf("eFSP_MSGDMSGETST_Common 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGDMSGETST_Common 10 -- FAIL \n");
    }
    decTotCounter += decCurCounter;

    if( e_eFSP_MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctxDec, &mostEfficientDataExtr) )
    {
        if( 8u == mostEfficientDataExtr )
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

    if( e_eFSP_MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctxDec, &encDataSend[decTotCounter], 30u, &decCurCounter) )
    {
        if( 13u == decCurCounter )
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