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
static void eFSP_MSGRXTST_BadPointer(void);
static void eFSP_MSGRXTST_BadInit(void);
static void eFSP_MSGRXTST_BadParamEntr(void);
static void eFSP_MSGRXTST_CorruptContext(void);
static void eFSP_MSGRXTST_BadClBckCrc(void);
static void eFSP_MSGRXTST_BadClBckReceive(void);
static void eFSP_MSGRXTST_BadClBckTim(void);
static void eFSP_MSGRXTST_BadFrame(void);
static void eFSP_MSGRXTST_CornerCase(void);
static void eFSP_MSGRXTST_CornerCase2(void);
static void eFSP_MSGRXTST_CornerCase3(void);
static void eFSP_MSGRXTST_CornerCase4(void);
static void eFSP_MSGRXTST_CornerCase5(void);
static void eFSP_MSGRXTST_CornerCase6(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_MSGRXTST_ExeTest(void)
{
	(void)printf("\n\nMESSAGE RECEIVER TEST START \n\n");

    eFSP_MSGRXTST_BadPointer();
    eFSP_MSGRXTST_BadInit();
    eFSP_MSGRXTST_BadParamEntr();
    eFSP_MSGRXTST_CorruptContext();
    eFSP_MSGRXTST_BadClBckCrc();
    eFSP_MSGRXTST_BadClBckReceive();
    eFSP_MSGRXTST_BadClBckTim();
    eFSP_MSGRXTST_BadFrame();
    eFSP_MSGRXTST_CornerCase();
    eFSP_MSGRXTST_CornerCase2();
    eFSP_MSGRXTST_CornerCase3();
    eFSP_MSGRXTST_CornerCase4();
    eFSP_MSGRXTST_CornerCase5();
    eFSP_MSGRXTST_CornerCase6();

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
void eFSP_MSGRXTST_BadPointer(void)
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
        (void)printf("eFSP_MSGRXTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&ctx, NULL) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 2  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 3  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 4  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 5  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 6  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 7  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 8  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 9  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 10 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_IsInit(NULL, &isInit) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_IsInit(&ctx, NULL) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_NewMsg(NULL) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 14 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_NewMsgNClean(NULL) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 15 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_GetDecodedData(NULL, &dataP, &dataL) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_GetDecodedData(&ctx, NULL, &dataL) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, NULL) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_ReceiveChunk(NULL) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 19 -- FAIL \n");
    }
}

