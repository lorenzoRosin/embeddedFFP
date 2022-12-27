/**
 * @file       eFSP_MSGRXTST.h
 *
 * @brief      Message receiver test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_MSGRXTST.h"
#include "eFSP_MSGRX.h"
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
struct t_eFSP_MSGD_CrcCtxUser
{
    e_eCU_CRC_RES lastError;
};

struct t_eFSP_MSGRX_RxCtxUser
{
    bool sendIsError;
};

struct t_eFSP_MSGRX_TimCtxUser
{
    bool sendIsError;
};

static bool_t c32SAdapt(t_eFSP_MSGD_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptEr(t_eFSP_MSGD_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);
static bool_t receiveMsg( t_eFSP_MSGRX_RxCtx* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                       const uint32_t timeToRx );
static bool_t receiveMsgCrrupt( t_eFSP_MSGRX_RxCtx* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                       const uint32_t timeToRx );
static bool_t receiveMsgJump( t_eFSP_MSGRX_RxCtx* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                       const uint32_t timeToRx );
static bool_t receiveMsgJumpLong( t_eFSP_MSGRX_RxCtx* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                       const uint32_t timeToRx );
static bool_t receiveMsgErr( t_eFSP_MSGRX_RxCtx* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                          const uint32_t timeToRx );
static bool_t timStart ( t_eFSP_MSGRX_TimCtx* cntx, const uint32_t timeoutVal );
static bool_t timGetRemaining ( t_eFSP_MSGRX_TimCtx* cntx, uint32_t* const remainings );
static bool_t timGetRemainingCorr ( t_eFSP_MSGRX_TimCtx* cntx, uint32_t* const remainings );
static bool_t timStartErr ( t_eFSP_MSGRX_TimCtx* cntx, const uint32_t timeoutVal );
static bool_t timGetRemainingErr ( t_eFSP_MSGRX_TimCtx* cntx, uint32_t* const remainings );
static bool_t timGetRemainingErrCntrl( t_eFSP_MSGRX_TimCtx* cntx, uint32_t* const remainings );



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void eFSP_TEST_msgReceiverBadPointer(void);
static void eFSP_TEST_msgReceiverBadInit(void);
static void eFSP_TEST_msgReceiverBadParamEntr(void);
static void eFSP_TEST_msgReceiverCorruptContext(void);
static void eFSP_TEST_msgReceiverBadClBckCrc(void);
static void eFSP_TEST_msgReceiverBadClBckReceive(void);
static void eFSP_TEST_msgReceiverBadClBckTim(void);
static void eFSP_TEST_msgReceiverBadFrame(void);
static void eFSP_TEST_msgReceiverCornerCase(void);
static void eFSP_TEST_msgReceiverCornerCase2(void);
static void eFSP_TEST_msgReceiverCornerCase3(void);
static void eFSP_TEST_msgReceiverCornerCase4(void);
static void eFSP_TEST_msgReceiverCornerCase5(void);
static void eFSP_TEST_msgReceiverCornerCase6(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_MSGRXTST_ExeTest(void)
{
	(void)printf("\n\nMESSAGE RECEIVER TEST START \n\n");

    eFSP_TEST_msgReceiverBadPointer();
    eFSP_TEST_msgReceiverBadInit();
    eFSP_TEST_msgReceiverBadParamEntr();
    eFSP_TEST_msgReceiverCorruptContext();
    eFSP_TEST_msgReceiverBadClBckCrc();
    eFSP_TEST_msgReceiverBadClBckReceive();
    eFSP_TEST_msgReceiverBadClBckTim();
    eFSP_TEST_msgReceiverBadFrame();
    eFSP_TEST_msgReceiverCornerCase();
    eFSP_TEST_msgReceiverCornerCase2();
    eFSP_TEST_msgReceiverCornerCase3();
    eFSP_TEST_msgReceiverCornerCase4();
    eFSP_TEST_msgReceiverCornerCase5();
    eFSP_TEST_msgReceiverCornerCase6();

    (void)printf("\n\nMESSAGE RECEIVER TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
bool_t c32SAdapt(t_eFSP_MSGD_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val)
{
    bool_t result;
    t_eFSP_MSGD_CrcCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == c32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGD_CrcCtx*)cntx;

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

bool_t c32SAdaptEr(t_eFSP_MSGD_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val)
{
    bool_t result;
    t_eFSP_MSGD_CrcCtx* ctxCur;

    (void)s;
    (void)d;
    (void)dLen;

    if( ( NULL == cntx ) || ( NULL == c32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGD_CrcCtx*)cntx;

        ctxCur->lastError = e_eCU_CRC_RES_BADPOINTER;
        result = false;
        *c32Val = 0u;
    }

    return result;
}

static uint8_t  m_rxPayload[120u];
static uint32_t m_payloadSize;
static uint32_t m_payloadCounter;
static uint32_t m_read_jump;
static uint32_t m_read_jumpLong;

bool_t receiveMsg( t_eFSP_MSGRX_RxCtx* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
{
    bool_t result;
    t_eFSP_MSGRX_RxCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == dataToRx ) || ( NULL == dataRecevd ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGRX_RxCtx*)cntx;
        ctxCur->sendIsError = true;
        result = true;


        if( m_payloadCounter < m_payloadSize )
        {
            if( ( m_payloadSize - m_payloadCounter ) >= dataRxMax )
            {
                (void)memcpy(dataToRx, &m_rxPayload[m_payloadCounter], dataRxMax);
                m_payloadCounter += dataRxMax;
                *dataRecevd = dataRxMax;
            }
            else
            {
                (void)memcpy(dataToRx, &m_rxPayload[m_payloadCounter], ( m_payloadSize - m_payloadCounter ) );
                m_payloadCounter += ( m_payloadSize - m_payloadCounter ) ;
                *dataRecevd = ( m_payloadSize - m_payloadCounter );
            }
        }
        else
        {
            *dataRecevd = 0u;
        }
    }

    return result;
}

bool_t receiveMsgCrrupt (t_eFSP_MSGRX_RxCtx* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
{
    bool_t result;
    t_eFSP_MSGRX_RxCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == dataToRx ) || ( NULL == dataRecevd ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGRX_RxCtx*)cntx;
        ctxCur->sendIsError = true;
        result = true;

        if( m_payloadCounter < m_payloadSize )
        {
            if( ( m_payloadSize - m_payloadCounter ) >= dataRxMax )
            {
                (void)memcpy(dataToRx, &m_rxPayload[m_payloadCounter], dataRxMax);
                m_payloadCounter += dataRxMax;
                *dataRecevd = dataRxMax +1u;
            }
            else
            {
                (void)memcpy(dataToRx, &m_rxPayload[m_payloadCounter], ( m_payloadSize - m_payloadCounter ) );
                m_payloadCounter += ( m_payloadSize - m_payloadCounter ) ;
                *dataRecevd = ( m_payloadSize - m_payloadCounter );
            }
        }
        else
        {
            *dataRecevd = 0u;
        }
    }

    return result;
}

bool_t receiveMsgJump( t_eFSP_MSGRX_RxCtx* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
{
    bool_t result;
    t_eFSP_MSGRX_RxCtx* ctxCur;

    ctxCur = (t_eFSP_MSGRX_RxCtx*)cntx;
    ctxCur->sendIsError = true;
    result = true;

    if( ( NULL == cntx ) || ( NULL == dataToRx ) || ( NULL == dataRecevd ) )
    {
        result = false;
    }
    else
    {
        if( 0u == m_read_jump )
        {
            if( m_payloadCounter < m_payloadSize )
            {
                if( ( m_payloadSize - m_payloadCounter ) >= dataRxMax )
                {
                    (void)memcpy(dataToRx, &m_rxPayload[m_payloadCounter], dataRxMax);
                    m_payloadCounter += dataRxMax;
                    *dataRecevd = dataRxMax;
                }
                else
                {
                    (void)memcpy(dataToRx, &m_rxPayload[m_payloadCounter], ( m_payloadSize - m_payloadCounter ) );
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
    }

    return result;
}

bool_t receiveMsgJumpLong( t_eFSP_MSGRX_RxCtx* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
{
    bool_t result;
    t_eFSP_MSGRX_RxCtx* ctxCur;

    ctxCur = (t_eFSP_MSGRX_RxCtx*)cntx;
    ctxCur->sendIsError = true;
    result = true;

    if( ( NULL == cntx ) || ( NULL == dataToRx ) || ( NULL == dataRecevd ) )
    {
        result = false;
    }
    else
    {
        if( m_read_jumpLong >= 10u )
        {
            if( m_payloadCounter < m_payloadSize )
            {
                if( ( m_payloadSize - m_payloadCounter ) >= dataRxMax )
                {
                    (void)memcpy(dataToRx, &m_rxPayload[m_payloadCounter], dataRxMax);
                    m_payloadCounter += dataRxMax;
                    *dataRecevd = dataRxMax;
                }
                else
                {
                    (void)memcpy(dataToRx, &m_rxPayload[m_payloadCounter], ( m_payloadSize - m_payloadCounter ) );
                    m_payloadCounter += ( m_payloadSize - m_payloadCounter ) ;
                    *dataRecevd = ( m_payloadSize - m_payloadCounter );
                }
            }
            else
            {
                *dataRecevd = 0u;
            }
            m_read_jumpLong = 0u;
        }
        else
        {
            m_read_jumpLong++;
            *dataRecevd = 0u;
        }
    }

    return result;
}

bool_t receiveMsgErr( t_eFSP_MSGRX_RxCtx* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
{
    bool_t result;
    t_eFSP_MSGRX_RxCtx* ctxCur;

    (void)timeToRx;
    (void)dataRxMax;
    if( ( NULL == cntx ) || ( NULL == dataToRx ) || ( NULL == dataRecevd ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGRX_RxCtx*)cntx;
        ctxCur->sendIsError = true;
        result = false;
    }

    return result;
}

static uint32_t m_tim_remainingTime;
static uint32_t m_tim_timCnt;
static uint32_t m_tim_timCntInc;

bool_t timStart ( t_eFSP_MSGRX_TimCtx* cntx, const uint32_t timeoutVal )
{
    bool_t result;
    t_eFSP_MSGRX_TimCtx* ctxCur;

    if( NULL == cntx )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGRX_TimCtx*)cntx;
        ctxCur->sendIsError = true;
        m_tim_timCnt = 0u;
        m_tim_timCntInc = 0u;
        m_tim_remainingTime = timeoutVal;
        result = true;
    }

    return result;
}

bool_t timGetRemaining ( t_eFSP_MSGRX_TimCtx* cntx, uint32_t* const remainings )
{
    bool_t result;
    t_eFSP_MSGRX_TimCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == remainings ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGRX_TimCtx*)cntx;
        ctxCur->sendIsError = true;

        if( m_tim_timCnt < MAX_UINT32VAL )
        {
            m_tim_timCnt++;
        }

        if( m_tim_remainingTime > 0u )
        {
            m_tim_remainingTime--;
        }

        *remainings = m_tim_remainingTime;
        result = true;
    }

    return result;
}

bool_t timGetRemainingCorr ( t_eFSP_MSGRX_TimCtx* cntx, uint32_t* const remainings )
{
    bool_t result;
    t_eFSP_MSGRX_TimCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == remainings ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGRX_TimCtx*)cntx;
        ctxCur->sendIsError = true;
        if( m_tim_timCnt < MAX_UINT32VAL )
        {
            m_tim_timCnt++;
        }
        *remainings = m_tim_remainingTime + m_tim_timCntInc;

        if( m_tim_timCntInc < MAX_UINT32VAL )
        {
            m_tim_timCntInc++;
        }
        result = true;
    }

    return result;
}

bool_t timStartErr ( t_eFSP_MSGRX_TimCtx* cntx, const uint32_t timeoutVal )
{
    bool_t result;
    t_eFSP_MSGRX_TimCtx* ctxCur;

    if( NULL == cntx )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGRX_TimCtx*)cntx;
        ctxCur->sendIsError = true;
        m_tim_timCnt = 0u;

        m_tim_remainingTime = timeoutVal;
        result = false;
    }

    return result;
}

bool_t timGetRemainingErr ( t_eFSP_MSGRX_TimCtx* cntx, uint32_t* const remainings )
{
    bool_t result;
    t_eFSP_MSGRX_TimCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == remainings) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGRX_TimCtx*)cntx;
        ctxCur->sendIsError = true;
        if( m_tim_timCnt < MAX_UINT32VAL )
        {
            m_tim_timCnt++;
        }
        if( m_tim_remainingTime > 0u )
        {
            m_tim_remainingTime--;
        }

        *remainings = m_tim_remainingTime;
        result = false;
    }

    return result;
}

bool_t timGetRemainingErrCntrl ( t_eFSP_MSGRX_TimCtx* cntx, uint32_t* const remainings )
{
    bool_t result;
    t_eFSP_MSGRX_TimCtx* ctxCur;

    if( ( NULL == cntx ) || ( NULL == remainings) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGRX_TimCtx*)cntx;
        ctxCur->sendIsError = true;
        if( m_tim_timCnt < MAX_UINT32VAL )
        {
            m_tim_timCnt++;
        }
        if( m_tim_remainingTime > 0u )
        {
            m_tim_remainingTime--;
        }

        *remainings = m_tim_remainingTime;

        if( m_tim_timCnt < 2u )
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

/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSP_TEST_msgReceiverBadPointer(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;
    bool_t isInit;

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(NULL, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&ctx, NULL) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 2  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = NULL;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 3  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = NULL;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 4  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = NULL;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 5  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = NULL;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 6  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = NULL;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 7  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = NULL;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 8  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = NULL;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 9  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = NULL;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 10 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = NULL;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_IsInit(NULL, &isInit) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_IsInit(&ctx, NULL) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_NewMsg(NULL) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_NewMsgNClean(NULL) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_GetDecodedData(NULL, &dataP, &dataL) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_GetDecodedData(&ctx, NULL, &dataL) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, NULL) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_ReceiveChunk(NULL) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadPointer 19 -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverBadInit(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;
    bool_t isInit;

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadInit 1  -- FAIL \n");
    }

    ctx.tMsgdCtx.tBUNSTFCtx.bIsInit = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_IsInit(&ctx, &isInit) )
    {
        if( false == isInit )
        {
            (void)printf("eFSP_TEST_msgReceiverBadInit 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgReceiverBadInit 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadInit 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_NOINITLIB == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadInit 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_NOINITLIB == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadInit 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_NOINITLIB == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadInit 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_NOINITLIB == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadInit 6  -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverBadParamEntr(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];
    bool_t isInit;

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = 0u;
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPARAM == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 1  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 0u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPARAM == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 2  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 0u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPARAM == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 3  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 100u;
    initData.uITimePerRecMs = 1000u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPARAM == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 4  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = 1u;
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPARAM == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 5  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_IsInit(&ctx, &isInit) )
    {
        if( true == isInit )
        {
            (void)printf("eFSP_TEST_msgReceiverBadParamEntr 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgReceiverBadParamEntr 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadParamEntr 7  -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverCorruptContext(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 1  -- FAIL \n");
    }

    /* Function */
    ctx.puRxBuff = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 2  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 3  -- FAIL \n");
    }

    /* Function */
    ctx.fRx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 4  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 5  -- FAIL \n");
    }

    /* Function */
    ctx.ptRxCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 6  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 7  -- FAIL \n");
    }

    /* Function */
    ctx.tRxTim.ptTimCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 8  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 9  -- FAIL \n");
    }

    /* Function */
    ctx.tRxTim.fTimStart = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 10 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 11 -- FAIL \n");
    }

    /* Function */
    ctx.tRxTim.fTimGetRemain = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 12 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 13 -- FAIL \n");
    }

    /* Function */
    ctx.uRxBuffL = 0u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 14 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 15 -- FAIL \n");
    }

    /* Function */
    ctx.uRxBuffFill = ctx.uRxBuffL + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 16 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 17 -- FAIL \n");
    }

    /* Function */
    ctx.uRxBuffCntr = ctx.uRxBuffFill + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 18 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 19 -- FAIL \n");
    }

    /* Function */
    ctx.uTimeoutMs = 0u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 20 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 21 -- FAIL \n");
    }

    /* Function */
    ctx.uTimePerRecMs = 0u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 22 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 23 -- FAIL \n");
    }

    /* Function */
    ctx.uTimePerRecMs = ctx.uTimeoutMs + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 24 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 25 -- FAIL \n");
    }

    /* Function */
    ctx.uTimePerRecMs = ctx.uTimeoutMs + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 26 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 27 -- FAIL \n");
    }

    /* Function */
    ctx.uTimePerRecMs = ctx.uTimeoutMs + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 28 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 29 -- FAIL \n");
    }

    /* Function */
    ctx.uTimePerRecMs = ctx.uTimeoutMs + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 30 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 31 -- FAIL \n");
    }

    /* Function */
    ctx.tMsgdCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 32 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 33 -- FAIL \n");
    }

    /* Function */
    ctx.tMsgdCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 34 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 35 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 35 -- FAIL \n");
    }

    /* Function */
    ctx.tMsgdCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL ) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 36 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 36 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 37 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 37 -- FAIL \n");
    }

    /* Function */
    ctx.tMsgdCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 38 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCorruptContext 38 -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverBadClBckCrc(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdaptEr;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckCrc 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckCrc 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckCrc 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckCrc 2  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00u;
    m_rxPayload[2u] = 0x00u;
    m_rxPayload[3u] = 0x00u;
    m_rxPayload[4u] = 0x00u;
    m_rxPayload[5u] = 0x02u;
    m_rxPayload[6u] = 0x00u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0xCCu;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_CRCCLBKERROR == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckCrc 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckCrc 3  -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverBadClBckReceive(void)
{
    /* Static var init */
    m_read_jump = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsgErr;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckReceive 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckReceive 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckReceive 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckReceive 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckReceive 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckReceive 3  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00u;
    m_rxPayload[2u] = 0x00u;
    m_rxPayload[3u] = 0x00u;
    m_rxPayload[4u] = 0x00u;
    m_rxPayload[5u] = 0x02u;
    m_rxPayload[6u] = 0x00u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0xCCu;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_RXCLBKERROR == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckReceive 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckReceive 4  -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverBadClBckTim(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStartErr;
    initData.tIRxTim.fTimGetRemain = &timGetRemainingErr;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 3  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00u;
    m_rxPayload[2u] = 0x00u;
    m_rxPayload[3u] = 0x00u;
    m_rxPayload[4u] = 0x00u;
    m_rxPayload[5u] = 0x02u;
    m_rxPayload[6u] = 0x00u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0xCCu;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 4  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStartErr;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 7  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00u;
    m_rxPayload[2u] = 0x00u;
    m_rxPayload[3u] = 0x00u;
    m_rxPayload[4u] = 0x00u;
    m_rxPayload[5u] = 0x02u;
    m_rxPayload[6u] = 0x00u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0xCCu;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 8  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemainingErrCntrl;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 9  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 10 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00u;
    m_rxPayload[2u] = 0x00u;
    m_rxPayload[3u] = 0x00u;
    m_rxPayload[4u] = 0x00u;
    m_rxPayload[5u] = 0x02u;
    m_rxPayload[6u] = 0x00u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0xCCu;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadClBckTim 11 -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverBadFrame(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 2  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00u;
    m_rxPayload[2u] = 0x00u;
    m_rxPayload[3u] = 0x00u;
    m_rxPayload[4u] = 0x00u;
    m_rxPayload[5u] = 0x02u;
    m_rxPayload[6u] = 0x00u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0xCCu;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 3  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 5  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xCCu;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 6  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 8  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x00u;
    m_rxPayload[2u] = 0x00u;
    m_rxPayload[3u] = 0x00u;
    m_rxPayload[4u] = 0x00u;
    m_rxPayload[5u] = 0x03u;
    m_rxPayload[6u] = 0x00u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0xCCu;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xCCu;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_OUTOFMEM == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 9  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 10 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 11 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x50u;
    m_rxPayload[2u] = 0x7Bu;
    m_rxPayload[3u] = 0x0Au;
    m_rxPayload[4u] = 0x68u;
    m_rxPayload[5u] = 0x01u;
    m_rxPayload[6u] = 0x00u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0xCCu;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = ECU_EOF;

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 12 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 14 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = 0x50u;
    m_rxPayload[2u] = 0x7Bu;
    m_rxPayload[3u] = 0x0Au;
    m_rxPayload[4u] = 0x68u;
    m_rxPayload[5u] = 0x01u;
    m_rxPayload[6u] = 0x00u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = ECU_EOF;

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 15 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 17 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xCCu;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverBadFrame 18 -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverCornerCase(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 1u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 2  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xCCu;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 3  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 5  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 6  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 5u;
    initData.uITimePerRecMs = 1u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 8  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xCCu;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 9  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 10 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 11 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 12 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 6u;
    initData.uITimePerRecMs = 1u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 14 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xCCu;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 15 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 16 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 17 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 18 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsgJump;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 20 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xCCu;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 21 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 22 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 23 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 100u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 24 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 25 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = ECU_ESC;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 26 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 27 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase 28 -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverCornerCase2(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;


    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsgJump;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 9u;
    initData.uITimePerRecMs = 9u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 2  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xCCu;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 3  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 5  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsgJump;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 10u;
    initData.uITimePerRecMs = 10u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 7  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xCCu;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 8  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 9  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 10 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsgJump;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 10u;
    initData.uITimePerRecMs = 10u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 12 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] =  0x83u;
    m_rxPayload[3u] =  0xFDu;
    m_rxPayload[4u] =  0xC7u;
    m_rxPayload[5u] =  0x59u;
    m_rxPayload[6u] =  0x02u;
    m_rxPayload[7u] =  0x00u;
    m_rxPayload[8u] =  0x00u;
    m_rxPayload[9u] =  0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xC1u;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 13 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 14 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 15 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 16 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            if( (  0xCCu == dataP[0u] ) || (  0xC1u == dataP[1u] ) )
            {
                (void)printf("eFSP_TEST_msgReceiverCornerCase2 17 -- OK \n");
            }
            else
            {
                (void)printf("eFSP_TEST_msgReceiverCornerCase2 17 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase2 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 17 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsgCrrupt;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 10u;
    initData.uITimePerRecMs = 10u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 19 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xCCu;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 20 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemainingCorr;
    initData.uITimeoutMs = 10u;
    initData.uITimePerRecMs = 10u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 22 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 0u == dataL )
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase2 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase2 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 23 -- FAIL \n");
    }


    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = 0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xCCu;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase2 24 -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverCornerCase3(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[10u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsgJump;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 1000u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 2  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 21u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_EOF;
    m_rxPayload[1u] = 0xFFu;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = ECU_SOF;
    m_rxPayload[10u] = 0x83u;
    m_rxPayload[11u] = 0xFDu;
    m_rxPayload[12u] = 0xC7u;
    m_rxPayload[13u] = 0x59u;
    m_rxPayload[14u] = 0x02u;
    m_rxPayload[15u] = 0x00u;
    m_rxPayload[16u] = 0x00u;
    m_rxPayload[17u] = 0x00u;
    m_rxPayload[18u] = 0xCCu;
    m_rxPayload[19u] = 0xC1u;
    m_rxPayload[20u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 3  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 5  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 7  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 8  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 9  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 10 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 11 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 12 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 13 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 14 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 15 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 16 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 17 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 18 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 19 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 0u == dataL )
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase3 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase3 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 20 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 21 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 0u == dataL )
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase3 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase3 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 22 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 23 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            if( (  0xCCu == dataP[0u] ) || (  0xC1u == dataP[1u] ) )
            {
                (void)printf("eFSP_TEST_msgReceiverCornerCase3 24 -- OK \n");
            }
            else
            {
                (void)printf("eFSP_TEST_msgReceiverCornerCase3 24 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase3 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 24 -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsgJump;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 1000u;
    initData.uITimePerRecMs = 1000u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 25 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 26 -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 21u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_EOF;
    m_rxPayload[1u] = 0xFFu;
    m_rxPayload[2u] = 0x50u;
    m_rxPayload[3u] = 0x7Bu;
    m_rxPayload[4u] = 0x0Au;
    m_rxPayload[5u] = 0x68u;
    m_rxPayload[6u] = 0x02u;
    m_rxPayload[7u] = 0x00u;
    m_rxPayload[8u] = 0x00u;
    m_rxPayload[9u] = ECU_SOF;
    m_rxPayload[10u] = 0x83u;
    m_rxPayload[11u] = 0xFDu;
    m_rxPayload[12u] = 0xC7u;
    m_rxPayload[13u] = 0x59u;
    m_rxPayload[14u] = 0x02u;
    m_rxPayload[15u] = 0x00u;
    m_rxPayload[16u] = 0x00u;
    m_rxPayload[17u] = 0x00u;
    m_rxPayload[18u] = 0xCCu;
    m_rxPayload[19u] = 0xC1u;
    m_rxPayload[20u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 24 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 0u == dataL )
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase3 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase3 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 25 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 26 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 0u == dataL )
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase3 27 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase3 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 27 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 28 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            if( (  0xCCu == dataP[0u] ) || (  0xC1u == dataP[1u] ) )
            {
                (void)printf("eFSP_TEST_msgReceiverCornerCase3 29 -- OK \n");
            }
            else
            {
                (void)printf("eFSP_TEST_msgReceiverCornerCase3 29 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase3 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase3 29 -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverCornerCase4(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[10u];
    uint8_t  recBuff[5u];

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsgJump;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 8u;
    initData.uITimePerRecMs = 8u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 2  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] =  0x83u;
    m_rxPayload[3u] =  0xFDu;
    m_rxPayload[4u] =  0xC7u;
    m_rxPayload[5u] =  0x59u;
    m_rxPayload[6u] =  0x02u;
    m_rxPayload[7u] =  0x00u;
    m_rxPayload[8u] =  0x00u;
    m_rxPayload[9u] =  0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xC1u;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 3  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 5  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 7  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = 1u;
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 25u;
    initData.uITimePerRecMs = 1u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 9  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 13u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] = ECU_SOF;
    m_rxPayload[2u] =  0x83u;
    m_rxPayload[3u] =  0xFDu;
    m_rxPayload[4u] =  0xC7u;
    m_rxPayload[5u] =  0x59u;
    m_rxPayload[6u] =  0x02u;
    m_rxPayload[7u] =  0x00u;
    m_rxPayload[8u] =  0x00u;
    m_rxPayload[9u] =  0x00u;
    m_rxPayload[10u] = 0xCCu;
    m_rxPayload[11u] = 0xC1u;
    m_rxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 10 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 11 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 12 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 13 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 14 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 15 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 16 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 17 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 18 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 19 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 20 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 21 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase4 22 -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverCornerCase5(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[20u];
    uint8_t  recBuff[20u];
    uint8_t* dataP;
    uint32_t dataL;

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsgJumpLong;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 8u;
    initData.uITimePerRecMs = 8u;
    initData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 2  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] =  0x83u;
    m_rxPayload[2u] =  0xFDu;
    m_rxPayload[3u] =  0xC7u;
    m_rxPayload[4u] =  0x59u;
    m_rxPayload[5u] =  0x02u;
    m_rxPayload[6u] =  0x00u;
    m_rxPayload[7u] =  0x00u;
    m_rxPayload[8u] =  0x00u;
    m_rxPayload[9u]  = 0xCCu;
    m_rxPayload[10u] = 0xC1u;
    m_rxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 3  -- FAIL \n");
    }

    m_read_jumpLong = 0u;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 0u == dataL )
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase5 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgReceiverCornerCase5 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 5  -- FAIL \n");
    }

    ctx.fRx = &receiveMsg;
    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 6  -- FAIL \n");
    }

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsgJumpLong;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 8u;
    initData.uITimePerRecMs = 8u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 8  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = 12u;
    m_payloadCounter = 0u;
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] =  0x83u;
    m_rxPayload[2u] =  0xFDu;
    m_rxPayload[3u] =  0xC7u;
    m_rxPayload[4u] =  0x59u;
    m_rxPayload[5u] =  0x02u;
    m_rxPayload[6u] =  0x00u;
    m_rxPayload[7u] =  0x00u;
    m_rxPayload[8u] =  0x00u;
    m_rxPayload[9u]  = 0xCCu;
    m_rxPayload[10u] = 0xC1u;
    m_rxPayload[11u] = ECU_EOF;
    m_read_jumpLong = 0u;
    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase5 9  -- FAIL \n");
    }
}

