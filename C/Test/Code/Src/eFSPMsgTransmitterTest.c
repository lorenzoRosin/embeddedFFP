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

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "MISRAC2012-Rule-8.9_a", "MISRAC2012-Rule-8.13", "MISRAC2012-Rule-2.2_b", "MISRAC2004-16.7"
    /* Suppressed for code clarity in test execution*/
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

static bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptEr(void* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);
static bool_t sendMsg( void* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                       const uint32_t timeToSend );
static bool_t sendMsgCorr( void* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                       const uint32_t timeToSend );
static bool_t sendMsgOnce( void* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                       const uint32_t timeToSend );
static bool_t sendMsgErr( void* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                          const uint32_t timeToSend );
static bool_t timStart ( void* cntx, const uint32_t timeoutVal );
static bool_t timGetRemaining ( void* cntx, uint32_t* const remainings );
static bool_t timStartErr ( void* cntx, const uint32_t timeoutVal );
static bool_t timGetRemainingErr ( void* cntx, uint32_t* const remainings );
static bool_t timGetRemainingCorr ( void* cntx, uint32_t* const remainings );



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
static void msgTransmitterTestCornerCase(void);
static void msgTransmitterTestCornerCase2(void);
static void msgTransmitterTestCornerCase3(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_TEST_msgTransmitter(void)
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
    msgTransmitterTestCornerCase();
    msgTransmitterTestCornerCase2();
    msgTransmitterTestCornerCase3();

    (void)printf("\n\nMESSAGE TRANSMITTER TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
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

bool_t c32SAdaptEr(void* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val)
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


static uint8_t m_txBuff[100u];
static uint32_t m_txBuffCounter;
static uint32_t m_send_when;

bool_t sendMsg( void* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                const uint32_t timeToSend )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur;

    (void)timeToSend;

    if( ( NULL == cntx ) || ( NULL == dataToSend ) || ( NULL == dataSended ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (s_eCU_msgSendAdapterCtx*)cntx;
        ctxCur->sendIsError = true;


        if( ( m_txBuffCounter + dataToSendLen ) < sizeof(m_txBuff) )
        {
            (void)memcpy(&m_txBuff[m_txBuffCounter], dataToSend, dataToSendLen);
            m_txBuffCounter += dataToSendLen;
            *dataSended = dataToSendLen;

            result = true;
        }
        else
        {
            result = false;
        }
    }

    return result;
}


bool_t sendMsgCorr( void* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                const uint32_t timeToSend )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur;

    (void)timeToSend;

    if( ( NULL == cntx ) || ( NULL == dataToSend ) || ( NULL == dataSended ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (s_eCU_msgSendAdapterCtx*)cntx;
        ctxCur->sendIsError = true;


        if( ( m_txBuffCounter + dataToSendLen ) < sizeof(m_txBuff) )
        {
            (void)memcpy(&m_txBuff[m_txBuffCounter], dataToSend, dataToSendLen);
            m_txBuffCounter += dataToSendLen;
            *dataSended = dataToSendLen + 1u;

            result = true;
        }
        else
        {
            result = false;
        }
    }

    return result;
}

bool_t sendMsgOnce( void* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                const uint32_t timeToSend )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur;

    (void)timeToSend;

    if( ( NULL == cntx ) || ( NULL == dataToSend ) || ( NULL == dataSended ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (s_eCU_msgSendAdapterCtx*)cntx;
        ctxCur->sendIsError = true;


        if( ( m_txBuffCounter + dataToSendLen ) < sizeof(m_txBuff) )
        {
            if( 0u == m_send_when )
            {
                (void)memcpy(&m_txBuff[m_txBuffCounter], dataToSend, dataToSendLen);
                m_txBuffCounter += dataToSendLen;
                *dataSended = dataToSendLen;
                m_send_when = 1u;
            }
            else
            {
                *dataSended = 0u;
                m_send_when = 0u;
            }

            result = true;
        }
        else
        {
            result = false;
        }
    }

    return result;
}

bool_t sendMsgErr( void* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                const uint32_t timeToSend )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur;

    (void)dataToSendLen;
    (void)timeToSend;

    if( ( NULL == cntx ) || ( NULL == dataToSend ) || ( NULL == dataSended ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (s_eCU_msgSendAdapterCtx*)cntx;
        ctxCur->sendIsError = true;
        result = false;
    }

    return result;
}

static uint32_t m_tim_remainingTime;

bool_t timStart ( void* cntx, const uint32_t timeoutVal )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur;

    if( NULL == cntx )
    {
        result = false;
    }
    else
    {
        ctxCur = (s_eCU_timerAdapterCtx*)cntx;
        ctxCur->sendIsError = true;

        m_tim_remainingTime = timeoutVal;
        result = true;
    }

    return result;
}

bool_t timGetRemaining ( void* cntx, uint32_t* const remainings )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == remainings ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (s_eCU_timerAdapterCtx*)cntx;
        ctxCur->sendIsError = true;

        if( m_tim_remainingTime > 0u )
        {
            m_tim_remainingTime--;
        }

        *remainings = m_tim_remainingTime;
        result = true;
    }

    return result;
}


