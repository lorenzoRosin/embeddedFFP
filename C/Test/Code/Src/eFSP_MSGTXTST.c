/**
 * @file       eFSP_MSGTXTST.c
 *
 * @brief      Message trasmitter test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_MSGTXTST.h"
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
                            "MISRAC2012-Rule-8.9_a", "MISRAC2012-Rule-8.13", "MISRAC2012-Rule-2.2_b", "MISRAC2004-16.7"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
typedef struct
{
    e_eCU_CRC_RES lastError;
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
static void eFSP_TEST_msgTransmitterBadPointer(void);
static void eFSP_TEST_msgTransmitterBadInit(void);
static void eFSP_TEST_msgTransmitterBadIniMsg(void);
static void eFSP_TEST_msgTransmitterBadParamEntr(void);
static void eFSP_TEST_msgTransmitterCorruptContext(void);
static void eFSP_TEST_msgTransmitterBadClBckCrc(void);
static void eFSP_TEST_msgTransmitterBadClBckSend(void);
static void eFSP_TEST_msgTransmitterBadClBckTim(void);
static void eFSP_TEST_msgTransmitterCornerCase(void);
static void eFSP_TEST_msgTransmitterCornerCase2(void);
static void eFSP_TEST_msgTransmitterCornerCase3(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_TEST_msgTransmitter(void)
{
	(void)printf("\n\nMESSAGE TRANSMITTER TEST START \n\n");

    eFSP_TEST_msgTransmitterBadPointer();
    eFSP_TEST_msgTransmitterBadInit();
    eFSP_TEST_msgTransmitterBadIniMsg();
    eFSP_TEST_msgTransmitterBadParamEntr();
    eFSP_TEST_msgTransmitterCorruptContext();
    eFSP_TEST_msgTransmitterBadClBckCrc();
    eFSP_TEST_msgTransmitterBadClBckSend();
    eFSP_TEST_msgTransmitterBadClBckTim();
    eFSP_TEST_msgTransmitterCornerCase();
    eFSP_TEST_msgTransmitterCornerCase2();
    eFSP_TEST_msgTransmitterCornerCase3();

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

        ctxCur->lastError = e_eCU_CRC_RES_BADPOINTER;
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
void eFSP_TEST_msgTransmitterBadPointer(void)
{
    /* Local variable */
    t_eFSP_MSGTX_Ctx ctx;
    t_eFSP_MSGTX_InitData initData;
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(NULL, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, NULL) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 2  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 3  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 4  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 5  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 6  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 7  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 8  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 9  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 10 -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 11 -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 12 -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 13 -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_IsInit(NULL, &isInit) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_IsInit(&ctx, NULL) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_NewMessage(NULL, 10u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_GetWherePutData(NULL, &dataP, &dataL) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_GetWherePutData(&ctx, NULL, &dataL) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, NULL) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 20 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_RestartMessage(NULL) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_SendChunk(NULL) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 22 -- FAIL \n");
    }
}

void eFSP_TEST_msgTransmitterBadInit(void)
{
    /* Local variable */
    t_eFSP_MSGTX_Ctx ctx;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;
    bool_t isInit;

    /* Function */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    ctx.msge_Ctx.bstf_Ctx.bIsInit = false;
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
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_IsInit(&ctx, &isInit) )
    {
        if( false == isInit )
        {
            (void)printf("eFSP_TEST_msgTransmitterBadInit 1  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgTransmitterBadInit 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadInit 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_NOINITLIB == eFSP_MSGTX_NewMessage(&ctx, 10u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadInit 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadInit 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_NOINITLIB == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadInit 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_NOINITLIB == eFSP_MSGTX_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadInit 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_NOINITLIB == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadInit 5  -- FAIL \n");
    }
}

void eFSP_TEST_msgTransmitterBadIniMsg(void)
{
    /* Local variable */
    t_eFSP_MSGTX_Ctx ctx;
    t_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    bool_t isInit;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

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
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadIniMsg 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadIniMsg 1  -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_NOINITMESSAGE == eFSP_MSGTX_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadIniMsg 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadIniMsg 2  -- FAIL \n");
    }

    m_tim_remainingTime = 100u;
    if( e_eFSP_MSGTX_RES_NOINITMESSAGE == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadIniMsg 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadIniMsg 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_IsInit(&ctx, &isInit) )
    {
        if( true == isInit )
        {
            (void)printf("eFSP_TEST_msgTransmitterBadIniMsg 4  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgTransmitterBadIniMsg 4  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadIniMsg 4  -- FAIL \n");
    }
}