void eFSP_MSGRXTST_BadInit(void)
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
        (void)printf("eFSP_MSGRXTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadInit 1  -- FAIL \n");
    }

    ctx.tMsgdCtx.tBUNSTFCtx.bIsInit = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_IsInit(&ctx, &isInit) )
    {
        if( false == isInit )
        {
            (void)printf("eFSP_MSGRXTST_BadInit 2  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGRXTST_BadInit 2  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadInit 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_NOINITLIB == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadInit 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_NOINITLIB == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadInit 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_NOINITLIB == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        (void)printf("eFSP_MSGRXTST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadInit 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_NOINITLIB == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadInit 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadInit 6  -- FAIL \n");
    }
}

void eFSP_MSGRXTST_BadParamEntr(void)
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
        (void)printf("eFSP_MSGRXTST_BadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 1  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 2  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 3  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 4  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 5  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_IsInit(&ctx, &isInit) )
    {
        if( true == isInit )
        {
            (void)printf("eFSP_MSGRXTST_BadParamEntr 7  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGRXTST_BadParamEntr 7  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 7  -- FAIL \n");
    }
}

void eFSP_MSGRXTST_CorruptContext(void)
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 1  -- FAIL \n");
    }

    /* Function */
    ctx.puRxBuff = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 2  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 3  -- FAIL \n");
    }

    /* Function */
    ctx.fRx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 4  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 5  -- FAIL \n");
    }

    /* Function */
    ctx.ptRxCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 6  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 7  -- FAIL \n");
    }

    /* Function */
    ctx.tRxTim.ptTimCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 8  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 9  -- FAIL \n");
    }

    /* Function */
    ctx.tRxTim.fTimStart = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 10 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 11 -- FAIL \n");
    }

    /* Function */
    ctx.tRxTim.fTimGetRemain = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 12 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 13 -- FAIL \n");
    }

    /* Function */
    ctx.uRxBuffL = 0u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 14 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 15 -- FAIL \n");
    }

    /* Function */
    ctx.uRxBuffFill = ctx.uRxBuffL + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 16 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 17 -- FAIL \n");
    }

    /* Function */
    ctx.uRxBuffCntr = ctx.uRxBuffFill + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 18 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 19 -- FAIL \n");
    }

    /* Function */
    ctx.uTimeoutMs = 0u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 20 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 21 -- FAIL \n");
    }

    /* Function */
    ctx.uTimePerRecMs = 0u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 22 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 23 -- FAIL \n");
    }

    /* Function */
    ctx.uTimePerRecMs = ctx.uTimeoutMs + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 24 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 25 -- FAIL \n");
    }

    /* Function */
    ctx.uTimePerRecMs = ctx.uTimeoutMs + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 26 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 27 -- FAIL \n");
    }

    /* Function */
    ctx.uTimePerRecMs = ctx.uTimeoutMs + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 28 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 29 -- FAIL \n");
    }

    /* Function */
    ctx.uTimePerRecMs = ctx.uTimeoutMs + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 30 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 31 -- FAIL \n");
    }

    /* Function */
    ctx.tMsgdCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 32 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 33 -- FAIL \n");
    }

    /* Function */
    ctx.tMsgdCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 34 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 35 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 35 -- FAIL \n");
    }

    /* Function */
    ctx.tMsgdCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL ) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 36 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 36 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CorruptContext 37 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 37 -- FAIL \n");
    }

    /* Function */
    ctx.tMsgdCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 38 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 38 -- FAIL \n");
    }
}

void eFSP_MSGRXTST_BadClBckCrc(void)
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
        (void)printf("eFSP_MSGRXTST_BadClBckCrc 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckCrc 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckCrc 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckCrc 2  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadClBckCrc 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckCrc 3  -- FAIL \n");
    }
}

void eFSP_MSGRXTST_BadClBckReceive(void)
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
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 3  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 4  -- FAIL \n");
    }
}

void eFSP_MSGRXTST_BadClBckTim(void)
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
        (void)printf("eFSP_MSGRXTST_BadClBckTim 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 3  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadClBckTim 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 4  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadClBckTim 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 7  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadClBckTim 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 8  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadClBckTim 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 9  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 10 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadClBckTim 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 11 -- FAIL \n");
    }
}

void eFSP_MSGRXTST_BadFrame(void)
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
        (void)printf("eFSP_MSGRXTST_BadFrame 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 2  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadFrame 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 3  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadFrame 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 5  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadFrame 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 6  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadFrame 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 8  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadFrame 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 9  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadFrame 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 10 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 11 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadFrame 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 12 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctx, &initData) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 14 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadFrame 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 15 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadFrame 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 17 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_BadFrame 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 18 -- FAIL \n");
    }
}

void eFSP_MSGRXTST_CornerCase(void)
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
        (void)printf("eFSP_MSGRXTST_CornerCase 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 2  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 3  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 5  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 6  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 8  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 9  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 10 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 11 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 12 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 14 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 15 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 16 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 17 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 18 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 20 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 21 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 22 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 23 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 24 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 25 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 26 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 27 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 28 -- FAIL \n");
    }
}

