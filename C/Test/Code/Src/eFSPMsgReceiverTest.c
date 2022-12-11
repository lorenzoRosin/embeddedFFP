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
static bool_t receiveMsg( void* cntx, uint8_t dataToRx[], uint32_t* const dataRecevd, const uint32_t dataRxMax,
                       const uint32_t timeToRx );
static bool_t receiveMsgCrrupt( void* cntx, uint8_t dataToRx[], uint32_t* const dataRecevd, const uint32_t dataRxMax,
                       const uint32_t timeToRx );
static bool_t receiveMsgJump( void* cntx, uint8_t dataToRx[], uint32_t* const dataRecevd, const uint32_t dataRxMax,
                       const uint32_t timeToRx );
static bool_t receiveMsgErr( void* cntx, uint8_t dataToRx[], uint32_t* const dataRecevd, const uint32_t dataRxMax,
                          const uint32_t timeToRx );
static bool_t timStart ( void* cntx, const uint32_t timeoutVal );
static bool_t timGetRemaining ( void* cntx, uint32_t* const remainings );
static bool_t timGetRemainingCorr ( void* cntx, uint32_t* const remainings );
static bool_t timStartErr ( void* cntx, const uint32_t timeoutVal );
static bool_t timGetRemainingErr ( void* cntx, uint32_t* const remainings );
static bool_t timGetRemainingErrCntrl( void* cntx, uint32_t* const remainings );



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void msgReceiverTestBadPointer(void);
static void msgReceiverTestBadInit(void);
static void msgReceiverTestBadParamEntr(void);
static void msgReceiverTestCorruptContext(void);
static void msgReceiverTestBadClBckCrc(void);
static void msgReceiverTestBadClBckReceive(void);
static void msgReceiverTestBadClBckTim(void);
static void msgReceiverTestBadFrame(void);
static void msgReceiverTestCornerCase(void);
static void msgReceiverTestCornerCase2(void);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void msgReceiverTest(void)
{
	(void)printf("\n\nMESSAGE RECEIVER TEST START \n\n");

    msgReceiverTestBadPointer();
    msgReceiverTestBadInit();
    msgReceiverTestBadParamEntr();
    msgReceiverTestCorruptContext();
    msgReceiverTestBadClBckCrc();
    msgReceiverTestBadClBckReceive();
    msgReceiverTestBadClBckTim();
    msgReceiverTestBadFrame();
    msgReceiverTestCornerCase();
    msgReceiverTestCornerCase2();

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

static uint8_t  m_rxPayload[100u];
static uint32_t m_payloadSize;
static uint32_t m_payloadCounter;
static uint32_t m_read_jump;

bool_t receiveMsg( void* cntx, uint8_t dataToRx[], uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur = (s_eCU_msgSendAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    result = true;


    if( m_payloadCounter < m_payloadSize )
    {
        if( ( m_payloadSize - m_payloadCounter ) >= dataRxMax )
        {
            memcpy(dataToRx, &m_rxPayload[m_payloadCounter], dataRxMax);
            m_payloadCounter += dataRxMax;
            *dataRecevd = dataRxMax;
        }
        else
        {
            memcpy(dataToRx, &m_rxPayload[m_payloadCounter], ( m_payloadSize - m_payloadCounter ) );
            m_payloadCounter += ( m_payloadSize - m_payloadCounter ) ;
            *dataRecevd = ( m_payloadSize - m_payloadCounter );
        }
    }
    else
    {
        *dataRecevd = 0u;
    }


    return result;
}

bool_t receiveMsgCrrupt (void* cntx, uint8_t dataToRx[], uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur = (s_eCU_msgSendAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    result = true;


    if( m_payloadCounter < m_payloadSize )
    {
        if( ( m_payloadSize - m_payloadCounter ) >= dataRxMax )
        {
            memcpy(dataToRx, &m_rxPayload[m_payloadCounter], dataRxMax);
            m_payloadCounter += dataRxMax;
            *dataRecevd = dataRxMax +1u;
        }
        else
        {
            memcpy(dataToRx, &m_rxPayload[m_payloadCounter], ( m_payloadSize - m_payloadCounter ) );
            m_payloadCounter += ( m_payloadSize - m_payloadCounter ) ;
            *dataRecevd = ( m_payloadSize - m_payloadCounter );
        }
    }
    else
    {
        *dataRecevd = 0u;
    }


    return result;
}

bool_t receiveMsgJump( void* cntx, uint8_t dataToRx[], uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur = (s_eCU_msgSendAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    result = true;


    if( 0 == m_read_jump )
    {
        if( m_payloadCounter < m_payloadSize )
        {
            if( ( m_payloadSize - m_payloadCounter ) >= dataRxMax )
            {
                memcpy(dataToRx, &m_rxPayload[m_payloadCounter], dataRxMax);
                m_payloadCounter += dataRxMax;
                *dataRecevd = dataRxMax;
            }
            else
            {
                memcpy(dataToRx, &m_rxPayload[m_payloadCounter], ( m_payloadSize - m_payloadCounter ) );
                m_payloadCounter += ( m_payloadSize - m_payloadCounter ) ;
                *dataRecevd = ( m_payloadSize - m_payloadCounter );
            }
        }
        else
        {
            *dataRecevd = 0u;
        }
        m_read_jump = 1u;
    }
    else
    {
        m_read_jump = 0u;
        *dataRecevd = 0u;
    }


    return result;
}

bool_t receiveMsgErr( void* cntx, uint8_t dataToRx[], uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur = (s_eCU_msgSendAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    result = false;


    return result;
}

static uint32_t m_tim_remainingTime;
static uint32_t m_tim_timCnt;
static uint32_t m_tim_timCntInc;

bool_t timStart ( void* cntx, const uint32_t timeoutVal )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur = (s_eCU_timerAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    m_tim_timCnt = 0u;
    m_tim_timCntInc = 0u;
    m_tim_remainingTime = timeoutVal;
    result = true;

    return result;
}

bool_t timGetRemaining ( void* cntx, uint32_t* const remainings )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur = (s_eCU_timerAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    m_tim_timCnt++;
    if( m_tim_remainingTime > 0u )
    {
        m_tim_remainingTime--;
    }

    *remainings = m_tim_remainingTime;
    result = true;

    return result;
}

bool_t timGetRemainingCorr ( void* cntx, uint32_t* const remainings )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur = (s_eCU_timerAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    m_tim_timCnt++;
    *remainings = m_tim_remainingTime + m_tim_timCntInc;
    m_tim_timCntInc++;
    result = true;

    return result;
}

bool_t timStartErr ( void* cntx, const uint32_t timeoutVal )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur = (s_eCU_timerAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    m_tim_timCnt = 0u;

    m_tim_remainingTime = timeoutVal;
    result = false;

    return result;
}

bool_t timGetRemainingErr ( void* cntx, uint32_t* const remainings )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur = (s_eCU_timerAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    m_tim_timCnt++;
    if( m_tim_remainingTime > 0u )
    {
        m_tim_remainingTime--;
    }

    *remainings = m_tim_remainingTime;
    result = false;

    return result;
}

bool_t timGetRemainingErrCntrl ( void* cntx, uint32_t* const remainings )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur = (s_eCU_timerAdapterCtx*)cntx;
    ctxCur->sendIsError = true;
    m_tim_timCnt++;
    if( m_tim_remainingTime > 0u )
    {
        m_tim_remainingTime--;
    }

    *remainings = m_tim_remainingTime;

    if( m_tim_timCnt < 2 )
    {
        result = true;
    }
    else
    {
        result = false;
    }

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
        (void)printf("msgReceiverTestBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 5  -- FAIL \n");
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
        (void)printf("msgReceiverTestBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 6  -- FAIL \n");
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
        (void)printf("msgReceiverTestBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 7  -- FAIL \n");
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
        (void)printf("msgReceiverTestBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 8  -- FAIL \n");
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
        (void)printf("msgReceiverTestBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 9  -- FAIL \n");
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
        (void)printf("msgReceiverTestBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 10 -- FAIL \n");
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
        (void)printf("msgReceiverTestBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_BADPOINTER == MSGRX_IsInit(NULL, &isInit) )
    {
        (void)printf("msgReceiverTestBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_BADPOINTER == MSGRX_IsInit(&ctx, NULL) )
    {
        (void)printf("msgReceiverTestBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_BADPOINTER == MSGRX_StartNewMsg(NULL) )
    {
        (void)printf("msgReceiverTestBadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_BADPOINTER == MSGRX_StartNewMsgNClean(NULL) )
    {
        (void)printf("msgReceiverTestBadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_BADPOINTER == MSGRX_GetDecodedData(NULL, &dataP, &dataL) )
    {
        (void)printf("msgReceiverTestBadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_BADPOINTER == MSGRX_GetDecodedData(&ctx, NULL, &dataL) )
    {
        (void)printf("msgReceiverTestBadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_BADPOINTER == MSGRX_GetDecodedData(&ctx, &dataP, NULL) )
    {
        (void)printf("msgReceiverTestBadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 18 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_BADPOINTER == MSGRX_ReceiveChunk(NULL) )
    {
        (void)printf("msgReceiverTestBadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadPointer 19 -- FAIL \n");
    }
}

void msgReceiverTestBadInit(void)
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;

    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadInit 1  -- FAIL \n");
    }

    ctx.msgDecoderCtnx.byteUStufferCtnx.isInit = false;
    if( MSGRX_RES_OK == MSGRX_IsInit(&ctx, &isInit) )
    {
        if( false == isInit )
        {
            (void)printf("msgReceiverTestBadInit 2  -- OK \n");
        }
        else
        {
            (void)printf("msgReceiverTestBadInit 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgReceiverTestBadInit 2  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_NOINITLIB == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadInit 3  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_NOINITLIB == MSGRX_StartNewMsgNClean(&ctx) )
    {
        (void)printf("msgReceiverTestBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadInit 4  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_NOINITLIB == MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        (void)printf("msgReceiverTestBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadInit 5  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_NOINITLIB == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadInit 6  -- FAIL \n");
    }
}

void msgReceiverTestBadParamEntr(void)
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
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = 0u;
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
    if( MSGRX_RES_BADPARAM == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadParamEntr 1  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 0u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_BADPARAM == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadParamEntr 2  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 0u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_BADPARAM == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadParamEntr 3  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 100u;
    initData.i_timePerRecMs = 1000u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_BADPARAM == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadParamEntr 4  -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = 1u;
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
    if( MSGRX_RES_BADPARAM == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadParamEntr 5  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadParamEntr 6  -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_IsInit(&ctx, &isInit) )
    {
        if( true == isInit )
        {
            (void)printf("msgReceiverTestBadParamEntr 7  -- OK \n");
        }
        else
        {
            (void)printf("msgReceiverTestBadParamEntr 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgReceiverTestBadParamEntr 7  -- FAIL \n");
    }
}

void msgReceiverTestCorruptContext(void)
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 1  -- FAIL \n");
    }

    /* Function */
    ctx.rxBuff = NULL;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 2  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 3  -- FAIL \n");
    }

    /* Function */
    ctx.cbRxP = NULL;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 4  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 5  -- FAIL \n");
    }

    /* Function */
    ctx.cbRxCtx = NULL;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 6  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 7  -- FAIL \n");
    }

    /* Function */
    ctx.rxTimer.timerCtx = NULL;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 8  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 9  -- FAIL \n");
    }

    /* Function */
    ctx.rxTimer.tim_start = NULL;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 10 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 11 -- FAIL \n");
    }

    /* Function */
    ctx.rxTimer.tim_getRemaining = NULL;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 12 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 13 -- FAIL \n");
    }

    /* Function */
    ctx.rxBuffSize = 0u;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 14 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 15 -- FAIL \n");
    }

    /* Function */
    ctx.rxBuffFill = ctx.rxBuffSize + 1u;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 16 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 17 -- FAIL \n");
    }

    /* Function */
    ctx.rxBuffCntr = ctx.rxBuffFill + 1u;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 18 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 19 -- FAIL \n");
    }

    /* Function */
    ctx.frameTimeoutMs = 0u;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 20 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 21 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 21 -- FAIL \n");
    }

    /* Function */
    ctx.timePerRecMs = 0u;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 22 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 22 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 23 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 23 -- FAIL \n");
    }

    /* Function */
    ctx.timePerRecMs = ctx.frameTimeoutMs + 1u;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 24 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 24 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 25 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 25 -- FAIL \n");
    }

    /* Function */
    ctx.timePerRecMs = ctx.frameTimeoutMs + 1u;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsgNClean(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 26 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 26 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 27 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 27 -- FAIL \n");
    }

    /* Function */
    ctx.timePerRecMs = ctx.frameTimeoutMs + 1u;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        (void)printf("msgReceiverTestCorruptContext 28 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 28 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 29 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 29 -- FAIL \n");
    }

    /* Function */
    ctx.timePerRecMs = ctx.frameTimeoutMs + 1u;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 30 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 30 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 31 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 31 -- FAIL \n");
    }

    /* Function */
    ctx.msgDecoderCtnx.cbCrcCtx = NULL;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 32 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 32 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 33 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 33 -- FAIL \n");
    }

    /* Function */
    ctx.msgDecoderCtnx.cbCrcCtx = NULL;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_StartNewMsgNClean(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 34 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 34 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 35 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 35 -- FAIL \n");
    }

    /* Function */
    ctx.msgDecoderCtnx.cbCrcCtx = NULL;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_GetDecodedData(&ctx, &dataP, &dataL ) )
    {
        (void)printf("msgReceiverTestCorruptContext 36 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 36 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCorruptContext 37 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 37 -- FAIL \n");
    }

    /* Function */
    ctx.msgDecoderCtnx.cbCrcCtx = NULL;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCorruptContext 38 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCorruptContext 38 -- FAIL \n");
    }
}

void msgReceiverTestBadClBckCrc(void)
{
    /* Local variable */
    s_eFSP_MSGRX_Ctx ctx;
    s_eFSP_MSGRX_InitData initData;
    cb_crc32_msgd cbCrcPTest = &c32SAdaptEr;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterRx;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;
    bool_t isInit;

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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadClBckCrc 1  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckCrc 1  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckCrc 2  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckCrc 2  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00;
    m_rxPayload[2u] = 0x00;
    m_rxPayload[3u] = 0x00;
    m_rxPayload[4u] = 0x00;
    m_rxPayload[5u] = 0x02;
    m_rxPayload[6u] = 0x00;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0xCC;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = ECU_EOF;
    if( MSGRX_RES_CRCCLBKERROR == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckCrc 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckCrc 3  -- FAIL \n");
    }
}

void msgReceiverTestBadClBckReceive(void)
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
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsgErr;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadClBckReceive 1  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckReceive 1  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckReceive 2  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckReceive 2  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsgNClean(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckReceive 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckReceive 3  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00;
    m_rxPayload[2u] = 0x00;
    m_rxPayload[3u] = 0x00;
    m_rxPayload[4u] = 0x00;
    m_rxPayload[5u] = 0x02;
    m_rxPayload[6u] = 0x00;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0xCC;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = ECU_EOF;
    if( MSGRX_RES_RXCLBKERROR == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckReceive 4  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckReceive 4  -- FAIL \n");
    }
}

void msgReceiverTestBadClBckTim(void)
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
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsg;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStartErr;
    initData.i_rxTimer.tim_getRemaining = &timGetRemainingErr;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadClBckTim 1  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckTim 1  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_TIMCLBKERROR == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckTim 2  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckTim 2  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_TIMCLBKERROR == MSGRX_StartNewMsgNClean(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckTim 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckTim 3  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00;
    m_rxPayload[2u] = 0x00;
    m_rxPayload[3u] = 0x00;
    m_rxPayload[4u] = 0x00;
    m_rxPayload[5u] = 0x02;
    m_rxPayload[6u] = 0x00;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0xCC;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = ECU_EOF;
    if( MSGRX_RES_TIMCLBKERROR == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckTim 4  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckTim 4  -- FAIL \n");
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
    initData.i_rxTimer.tim_start = &timStartErr;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadClBckTim 5  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckTim 5  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_TIMCLBKERROR == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckTim 6  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckTim 6  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_TIMCLBKERROR == MSGRX_StartNewMsgNClean(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckTim 7  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckTim 7  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00;
    m_rxPayload[2u] = 0x00;
    m_rxPayload[3u] = 0x00;
    m_rxPayload[4u] = 0x00;
    m_rxPayload[5u] = 0x02;
    m_rxPayload[6u] = 0x00;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0xCC;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = ECU_EOF;
    if( MSGRX_RES_TIMCLBKERROR == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckTim 8  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckTim 8  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemainingErrCntrl;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = false;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadClBckTim 9  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckTim 9  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckTim 10 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckTim 10 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00;
    m_rxPayload[2u] = 0x00;
    m_rxPayload[3u] = 0x00;
    m_rxPayload[4u] = 0x00;
    m_rxPayload[5u] = 0x02;
    m_rxPayload[6u] = 0x00;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0xCC;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = ECU_EOF;
    if( MSGRX_RES_TIMCLBKERROR == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadClBckTim 11 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadClBckTim 11 -- FAIL \n");
    }
}

void msgReceiverTestBadFrame(void)
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadFrame 1  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 1  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 2  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 2  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00;
    m_rxPayload[2u] = 0x00;
    m_rxPayload[3u] = 0x00;
    m_rxPayload[4u] = 0x00;
    m_rxPayload[5u] = 0x02;
    m_rxPayload[6u] = 0x00;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0xCC;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = ECU_EOF;
    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 3  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadFrame 4  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 4  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 5  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 5  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0x00;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = 0xCC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_FRAMERESTART == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 6  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 6  -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGERECEIVED == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 6  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 6  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadFrame 7  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 7  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 8  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 8  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00;
    m_rxPayload[2u] = 0x00;
    m_rxPayload[3u] = 0x00;
    m_rxPayload[4u] = 0x00;
    m_rxPayload[5u] = 0x03;
    m_rxPayload[6u] = 0x00;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0xCC;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = 0xCC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_OUTOFMEM == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 9  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 9  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadFrame 10 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 10 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 11 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 11 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x50;
    m_rxPayload[2u] = 0x7B;
    m_rxPayload[3u] = 0x0A;
    m_rxPayload[4u] = 0x68;
    m_rxPayload[5u] = 0x01;
    m_rxPayload[6u] = 0x00;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0xCC;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = ECU_EOF;

    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 12 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 12 -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadFrame 13 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 13 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 14 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 14 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x50;
    m_rxPayload[2u] = 0x7B;
    m_rxPayload[3u] = 0x0A;
    m_rxPayload[4u] = 0x68;
    m_rxPayload[5u] = 0x01;
    m_rxPayload[6u] = 0x00;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = ECU_EOF;

    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 15 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 15 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = false;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestBadFrame 16 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 16 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 17 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 17 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0x00;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = 0xCC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_FRAMERESTART == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestBadFrame 18 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestBadFrame 18 -- FAIL \n");
    }
}

void msgReceiverTestCornerCase(void)
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 1u;
    initData.i_needWaitFrameStart = false;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCornerCase 1  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 1  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 2  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 2  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0x00;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = 0xCC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_FRAMERESTART == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 3  -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 4  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 4  -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 5  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 5  -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGERECEIVED == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 6  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 6  -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 5u;
    initData.i_timePerRecMs = 1u;
    initData.i_needWaitFrameStart = false;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCornerCase 7  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 7  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 8  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 8  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0x00;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = 0xCC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_FRAMERESTART == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 9  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 9  -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 10 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 10 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGETIMEOUT == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 11 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 11 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGETIMEOUT == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 12 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 12 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 6u;
    initData.i_timePerRecMs = 1u;
    initData.i_needWaitFrameStart = false;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCornerCase 13 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 13 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 14 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 14 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0x00;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = 0xCC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_FRAMERESTART == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 15 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 15 -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 16 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 16 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGETIMEOUT == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 17 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 17 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGETIMEOUT == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 18 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 18 -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsgJump;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = false;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCornerCase 19 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 19 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 20 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 20 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0x00;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = 0xCC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_FRAMERESTART == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 21 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 21 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGERECEIVED == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 22 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 22 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGERECEIVED == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 23 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 23 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 100u;
    initData.i_needWaitFrameStart = false;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCornerCase 24 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 24 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 25 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 25 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0x00;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = ECU_ESC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_FRAMERESTART == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 26 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 26 -- FAIL \n");
    }

    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 27 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 27 -- FAIL \n");
    }

    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 28 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 28 -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsgJump;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 9u;
    initData.i_timePerRecMs = 9u;
    initData.i_needWaitFrameStart = false;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCornerCase 29 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 29 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 30 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 30 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0x00;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = 0xCC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_FRAMERESTART == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 31 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 31 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGETIMEOUT == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 32 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 32 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGETIMEOUT == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 33 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 33 -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsgJump;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 10u;
    initData.i_timePerRecMs = 10u;
    initData.i_needWaitFrameStart = false;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCornerCase 34 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 34 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 35 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 35 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0x00;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = 0xCC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_FRAMERESTART == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 36 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 36 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGERECEIVED == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 37 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 37 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGETIMEOUT == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 38 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 38 -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsgJump;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 10u;
    initData.i_timePerRecMs = 10u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCornerCase 39 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 39 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 40 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 40 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0x00;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = 0xCC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_FRAMERESTART == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 41 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 41 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGERECEIVED == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 42 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 42 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGERECEIVED == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 43 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 43 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGETIMEOUT == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 44 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 44 -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsgCrrupt;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 10u;
    initData.i_timePerRecMs = 10u;
    initData.i_needWaitFrameStart = false;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCornerCase 45 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 45 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 46 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 46 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0x00;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = 0xCC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 47 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 47 -- FAIL \n");
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
    initData.i_rxTimer.tim_getRemaining = &timGetRemainingCorr;
    initData.i_frameTimeoutMs = 10u;
    initData.i_timePerRecMs = 10u;
    initData.i_needWaitFrameStart = false;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCornerCase 48 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 48 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 49 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 49 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = 0x00;
    m_rxPayload[10u] = 0xCC;
    m_rxPayload[11u] = 0xCC;
    m_rxPayload[12u] = ECU_EOF;
    if( MSGRX_RES_CORRUPTCTX == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase 50 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase 50 -- FAIL \n");
    }
}

void msgReceiverTestCornerCase2(void)
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
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsgJump;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 1000u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCornerCase2 1  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 1  -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 2  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 2  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 21u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_EOF;
    m_rxPayload[1u] = 0xFF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = ECU_SOF;
    m_rxPayload[10u] = 0x50;
    m_rxPayload[11u] = 0x7B;
    m_rxPayload[12u] = 0x0A;
    m_rxPayload[13u] = 0x68;
    m_rxPayload[14u] = 0x02;
    m_rxPayload[15u] = 0x00;
    m_rxPayload[16u] = 0x00;
    m_rxPayload[17u] = 0x00;
    m_rxPayload[18u] = 0xCC;
    m_rxPayload[19u] = 0xCC;
    m_rxPayload[20u] = ECU_EOF;
    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 3  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 3  -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 4  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 4  -- FAIL \n");
    }

    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 5  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 5  -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 6  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 6  -- FAIL \n");
    }

    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 7  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 7  -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 8  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 8  -- FAIL \n");
    }

    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 9  -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 9  -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 10 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 10 -- FAIL \n");
    }

    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 11 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 11 -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 12 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 12 -- FAIL \n");
    }

    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 13 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 13 -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 14 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 14 -- FAIL \n");
    }

    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 15 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 15 -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 16 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 16 -- FAIL \n");
    }

    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 17 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 17 -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 18 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 18 -- FAIL \n");
    }

    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 19 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 19 -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 20 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 20 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGERECEIVED == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 21 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 21 -- FAIL \n");
    }

    /* Function */
    initData.i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.i_receiveBuffArea = recBuff;
    initData.i_receiveBuffAreaSize = sizeof(recBuff);
    initData.i_cbCrcP = cbCrcPTest;
    initData.i_cbCrcCrx = &ctxAdapterCrc;
    initData.i_cbRxP = &receiveMsgJump;
    initData.i_cbRxCtx = &ctxAdapterRx;
    initData.i_rxTimer.timerCtx = &ctxAdapterTim;
    initData.i_rxTimer.tim_start = &timStart;
    initData.i_rxTimer.tim_getRemaining = &timGetRemaining;
    initData.i_frameTimeoutMs = 1000u;
    initData.i_timePerRecMs = 1000u;
    initData.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgReceiverTestCornerCase2 22 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 22 -- FAIL \n");
    }

    /* Function */
    if( MSGRX_RES_OK == MSGRX_StartNewMsg(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 23 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 23 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 21u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_EOF;
    m_rxPayload[1u] = 0xFF;
    m_rxPayload[2u] = 0x50;
    m_rxPayload[3u] = 0x7B;
    m_rxPayload[4u] = 0x0A;
    m_rxPayload[5u] = 0x68;
    m_rxPayload[6u] = 0x02;
    m_rxPayload[7u] = 0x00;
    m_rxPayload[8u] = 0x00;
    m_rxPayload[9u] = ECU_SOF;
    m_rxPayload[10u] = 0x50;
    m_rxPayload[11u] = 0x7B;
    m_rxPayload[12u] = 0x0A;
    m_rxPayload[13u] = 0x68;
    m_rxPayload[14u] = 0x02;
    m_rxPayload[15u] = 0x00;
    m_rxPayload[16u] = 0x00;
    m_rxPayload[17u] = 0x00;
    m_rxPayload[18u] = 0xCC;
    m_rxPayload[19u] = 0xCC;
    m_rxPayload[20u] = ECU_EOF;
    if( MSGRX_RES_BADFRAME == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 24 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 24 -- FAIL \n");
    }

    if( MSGRX_RES_OK == MSGRX_StartNewMsgNClean(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 25 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 25 -- FAIL \n");
    }

    if( MSGRX_RES_MESSAGERECEIVED == MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("msgReceiverTestCornerCase2 26 -- OK \n");
    }
    else
    {
        (void)printf("msgReceiverTestCornerCase2 26 -- FAIL \n");
    }
}






