void eFSP_TEST_msgTransmitterBadParamEntr(void)
{
    /* Local variable */
    t_eFSP_MSGTX_Ctx ctx;
    t_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

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
    if( e_eFSP_MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 1  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 2  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 3  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 4  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 5  -- FAIL \n");
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
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_BADPARAM == eFSP_MSGTX_NewMessage(&ctx, 0u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 2  -- FAIL \n");
    }
}

void eFSP_TEST_msgTransmitterCorruptContext(void)
{
    /* Local variable */
    t_eFSP_MSGTX_Ctx ctx;
    t_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

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
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 1  -- FAIL \n");
    }

    /* Function */
    ctx.p_rxBuff = NULL;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 3  -- FAIL \n");
    }

    /* Function */
    ctx.f_Tx = NULL;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 5  -- FAIL \n");
    }

    /* Function */
    ctx.p_TxCtx = NULL;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 7  -- FAIL \n");
    }

    /* Function */
    ctx.txTim.p_timCtx = NULL;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 9  -- FAIL \n");
    }

    /* Function */
    ctx.txTim.f_timStart = NULL;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 10 -- FAIL \n");
    }


    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 11 -- FAIL \n");
    }

    /* Function */
    ctx.txTim.f_timGetRemaining = NULL;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 13 -- FAIL \n");
    }

    /* Function */
    ctx.txBuffSize = 0u;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 15 -- FAIL \n");
    }

    /* Function */
    ctx.txBuffFill = ctx.txBuffSize + 1u;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 17 -- FAIL \n");
    }

    /* Function */
    ctx.txBuffCntr = ctx.txBuffFill + 1u;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 19 -- FAIL \n");
    }

    /* Function */
    ctx.timeoutMs = 0u;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 20 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 21 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = 0u;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 22 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 23 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = ctx.timeoutMs + 1u;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 24 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 25 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = 0u;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 26 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 27 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = 0u;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 28 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 29 -- FAIL \n");
    }

    /* Function */
    ctx.timePerSendMs = 0u;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 30 -- FAIL \n");
    }

    /* Function */
    ctx.msge_Ctx.p_crcCtx = NULL;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 31 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 32 -- FAIL \n");
    }

    /* Function */
    ctx.msge_Ctx.p_crcCtx = NULL;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 33 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 34 -- FAIL \n");
    }

    /* Function */
    ctx.msge_Ctx.p_crcCtx = NULL;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 35 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 35 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 36 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 36 -- FAIL \n");
    }

    /* Function */
    ctx.msge_Ctx.p_crcCtx = NULL;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 37 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 37 -- FAIL \n");
    }
}

void eFSP_TEST_msgTransmitterBadClBckCrc(void)
{
    /* Local variable */
    t_eFSP_MSGTX_Ctx ctx;
    t_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdaptEr;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

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
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckCrc 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckCrc 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_CRCCLBKERROR == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        if( e_eCU_CRC_RES_BADPOINTER == ctxAdapterCrc.lastError )
        {
            (void)printf("eFSP_TEST_msgTransmitterBadClBckCrc 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgTransmitterBadClBckCrc 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckCrc 2  -- FAIL \n");
    }
}

void eFSP_TEST_msgTransmitterBadClBckSend(void)
{
    /* Local variable */
    t_eFSP_MSGTX_Ctx ctx;
    t_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

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

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckSend 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckSend 1  -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("eFSP_TEST_msgTransmitterBadClBckSend 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgTransmitterBadClBckSend 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckSend 2  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckSend 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckSend 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_TXCLBKERROR == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckSend 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckSend 4  -- FAIL \n");
    }
}

