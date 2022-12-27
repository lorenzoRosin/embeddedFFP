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
#include "eFSP_MSGTX.h"
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
struct t_eFSP_MSGE_CrcCtxUser
{
    e_eCU_CRC_RES lastError;
};

struct t_eFSP_MSGTX_TxCtxUser
{
    bool sendIsError;
};

struct t_eFSP_MSGTX_TimCtxUser
{
    bool sendIsError;
};

static bool_t c32SAdapt(t_eFSP_MSGE_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptEr(t_eFSP_MSGE_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);
static bool_t sendMsg( t_eFSP_MSGTX_TxCtx* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                       const uint32_t timeToSend );
static bool_t sendMsgCorr( t_eFSP_MSGTX_TxCtx* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                       const uint32_t timeToSend );
static bool_t sendMsgOnce( t_eFSP_MSGTX_TxCtx* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                       const uint32_t timeToSend );
static bool_t sendMsgErr( t_eFSP_MSGTX_TxCtx* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                          const uint32_t timeToSend );
static bool_t timStart ( t_eFSP_MSGTX_TimCtx* cntx, const uint32_t timeoutVal );
static bool_t timGetRemaining ( t_eFSP_MSGTX_TimCtx* cntx, uint32_t* const remainings );
static bool_t timStartErr ( t_eFSP_MSGTX_TimCtx* cntx, const uint32_t timeoutVal );
static bool_t timGetRemainingErr ( t_eFSP_MSGTX_TimCtx* cntx, uint32_t* const remainings );
static bool_t timGetRemainingCorr ( t_eFSP_MSGTX_TimCtx* cntx, uint32_t* const remainings );



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
void eFSP_MSGTXTST_ExeTest(void)
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
bool_t c32SAdapt(t_eFSP_MSGE_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val)
{
    bool_t result;
    t_eFSP_MSGE_CrcCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == c32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGE_CrcCtx*)cntx;

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

bool_t c32SAdaptEr(t_eFSP_MSGE_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val)
{
    bool_t result;
    t_eFSP_MSGE_CrcCtx* ctxCur;

    (void)s;
    (void)d;
    (void)dLen;

    if( ( NULL == cntx ) || ( NULL == c32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGE_CrcCtx*)cntx;

        ctxCur->lastError = e_eCU_CRC_RES_BADPOINTER;
        result = false;
        *c32Val = 0u;
    }

    return result;
}


static uint8_t m_txBuff[100u];
static uint32_t m_txBuffCounter;
static uint32_t m_send_when;

bool_t sendMsg( t_eFSP_MSGTX_TxCtx* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                const uint32_t timeToSend )
{
    bool_t result;
    t_eFSP_MSGTX_TxCtx* ctxCur;

    (void)timeToSend;

    if( ( NULL == cntx ) || ( NULL == dataToSend ) || ( NULL == dataSended ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TxCtx*)cntx;
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


bool_t sendMsgCorr( t_eFSP_MSGTX_TxCtx* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                const uint32_t timeToSend )
{
    bool_t result;
    t_eFSP_MSGTX_TxCtx* ctxCur;

    (void)timeToSend;

    if( ( NULL == cntx ) || ( NULL == dataToSend ) || ( NULL == dataSended ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TxCtx*)cntx;
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

bool_t sendMsgOnce( t_eFSP_MSGTX_TxCtx* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                const uint32_t timeToSend )
{
    bool_t result;
    t_eFSP_MSGTX_TxCtx* ctxCur;

    (void)timeToSend;

    if( ( NULL == cntx ) || ( NULL == dataToSend ) || ( NULL == dataSended ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TxCtx*)cntx;
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

bool_t sendMsgErr( t_eFSP_MSGTX_TxCtx* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                const uint32_t timeToSend )
{
    bool_t result;
    t_eFSP_MSGTX_TxCtx* ctxCur;

    (void)dataToSendLen;
    (void)timeToSend;

    if( ( NULL == cntx ) || ( NULL == dataToSend ) || ( NULL == dataSended ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TxCtx*)cntx;
        ctxCur->sendIsError = true;
        result = false;
    }

    return result;
}

static uint32_t m_tim_remainingTime;

bool_t timStart ( t_eFSP_MSGTX_TimCtx* cntx, const uint32_t timeoutVal )
{
    bool_t result;
    t_eFSP_MSGTX_TimCtx* ctxCur;

    if( NULL == cntx )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TimCtx*)cntx;
        ctxCur->sendIsError = true;

        m_tim_remainingTime = timeoutVal;
        result = true;
    }

    return result;
}

bool_t timGetRemaining ( t_eFSP_MSGTX_TimCtx* cntx, uint32_t* const remainings )
{
    bool_t result;
    t_eFSP_MSGTX_TimCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == remainings ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TimCtx*)cntx;
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


bool_t timStartErr ( t_eFSP_MSGTX_TimCtx* cntx, const uint32_t timeoutVal )
{
    bool_t result;
    t_eFSP_MSGTX_TimCtx* ctxCur;

    if( NULL == cntx )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TimCtx*)cntx;
        ctxCur->sendIsError = true;

        m_tim_remainingTime = timeoutVal;
        result = false;
    }

    return result;
}

bool_t timGetRemainingErr ( t_eFSP_MSGTX_TimCtx* cntx, uint32_t* const remainings )
{
    bool_t result;
    t_eFSP_MSGTX_TimCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == remainings ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TimCtx*)cntx;
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

bool_t timGetRemainingCorr ( t_eFSP_MSGTX_TimCtx* cntx, uint32_t* const remainings )
{
    bool_t result;
    t_eFSP_MSGTX_TimCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == remainings ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TimCtx*)cntx;
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
    f_eFSP_MSGE_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGTX_TxCtx ctxAdapterSend;
    t_eFSP_MSGTX_TimCtx ctxAdapterTim;
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
    initData.puIMemArea = NULL;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 3  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = NULL;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 4  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = NULL;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 5  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = NULL;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 6  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = NULL;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 7  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = NULL;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 8  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = NULL;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 9  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = NULL;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 10 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = NULL;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 11 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = NULL;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 12 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = NULL;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPOINTER == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadPointer 13 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = NULL;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
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
    f_eFSP_MSGE_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGTX_TxCtx ctxAdapterSend;
    t_eFSP_MSGTX_TimCtx ctxAdapterTim;
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;
    bool_t isInit;

    /* Function */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    ctx.tMsgeCtx.tBSTFCtx.bIsInit = false;
    ctx.tMsgeCtx.ptCrcCtx = &ctxAdapterCrc;
    ctx.tMsgeCtx.fCrc = cbCrcPTest;
    ctx.puRxBuff = sendBuff;
    ctx.uTxBuffL = sizeof(sendBuff);
    ctx.uTxBuffCntr = 0u;
    ctx.uTxBuffFill = 0u;
    ctx.fTx = &sendMsg;
    ctx.ptTxCtx = &ctxAdapterSend;
    ctx.tTxTim.ptTimCtx = &ctxAdapterTim;
    ctx.tTxTim.fTimStart = &timStart;
    ctx.tTxTim.fTimGetRemain = &timGetRemaining;
    ctx.uTimeoutMs = 1000u;
    ctx.uTimePerSendMs = 100u;

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
    f_eFSP_MSGE_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGTX_TxCtx ctxAdapterSend;
    t_eFSP_MSGTX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    bool_t isInit;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
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
    f_eFSP_MSGE_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGTX_TxCtx ctxAdapterSend;
    t_eFSP_MSGTX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = 0u;
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 1  -- FAIL \n");
    }

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 0u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 2  -- FAIL \n");
    }

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 0u;
    if( e_eFSP_MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 3  -- FAIL \n");
    }

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 100u;
    initData.uITimePerSendMs = 1000u;
    if( e_eFSP_MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 4  -- FAIL \n");
    }

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = 0u;
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_BADPARAM == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterBadParamEntr 5  -- FAIL \n");
    }

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
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
    f_eFSP_MSGE_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGTX_TxCtx ctxAdapterSend;
    t_eFSP_MSGTX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 1  -- FAIL \n");
    }

    /* Function */
    ctx.puRxBuff = NULL;
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
    ctx.fTx = NULL;
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
    ctx.ptTxCtx = NULL;
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
    ctx.tTxTim.ptTimCtx = NULL;
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
    ctx.tTxTim.fTimStart = NULL;
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
    ctx.tTxTim.fTimGetRemain = NULL;
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
    ctx.uTxBuffL = 0u;
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
    ctx.uTxBuffFill = ctx.uTxBuffL + 1u;
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
    ctx.uTxBuffCntr = ctx.uTxBuffFill + 1u;
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
    ctx.uTimeoutMs = 0u;
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
    ctx.uTimePerSendMs = 0u;
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
    ctx.uTimePerSendMs = ctx.uTimeoutMs + 1u;
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
    ctx.uTimePerSendMs = 0u;
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
    ctx.uTimePerSendMs = 0u;
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
    ctx.uTimePerSendMs = 0u;
    if( e_eFSP_MSGTX_RES_CORRUPTCTX == eFSP_MSGTX_SendChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCorruptContext 30 -- FAIL \n");
    }

    /* Function */
    ctx.tMsgeCtx.ptCrcCtx = NULL;
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
    ctx.tMsgeCtx.ptCrcCtx = NULL;
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
    ctx.tMsgeCtx.ptCrcCtx = NULL;
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
    ctx.tMsgeCtx.ptCrcCtx = NULL;
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
    f_eFSP_MSGE_CrcCb cbCrcPTest = &c32SAdaptEr;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGTX_TxCtx ctxAdapterSend;
    t_eFSP_MSGTX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;
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
    f_eFSP_MSGE_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGTX_TxCtx ctxAdapterSend;
    t_eFSP_MSGTX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsgErr;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;

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
    f_eFSP_MSGE_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGTX_TxCtx ctxAdapterSend;
    t_eFSP_MSGTX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStartErr;
    initData.tITxTim.fTimGetRemain = &timGetRemainingErr;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;

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
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemainingErr;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;

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
    f_eFSP_MSGE_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGTX_TxCtx ctxAdapterSend;
    t_eFSP_MSGTX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;

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
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = 1u;
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;

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
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = 1u;
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 1u;

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
    f_eFSP_MSGE_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGTX_TxCtx ctxAdapterSend;
    t_eFSP_MSGTX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = 5u;
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsgOnce;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 3u;
    initData.uITimePerSendMs = 3u;

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
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = 3u;
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsgOnce;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 12u;
    initData.uITimePerSendMs = 5u;

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
    f_eFSP_MSGE_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGTX_TxCtx ctxAdapterSend;
    t_eFSP_MSGTX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  sendBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initData, 0, sizeof(t_eFSP_MSGTX_InitData));

    /* Init */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsgCorr;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;

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
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puITxBuffArea = sendBuff;
    initData.uITxBuffAreaL = sizeof(sendBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fITx = &sendMsg;
    initData.ptICbTxCtx = &ctxAdapterSend;
    initData.tITxTim.ptTimCtx = &ctxAdapterTim;
    initData.tITxTim.fTimStart = &timStart;
    initData.tITxTim.fTimGetRemain = &timGetRemainingCorr;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerSendMs = 100u;

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