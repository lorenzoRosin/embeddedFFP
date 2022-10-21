/**
 * @file eFSPMsgDeEncoderTest.c
 *
 */



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgDeEncoderTest.h"
#include "eCUCrc.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void msgDeEncoderTestCommon(void);



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
typedef struct
{
    e_eCU_Crc_Res lastError;
}s_eCU_crcAdapterCtx;

static bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void msgDeEncoderTest(void)
{
	(void)printf("\n\nMESSAGE DECODER ENCODER TEST START \n\n");

    msgDeEncoderTestCommon();

    (void)printf("\n\nMESSAGE DECODER ENCODER TEST END \n\n");
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

        ctxCur->lastError = crc32Seed(s, (const uint8_t*)d, dLen, c32Val);
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



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void msgDeEncoderTestCommon(void)
{
    /* Local variable for message ENCODER */
    s_eFSP_MsgECtx ctxEnc;
    s_eCU_crcAdapterCtx  ctxCrcEnc;
    cb_crc32_msge cbCrcPEnc = &c32SAdapt;
    uint8_t  memEncoderArea[100u] = {0u};
    uint32_t encMaxPaySize;
    uint8_t* encPayLoc;
    uint8_t encDataSend[200u];
    uint32_t encToSendSize;

    /* Local variable for message ENCODER */
    s_eFSP_MsgDCtx ctxDec;
    s_eCU_crcAdapterCtx  ctxCrcDec;
    cb_crc32_msgd cbCrcPDec = &c32SAdapt;
    uint8_t  memDecoderArea[100u] = {0u};
    uint32_t decTotCounter;
    uint32_t decCurCounter;
    uint32_t errorFound;
    uint32_t mostEfficientDataExtr;

    /* INIT MESSAGE ENCODER */
    if( MSGE_RES_OK == msgEncoderInitCtx(&ctxEnc, memEncoderArea, sizeof(memEncoderArea), cbCrcPEnc, &ctxCrcEnc) )
    {
        (void)printf("msgDeEncoderTestCommon 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDeEncoderTestCommon 1  -- FAIL \n");
    }

    /* INIT MESSAGE DECODER */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctxDec, memDecoderArea, sizeof(memDecoderArea), cbCrcPDec, &ctxCrcDec) )
    {
        (void)printf("msgDeEncoderTestCommon 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDeEncoderTestCommon 2  -- FAIL \n");
    }

    /* LOAD A MESSAGE IN THE ENCODER */
    if( MSGE_RES_OK == msgEncoderGetPayloadLocation(&ctxEnc, &encPayLoc, &encMaxPaySize) )
    {
        if( ( sizeof(memEncoderArea) - EFSP_MSGEN_HEADERSIZE ) == encMaxPaySize )
        {
            (void)printf("msgDeEncoderTestCommon 3  -- OK \n");
        }
        else
        {
            (void)printf("msgDeEncoderTestCommon 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDeEncoderTestCommon 3  -- FAIL \n");
    }

    memset(encPayLoc, 0xA5u, encMaxPaySize);

    if( MSGE_RES_OK == msgEncoderStartNewMessage(&ctxEnc, encMaxPaySize) )
    {
        (void)printf("msgDeEncoderTestCommon 4  -- OK \n");
    }
    else
    {
        (void)printf("msgDeEncoderTestCommon 4  -- FAIL \n");
    }

    /* RETRIVE DATA USING DECODER */
    if( MSGD_RES_OK == msgDecoderStartNewMsg(&ctxDec) )
    {
        (void)printf("msgDeEncoderTestCommon 5  -- OK \n");
    }
    else
    {
        (void)printf("msgDeEncoderTestCommon 5  -- FAIL \n");
    }

    /* RETRIVE ENCODED DATA */
    if( MSGE_RES_MESSAGEENDED == msgEncoderRetriveEChunk(&ctxEnc, encDataSend, sizeof(encDataSend), &encToSendSize) )
    {
        if( ( sizeof(memEncoderArea) + 2u ) == encToSendSize )
        {
            (void)printf("msgDeEncoderTestCommon 6  -- OK \n");
        }
        else
        {
            (void)printf("msgDeEncoderTestCommon 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDeEncoderTestCommon 6  -- FAIL \n");
    }

    /* DECODER DATA */
    decTotCounter = 0u;
    decCurCounter = 0u;
    errorFound = 0u;

    if( MSGD_RES_OK == msgDecoderGetMostEffDatLen(&ctxDec, &mostEfficientDataExtr) )
    {
        if( 8u == mostEfficientDataExtr )
        {
            (void)printf("msgDeEncoderTestCommon 7  -- OK \n");
        }
        else
        {
            (void)printf("msgDeEncoderTestCommon 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDeEncoderTestCommon 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderInsEncChunk(&ctxDec, encDataSend, mostEfficientDataExtr, &decCurCounter, &errorFound) )
    {
        (void)printf("msgDeEncoderTestCommon 8  -- OK \n");
    }
    else
    {
        (void)printf("msgDeEncoderTestCommon 8  -- FAIL \n");
    }
    decTotCounter += decCurCounter;

    if( MSGD_RES_OK == msgDecoderGetMostEffDatLen(&ctxDec, &mostEfficientDataExtr) )
    {
        if( 1u == mostEfficientDataExtr )
        {
            (void)printf("msgDeEncoderTestCommon 9  -- OK \n");
        }
        else
        {
            (void)printf("msgDeEncoderTestCommon 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDeEncoderTestCommon 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderInsEncChunk(&ctxDec, &encDataSend[decTotCounter], mostEfficientDataExtr, &decCurCounter, &errorFound) )
    {
        (void)printf("msgDeEncoderTestCommon 10 -- OK \n");
    }
    else
    {
        (void)printf("msgDeEncoderTestCommon 10 -- FAIL \n");
    }
    decTotCounter += decCurCounter;

    if( MSGD_RES_OK == msgDecoderGetMostEffDatLen(&ctxDec, &mostEfficientDataExtr) )
    {
        if( 93u == mostEfficientDataExtr )
        {
            (void)printf("msgDeEncoderTestCommon 11 -- OK \n");
        }
        else
        {
            (void)printf("msgDeEncoderTestCommon 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDeEncoderTestCommon 11 -- FAIL \n");
    }

    if( MSGD_RES_MESSAGEENDED == msgDecoderInsEncChunk(&ctxDec, &encDataSend[decTotCounter], mostEfficientDataExtr, &decCurCounter, &errorFound) )
    {
        if( mostEfficientDataExtr == decCurCounter )
        {
            (void)printf("msgDeEncoderTestCommon 12 -- OK \n");
        }
        else
        {
            (void)printf("msgDeEncoderTestCommon 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDeEncoderTestCommon 12 -- FAIL \n");
    }
    decTotCounter += decCurCounter;
}
