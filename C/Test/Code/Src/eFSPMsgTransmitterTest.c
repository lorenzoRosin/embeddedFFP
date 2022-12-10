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
static bool_t timStartErr ( void* cntx, const uint32_t timeoutVal );
static bool_t timGetRemainingErr ( void* cntx, uint32_t* const remainings );



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void msgTransmitterTestBadPointer(void);
static void msgTransmitterTestBadInit(void);
static void msgTransmitterTestBadIniMsg(void);
static void msgTransmitterTestBadParamEntr(void);
static void msgTransmitterTestCorruptContext(void);
static void msgTransmitterTestBadClBckCrc(void);
static void msgTransmitterTestBadClBckSend(void);
static void msgTransmitterTestBadClBckTim(void);



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
    msgTransmitterTestBadClBckCrc();
    msgTransmitterTestBadClBckSend();
    msgTransmitterTestBadClBckTim();

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
    s_eCU_msgSendAdapterCtx* ctxCur = (s_eCU_msgSendAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    result = true;


    return result;
}


bool_t sendMsgErr( void* cntx, const uint8_t dataToSend[], const uint32_t dataToSendLen, uint32_t* const dataSended,
                const uint32_t timeToSend )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur = (s_eCU_msgSendAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    result = false;


    return result;
}

static uint32_t m_tim_remainingTime;

bool_t timStart ( void* cntx, const uint32_t timeoutVal )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur = (s_eCU_timerAdapterCtx*)cntx;
    ctxCur->sendIsError = true;

    m_tim_remainingTime = timeoutVal;
    result = true;

    return result;
}

bool_t timGetRemaining ( void* cntx, uint32_t* const remainings )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur = (s_eCU_timerAdapterCtx*)cntx;
    ctxCur->sendIsError = true;

    if( m_tim_remainingTime > 0u )
    {
        m_tim_remainingTime--;
    }

    *remainings = m_tim_remainingTime;
    result = true;

    return result;
}


bool_t timStartErr ( void* cntx, const uint32_t timeoutVal )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur = (s_eCU_timerAdapterCtx*)cntx;
    ctxCur->sendIsError = true;

    m_tim_remainingTime = timeoutVal;
    result = false;

    return result;
}

