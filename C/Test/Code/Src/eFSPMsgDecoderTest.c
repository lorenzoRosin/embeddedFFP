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
static void msgDecoderTestBadClBck(void);
static void msgDecoderTestMsgEnd(void);
static void msgDecoderTestOutOfMem();
static void msgDecoderTestGeneral(void);
static void msgDecoderTestCorernerMulti(void);


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
    msgDecoderTestCorernerMulti();

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
    uint8_t  memArea[10u];
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
    uint8_t  memArea[10u];
    uint32_t var32;
    uint8_t* dataP;
    bool_t isMsgDec;
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;

    /* Function */
    ctx.byteUStufferCtnx.isInit = false;
    ctx.cbCrcCtx = &ctxAdapterCrc;
    ctx.cbCrcPtr = cbCrcPTest;

    if( MSGD_RES_NOINITLIB == msgDecoderStartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 1  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == msgDecoderGetDecodedData(&ctx, &dataP, &var32) )
    {
        (void)printf("msgDecoderTestBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == msgDecoderGetDecodedLen(&ctx, &var32) )
    {
        (void)printf("msgDecoderTestBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 3  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == msgDecoderIsAFullMsgUnstuff(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 4  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == msgDecoderGetMostEffDatLen(&ctx, &var32) )
    {
        (void)printf("msgDecoderTestBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadInit 5  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_NOINITLIB == msgDecoderInsEncChunk(&ctx, memArea, sizeof(memArea), &var32, &var32) )
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
    /* Local variable */
    s_eFSP_MsgDCtx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_BADPARAM == msgDecoderInitCtx(&ctx, memArea, 8u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamEntr 1  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamEntr 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_BADPARAM == msgDecoderInsEncChunk(&ctx, memArea, 0u, &var32, &var32) )
    {
        (void)printf("msgDecoderTestBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamEntr 3  -- FAIL \n");
    }
}

void msgDecoderTestBadParamStatus(void)
{
    /* Local variable */
    s_eFSP_MsgDCtx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;
    uint8_t* dataP;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadParamStatus 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 1  -- FAIL \n");
    }

    ctx.cbCrcPtr = NULL;
    if( MSGD_RES_CORRUPTCTX == msgDecoderStartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestBadParamStatus 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 2  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadParamStatus 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 3  -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == msgDecoderStartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestBadParamStatus 4  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 4  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadParamStatus 5  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 5  -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == msgDecoderStartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestBadParamStatus 6  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 6  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadParamStatus 7  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 7  -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == msgDecoderGetDecodedData(&ctx, &dataP, &var32) )
    {
        (void)printf("msgDecoderTestBadParamStatus 8  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 8  -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadParamStatus 9  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 9  -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == msgDecoderGetDecodedLen(&ctx,&var32) )
    {
        (void)printf("msgDecoderTestBadParamStatus 10 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 10 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadParamStatus 11 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 11 -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == msgDecoderIsAFullMsgUnstuff(&ctx, &isMsgDec) )
    {
        (void)printf("msgDecoderTestBadParamStatus 12 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 12 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadParamStatus 13 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 13 -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == msgDecoderGetMostEffDatLen(&ctx, &var32) )
    {
        (void)printf("msgDecoderTestBadParamStatus 14 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 14 -- FAIL \n");
    }

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, 9u, cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadParamStatus 15 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 15 -- FAIL \n");
    }

    ctx.cbCrcCtx = NULL;
    if( MSGD_RES_CORRUPTCTX == msgDecoderInsEncChunk(&ctx, memArea, 10u, &var32, &var32) )
    {
        (void)printf("msgDecoderTestBadParamStatus 16 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadParamStatus 16 -- FAIL \n");
    }
}


void msgDecoderTestBadClBck(void)
{
    /* Local variable */
    s_eFSP_MsgDCtx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdaptEr;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestBadClBck 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadClBck 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderStartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestBadClBck 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestBadClBck 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0x00u, 0x00u, 0x00u, 0x00u, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( MSGD_RES_CRCCLBKERROR == msgDecoderInsEncChunk(&ctx, testData, sizeof(testData), &var32, &var32) )
    {
        if( CRC_RES_BADPOINTER == ctxAdapterCrc.lastError )
        {
            (void)printf("msgDecoderTestBadClBck 3  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestBadClBck 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestBadClBck 3  -- FAIL \n");
    }
}

void msgDecoderTestMsgEnd(void)
{
    /* Local variable */
    s_eFSP_MsgDCtx ctx;
    uint8_t  memArea[10u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestMsgEnd 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestMsgEnd 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderStartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestMsgEnd 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestMsgEnd 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xA6u, 0xC1u, 0xDCu, 0x0Au, 0x01, 0x00u, 0x00u, 0x00u, 0x01, ECU_EOF};

    if( MSGD_RES_FRAMEENDED == msgDecoderInsEncChunk(&ctx, testData, sizeof(testData), &var32, &var32) )
    {
        (void)printf("msgDecoderTestMsgEnd 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestMsgEnd 3  -- FAIL \n");
    }
}

void msgDecoderTestOutOfMem(void)
{
    /* Local variable */
    s_eFSP_MsgDCtx ctx;
    uint8_t  memArea[9u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t var32;

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestOutOfMem 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestOutOfMem 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderStartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestOutOfMem 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestOutOfMem 2  -- FAIL \n");
    }

    /* Decode */
    uint8_t testData[] = {ECU_SOF, 0xE9u, 0x7Au, 0xF2u, 0xDAu, 0x02, 0x00u, 0x00u, 0x00u, 0x01, 0x01, ECU_EOF};

    if( MSGD_RES_OUTOFMEM == msgDecoderInsEncChunk(&ctx, testData, sizeof(testData), &var32, &var32) )
    {
        (void)printf("msgDecoderTestOutOfMem 3  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestOutOfMem 3  -- FAIL \n");
    }
}

void msgDecoderTestGeneral(void)
{
    /* Local variable */
    s_eFSP_MsgDCtx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t errorFound;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestGeneral 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderStartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestGeneral 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestGeneral 3  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestGeneral 4  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderIsAFullMsgUnstuff(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestGeneral 5  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestGeneral 6  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x80u, 0xE4u, 0xB1u, 0x84u, 0x02, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), ECU_EOF};

    if( MSGD_RES_FRAMEENDED == msgDecoderInsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( ( 14u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestGeneral 7  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("msgDecoderTestGeneral 8  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderIsAFullMsgUnstuff(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("msgDecoderTestGeneral 9  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("msgDecoderTestGeneral 10 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x02u == payLoadLen )
        {
            (void)printf("msgDecoderTestGeneral 11 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 11 -- FAIL \n");
    }

    if( ( ECU_SOF == payLoadLoc[0u] ) && ( ECU_EOF == payLoadLoc[1u] ) )
    {
        (void)printf("msgDecoderTestGeneral 12 -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 12 -- FAIL \n");
    }

    if( MSGD_RES_FRAMEENDED == msgDecoderInsEncChunk(&ctx, testData, sizeof(testData), &consumed, &errorFound) )
    {
        if( 0u == consumed )
        {
            (void)printf("msgDecoderTestGeneral 13 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestGeneral 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestGeneral 13 -- FAIL \n");
    }
}

void msgDecoderTestCorernerMulti(void)
{
    /* Local variable */
    s_eFSP_MsgDCtx ctx;
    uint8_t  memArea[40u];
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    uint32_t consumed;
    uint32_t errorFound;
    uint32_t mostEfficient;
    uint32_t payLoadLen;
    uint8_t* payLoadLoc;
    bool_t isMsgDec;

    /* Function */
    if( MSGD_RES_OK == msgDecoderInitCtx(&ctx, memArea, sizeof(memArea), cbCrcPTest, &ctxAdapterCrc) )
    {
        (void)printf("msgDecoderTestCorernerMulti 1  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 1  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderStartNewMsg(&ctx) )
    {
        (void)printf("msgDecoderTestCorernerMulti 2  -- OK \n");
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 2  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 3  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 3  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 4  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 4  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderIsAFullMsgUnstuff(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestCorernerMulti 5  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 5  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 8u == mostEfficient )
        {
            (void)printf("msgDecoderTestCorernerMulti 6  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 6  -- FAIL \n");
    }

    /* Remove */
    uint8_t testData[] = {ECU_SOF, 0x73u, 0x9Fu, 0x52u, 0xD9u, 0x07, 0x00u, 0x00u, 0x00u, ECU_ESC, (uint8_t)(~ECU_SOF),
                          ECU_ESC, (uint8_t)(~ECU_EOF), 0x01, 0x02, 0x03, 0x04, 0x05, ECU_EOF};

    if( MSGD_RES_OK == msgDecoderInsEncChunk(&ctx, testData, 4u, &consumed, &errorFound) )
    {
        if( ( 4u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestCorernerMulti 7  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 7  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 5u == mostEfficient )
        {
            (void)printf("msgDecoderTestCorernerMulti 8  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 8  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderIsAFullMsgUnstuff(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestCorernerMulti 9  -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 9  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 9  -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 10 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 10 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 11 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 11 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 11 -- FAIL \n");
    }

    consumed = 0u;
    errorFound = 0u;
    if( MSGD_RES_OK == msgDecoderInsEncChunk(&ctx, &testData[4u], 4u, &consumed, &errorFound) )
    {
        if( ( 4u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestCorernerMulti 12 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 12 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 12 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 1u == mostEfficient )
        {
            (void)printf("msgDecoderTestCorernerMulti 13 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 13 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 13 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderIsAFullMsgUnstuff(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestCorernerMulti 14 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 14 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 14 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 15 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 15 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x00u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 16 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 16 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 16 -- FAIL \n");
    }

    consumed = 0u;
    errorFound = 0u;
    if( MSGD_RES_OK == msgDecoderInsEncChunk(&ctx, &testData[8u], 4u, &consumed, &errorFound) )
    {
        if( ( 4u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestCorernerMulti 17 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 17 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 7u == mostEfficient )
        {
            (void)printf("msgDecoderTestCorernerMulti 18 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 18 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 18 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderIsAFullMsgUnstuff(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestCorernerMulti 19 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 19 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 19 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 20 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 20 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x01u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 21 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 21 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 21 -- FAIL \n");
    }

    consumed = 0u;
    errorFound = 0u;
    if( MSGD_RES_OK == msgDecoderInsEncChunk(&ctx, &testData[12u], 4u, &consumed, &errorFound) )
    {
        if( ( 4u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestCorernerMulti 22 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 22 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 3u == mostEfficient )
        {
            (void)printf("msgDecoderTestCorernerMulti 23 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 23 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderIsAFullMsgUnstuff(&ctx, &isMsgDec) )
    {
        if( false == isMsgDec )
        {
            (void)printf("msgDecoderTestCorernerMulti 24 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 24 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x05u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 25 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 25 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x05u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 26 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 26 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 26 -- FAIL \n");
    }

    consumed = 0u;
    errorFound = 0u;
    if( MSGD_RES_FRAMEENDED == msgDecoderInsEncChunk(&ctx, &testData[16u], 4u, &consumed, &errorFound) )
    {
        if( ( 3u == consumed ) && ( 0u == errorFound) )
        {
            (void)printf("msgDecoderTestCorernerMulti 27 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 27 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetMostEffDatLen(&ctx, &mostEfficient) )
    {
        if( 0u == mostEfficient )
        {
            (void)printf("msgDecoderTestCorernerMulti 28 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 28 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 28 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderIsAFullMsgUnstuff(&ctx, &isMsgDec) )
    {
        if( true == isMsgDec )
        {
            (void)printf("msgDecoderTestCorernerMulti 29 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 29 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedLen(&ctx, &payLoadLen) )
    {
        if( 0x07u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 30 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 30 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 30 -- FAIL \n");
    }

    if( MSGD_RES_OK == msgDecoderGetDecodedData(&ctx, &payLoadLoc, &payLoadLen) )
    {
        if( 0x07u == payLoadLen )
        {
            (void)printf("msgDecoderTestCorernerMulti 31 -- OK \n");
        }
        else
        {
            (void)printf("msgDecoderTestCorernerMulti 31 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgDecoderTestCorernerMulti 31 -- FAIL \n");
    }

}
