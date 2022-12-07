/**
 * @file       eFSPMsgTransmitterTest.c
 *
 * @brief      Message trasmitter test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgTransmitterTest.h"
#include "eCUCrc.h"

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
    /* Suppressed for code clarity in test execution*/
#endif

#include <stdio.h>

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
#endif

/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
typedef struct
{
    e_eCU_CRC_Res lastError;
}s_eCU_crcAdapterCtx;

typedef struct
{
    bool sendIsError;
}s_eCU_msgSendAdapterCtx;

typedef struct
{
    bool sendIsError;
}s_eCU_timerAdapterCtx;

static bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptEr(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);
static bool_t sendMsg( void* cntx, const uint8_t dataToSend[], const uint32_t dataToSendLen, uint32_t* const dataSended,
                       const uint32_t timeToSend );
static bool_t sendMsgErr( void* cntx, const uint8_t dataToSend[], const uint32_t dataToSendLen, uint32_t* const dataSended,
                          const uint32_t timeToSend );
static bool_t timStart ( void* cntx, const uint32_t timeoutVal );
static bool_t timGetRemaining ( void* cntx, uint32_t* const remainings );
/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void msgTransmitterTestBadPointer(void);
static void msgTransmitterTestBadInit(void);
static void msgTransmitterTestBadIniMsg(void);
static void msgTransmitterTestBadParamEntr(void);
static void msgTransmitterTestCorruptContext(void);
static void msgTransmitterTestBadClBck(void);
static void msgTransmitterTestMsgEnd(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void msgTransmitterTest(void)
{
	(void)printf("\n\nMESSAGE TRANSMITTER TEST START \n\n");

    msgTransmitterTestBadPointer();
    msgTransmitterTestBadInit();
    msgTransmitterTestBadIniMsg();
    msgTransmitterTestBadParamEntr();
    msgTransmitterTestCorruptContext();
    msgTransmitterTestBadClBck();
    msgTransmitterTestMsgEnd();

    (void)printf("\n\nMESSAGE TRANSMITTER TEST END \n\n");
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

        ctxCur->lastError = CRC_32Seed(s, (const uint8_t*)d, dLen, c32Val);
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

bool_t sendMsg( void* cntx, const uint8_t dataToSend[], const uint32_t dataToSendLen, uint32_t* const dataSended,
                const uint32_t timeToSend )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur;

    result = true;


    return result;
}


bool_t sendMsgErr( void* cntx, const uint8_t dataToSend[], const uint32_t dataToSendLen, uint32_t* const dataSended,
                const uint32_t timeToSend )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur;

    result = false;


    return result;
}

bool_t timStart ( void* cntx, const uint32_t timeoutVal )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur;

    result = true;

    return result;
}

bool_t timGetRemaining ( void* cntx, uint32_t* const remainings )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur;

    result = true;


    return result;
}