bool_t timGetRemainingErr ( void* cntx, uint32_t* const remainings )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur = (s_eCU_timerAdapterCtx*)cntx;
    ctxCur->sendIsError = true;

    if( m_tim_remainingTime > 0u )
    {
        m_tim_remainingTime--;
    }

    *remainings = m_tim_remainingTime;
    result = false;

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
    bool_t isInit;

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
        (void)printf("msgTransmitterTestBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 12 -- FAIL \n");
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
        (void)printf("msgTransmitterTestBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 13 -- FAIL \n");
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
        (void)printf("msgTransmitterTestBadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_IsInit(NULL, &isInit) )
    {
        (void)printf("msgTransmitterTestBadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_IsInit(&ctx, NULL) )
    {
        (void)printf("msgTransmitterTestBadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_StartNewMessage(NULL, 10u) )
    {
        (void)printf("msgTransmitterTestBadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_GetPayloadLocation(NULL, &dataP, &dataL) )
    {
        (void)printf("msgTransmitterTestBadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 18 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_GetPayloadLocation(&ctx, NULL, &dataL) )
    {
        (void)printf("msgTransmitterTestBadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 19 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_GetPayloadLocation(&ctx, &dataP, NULL) )
    {
        (void)printf("msgTransmitterTestBadPointer 20 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 20 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_RestartCurrentMessage(NULL) )
    {
        (void)printf("msgTransmitterTestBadPointer 21 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 21 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == MSGTX_SendChunk(NULL) )
    {
        (void)printf("msgTransmitterTestBadPointer 22 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 22 -- FAIL \n");
    }
}

void msgTransmitterTestBadInit(void)
{
    /* Local variable */
    s_eFSP_MSGTX_Ctx ctx;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;
    bool_t isInit;

    /* Function */
    memset(&ctx, 0u, sizeof(s_eFSP_MSGTX_Ctx));
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
    if( MSGTX_RES_OK == MSGTX_IsInit(&ctx, &isInit) )
    {
        if( false == isInit )
        {
            (void)printf("msgTransmitterTestBadInit 1  -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestBadInit 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestBadInit 1  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_NOINITLIB == MSGTX_StartNewMessage(&ctx, 10u) )
    {
        (void)printf("msgTransmitterTestBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadInit 2  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_NOINITLIB == MSGTX_GetPayloadLocation(&ctx, &dataP, &dataL) )
    {
        (void)printf("msgTransmitterTestBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadInit 3  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_NOINITLIB == MSGTX_RestartCurrentMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadInit 4  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_NOINITLIB == MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadInit 5  -- FAIL \n");
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
    bool_t isInit;

    /* Clear */
    memset(&ctx, 0u, sizeof(s_eFSP_MSGTX_Ctx));
    memset(&initData, 0u, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
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

    m_tim_remainingTime = 100u;
    if( MSGTX_RES_NOINITMESSAGE == MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestBadIniMsg 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadIniMsg 3  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_IsInit(&ctx, &isInit) )
    {
        if( true == isInit )
        {
            (void)printf("msgTransmitterTestBadIniMsg 4  -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestBadIniMsg 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestBadIniMsg 4  -- FAIL \n");
    }
}

void msgTransmitterTestBadParamEntr(void)
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

    /* Clear */
    memset(&ctx, 0u, sizeof(s_eFSP_MSGTX_Ctx));
    memset(&initData, 0u, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = sendBuff;
    initData.i_sendBuffAreaSize = 0u;
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCtx = &ctxAdapterCrc;
    initData.i_cbTxP = &sendMsg;
    initData.i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTimer.timerCtx = &ctxAdapterTim;
    initData.i_txTimer.tim_start = &timStart;
    initData.i_txTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPARAM == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 1  -- FAIL \n");
    }

    /* Init */
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
    initData.i_frameTimeoutMs = 0u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPARAM == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 2  -- FAIL \n");
    }

    /* Init */
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
    initData.i_timePerSendMs = 0u;
    if( MSGTX_RES_BADPARAM == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 3  -- FAIL \n");
    }

    /* Init */
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
    initData.i_frameTimeoutMs = 100u;
    initData.i_timePerSendMs = 1000u;
    if( MSGTX_RES_BADPARAM == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 4  -- FAIL \n");
    }

    /* Init */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = 0u;
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
    if( MSGTX_RES_BADPARAM == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 5  -- FAIL \n");
    }

    /* Init */
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
        (void)printf("msgTransmitterTestBadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 6  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPARAM == MSGTX_StartNewMessage(&ctx, 0u) )
    {
        (void)printf("msgTransmitterTestBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 2  -- FAIL \n");
    }
}

void msgTransmitterTestCorruptContext(void)
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

    /* Clear */
    memset(&ctx, 0u, sizeof(s_eFSP_MSGTX_Ctx));
    memset(&initData, 0u, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
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
        (void)printf("msgTransmitterTestCorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 1  -- FAIL \n");
    }

    /* Function */
    ctx.sendBuff = NULL;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 2  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 3  -- FAIL \n");
    }

    /* Function */
    ctx.cbTxP = NULL;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 4  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 5  -- FAIL \n");
    }

    /* Function */
    ctx.cbTxCtx = NULL;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 6  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 7  -- FAIL \n");
    }

    /* Function */
    ctx.txTimer.timerCtx = NULL;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 8  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 9  -- FAIL \n");
    }

    /* Function */
    ctx.txTimer.tim_start = NULL;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 10 -- FAIL \n");
    }


    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 11 -- FAIL \n");
    }

    /* Function */
    ctx.txTimer.tim_getRemaining = NULL;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 12 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 13 -- FAIL \n");
    }

    /* Function */
    ctx.sendBuffSize = 0u;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 14 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 15 -- FAIL \n");
    }

    /* Function */
    ctx.sendBuffFill = ctx.sendBuffSize + 1u;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 16 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 17 -- FAIL \n");
    }

    /* Function */
    ctx.sendBuffCntr = ctx.sendBuffFill + 1u;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 18 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 19 -- FAIL \n");
    }

    /* Function */
    ctx.frameTimeoutMs = 0u;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 20 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 21 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 21 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = 0u;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 22 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 22 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 23 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 23 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = ctx.frameTimeoutMs + 1u;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 24 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 24 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 25 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 25 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = 0u;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_GetPayloadLocation(&ctx, &dataP, &dataL) )
    {
        (void)printf("msgTransmitterTestCorruptContext 26 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 26 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 27 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 27 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = 0u;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_RestartCurrentMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestCorruptContext 28 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 28 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 29 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 29 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = 0u;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCorruptContext 30 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 30 -- FAIL \n");
    }

    /* Function */
    ctx.msgEncoderCtnx.cbCrcCtx = NULL;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 31 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 31 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 32 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 32 -- FAIL \n");
    }

    /* Function */
    ctx.msgEncoderCtnx.cbCrcCtx = NULL;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_GetPayloadLocation(&ctx, &dataP, &dataL) )
    {
        (void)printf("msgTransmitterTestCorruptContext 33 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 33 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 34 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 34 -- FAIL \n");
    }

    /* Function */
    ctx.msgEncoderCtnx.cbCrcCtx = NULL;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_RestartCurrentMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestCorruptContext 35 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 35 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 36 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 36 -- FAIL \n");
    }

    /* Function */
    ctx.msgEncoderCtnx.cbCrcCtx = NULL;
    if( MSGTX_RES_CORRUPTCTX == MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCorruptContext 37 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 37 -- FAIL \n");
    }
}

void msgTransmitterTestBadClBckCrc(void)
{
    /* Local variable */
    s_eFSP_MSGTX_Ctx ctx;
    s_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdaptEr;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];

    /* Clear */
    memset(&ctx, 0u, sizeof(s_eFSP_MSGTX_Ctx));
    memset(&initData, 0u, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
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
        (void)printf("msgTransmitterTestBadClBckCrc 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckCrc 1  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_CRCCLBKERROR == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        if( CRC_RES_BADPOINTER == ctxAdapterCrc.lastError )
        {
            (void)printf("msgTransmitterTestBadClBckCrc 2  -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestBadClBckCrc 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckCrc 2  -- FAIL \n");
    }
}

void msgTransmitterTestBadClBckSend(void)
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

    /* Clear */
    memset(&ctx, 0u, sizeof(s_eFSP_MSGTX_Ctx));
    memset(&initData, 0u, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = sendBuff;
    initData.i_sendBuffAreaSize = sizeof(sendBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCtx = &ctxAdapterCrc;
    initData.i_cbTxP = &sendMsgErr;
    initData.i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTimer.timerCtx = &ctxAdapterTim;
    initData.i_txTimer.tim_start = &timStart;
    initData.i_txTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;

    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadClBckSend 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckSend 1  -- FAIL \n");
    }

    if( MSGTX_RES_OK == MSGTX_GetPayloadLocation(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("msgTransmitterTestBadClBckSend 2  -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestBadClBckSend 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckSend 2  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01;
    dataP[1u] = 0x02;
    if( MSGTX_RES_OK == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestBadClBckSend 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckSend 3  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_TXCLBKERROR == MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestBadClBckSend 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckSend 4  -- FAIL \n");
    }
}

void msgTransmitterTestBadClBckTim(void)
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

    /* Clear */
    memset(&ctx, 0u, sizeof(s_eFSP_MSGTX_Ctx));
    memset(&initData, 0u, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_sendBuffArea = sendBuff;
    initData.i_sendBuffAreaSize = sizeof(sendBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCtx = &ctxAdapterCrc;
    initData.i_cbTxP = &sendMsg;
    initData.i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTimer.timerCtx = &ctxAdapterTim;
    initData.i_txTimer.tim_start = &timStartErr;
    initData.i_txTimer.tim_getRemaining = &timGetRemainingErr;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;

    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadClBckTim 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 1  -- FAIL \n");
    }

    if( MSGTX_RES_OK == MSGTX_GetPayloadLocation(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("msgTransmitterTestBadClBckTim 2  -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestBadClBckTim 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 2  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01;
    dataP[1u] = 0x02;
    if( MSGTX_RES_TIMCLBKERROR == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestBadClBckTim 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 3  -- FAIL \n");
    }

     /* Function */
    if( MSGTX_RES_TIMCLBKERROR == MSGTX_RestartCurrentMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestBadClBckTim 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 4  -- FAIL \n");
    }

    /* Clear */
    memset(&ctx, 0u, sizeof(s_eFSP_MSGTX_Ctx));
    memset(&initData, 0u, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
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
    initData.i_txTimer.tim_getRemaining = &timGetRemainingErr;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;

    if( MSGTX_RES_OK == MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadClBckTim 5  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 5  -- FAIL \n");
    }

    if( MSGTX_RES_OK == MSGTX_GetPayloadLocation(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("msgTransmitterTestBadClBckTim 6  -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestBadClBckTim 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 6  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01;
    dataP[1u] = 0x02;
    if( MSGTX_RES_OK == MSGTX_StartNewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestBadClBckTim 7  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 7  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_TIMCLBKERROR == MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestBadClBckTim 8  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 8  -- FAIL \n");
    }

}