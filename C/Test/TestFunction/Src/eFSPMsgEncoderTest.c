/**
 * @file eFSPMsgEncoderTest.c
 *
 */



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgEncoderTest.h"
#include <stdio.h>
#include "eCUCrc.h"



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
typedef struct
{
    e_eCU_Crc_Res lastError;
}s_eCU_crcAdapterCtx;

static bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptEr(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void msgEncoderTestBadPointer(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void msgEncoderTest(void)
{
	(void)printf("\n\nMESSAGE ENCODER TEST START \n\n");

    msgEncoderTestBadPointer();

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
    s_eFSP_MsgECtx ctx;
    uint8_t  memArea[5u];
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;

    /* Function */
    if( MSGE_RES_BADPOINTER == msgEncoderInitCtx(NULL, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == msgEncoderInitCtx(&ctx, NULL, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 2  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == msgEncoderInitCtx(&ctx, memArea, sizeof(memArea), NULL, &ctxAdapterCrc) )
    {
        (void)printf("msgEncoderTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == msgEncoderInitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, NULL) )
    {
        (void)printf("msgEncoderTestBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 4  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == msgEncoderStartNewMessage(NULL, sizeof(memArea)) )
    {
        (void)printf("msgEncoderTestBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 5  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == msgEncoderGetPayloadLocation(NULL, &dataP, &var32) )
    {
        (void)printf("msgEncoderTestBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 6  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == msgEncoderGetPayloadLocation(&ctx, NULL, &var32) )
    {
        (void)printf("msgEncoderTestBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 7  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == msgEncoderGetPayloadLocation(&ctx, &dataP, NULL) )
    {
        (void)printf("msgEncoderTestBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 8  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == msgEncoderRestartCurrentMessage(NULL) )
    {
        (void)printf("msgEncoderTestBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 9  -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == msgEncoderGetRemToRetrive(NULL, &var32) )
    {
        (void)printf("msgEncoderTestBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 10 -- FAIL \n");
    }

    /* Function */
    if( MSGE_RES_BADPOINTER == msgEncoderGetRemToRetrive(&ctx, NULL) )
    {
        (void)printf("msgEncoderTestBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("msgEncoderTestBadPointer 11 -- FAIL \n");
    }
}








































