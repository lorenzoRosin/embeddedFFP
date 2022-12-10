/**
 * @file       eFSPMsgReceiverTest.h
 *
 * @brief      Message receiver test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgReceiverTest.h"
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
static bool_t receiveMsg( void* cntx, const uint8_t dataToRx[], uint32_t* const dataRecevd, const uint32_t dataRxMax,
                       const uint32_t timeToRx );
static bool_t receiveMsgErr( void* cntx, const uint8_t dataToRx[], uint32_t* const dataRecevd, const uint32_t dataRxMax,
                          const uint32_t timeToRx );
static bool_t timStart ( void* cntx, const uint32_t timeoutVal );
static bool_t timGetRemaining ( void* cntx, uint32_t* const remainings );
static bool_t timStartErr ( void* cntx, const uint32_t timeoutVal );
static bool_t timGetRemainingErr ( void* cntx, uint32_t* const remainings );



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void msgReceiverTestBadPointer(void);
static void msgReceiverTestBadInit(void);
static void msgReceiverTestBadIniMsg(void);
static void msgReceiverTestBadParamEntr(void);
static void msgReceiverTestCorruptContext(void);
static void msgReceiverTestBadClBckCrc(void);
static void msgReceiverTestBadClBckReceive(void);
static void msgReceiverTestBadClBckTim(void);




/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void msgReceiverTest(void)
{
	(void)printf("\n\nMESSAGE RECEIVER TEST START \n\n");

    msgReceiverTestBadPointer();
    msgReceiverTestBadInit();
    msgReceiverTestBadIniMsg();
    msgReceiverTestBadParamEntr();
    msgReceiverTestCorruptContext();
    msgReceiverTestBadClBckCrc();
    msgReceiverTestBadClBckReceive();
    msgReceiverTestBadClBckTim();

    (void)printf("\n\nMESSAGE RECEIVER TEST END \n\n");
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

bool_t receiveMsg( void* cntx, const uint8_t dataToRx[], uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur = (s_eCU_msgSendAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    result = true;


    return result;
}


bool_t receiveMsgErr( void* cntx, const uint8_t dataToRx[], uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
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
void msgReceiverTestBadPointer(void)
{
    /* Local variable */
    s_eFSP_MSGRX_Ctx ctx;
    s_eFSP_MSGRX_InitData initData;
    cb_crc32_msgd cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterRx;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;
    bool_t isInit;

    /* Function */
    if( MSGRX_RES_BADPOINTER == MSGRX_InitCtx(NULL, &initData) )
    {
        (void)printf("msgReceiverTestBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_BADPOINTER == MSGRX_InitCtx(&ctx, NULL) )
    {
        (void)printf("msgReceiverTestBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 2  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = NULL;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsg;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_BADPOINTER == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = NULL;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsg;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_BADPOINTER == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 4  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = NULL;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsg;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_BADPOINTER == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = NULL;
    initData.i_cbRxP = &receiveMsg;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_BADPOINTER == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = NULL;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_BADPOINTER == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsg;
    initData.i_cbRxCtx = NULL;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_BADPOINTER == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsg;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = NULL;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_BADPOINTER == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsg;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = NULL;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_BADPOINTER == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsg;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = NULL;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_BADPOINTER == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 3  -- FAIL \n");
    }

}

void msgReceiverTestBadInit(void)
{

}

void msgReceiverTestBadIniMsg(void)
{

}

void msgReceiverTestBadParamEntr(void)
{

}

void msgReceiverTestCorruptContext(void)
{

}

void msgReceiverTestBadClBckCrc(void)
{

}

void msgReceiverTestBadClBckReceive(void)
{

}

void msgReceiverTestBadClBckTim(void)
{

}