void eFSP_TEST_msgReceiverCornerCase6(void)
{
    /* Static var init */
    m_read_jump = 0u;
	m_read_jumpLong = 0u;
    uint32_t temp;

    /* Local variable */
    t_eFSP_MSGRX_Ctx ctx;
    t_eFSP_MSGRX_InitData initData;
    f_eFSP_MSGD_CrcCb cbCrcPTest = &c32SAdapt;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrc;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memArea[100u];
    uint8_t  recBuff[100u];

    /* Function */
    initData.puIMemArea = memArea;
    initData.uIMemAreaL = sizeof(memArea);
    initData.puIRxBuffArea = recBuff;
    initData.uIRxBuffAreaL = sizeof(recBuff);
    initData.fICrc = cbCrcPTest;
    initData.ptICbCrcCtx = &ctxAdapterCrc;
    initData.fIRx = &receiveMsg;
    initData.ptICbRxCtx = &ctxAdapterRx;
    initData.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initData.tIRxTim.fTimStart = &timStart;
    initData.tIRxTim.fTimGetRemain = &timGetRemaining;
    initData.uITimeoutMs = 2000u;
    initData.uITimePerRecMs = 2000u;
    initData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase6 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase6 1  -- FAIL \n");
    }


    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase6 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase6 2  -- FAIL \n");
    }

    /* Fucntion */
    m_payloadSize = sizeof(m_rxPayload);
    m_payloadCounter = 0u;
    for( temp = 0u; temp < sizeof(m_rxPayload) ; temp++ )
    {
        m_rxPayload[temp] = ECU_SOF;
    }
    m_rxPayload[0u] = ECU_SOF;
    m_rxPayload[1u] =  0x83u;
    m_rxPayload[2u] =  0xFDu;
    m_rxPayload[3u] =  0xC7u;
    m_rxPayload[4u] =  0x59u;
    m_rxPayload[5u] =  0x50u;
    m_rxPayload[6u] =  0x00u;
    m_rxPayload[7u] =  0x00u;
    m_rxPayload[8u] =  0x00u;
    m_rxPayload[9u] =  ECU_SOF;
    m_rxPayload[10u] = 0x83u;
    m_rxPayload[11u] = 0xFDu;
    m_rxPayload[12u] = 0xC7u;
    m_rxPayload[13u] = 0x59u;
    m_rxPayload[14u] = 0x02u;
    m_rxPayload[15u] = 0x00u;
    m_rxPayload[16u] = 0x00u;
    m_rxPayload[17u] = 0x00u;
    m_rxPayload[18u] = 0xCCu;
    m_rxPayload[19u] = 0xC1u;
    m_rxPayload[20u] = ECU_EOF;

    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase6 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase6 3  -- FAIL \n");
    }

    if( ( 81u == ctx.uRxBuffFill) || ( 1u == ctx.uRxBuffCntr ) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase6 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase6 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase6 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase6 5  -- FAIL \n");
    }

    if( ( 81u == ctx.uRxBuffFill) || ( 12u == ctx.uRxBuffCntr ) )
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase6 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgReceiverCornerCase6 6  -- FAIL \n");
    }
}


#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "MISRAC2012-Rule-8.9_a", "MISRAC2012-Rule-8.13", "MISRAC2012-Rule-2.2_b", "MISRAC2004-16.7"
#endif