/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void msgTransmitterTestBadPointer(void)
{
    /* Local variable */
    s_eFSP_MSGTX_Ctx ctx;
    s_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_InitCtx(NULL, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_InitCtx(&ctx, NULL) )
    {
        (void)printf("msgTransmitterTestBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 2  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = NULL;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = sendBuff;
    initData.i_sendBuffAreaSize = sizeof(sendBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCtx = &ctxAdapterCrc;
    initData.i_cbTxP = &sendMsg;
    initData.i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTimer.timerCtx = &ctxAdapterTim;
    initData.i_txTimer.tim_start = &timStart;
    initData.i_txTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = NULL;
    initData.i_sendBuffAreaSize = sizeof(sendBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCtx = &ctxAdapterCrc;
    initData.i_cbTxP = &sendMsg;
    initData.i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTimer.timerCtx = &ctxAdapterTim;
    initData.i_txTimer.tim_start = &timStart;
    initData.i_txTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 4  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = sendBuff;
    initData.i_sendBuffAreaSize = sizeof(sendBuff);
    initData.i_cbCrcP = NULL;
    initData.i_cbCrcCtx = &ctxAdapterCrc;
    initData.i_cbTxP = &sendMsg;
    initData.i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTimer.timerCtx = &ctxAdapterTim;
    initData.i_txTimer.tim_start = &timStart;
    initData.i_txTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 5  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = sendBuff;
    initData.i_sendBuffAreaSize = sizeof(sendBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCtx = NULL;
    initData.i_cbTxP = &sendMsg;
    initData.i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTimer.timerCtx = &ctxAdapterTim;
    initData.i_txTimer.tim_start = &timStart;
    initData.i_txTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 6  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = sendBuff;
    initData.i_sendBuffAreaSize = sizeof(sendBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCtx = &ctxAdapterCrc;
    initData.i_cbTxP = NULL;
    initData.i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTimer.timerCtx = &ctxAdapterTim;
    initData.i_txTimer.tim_start = &timStart;
    initData.i_txTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 7  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = sendBuff;
    initData.i_sendBuffAreaSize = sizeof(sendBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCtx = &ctxAdapterCrc;
    initData.i_cbTxP = &sendMsg;
    initData.i_cbTxCtx = NULL;
    initData.i_txTimer.timerCtx = &ctxAdapterTim;
    initData.i_txTimer.tim_start = &timStart;
    initData.i_txTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 8  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = sendBuff;
    initData.i_sendBuffAreaSize = sizeof(sendBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCtx = &ctxAdapterCrc;
    initData.i_cbTxP = &sendMsg;
    initData.i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTimer.timerCtx = NULL;
    initData.i_txTimer.tim_start = &timStart;
    initData.i_txTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 9  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = sendBuff;
    initData.i_sendBuffAreaSize = sizeof(sendBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCtx = &ctxAdapterCrc;
    initData.i_cbTxP = &sendMsg;
    initData.i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTimer.timerCtx = &ctxAdapterTim;
    initData.i_txTimer.tim_start = NULL;
    initData.i_txTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 10 -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = sendBuff;
    initData.i_sendBuffAreaSize = sizeof(sendBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCtx = &ctxAdapterCrc;
    initData.i_cbTxP = &sendMsg;
    initData.i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTimer.timerCtx = &ctxAdapterTim;
    initData.i_txTimer.tim_start = &timStart;
    initData.i_txTimer.tim_getRemaining = NULL;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_StartNewMessage(NULL, 10u) )
    {
        (void)printf("msgTransmitterTestBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_GetPayloadLocation(NULL, &dataP, &dataL) )
    {
        (void)printf("msgTransmitterTestBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_GetPayloadLocation(&ctx, NULL, &dataL) )
    {
        (void)printf("msgTransmitterTestBadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_GetPayloadLocation(&ctx, &dataP, NULL) )
    {
        (void)printf("msgTransmitterTestBadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_RestartCurrentMessage(NULL) )
    {
        (void)printf("msgTransmitterTestBadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_SendChunk(NULL) )
    {
        (void)printf("msgTransmitterTestBadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 17 -- FAIL \n");
    }
}

void msgTransmitterTestBadInit(void)
{
    /* Local variable */
    s_eFSP_MSGTX_Ctx ctx;
    s_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Function */
    ctx.msgEncoderCtnx.byteStufferCtnx.isInit = false;
    ctx.msgEncoderCtnx.cbCrcCtx = &ctxAdapterCrc;
    ctx.msgEncoderCtnx.cbCrcPtr = cbCrcPTest;
    ctx.sendBuff = sendBuff;
    ctx.sendBuffSize = sizeof(sendBuff);
    ctx.sendBuffCntr = 0u;
    ctx.sendBuffFill = 0u;
    ctx.cbTxP = &sendMsg;
    ctx.cbTxCtx = &ctxAdapterSend;
    ctx.txTimer.timerCtx = &ctxAdapterTim;
    ctx.txTimer.tim_start = &timStart;
    ctx.txTimer.tim_getRemaining = &timGetRemaining;
    ctx.frameTimeoutMs = 1000u;
    ctx.timePerSendMs = 100u;

    /* Function */
    if( MSGTX_RES_NOINITLIB == MSGTX_StartNewMessage(&ctx, 10u) )
    {
        (void)printf("msgTransmitterTestBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadInit 1  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_NOINITLIB == MSGTX_GetPayloadLocation(&ctx, &dataP, &dataL) )
    {
        (void)printf("msgTransmitterTestBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadInit 2  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_NOINITLIB == MSGTX_RestartCurrentMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadInit 3  -- FAIL \n");
    }
}

void msgTransmitterTestBadIniMsg(void)
{
    /* Local variable */
    s_eFSP_MSGTX_Ctx ctx;
    s_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = sendBuff;
    initData.i_sendBuffAreaSize = sizeof(sendBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCtx = &ctxAdapterCrc;
    initData.i_cbTxP = &sendMsg;
    initData.i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTimer.timerCtx = &ctxAdapterTim;
    initData.i_txTimer.tim_start = &timStart;
    initData.i_txTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadIniMsg 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadIniMsg 1  -- FAIL \n");
    }

    if( MSGTX_RES_NOINITMESSAGE == MSGTX_RestartCurrentMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestBadIniMsg 2  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadIniMsg 2  -- FAIL \n");
    }

    if( MSGTX_RES_NOINITMESSAGE == MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestBadIniMsg 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadIniMsg 3  -- FAIL \n");
    }
}

void msgTransmitterTestBadParamEntr(void)
{

}

void msgTransmitterTestCorruptContext(void)
{

}

void msgTransmitterTestBadClBck(void)
{

}

void msgTransmitterTestMsgEnd(void)
{

}

