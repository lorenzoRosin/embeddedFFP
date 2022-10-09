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
    e_eCU_Crc_Res lastError;
}s_eCU_crcAdapterCtx;

static bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptEr(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void msgDecoderTestBadPointer(void);
static void msgDecoderTestBadInit(void);
static void msgDecoderTestBadParamEntr(void);
static void msgDecoderTestBadParamStatus(void);
static void msgDecoderTestOutOfMem();
static void msgDecoderTestBadClBck(void);
static void msgDecoderTestMsgEnd(void);
static void msgDecoderTestGeneral(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void msgDecoderTest(void)
{
	(void)printf("\n\nMESSAGE DECODER TEST START \n\n");

    msgDecoderTestBadPointer();
    msgDecoderTestBadInit();
    msgDecoderTestBadParamEntr();
    msgDecoderTestBadParamStatus();
    msgDecoderTestBadClBck();
    msgDecoderTestOutOfMem();
    msgDecoderTestMsgEnd();
    msgDecoderTestGeneral();

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
void msgDecoderTestBadPointer(void)
{
    /* Local variable */
    s_eFSP_MsgDCtx ctx;
    uint8_t  memArea[5u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderInitCtx(NULL, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderInitCtx(&ctx, NULL, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderInitCtx(&ctx, memArea, sizeof(memArea), NULL, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderInitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 4  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderStartNewMsg(NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 5  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderGetDecodedData(NULL, &dataP, &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 6  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderGetDecodedData(&ctx, NULL, &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 7  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderGetDecodedData(&ctx, &dataP, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 8  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderGetDecodedLen(NULL, &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 9  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderGetDecodedLen(&ctx, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 10 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderIsAFullMsgUnstuff(NULL, &isMsgDec) )
    {
        (void)printf("msgDecoderTestBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderIsAFullMsgUnstuff(&ctx, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderGetMostEffDatLen(NULL, &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderGetMostEffDatLen(&ctx, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderInsEncChunk(NULL, memArea, sizeof(memArea), &var32, &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderInsEncChunk(&ctx, NULL, sizeof(memArea), &var32, &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderInsEncChunk(&ctx, memArea, sizeof(memArea), NULL, &var32) )
    {
        (void)printf("msgDecoderTestBadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderInsEncChunk(&ctx, memArea, sizeof(memArea), &var32, NULL) )
    {
        (void)printf("msgDecoderTestBadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadPointer 18 -- FAIL \n");
    }
}

void msgDecoderTestBadInit(void)
{
    /* Local variable */
    s_eFSP_MsgDCtx ctx;
    uint8_t  memArea[5u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_NOINITLIB == msgDecoderStartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 1  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderGetDecodedData(&ctx, &dataP, &var32) )
    {
        (void)printf("msgDecoderTestBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderGetDecodedLen(&ctx, &var32) )
    {
        (void)printf("msgDecoderTestBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 3  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderIsAFullMsgUnstuff(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 4  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderGetMostEffDatLen(&ctx, &var32) )
    {
        (void)printf("msgDecoderTestBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 5  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPOINTER == msgDecoderInsEncChunk(&ctx, memArea, sizeof(memArea), &var32, &var32) )
    {
        (void)printf("msgDecoderTestBadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 6  -- FAIL \n");
    }
}

void msgDecoderTestBadParamEntr(void)
{


}

void msgDecoderTestBadParamStatus(void)
{


}

void msgDecoderTestOutOfMem(void)
{


}

void msgDecoderTestBadClBck(void)
{


}

void msgDecoderTestMsgEnd(void)
{


}

void msgDecoderTestGeneral(void)
{


}

