/**
 * @file       eFSPMsgDeEncoderTest.c
 *
 * @brief      Message encoder dencoder test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgDeEncoderTest.h"
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
typedef struct
{
    e_eCU_CRC_RES lastError;
}s_eCU_crcAdapterCtx;

static bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSP_TEST_msgDeEncoderCommon(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_TEST_msgDeEncoder(void)
{
	(void)printf("\n\nMESSAGE DECODER ENCODER TEST START \n\n");

    eFSP_TEST_msgDeEncoderCommon();

    (void)printf("\n\nMESSAGE DECODER ENCODER TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION IMPLEMENTATION
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



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSP_TEST_msgDeEncoderCommon(void)
{
    /* Local variable for message ENCODER */
    s_eFSP_MSGE_Ctx ctxEnc;
    s_eCU_crcAdapterCtx  ctxCrcEnc;
    cb_crc32_msge cbCrcPEnc = &c32SAdapt;
    uint8_t  memEncoderArea[100u] = {0u};
    uint32_t encMaxPaySize;
    uint8_t* encPayLoc;
    uint8_t encDataSend[200u];
    uint32_t encToSendSize;
    e_eFSP_MSGE_Res encoderRet;
    uint32_t encCounter;

    /* Local variable for message ENCODER */
    s_eFSP_MSGD_Ctx ctxDec;
    s_eCU_crcAdapterCtx  ctxCrcDec;
    cb_crc32_msgd cbCrcPDec = &c32SAdapt;
    uint8_t  memDecoderArea[100u] = {0u};
    uint32_t decTotCounter;
    uint32_t decCurCounter;
    uint32_t mostEfficientDataExtr;

    /* INIT MESSAGE ENCODER */
    if( MSGE_RES_OK == eFSP_MSGE_InitCtx(&ctxEnc, memEncoderArea, sizeof(memEncoderArea), cbCrcPEnc, &ctxCrcEnc) )
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 1  -- FAIL \n");
    }

    /* INIT MESSAGE DECODER */
    if( MSGD_RES_OK == eFSP_MSGD_InitCtx(&ctxDec, memDecoderArea, sizeof(memDecoderArea), cbCrcPDec, &ctxCrcDec) )
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 2  -- FAIL \n");
    }

    /* LOAD A MESSAGE IN THE ENCODER */
    if( MSGE_RES_OK == eFSP_MSGE_GetWherePutData(&ctxEnc, &encPayLoc, &encMaxPaySize) )
    {
        if( ( sizeof(memEncoderArea) - EFSP_MSGEN_HEADERSIZE ) == encMaxPaySize )
        {
            (void)printf("eFSP_TEST_msgDeEncoderCommon 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDeEncoderCommon 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 3  -- FAIL \n");
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
    if( MSGE_RES_OK == eFSP_MSGE_NewMessage(&ctxEnc, 0x07u) )
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 4  -- FAIL \n");
    }

    /* Start deconding a new message */
    if( MSGD_RES_OK == eFSP_MSGD_NewMsg(&ctxDec) )
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 5  -- FAIL \n");
    }

    /* RETRIVE ENCODED DATA */
    encoderRet = MSGE_RES_OK;
    encCounter = 0u;

    while( MSGE_RES_OK == encoderRet )
    {
        encoderRet = eFSP_MSGE_GetEncChunk(&ctxEnc, &encDataSend[encCounter], 1u, &encToSendSize);

        if( ( MSGE_RES_OK == encoderRet ) || ( MSGE_RES_MESSAGEENDED == encoderRet ) )
        {
            encCounter += encToSendSize;
        }
    }

    if( MSGE_RES_MESSAGEENDED == encoderRet )
    {
        if( 22u == encCounter )
        {
            (void)printf("eFSP_TEST_msgDeEncoderCommon 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDeEncoderCommon 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 6  -- FAIL \n");
    }

    /* DECODER DATA */
    decTotCounter = 0u;
    decCurCounter = 0u;

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctxDec, &mostEfficientDataExtr) )
    {
        if( 9u == mostEfficientDataExtr )
        {
            (void)printf("eFSP_TEST_msgDeEncoderCommon 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDeEncoderCommon 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctxDec, encDataSend, 8u, &decCurCounter) )
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 8  -- FAIL \n");
    }
    decTotCounter += decCurCounter;

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctxDec, &mostEfficientDataExtr) )
    {
        if( 1u == mostEfficientDataExtr )
        {
            (void)printf("eFSP_TEST_msgDeEncoderCommon 9  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDeEncoderCommon 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == eFSP_MSGD_InsEncChunk(&ctxDec, &encDataSend[decTotCounter], mostEfficientDataExtr, &decCurCounter) )
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 10 -- FAIL \n");
    }
    decTotCounter += decCurCounter;

    if( MSGD_RES_OK == eFSP_MSGD_GetMostEffDatLen(&ctxDec, &mostEfficientDataExtr) )
    {
        if( 8u == mostEfficientDataExtr )
        {
            (void)printf("eFSP_TEST_msgDeEncoderCommon 11 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDeEncoderCommon 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 11 -- FAIL \n");
    }

    if( MSGD_RES_MESSAGEENDED == eFSP_MSGD_InsEncChunk(&ctxDec, &encDataSend[decTotCounter], 30u, &decCurCounter) )
    {
        if( 13u == decCurCounter )
        {
            (void)printf("eFSP_TEST_msgDeEncoderCommon 12 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgDeEncoderCommon 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgDeEncoderCommon 12 -- FAIL \n");
    }

}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "CERT-INT30-C_b"
#endif