void eFSP_MSGRXTST_CornerCase2(void)
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
        (void)printf("eFSP_MSGRXTST_CornerCase2 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 2  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase2 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 3  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 5  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase2 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 7  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase2 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 8  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 9  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 10 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase2 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 12 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase2 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 13 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 14 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 15 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 16 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            if( (  0xCCu == dataP[0u] ) || (  0xC1u == dataP[1u] ) )
            {
                (void)printf("eFSP_MSGRXTST_CornerCase2 17 -- OK \n");
            }
            else
            {
                (void)printf("eFSP_MSGRXTST_CornerCase2 17 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_MSGRXTST_CornerCase2 17 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 17 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase2 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 19 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase2 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 20 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase2 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 22 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 0u == dataL )
        {
            (void)printf("eFSP_MSGRXTST_CornerCase2 23 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGRXTST_CornerCase2 23 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 23 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase2 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 24 -- FAIL \n");
    }
}

void eFSP_MSGRXTST_CornerCase3(void)
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
        (void)printf("eFSP_MSGRXTST_CornerCase3 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 2  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase3 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 3  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 5  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 7  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 8  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 9  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 10 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 11 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 12 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 13 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 14 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 15 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 16 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 17 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 18 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 19 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 0u == dataL )
        {
            (void)printf("eFSP_MSGRXTST_CornerCase3 20 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGRXTST_CornerCase3 20 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 20 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 21 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 0u == dataL )
        {
            (void)printf("eFSP_MSGRXTST_CornerCase3 22 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGRXTST_CornerCase3 22 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 22 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 23 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            if( (  0xCCu == dataP[0u] ) || (  0xC1u == dataP[1u] ) )
            {
                (void)printf("eFSP_MSGRXTST_CornerCase3 24 -- OK \n");
            }
            else
            {
                (void)printf("eFSP_MSGRXTST_CornerCase3 24 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_MSGRXTST_CornerCase3 24 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 24 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase3 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 25 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 26 -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase3 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 24 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 0u == dataL )
        {
            (void)printf("eFSP_MSGRXTST_CornerCase3 25 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGRXTST_CornerCase3 25 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 25 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsgNClean(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 26 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 0u == dataL )
        {
            (void)printf("eFSP_MSGRXTST_CornerCase3 27 -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGRXTST_CornerCase3 27 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 27 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 28 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 2u == dataL )
        {
            if( (  0xCCu == dataP[0u] ) || (  0xC1u == dataP[1u] ) )
            {
                (void)printf("eFSP_MSGRXTST_CornerCase3 29 -- OK \n");
            }
            else
            {
                (void)printf("eFSP_MSGRXTST_CornerCase3 29 -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_MSGRXTST_CornerCase3 29 -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 29 -- FAIL \n");
    }
}

void eFSP_MSGRXTST_CornerCase4(void)
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
        (void)printf("eFSP_MSGRXTST_CornerCase4 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 2  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase4 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 3  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 5  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 7  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase4 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 9  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase4 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 10 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 11 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 12 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 13 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 14 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 15 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 16 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 17 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 18 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 19 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 20 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 21 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 22 -- FAIL \n");
    }
}

void eFSP_MSGRXTST_CornerCase5(void)
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
        (void)printf("eFSP_MSGRXTST_CornerCase5 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 2  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase5 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 3  -- FAIL \n");
    }

    m_read_jumpLong = 0u;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctx, &dataP, &dataL) )
    {
        if( 0u == dataL )
        {
            (void)printf("eFSP_MSGRXTST_CornerCase5 5  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGRXTST_CornerCase5 5  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 5  -- FAIL \n");
    }

    ctx.fRx = &receiveMsg;
    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 6  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase5 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 8  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase5 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 9  -- FAIL \n");
    }
}

void eFSP_MSGRXTST_CornerCase6(void)
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
        (void)printf("eFSP_MSGRXTST_CornerCase6 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 1  -- FAIL \n");
    }


    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 2  -- FAIL \n");
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
        (void)printf("eFSP_MSGRXTST_CornerCase6 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 3  -- FAIL \n");
    }

    if( ( 81u == ctx.uRxBuffFill) || ( 1u == ctx.uRxBuffCntr ) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 5  -- FAIL \n");
    }

    if( ( 81u == ctx.uRxBuffFill) || ( 12u == ctx.uRxBuffCntr ) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 6  -- FAIL \n");
    }
}


#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "MISRAC2012-Rule-8.9_a", "MISRAC2012-Rule-8.13", "MISRAC2012-Rule-2.2_b", "MISRAC2004-16.7"
#endif