void eFSP_TEST_msgTransmitterBadClBckTim(void)
{
    /* Local variable */
    t_eFSP_MSGTX_Ctx ctx;
    t_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

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

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 1  -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 2  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    if( e_eFSP_MSGTX_RES_TIMCLBKERROR == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 3  -- FAIL \n");
    }

     /* Function */
    if( e_eFSP_MSGTX_RES_TIMCLBKERROR == eFSP_MSGTX_RestartMessage(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 4  -- FAIL \n");
    }

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

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

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 5  -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 6  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_TIMCLBKERROR == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadClBckTim 8  -- FAIL \n");
    }

}

void eFSP_TEST_msgTransmitterCornerCase(void)
{
    /* Init static */
    m_send_when = 0u;
    m_txBuffCounter = 0u;

    /* Local variable */
    t_eFSP_MSGTX_Ctx ctx;
    t_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

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

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCornerCase 1  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 1  -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
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
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCornerCase 3  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
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
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_RestartMessage(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 6  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
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

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCornerCase 9  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 9  -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
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
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCornerCase 11 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
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

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("msgTransmitterTestCornerCase 14 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 14 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
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
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("msgTransmitterTestCornerCase 16 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 17 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 17 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 18 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 18 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 19 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 19 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 20 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 20 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 21 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 21 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 22 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 22 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 23 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 23 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 24 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 24 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 25 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 25 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 26 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 26 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 27 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 27 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 28 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 28 -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("msgTransmitterTestCornerCase 29 -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 29 -- FAIL \n");
    }


    if( e_eFSP_MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
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

void eFSP_TEST_msgTransmitterCornerCase2(void)
{
    /* Init static */
    m_send_when = 0u;
    m_txBuffCounter = 0u;

    /* Local variable */
    t_eFSP_MSGTX_Ctx ctx;
    t_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

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

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 1  -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("eFSP_TEST_msgTransmitterCornerCase2 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgTransmitterCornerCase2 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 2  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    m_txBuffCounter = 0u;
    m_send_when = 0u;
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_MESSAGETIMEOUT == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 4  -- FAIL \n");
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

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 6  -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("eFSP_TEST_msgTransmitterCornerCase2 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgTransmitterCornerCase2 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 7  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    m_txBuffCounter = 0u;
    m_send_when = 0u;
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 9  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 10 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 11 -- FAIL \n");
    }


    /* Function */
    if( e_eFSP_MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_MESSAGETIMEOUT == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 13 -- FAIL \n");
    }

    if( ( ECU_SOF == m_txBuff[0u] ) || ( 0x30u == m_txBuff[1u] )  || ( 0x5Cu == m_txBuff[2u] ) ||
        ( 0xB1u == m_txBuff[3u]   ) || ( 0xD7u == m_txBuff[4u] )  || ( 0x02u == m_txBuff[5u] ) ||
        ( 0x00u == m_txBuff[6u]   ) || ( 0x00u == m_txBuff[7u] )  || ( 0x00u == m_txBuff[8u] ) ||
        ( 0x01u == m_txBuff[9u]   ) || ( 0x02u == m_txBuff[10u] ) || ( ECU_EOF == m_txBuff[11u] ) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase2 14 -- FAIL \n");
    }
}

void eFSP_TEST_msgTransmitterCornerCase3()
{
    /* Local variable */
    t_eFSP_MSGTX_Ctx ctx;
    t_eFSP_MSGTX_InitData initData;
    cb_crc32_msge cbCrcPTest = &c32SAdapt;
    s_eCU_crcAdapterCtx ctxAdapterCrc;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

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

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 1  -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("eFSP_TEST_msgTransmitterCornerCase3 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgTransmitterCornerCase3 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 2  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 4  -- FAIL \n");
    }

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

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

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 5  -- FAIL \n");
    }

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            (void)printf("eFSP_TEST_msgTransmitterCornerCase3 6  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgTransmitterCornerCase3 6  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 6  -- FAIL \n");
    }

    /* Function */
    dataP[0u] = 0x01u;
    dataP[1u] = 0x02u;
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctx, 2u) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase3 8  -- FAIL \n");
    }

}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "MISRAC2012-Rule-8.9_a", "MISRAC2012-Rule-8.13", "MISRAC2012-Rule-2.2_b", "MISRAC2004-16.7"
#endif