bool_t timStartErr ( void* cntx, const uint32_t timeoutVal )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur;

    if( NULL == cntx )
    {
        result = false;
    }
    else
    {
        ctxCur = (s_eCU_timerAdapterCtx*)cntx;
        ctxCur->sendIsError = true;

        m_tim_remainingTime = timeoutVal;
        result = false;
    }

    return result;
}

bool_t timGetRemainingErr ( void* cntx, uint32_t* const remainings )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == remainings ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (s_eCU_timerAdapterCtx*)cntx;
        ctxCur->sendIsError = true;

        if( m_tim_remainingTime > 0u )
        {
            m_tim_remainingTime--;
        }

        *remainings = m_tim_remainingTime;
        result = false;
    }

    return result;
}

bool_t timGetRemainingCorr ( void* cntx, uint32_t* const remainings )
{
    bool_t result;
    s_eCU_timerAdapterCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == remainings ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (s_eCU_timerAdapterCtx*)cntx;
        ctxCur->sendIsError = true;
        if( m_tim_remainingTime < MAX_UINT32VAL )
        {
            m_tim_remainingTime++;
        }
        *remainings = m_tim_remainingTime + m_tim_remainingTime;

        if( m_tim_remainingTime < MAX_UINT32VAL )
        {
            m_tim_remainingTime++;
        }
        result = true;
    }

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
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(NULL, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, NULL) )
    {
        (void)printf("msgTransmitterTestBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 2  -- FAIL \n");
    }

    /* Function */
    initData.p_i_memArea = NULL;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 3  -- FAIL \n");
    }

    /* Function */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = NULL;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 4  -- FAIL \n");
    }

    /* Function */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = NULL;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 5  -- FAIL \n");
    }

    /* Function */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = NULL;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 6  -- FAIL \n");
    }

    /* Function */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = NULL;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 7  -- FAIL \n");
    }

    /* Function */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = NULL;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 8  -- FAIL \n");
    }

    /* Function */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = NULL;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 9  -- FAIL \n");
    }

    /* Function */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = NULL;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 10 -- FAIL \n");
    }

    /* Function */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = NULL;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 11 -- FAIL \n");
    }

    /* Function */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = NULL;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 12 -- FAIL \n");
    }

    /* Function */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = NULL;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 13 -- FAIL \n");
    }

    /* Function */
    initData.p_i_memArea = NULL;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_IsInit(NULL, &isInit) )
    {
        (void)printf("msgTransmitterTestBadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_IsInit(&ctx, NULL) )
    {
        (void)printf("msgTransmitterTestBadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_NewMessage(NULL, 10u) )
    {
        (void)printf("msgTransmitterTestBadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_GetWherePutData(NULL, &dataP, &dataL) )
    {
        (void)printf("msgTransmitterTestBadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 18 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_GetWherePutData(&ctx, NULL, &dataL) )
    {
        (void)printf("msgTransmitterTestBadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 19 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, NULL) )
    {
        (void)printf("msgTransmitterTestBadPointer 20 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 20 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_RestartMessage(NULL) )
    {
        (void)printf("msgTransmitterTestBadPointer 21 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadPointer 21 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPOINTER == eFSP_MSGTX_SendChunk(NULL) )
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
    (void)memset(&ctx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    ctx.msge_Ctx.bstf_Ctx.isInit = false;
    ctx.msge_Ctx.p_crcCtx = &ctxAdapterCrc;
    ctx.msge_Ctx.f_Crc = cbCrcPTest;
    ctx.p_rxBuff = sendBuff;
    ctx.txBuffSize = sizeof(sendBuff);
    ctx.txBuffCntr = 0u;
    ctx.txBuffFill = 0u;
    ctx.f_Tx = &sendMsg;
    ctx.p_TxCtx = &ctxAdapterSend;
    ctx.txTim.p_timCtx = &ctxAdapterTim;
    ctx.txTim.f_timStart = &timStart;
    ctx.txTim.f_timGetRemaining = &timGetRemaining;
    ctx.timeoutMs = 1000u;
    ctx.timePerSendMs = 100u;

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_IsInit(&ctx, &isInit) )
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
    if( MSGTX_RES_NOINITLIB == eFSP_MSGTX_NewMessage(&ctx, 10u) )
    {
        (void)printf("msgTransmitterTestBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadInit 2  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_NOINITLIB == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        (void)printf("msgTransmitterTestBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadInit 3  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_NOINITLIB == eFSP_MSGTX_RestartMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadInit 4  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_NOINITLIB == eFSP_MSGTX_SendChunk(&ctx) )
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
    (void)memset(&ctx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadIniMsg 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadIniMsg 1  -- FAIL \n");
    }

    if( MSGTX_RES_NOINITMESSAGE == eFSP_MSGTX_RestartMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestBadIniMsg 2  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadIniMsg 2  -- FAIL \n");
    }

    m_tim_remainingTime = 100u;
    if( MSGTX_RES_NOINITMESSAGE == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestBadIniMsg 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadIniMsg 3  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_IsInit(&ctx, &isInit) )
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
    (void)memset(&ctx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = 0u;
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 1  -- FAIL \n");
    }

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 0u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 2  -- FAIL \n");
    }

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 0u;
    if( MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 3  -- FAIL \n");
    }

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 100u;
    initData.i_timePerSendMs = 1000u;
    if( MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 4  -- FAIL \n");
    }

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = 0u;
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 5  -- FAIL \n");
    }

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadParamEntr 6  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_BADPARAM == eFSP_MSGTX_NewMessage(&ctx, 0u) )
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
    (void)memset(&ctx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 1  -- FAIL \n");
    }

    /* Function */
    ctx.p_rxBuff = NULL;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 2  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 3  -- FAIL \n");
    }

    /* Function */
    ctx.f_Tx = NULL;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 4  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 5  -- FAIL \n");
    }

    /* Function */
    ctx.p_TxCtx = NULL;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 6  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 7  -- FAIL \n");
    }

    /* Function */
    ctx.txTim.p_timCtx = NULL;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 8  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 9  -- FAIL \n");
    }

    /* Function */
    ctx.txTim.f_timStart = NULL;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 10 -- FAIL \n");
    }


    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 11 -- FAIL \n");
    }

    /* Function */
    ctx.txTim.f_timGetRemaining = NULL;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 12 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 13 -- FAIL \n");
    }

    /* Function */
    ctx.txBuffSize = 0u;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 14 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 15 -- FAIL \n");
    }

    /* Function */
    ctx.txBuffFill = ctx.txBuffSize + 1u;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 16 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 17 -- FAIL \n");
    }

    /* Function */
    ctx.txBuffCntr = ctx.txBuffFill + 1u;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 18 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 19 -- FAIL \n");
    }

    /* Function */
    ctx.timeoutMs = 0u;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 20 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 21 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 21 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = 0u;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 22 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 22 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 23 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 23 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = ctx.timeoutMs + 1u;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 24 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 24 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 25 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 25 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = 0u;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        (void)printf("msgTransmitterTestCorruptContext 26 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 26 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 27 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 27 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = 0u;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_RestartMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestCorruptContext 28 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 28 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 29 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 29 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = 0u;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCorruptContext 30 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 30 -- FAIL \n");
    }

    /* Function */
    ctx.msge_Ctx.p_crcCtx = NULL;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCorruptContext 31 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 31 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 32 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 32 -- FAIL \n");
    }

    /* Function */
    ctx.msge_Ctx.p_crcCtx = NULL;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        (void)printf("msgTransmitterTestCorruptContext 33 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 33 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 34 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 34 -- FAIL \n");
    }

    /* Function */
    ctx.msge_Ctx.p_crcCtx = NULL;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_RestartMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestCorruptContext 35 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 35 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCorruptContext 36 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCorruptContext 36 -- FAIL \n");
    }

    /* Function */
    ctx.msge_Ctx.p_crcCtx = NULL;
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_SendChunk(&ctx) )
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
    (void)memset(&ctx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;
    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadClBckCrc 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckCrc 1  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_CRCCLBKERROR == eFSP_MSGTX_NewMessage(&ctx, 2u) )
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
    (void)memset(&ctx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsgErr;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;

    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadClBckSend 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckSend 1  -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
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
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    if( MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestBadClBckSend 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckSend 3  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_TXCLBKERROR == eFSP_MSGTX_SendChunk(&ctx) )
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
    (void)memset(&ctx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStartErr;
    initData.i_txTim.f_timGetRemaining = &timGetRemainingErr;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;

    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadClBckTim 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 1  -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
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
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    if( MSGTX_RES_TIMCLBKERROR == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestBadClBckTim 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 3  -- FAIL \n");
    }

     /* Function */
    if( MSGTX_RES_TIMCLBKERROR == eFSP_MSGTX_RestartMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestBadClBckTim 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 4  -- FAIL \n");
    }

    /* Clear */
    (void)memset(&ctx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemainingErr;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;

    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestBadClBckTim 5  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 5  -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
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
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    if( MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestBadClBckTim 7  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 7  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_TIMCLBKERROR == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestBadClBckTim 8  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestBadClBckTim 8  -- FAIL \n");
    }

}

void msgTransmitterTestCornerCase(void)
{
    /* Init static */
    m_send_when = 0u;
    m_txBuffCounter = 0u;

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
    (void)memset(&ctx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;

    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCornerCase 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 1  -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("msgTransmitterTestCornerCase 2  -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestCornerCase 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 2  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    m_txBuffCounter = 0u;
    m_send_when = 0u;
    if( MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCornerCase 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 3  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 4  -- FAIL \n");
    }

    if( ( ECU_SOF == m_txBuff[0u] ) || ( 0x30u == m_txBuff[1u] )  || ( 0x5Cu == m_txBuff[2u] ) ||
        ( 0xB1u == m_txBuff[3u]   ) || ( 0xD7u == m_txBuff[4u] )  || ( 0x02u == m_txBuff[5u] ) ||
        ( 0x00u == m_txBuff[6u]   ) || ( 0x00u == m_txBuff[7u] )  || ( 0x00u == m_txBuff[8u] ) ||
        ( 0x01u == m_txBuff[9u]   ) || ( 0x02u == m_txBuff[10u] ) || ( ECU_EOF == m_txBuff[11u] ) )
    {
        (void)printf("msgTransmitterTestCornerCase 5  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 5  -- FAIL \n");
    }


    /* Function */
    m_txBuffCounter = 0u;
    m_send_when = 0u;
    if( MSGTX_RES_OK == eFSP_MSGTX_RestartMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 6  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 6  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 7  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 7  -- FAIL \n");
    }

    if( ( ECU_SOF == m_txBuff[0u] ) || ( 0x30u == m_txBuff[1u] )  || ( 0x5Cu == m_txBuff[2u] ) ||
        ( 0xB1u == m_txBuff[3u]   ) || ( 0xD7u == m_txBuff[4u] )  || ( 0x02u == m_txBuff[5u] ) ||
        ( 0x00u == m_txBuff[6u]   ) || ( 0x00u == m_txBuff[7u] )  || ( 0x00u == m_txBuff[8u] ) ||
        ( 0x01u == m_txBuff[9u]   ) || ( 0x02u == m_txBuff[10u] ) || ( ECU_EOF == m_txBuff[11u] ) )
    {
        (void)printf("msgTransmitterTestCornerCase 8  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 8  -- FAIL \n");
    }

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = 1u;
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;

    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCornerCase 9  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 9  -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("msgTransmitterTestCornerCase 10 -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestCornerCase 10 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 10 -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    m_txBuffCounter = 0u;
    m_send_when = 0u;
    if( MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCornerCase 11 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 11 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 12 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 12 -- FAIL \n");
    }

    if( ( ECU_SOF == m_txBuff[0u] ) || ( 0x30u == m_txBuff[1u] )  || ( 0x5Cu == m_txBuff[2u] ) ||
        ( 0xB1u == m_txBuff[3u]   ) || ( 0xD7u == m_txBuff[4u] )  || ( 0x02u == m_txBuff[5u] ) ||
        ( 0x00u == m_txBuff[6u]   ) || ( 0x00u == m_txBuff[7u] )  || ( 0x00u == m_txBuff[8u] ) ||
        ( 0x01u == m_txBuff[9u]   ) || ( 0x02u == m_txBuff[10u] ) || ( ECU_EOF == m_txBuff[11u] ) )
    {
        (void)printf("msgTransmitterTestCornerCase 13 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 13 -- FAIL \n");
    }

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = 1u;
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 1u;

    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCornerCase 14 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 14 -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("msgTransmitterTestCornerCase 15 -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestCornerCase 15 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 15 -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    m_txBuffCounter = 0u;
    m_send_when = 0u;
    if( MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCornerCase 16 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 16 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 17 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 17 -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 18 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 18 -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 19 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 19 -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 20 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 20 -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 21 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 21 -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 22 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 22 -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 23 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 23 -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 24 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 24 -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 25 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 25 -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 26 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 26 -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 27 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 27 -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 28 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 28 -- FAIL \n");
    }

    if( MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 29 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 29 -- FAIL \n");
    }


    if( MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 30 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 30 -- FAIL \n");
    }

    if( ( ECU_SOF == m_txBuff[0u] ) || ( 0x30u == m_txBuff[1u] )  || ( 0x5Cu == m_txBuff[2u] ) ||
        ( 0xB1u == m_txBuff[3u]   ) || ( 0xD7u == m_txBuff[4u] )  || ( 0x02u == m_txBuff[5u] ) ||
        ( 0x00u == m_txBuff[6u]   ) || ( 0x00u == m_txBuff[7u] )  || ( 0x00u == m_txBuff[8u] ) ||
        ( 0x01u == m_txBuff[9u]   ) || ( 0x02u == m_txBuff[10u] ) || ( ECU_EOF == m_txBuff[11u] ) )
    {
        (void)printf("msgTransmitterTestCornerCase 31 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 31 -- FAIL \n");
    }
}

void msgTransmitterTestCornerCase2(void)
{
    /* Init static */
    m_send_when = 0u;
    m_txBuffCounter = 0u;

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
    (void)memset(&ctx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = 5u;
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsgOnce;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 3u;
    initData.i_timePerSendMs = 3u;

    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCornerCase2 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 1  -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("msgTransmitterTestCornerCase2 2  -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestCornerCase2 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 2  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    m_txBuffCounter = 0u;
    m_send_when = 0u;
    if( MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCornerCase2 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 3  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_MESSAGETIMEOUT == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase2 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 4  -- FAIL \n");
    }

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = 3u;
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsgOnce;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 12u;
    initData.i_timePerSendMs = 5u;

    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCornerCase2 6  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 6  -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("msgTransmitterTestCornerCase2 7  -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestCornerCase2 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 7  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    m_txBuffCounter = 0u;
    m_send_when = 0u;
    if( MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCornerCase2 8  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 8  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase2 9  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 9  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase2 10 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 10 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase2 11 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 11 -- FAIL \n");
    }


    /* Function */
    if( MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase2 12 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 12 -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_MESSAGETIMEOUT == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase2 13 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 13 -- FAIL \n");
    }

    if( ( ECU_SOF == m_txBuff[0u] ) || ( 0x30u == m_txBuff[1u] )  || ( 0x5Cu == m_txBuff[2u] ) ||
        ( 0xB1u == m_txBuff[3u]   ) || ( 0xD7u == m_txBuff[4u] )  || ( 0x02u == m_txBuff[5u] ) ||
        ( 0x00u == m_txBuff[6u]   ) || ( 0x00u == m_txBuff[7u] )  || ( 0x00u == m_txBuff[8u] ) ||
        ( 0x01u == m_txBuff[9u]   ) || ( 0x02u == m_txBuff[10u] ) || ( ECU_EOF == m_txBuff[11u] ) )
    {
        (void)printf("msgTransmitterTestCornerCase2 14 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase2 14 -- FAIL \n");
    }
}

void msgTransmitterTestCornerCase3()
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
    (void)memset(&ctx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsgCorr;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemaining;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;

    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCornerCase3 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase3 1  -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("msgTransmitterTestCornerCase3 2  -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestCornerCase3 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase3 2  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    if( MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCornerCase3 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase3 3  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase3 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase3 4  -- FAIL \n");
    }

    /* Clear */
    (void)memset(&ctx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(s_eFSP_MSGTX_InitData));

    /* Init */
    initData.p_i_memArea = memArea;
    initData.i_memAreaSize = sizeof(memArea);
    initData.p_i_txBuffArea = sendBuff;
    initData.i_txBuffAreaSize = sizeof(sendBuff);
    initData.f_i_Crc = cbCrcPTest;
    initData.p_i_cbCrcCtx = &ctxAdapterCrc;
    initData.f_i_Tx = &sendMsg;
    initData.p_i_cbTxCtx = &ctxAdapterSend;
    initData.i_txTim.p_timCtx = &ctxAdapterTim;
    initData.i_txTim.f_timStart = &timStart;
    initData.i_txTim.f_timGetRemaining = &timGetRemainingCorr;
    initData.i_timeoutMs = 1000u;
    initData.i_timePerSendMs = 100u;

    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCornerCase3 5  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase3 5  -- FAIL \n");
    }

    if( MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("msgTransmitterTestCornerCase3 6  -- OK \n");
        }
        else
        {
            (void)printf("msgTransmitterTestCornerCase3 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase3 6  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    if( MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCornerCase3 7  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase3 7  -- FAIL \n");
    }

    /* Function */
    if( MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase3 8  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase3 8  -- FAIL \n");
    }

}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "MISRAC2012-Rule-8.9_a", "MISRAC2012-Rule-8.13", "MISRAC2012-Rule-2.2_b", "MISRAC2004-16.7"
#endif