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
#include <stdio.h>

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3", "MISRAC2012-Rule-11.5",  \
                            "MISRAC2012-Rule-8.9_a", "MISRAC2012-Rule-8.13", "MISRAC2012-Rule-2.2_b"
    /* Suppressed for code clarity in test execution*/
#endif

/***********************************************************************************************************************
 *   PRIVATE TEST VARIABLE DECLARATION
 **********************************************************************************************************************/
static uint8_t  m_auRxPayload[120u];
static uint32_t m_uPayloadSize;
static uint32_t m_uPayloadCounter;
static uint32_t m_uReadJump;
static uint32_t m_uReadJumpLong;
static uint32_t m_uTimRemainingTime;
static uint32_t m_uTimTimCnt;
static uint32_t m_uTimTimCntInc;



/***********************************************************************************************************************
 *   PRIVATE TEST STRUCT DECLARATION
 **********************************************************************************************************************/
struct t_eFSP_MSGD_CrcCtxUser
{
    e_eCU_CRC_RES eLastEr;
};

struct t_eFSP_MSGRX_RxCtxUser
{
    bool bTxIsError;
};

struct t_eFSP_MSGRX_TimCtxUser
{
    bool bTxIsError;
};



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSP_MSGRXTST_c32SAdapt(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val);
static bool_t eFSP_MSGRXTST_c32SAdaptEr(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val);
static bool_t eFSP_MSGRXTST_receiveMsg( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                       const uint32_t p_uTimeToTx );
static bool_t eFSP_MSGRXTST_receiveMsgCrrupt( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                       const uint32_t p_uTimeToTx );
static bool_t eFSP_MSGRXTST_receiveMsgJump( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                       const uint32_t p_uTimeToTx );
static bool_t eFSP_MSGRXTST_receiveMsgJumpLong( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                       const uint32_t p_uTimeToTx );
static bool_t eFSP_MSGRXTST_receiveMsgErr( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                          const uint32_t p_uTimeToTx );
static bool_t eFSP_MSGRXTST_timStart ( t_eFSP_MSGRX_TimCtx* p_ptCtx, const uint32_t p_uTimeoutVal );
static bool_t eFSP_MSGRXTST_timGetRemaining ( t_eFSP_MSGRX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings );
static bool_t eFSP_MSGRXTST_timGetRemainingCorr ( t_eFSP_MSGRX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings );
static bool_t eFSP_MSGRXTST_timStartErr ( t_eFSP_MSGRX_TimCtx* p_ptCtx, const uint32_t p_uTimeoutVal );
static bool_t eFSP_MSGRXTST_timGetRemainingErr ( t_eFSP_MSGRX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings );
static bool_t eFSP_MSGRXTST_timGetRemainingErrCntrl( t_eFSP_MSGRX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings );



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
bool_t eFSP_MSGRXTST_c32SAdapt(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t l_bResult;
    t_eFSP_MSGD_CrcCtx* l_ptCtxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        l_bResult = false;
    }
    else
    {
        l_ptCtxCur = (t_eFSP_MSGD_CrcCtx*)p_ptCtx;

        l_ptCtxCur->eLastEr = eCU_CRC_32Seed(p_uS, (const uint8_t*)p_puD, p_uDLen, p_puC32Val);
        if( e_eCU_CRC_RES_OK == l_ptCtxCur->eLastEr )
        {
            l_bResult = true;
        }
        else
        {
            l_bResult = false;
        }
    }

    return l_bResult;
}

bool_t eFSP_MSGRXTST_c32SAdaptEr(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t l_bResult;
    t_eFSP_MSGD_CrcCtx* l_ptCtxCur;

    (void)p_uS;
    (void)p_puD;
    (void)p_uDLen;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        l_bResult = false;
    }
    else
    {
        l_ptCtxCur = (t_eFSP_MSGD_CrcCtx*)p_ptCtx;

        l_ptCtxCur->eLastEr = e_eCU_CRC_RES_BADPOINTER;
        l_bResult = false;
        *p_puC32Val = 0u;
    }

    return l_bResult;
}



bool_t eFSP_MSGRXTST_receiveMsg( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                const uint32_t p_uTimeToTx )
{
    bool_t l_bResult;
    t_eFSP_MSGRX_RxCtx* l_ptCtxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puDataToRx ) || ( NULL == p_puDataRxedL ) )
    {
        l_bResult = false;
    }
    else
    {
        l_ptCtxCur = (t_eFSP_MSGRX_RxCtx*)p_ptCtx;
        l_ptCtxCur->bTxIsError = true;
        l_bResult = true;


        if( m_uPayloadCounter < m_uPayloadSize )
        {
            if( ( m_uPayloadSize - m_uPayloadCounter ) >= p_uDataRxMax )
            {
                (void)memcpy(p_puDataToRx, &m_auRxPayload[m_uPayloadCounter], p_uDataRxMax);
                m_uPayloadCounter += p_uDataRxMax;
                *p_puDataRxedL = p_uDataRxMax;
            }
            else
            {
                (void)memcpy(p_puDataToRx, &m_auRxPayload[m_uPayloadCounter], ( m_uPayloadSize - m_uPayloadCounter ) );
                m_uPayloadCounter += ( m_uPayloadSize - m_uPayloadCounter ) ;
                *p_puDataRxedL = ( m_uPayloadSize - m_uPayloadCounter );
            }
        }
        else
        {
            *p_puDataRxedL = 0u;
        }
    }

    return l_bResult;
}

bool_t eFSP_MSGRXTST_receiveMsgCrrupt (t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                const uint32_t p_uTimeToTx )
{
    bool_t l_bResult;
    t_eFSP_MSGRX_RxCtx* l_ptCtxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puDataToRx ) || ( NULL == p_puDataRxedL ) )
    {
        l_bResult = false;
    }
    else
    {
        l_ptCtxCur = (t_eFSP_MSGRX_RxCtx*)p_ptCtx;
        l_ptCtxCur->bTxIsError = true;
        l_bResult = true;

        if( m_uPayloadCounter < m_uPayloadSize )
        {
            if( ( m_uPayloadSize - m_uPayloadCounter ) >= p_uDataRxMax )
            {
                (void)memcpy(p_puDataToRx, &m_auRxPayload[m_uPayloadCounter], p_uDataRxMax);
                m_uPayloadCounter += p_uDataRxMax;
                *p_puDataRxedL = p_uDataRxMax +1u;
            }
            else
            {
                (void)memcpy(p_puDataToRx, &m_auRxPayload[m_uPayloadCounter], ( m_uPayloadSize - m_uPayloadCounter ) );
                m_uPayloadCounter += ( m_uPayloadSize - m_uPayloadCounter ) ;
                *p_puDataRxedL = ( m_uPayloadSize - m_uPayloadCounter );
            }
        }
        else
        {
            *p_puDataRxedL = 0u;
        }
    }

    return l_bResult;
}

bool_t eFSP_MSGRXTST_receiveMsgJump( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                const uint32_t p_uTimeToTx )
{
    bool_t l_bResult;
    t_eFSP_MSGRX_RxCtx* l_ptCtxCur;

    l_ptCtxCur = (t_eFSP_MSGRX_RxCtx*)p_ptCtx;
    l_ptCtxCur->bTxIsError = true;
    l_bResult = true;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puDataToRx ) || ( NULL == p_puDataRxedL ) )
    {
        l_bResult = false;
    }
    else
    {
        if( 0u == m_uReadJump )
        {
            if( m_uPayloadCounter < m_uPayloadSize )
            {
                if( ( m_uPayloadSize - m_uPayloadCounter ) >= p_uDataRxMax )
                {
                    (void)memcpy(p_puDataToRx, &m_auRxPayload[m_uPayloadCounter], p_uDataRxMax);
                    m_uPayloadCounter += p_uDataRxMax;
                    *p_puDataRxedL = p_uDataRxMax;
                }
                else
                {
                    (void)memcpy(p_puDataToRx, &m_auRxPayload[m_uPayloadCounter], ( m_uPayloadSize - m_uPayloadCounter ) );
                    m_uPayloadCounter += ( m_uPayloadSize - m_uPayloadCounter ) ;
                    *p_puDataRxedL = ( m_uPayloadSize - m_uPayloadCounter );
                }
            }
            else
            {
                *p_puDataRxedL = 0u;
            }
            m_uReadJump = 1u;
        }
        else
        {
            m_uReadJump = 0u;
            *p_puDataRxedL = 0u;
        }
    }

    return l_bResult;
}

bool_t eFSP_MSGRXTST_receiveMsgJumpLong( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                const uint32_t p_uTimeToTx )
{
    bool_t l_bResult;
    t_eFSP_MSGRX_RxCtx* l_ptCtxCur;

    l_ptCtxCur = (t_eFSP_MSGRX_RxCtx*)p_ptCtx;
    l_ptCtxCur->bTxIsError = true;
    l_bResult = true;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puDataToRx ) || ( NULL == p_puDataRxedL ) )
    {
        l_bResult = false;
    }
    else
    {
        if( m_uReadJumpLong >= 10u )
        {
            if( m_uPayloadCounter < m_uPayloadSize )
            {
                if( ( m_uPayloadSize - m_uPayloadCounter ) >= p_uDataRxMax )
                {
                    (void)memcpy(p_puDataToRx, &m_auRxPayload[m_uPayloadCounter], p_uDataRxMax);
                    m_uPayloadCounter += p_uDataRxMax;
                    *p_puDataRxedL = p_uDataRxMax;
                }
                else
                {
                    (void)memcpy(p_puDataToRx, &m_auRxPayload[m_uPayloadCounter], ( m_uPayloadSize - m_uPayloadCounter ) );
                    m_uPayloadCounter += ( m_uPayloadSize - m_uPayloadCounter ) ;
                    *p_puDataRxedL = ( m_uPayloadSize - m_uPayloadCounter );
                }
            }
            else
            {
                *p_puDataRxedL = 0u;
            }
            m_uReadJumpLong = 0u;
        }
        else
        {
            m_uReadJumpLong++;
            *p_puDataRxedL = 0u;
        }
    }

    return l_bResult;
}

bool_t eFSP_MSGRXTST_receiveMsgErr( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                const uint32_t p_uTimeToTx )
{
    bool_t l_bResult;
    t_eFSP_MSGRX_RxCtx* l_ptCtxCur;

    (void)p_uTimeToTx;
    (void)p_uDataRxMax;
    if( ( NULL == p_ptCtx ) || ( NULL == p_puDataToRx ) || ( NULL == p_puDataRxedL ) )
    {
        l_bResult = false;
    }
    else
    {
        l_ptCtxCur = (t_eFSP_MSGRX_RxCtx*)p_ptCtx;
        l_ptCtxCur->bTxIsError = true;
        l_bResult = false;
    }

    return l_bResult;
}


bool_t eFSP_MSGRXTST_timStart ( t_eFSP_MSGRX_TimCtx* p_ptCtx, const uint32_t p_uTimeoutVal )
{
    bool_t l_bResult;
    t_eFSP_MSGRX_TimCtx* l_ptCtxCur;

    if( NULL == p_ptCtx )
    {
        l_bResult = false;
    }
    else
    {
        l_ptCtxCur = (t_eFSP_MSGRX_TimCtx*)p_ptCtx;
        l_ptCtxCur->bTxIsError = true;
        m_uTimTimCnt = 0u;
        m_uTimTimCntInc = 0u;
        m_uTimRemainingTime = p_uTimeoutVal;
        l_bResult = true;
    }

    return l_bResult;
}

bool_t eFSP_MSGRXTST_timGetRemaining ( t_eFSP_MSGRX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings )
{
    bool_t l_bResult;
    t_eFSP_MSGRX_TimCtx* l_ptCtxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puRemainings ) )
    {
        l_bResult = false;
    }
    else
    {
        l_ptCtxCur = (t_eFSP_MSGRX_TimCtx*)p_ptCtx;
        l_ptCtxCur->bTxIsError = true;

        if( m_uTimTimCnt < MAX_UINT32VAL )
        {
            m_uTimTimCnt++;
        }

        if( m_uTimRemainingTime > 0u )
        {
            m_uTimRemainingTime--;
        }

        *p_puRemainings = m_uTimRemainingTime;
        l_bResult = true;
    }

    return l_bResult;
}

bool_t eFSP_MSGRXTST_timGetRemainingCorr ( t_eFSP_MSGRX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings )
{
    bool_t l_bResult;
    t_eFSP_MSGRX_TimCtx* l_ptCtxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puRemainings ) )
    {
        l_bResult = false;
    }
    else
    {
        l_ptCtxCur = (t_eFSP_MSGRX_TimCtx*)p_ptCtx;
        l_ptCtxCur->bTxIsError = true;
        if( m_uTimTimCnt < MAX_UINT32VAL )
        {
            m_uTimTimCnt++;
        }
        *p_puRemainings = m_uTimRemainingTime + m_uTimTimCntInc;

        if( m_uTimTimCntInc < MAX_UINT32VAL )
        {
            m_uTimTimCntInc++;
        }
        l_bResult = true;
    }

    return l_bResult;
}

bool_t eFSP_MSGRXTST_timStartErr ( t_eFSP_MSGRX_TimCtx* p_ptCtx, const uint32_t p_uTimeoutVal )
{
    bool_t l_bResult;
    t_eFSP_MSGRX_TimCtx* l_ptCtxCur;

    if( NULL == p_ptCtx )
    {
        l_bResult = false;
    }
    else
    {
        l_ptCtxCur = (t_eFSP_MSGRX_TimCtx*)p_ptCtx;
        l_ptCtxCur->bTxIsError = true;
        m_uTimTimCnt = 0u;

        m_uTimRemainingTime = p_uTimeoutVal;
        l_bResult = false;
    }

    return l_bResult;
}

bool_t eFSP_MSGRXTST_timGetRemainingErr ( t_eFSP_MSGRX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings )
{
    bool_t l_bResult;
    t_eFSP_MSGRX_TimCtx* l_ptCtxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puRemainings) )
    {
        l_bResult = false;
    }
    else
    {
        l_ptCtxCur = (t_eFSP_MSGRX_TimCtx*)p_ptCtx;
        l_ptCtxCur->bTxIsError = true;
        if( m_uTimTimCnt < MAX_UINT32VAL )
        {
            m_uTimTimCnt++;
        }
        if( m_uTimRemainingTime > 0u )
        {
            m_uTimRemainingTime--;
        }

        *p_puRemainings = m_uTimRemainingTime;
        l_bResult = false;
    }

    return l_bResult;
}

bool_t eFSP_MSGRXTST_timGetRemainingErrCntrl ( t_eFSP_MSGRX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings )
{
    bool_t l_bResult;
    t_eFSP_MSGRX_TimCtx* l_ptCtxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puRemainings) )
    {
        l_bResult = false;
    }
    else
    {
        l_ptCtxCur = (t_eFSP_MSGRX_TimCtx*)p_ptCtx;
        l_ptCtxCur->bTxIsError = true;
        if( m_uTimTimCnt < MAX_UINT32VAL )
        {
            m_uTimTimCnt++;
        }
        if( m_uTimRemainingTime > 0u )
        {
            m_uTimRemainingTime--;
        }

        *p_puRemainings = m_uTimRemainingTime;

        if( m_uTimTimCnt < 2u )
        {
            l_bResult = true;
        }
        else
        {
            l_bResult = false;
        }
    }

    return l_bResult;
}

/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSP_MSGRXTST_BadPointer(void)
{
    /* Static var init */
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[10u];
    uint8_t  l_auRecBuff[10u];
    uint8_t* l_puData;
    uint32_t l_uDataL;
    bool_t l_bIsInit;

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(NULL, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&l_tCtx, NULL) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 2  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = NULL;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 3  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = NULL;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 4  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = NULL;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 5  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = NULL;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 6  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = NULL;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 7  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = NULL;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 8  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = NULL;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 9  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = NULL;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 10 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = NULL;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_IsInit(NULL, &l_bIsInit) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 12 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_IsInit(&l_tCtx, NULL) )
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
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_GetDecodedData(NULL, &l_puData, &l_uDataL) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_GetDecodedData(&l_tCtx, NULL, &l_uDataL) )
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadPointer 17 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_BADPOINTER == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, NULL) )
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
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[10u];
    uint8_t  l_auRecBuff[10u];
    uint8_t* l_puData;
    uint32_t l_uDataL;
    bool_t l_bIsInit;

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadInit 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadInit 1  -- FAIL \n");
    }

    l_tCtx.tMsgdCtx.tBUNSTFCtx.bIsInit = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_IsInit(&l_tCtx, &l_bIsInit) )
    {
        if( false == l_bIsInit )
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
    if( e_eFSP_MSGRX_RES_NOINITLIB == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadInit 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadInit 3  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_NOINITLIB == eFSP_MSGRX_NewMsgNClean(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadInit 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadInit 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_NOINITLIB == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, &l_uDataL) )
    {
        (void)printf("eFSP_MSGRXTST_BadInit 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadInit 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_NOINITLIB == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
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
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[10u];
    uint8_t  l_auRecBuff[10u];
    bool_t l_bIsInit;

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = 0u;
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPARAM == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 1  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 0u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPARAM == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 2  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 0u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPARAM == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 3  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 100u;
    l_tInitData.uITimePerRecMs = 1000u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPARAM == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 4  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = 1u;
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_BADPARAM == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 5  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadParamEntr 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_IsInit(&l_tCtx, &l_bIsInit) )
    {
        if( true == l_bIsInit )
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
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[10u];
    uint8_t  l_auRecBuff[10u];
    uint8_t* l_puData;
    uint32_t l_uDataL;

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 1  -- FAIL \n");
    }

    /* Function */
    l_tCtx.puRxBuff = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 2  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 3  -- FAIL \n");
    }

    /* Function */
    l_tCtx.fRx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 4  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 5  -- FAIL \n");
    }

    /* Function */
    l_tCtx.ptRxCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 6  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 7  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tRxTim.ptTimCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 8  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 9  -- FAIL \n");
    }

    /* Function */
    l_tCtx.tRxTim.fTimStart = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 10 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 11 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tRxTim.fTimGetRemain = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 12 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 13 -- FAIL \n");
    }

    /* Function */
    l_tCtx.uRxBuffL = 0u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 14 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 15 -- FAIL \n");
    }

    /* Function */
    l_tCtx.uRxBufFil = l_tCtx.uRxBuffL + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 16 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 17 -- FAIL \n");
    }

    /* Function */
    l_tCtx.uRxBuffCntr = l_tCtx.uRxBufFil + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 18 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 19 -- FAIL \n");
    }

    /* Function */
    l_tCtx.uTimeoutMs = 0u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 20 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 21 -- FAIL \n");
    }

    /* Function */
    l_tCtx.uTimePerRecMs = 0u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 22 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 23 -- FAIL \n");
    }

    /* Function */
    l_tCtx.uTimePerRecMs = l_tCtx.uTimeoutMs + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 24 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 25 -- FAIL \n");
    }

    /* Function */
    l_tCtx.uTimePerRecMs = l_tCtx.uTimeoutMs + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsgNClean(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 26 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 27 -- FAIL \n");
    }

    /* Function */
    l_tCtx.uTimePerRecMs = l_tCtx.uTimeoutMs + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, &l_uDataL) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 28 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 29 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 29 -- FAIL \n");
    }

    /* Function */
    l_tCtx.uTimePerRecMs = l_tCtx.uTimeoutMs + 1u;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 30 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 30 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 31 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 31 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tMsgdCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 32 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 32 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 33 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 33 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tMsgdCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_NewMsgNClean(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 34 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 34 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 35 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 35 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tMsgdCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, &l_uDataL ) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 36 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 36 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 37 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CorruptContext 37 -- FAIL \n");
    }

    /* Function */
    l_tCtx.tMsgdCtx.ptCrcCtx = NULL;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
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
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdaptEr;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[10u];
    uint8_t  l_auRecBuff[10u];

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckCrc 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckCrc 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckCrc 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckCrc 2  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 12u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = 0x00u;
    m_auRxPayload[2u] = 0x00u;
    m_auRxPayload[3u] = 0x00u;
    m_auRxPayload[4u] = 0x00u;
    m_auRxPayload[5u] = 0x02u;
    m_auRxPayload[6u] = 0x00u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0xCCu;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_CRCCLBKERROR == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
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
    m_uReadJump = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[10u];
    uint8_t  l_auRecBuff[10u];

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsgErr;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsgNClean(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckReceive 3  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 12u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = 0x00u;
    m_auRxPayload[2u] = 0x00u;
    m_auRxPayload[3u] = 0x00u;
    m_auRxPayload[4u] = 0x00u;
    m_auRxPayload[5u] = 0x02u;
    m_auRxPayload[6u] = 0x00u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0xCCu;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_RXCLBKERROR == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
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
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[10u];
    uint8_t  l_auRecBuff[10u];

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStartErr;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemainingErr;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 2  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_NewMsgNClean(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 3  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 12u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = 0x00u;
    m_auRxPayload[2u] = 0x00u;
    m_auRxPayload[3u] = 0x00u;
    m_auRxPayload[4u] = 0x00u;
    m_auRxPayload[5u] = 0x02u;
    m_auRxPayload[6u] = 0x00u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0xCCu;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 4  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStartErr;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 5  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_NewMsgNClean(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 7  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 12u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = 0x00u;
    m_auRxPayload[2u] = 0x00u;
    m_auRxPayload[3u] = 0x00u;
    m_auRxPayload[4u] = 0x00u;
    m_auRxPayload[5u] = 0x02u;
    m_auRxPayload[6u] = 0x00u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0xCCu;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 8  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemainingErrCntrl;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 9  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadClBckTim 10 -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 12u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = 0x00u;
    m_auRxPayload[2u] = 0x00u;
    m_auRxPayload[3u] = 0x00u;
    m_auRxPayload[4u] = 0x00u;
    m_auRxPayload[5u] = 0x02u;
    m_auRxPayload[6u] = 0x00u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0xCCu;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_TIMCLBKERROR == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
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
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[10u];
    uint8_t  l_auRecBuff[10u];

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 2  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 12u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = 0x00u;
    m_auRxPayload[2u] = 0x00u;
    m_auRxPayload[3u] = 0x00u;
    m_auRxPayload[4u] = 0x00u;
    m_auRxPayload[5u] = 0x02u;
    m_auRxPayload[6u] = 0x00u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0xCCu;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 3  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 4  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 5  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xCCu;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 6  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 8  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = 0x00u;
    m_auRxPayload[2u] = 0x00u;
    m_auRxPayload[3u] = 0x00u;
    m_auRxPayload[4u] = 0x00u;
    m_auRxPayload[5u] = 0x03u;
    m_auRxPayload[6u] = 0x00u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0xCCu;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xCCu;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_OUTOFMEM == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 9  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 10 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 11 -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 12u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = 0x50u;
    m_auRxPayload[2u] = 0x7Bu;
    m_auRxPayload[3u] = 0x0Au;
    m_auRxPayload[4u] = 0x68u;
    m_auRxPayload[5u] = 0x01u;
    m_auRxPayload[6u] = 0x00u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0xCCu;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = ECU_EOF;

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 12 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 14 -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 12u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = 0x50u;
    m_auRxPayload[2u] = 0x7Bu;
    m_auRxPayload[3u] = 0x0Au;
    m_auRxPayload[4u] = 0x68u;
    m_auRxPayload[5u] = 0x01u;
    m_auRxPayload[6u] = 0x00u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = ECU_EOF;

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 15 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 16 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_BadFrame 17 -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xCCu;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
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
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[10u];
    uint8_t  l_auRecBuff[10u];

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 1u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 2  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xCCu;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 3  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 5  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 6  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 5u;
    l_tInitData.uITimePerRecMs = 1u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 8  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xCCu;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 9  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 10 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 11 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 12 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 6u;
    l_tInitData.uITimePerRecMs = 1u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 13 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 14 -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xCCu;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 15 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 16 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 17 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 18 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsgJump;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 19 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 20 -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xCCu;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 21 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 22 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 23 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 100u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 24 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 25 -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = ECU_ESC;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 26 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 27 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase 27 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
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
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[10u];
    uint8_t  l_auRecBuff[10u];
    uint8_t* l_puData;
    uint32_t l_uDataL;


    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsgJump;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 9u;
    l_tInitData.uITimePerRecMs = 9u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 2  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xCCu;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 3  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 5  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsgJump;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 10u;
    l_tInitData.uITimePerRecMs = 10u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 6  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 7  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xCCu;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 8  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 9  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 10 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsgJump;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 10u;
    l_tInitData.uITimePerRecMs = 10u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 11 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 12 -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] =  0x83u;
    m_auRxPayload[3u] =  0xFDu;
    m_auRxPayload[4u] =  0xC7u;
    m_auRxPayload[5u] =  0x59u;
    m_auRxPayload[6u] =  0x02u;
    m_auRxPayload[7u] =  0x00u;
    m_auRxPayload[8u] =  0x00u;
    m_auRxPayload[9u] =  0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xC1u;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 13 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 14 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 15 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 16 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, &l_uDataL) )
    {
        if( 2u == l_uDataL )
        {
            if( (  0xCCu == l_puData[0u] ) || (  0xC1u == l_puData[1u] ) )
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
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsgCrrupt;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 10u;
    l_tInitData.uITimePerRecMs = 10u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 18 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 19 -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xCCu;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 20 -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemainingCorr;
    l_tInitData.uITimeoutMs = 10u;
    l_tInitData.uITimePerRecMs = 10u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 21 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 22 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase2 22 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, &l_uDataL) )
    {
        if( 0u == l_uDataL )
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
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = 0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xCCu;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_CORRUPTCTX == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
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
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[10u];
    uint8_t  l_auRecBuff[10u];
    uint8_t* l_puData;
    uint32_t l_uDataL;

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsgJump;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 1000u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 2  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 21u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_EOF;
    m_auRxPayload[1u] = 0xFFu;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = ECU_SOF;
    m_auRxPayload[10u] = 0x83u;
    m_auRxPayload[11u] = 0xFDu;
    m_auRxPayload[12u] = 0xC7u;
    m_auRxPayload[13u] = 0x59u;
    m_auRxPayload[14u] = 0x02u;
    m_auRxPayload[15u] = 0x00u;
    m_auRxPayload[16u] = 0x00u;
    m_auRxPayload[17u] = 0x00u;
    m_auRxPayload[18u] = 0xCCu;
    m_auRxPayload[19u] = 0xC1u;
    m_auRxPayload[20u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 3  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 5  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 7  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 8  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 9  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 10 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 11 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 12 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 13 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 14 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 15 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 16 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 17 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 18 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 19 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, &l_uDataL) )
    {
        if( 0u == l_uDataL )
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

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 21 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, &l_uDataL) )
    {
        if( 0u == l_uDataL )
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

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 23 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 23 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, &l_uDataL) )
    {
        if( 2u == l_uDataL )
        {
            if( (  0xCCu == l_puData[0u] ) || (  0xC1u == l_puData[1u] ) )
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
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsgJump;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 1000u;
    l_tInitData.uITimePerRecMs = 1000u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 25 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 25 -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 26 -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 21u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_EOF;
    m_auRxPayload[1u] = 0xFFu;
    m_auRxPayload[2u] = 0x50u;
    m_auRxPayload[3u] = 0x7Bu;
    m_auRxPayload[4u] = 0x0Au;
    m_auRxPayload[5u] = 0x68u;
    m_auRxPayload[6u] = 0x02u;
    m_auRxPayload[7u] = 0x00u;
    m_auRxPayload[8u] = 0x00u;
    m_auRxPayload[9u] = ECU_SOF;
    m_auRxPayload[10u] = 0x83u;
    m_auRxPayload[11u] = 0xFDu;
    m_auRxPayload[12u] = 0xC7u;
    m_auRxPayload[13u] = 0x59u;
    m_auRxPayload[14u] = 0x02u;
    m_auRxPayload[15u] = 0x00u;
    m_auRxPayload[16u] = 0x00u;
    m_auRxPayload[17u] = 0x00u;
    m_auRxPayload[18u] = 0xCCu;
    m_auRxPayload[19u] = 0xC1u;
    m_auRxPayload[20u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_BADFRAME == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 24 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 24 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, &l_uDataL) )
    {
        if( 0u == l_uDataL )
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

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsgNClean(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 26 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 26 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, &l_uDataL) )
    {
        if( 0u == l_uDataL )
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

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 28 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase3 28 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, &l_uDataL) )
    {
        if( 2u == l_uDataL )
        {
            if( (  0xCCu == l_puData[0u] ) || (  0xC1u == l_puData[1u] ) )
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
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[10u];
    uint8_t  l_auRecBuff[5u];

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsgJump;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 8u;
    l_tInitData.uITimePerRecMs = 8u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 2  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] =  0x83u;
    m_auRxPayload[3u] =  0xFDu;
    m_auRxPayload[4u] =  0xC7u;
    m_auRxPayload[5u] =  0x59u;
    m_auRxPayload[6u] =  0x02u;
    m_auRxPayload[7u] =  0x00u;
    m_auRxPayload[8u] =  0x00u;
    m_auRxPayload[9u] =  0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xC1u;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 3  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 5  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 7  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = 1u;
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 25u;
    l_tInitData.uITimePerRecMs = 1u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 8  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 9  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 9  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 13u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] = ECU_SOF;
    m_auRxPayload[2u] =  0x83u;
    m_auRxPayload[3u] =  0xFDu;
    m_auRxPayload[4u] =  0xC7u;
    m_auRxPayload[5u] =  0x59u;
    m_auRxPayload[6u] =  0x02u;
    m_auRxPayload[7u] =  0x00u;
    m_auRxPayload[8u] =  0x00u;
    m_auRxPayload[9u] =  0x00u;
    m_auRxPayload[10u] = 0xCCu;
    m_auRxPayload[11u] = 0xC1u;
    m_auRxPayload[12u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 10 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 10 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 11 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 11 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 12 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 12 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 13 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 13 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 14 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 14 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 15 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 15 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 16 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 16 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 17 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 17 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 18 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 18 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 19 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 19 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 20 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 20 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 21 -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase4 21 -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
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
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[20u];
    uint8_t  l_auRecBuff[20u];
    uint8_t* l_puData;
    uint32_t l_uDataL;

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsgJumpLong;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 8u;
    l_tInitData.uITimePerRecMs = 8u;
    l_tInitData.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 1  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 2  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 12u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] =  0x83u;
    m_auRxPayload[2u] =  0xFDu;
    m_auRxPayload[3u] =  0xC7u;
    m_auRxPayload[4u] =  0x59u;
    m_auRxPayload[5u] =  0x02u;
    m_auRxPayload[6u] =  0x00u;
    m_auRxPayload[7u] =  0x00u;
    m_auRxPayload[8u] =  0x00u;
    m_auRxPayload[9u]  = 0xCCu;
    m_auRxPayload[10u] = 0xC1u;
    m_auRxPayload[11u] = ECU_EOF;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 3  -- FAIL \n");
    }

    m_uReadJumpLong = 0u;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&l_tCtx, &l_puData, &l_uDataL) )
    {
        if( 0u == l_uDataL )
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

    l_tCtx.fRx = &eFSP_MSGRXTST_receiveMsg;
    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 6  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 6  -- FAIL \n");
    }

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsgJumpLong;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 8u;
    l_tInitData.uITimePerRecMs = 8u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 7  -- FAIL \n");
    }

    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 8  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase5 8  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = 12u;
    m_uPayloadCounter = 0u;
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] =  0x83u;
    m_auRxPayload[2u] =  0xFDu;
    m_auRxPayload[3u] =  0xC7u;
    m_auRxPayload[4u] =  0x59u;
    m_auRxPayload[5u] =  0x02u;
    m_auRxPayload[6u] =  0x00u;
    m_auRxPayload[7u] =  0x00u;
    m_auRxPayload[8u] =  0x00u;
    m_auRxPayload[9u]  = 0xCCu;
    m_auRxPayload[10u] = 0xC1u;
    m_auRxPayload[11u] = ECU_EOF;
    m_uReadJumpLong = 0u;
    if( e_eFSP_MSGRX_RES_MESSAGETIMEOUT == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
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
    m_uReadJump = 0u;
	m_uReadJumpLong = 0u;
    uint32_t temp;

    /* Local variable */
    t_eFSP_MSGRX_Ctx l_tCtx;
    t_eFSP_MSGRX_InitData l_tInitData;
    f_eFSP_MSGD_CrcCb l_fCrcPTest = &eFSP_MSGRXTST_c32SAdapt;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrc;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemArea[100u];
    uint8_t  l_auRecBuff[100u];

    /* Function */
    l_tInitData.puIMemArea = l_auMemArea;
    l_tInitData.uIMemAreaL = sizeof(l_auMemArea);
    l_tInitData.puIRxBuffArea = l_auRecBuff;
    l_tInitData.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitData.fICrc = l_fCrcPTest;
    l_tInitData.ptICbCrcCtx = &l_tCtxAdapterCrc;
    l_tInitData.fIRx = &eFSP_MSGRXTST_receiveMsg;
    l_tInitData.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitData.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitData.tIRxTim.fTimStart = &eFSP_MSGRXTST_timStart;
    l_tInitData.tIRxTim.fTimGetRemain = &eFSP_MSGRXTST_timGetRemaining;
    l_tInitData.uITimeoutMs = 2000u;
    l_tInitData.uITimePerRecMs = 2000u;
    l_tInitData.bINeedWaitFrameStart = false;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtx, &l_tInitData) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 1  -- FAIL \n");
    }


    /* Function */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 2  -- FAIL \n");
    }

    /* Fucntion */
    m_uPayloadSize = sizeof(m_auRxPayload);
    m_uPayloadCounter = 0u;
    for( temp = 0u; temp < sizeof(m_auRxPayload) ; temp++ )
    {
        m_auRxPayload[temp] = ECU_SOF;
    }
    m_auRxPayload[0u] = ECU_SOF;
    m_auRxPayload[1u] =  0x83u;
    m_auRxPayload[2u] =  0xFDu;
    m_auRxPayload[3u] =  0xC7u;
    m_auRxPayload[4u] =  0x59u;
    m_auRxPayload[5u] =  0x50u;
    m_auRxPayload[6u] =  0x00u;
    m_auRxPayload[7u] =  0x00u;
    m_auRxPayload[8u] =  0x00u;
    m_auRxPayload[9u] =  ECU_SOF;
    m_auRxPayload[10u] = 0x83u;
    m_auRxPayload[11u] = 0xFDu;
    m_auRxPayload[12u] = 0xC7u;
    m_auRxPayload[13u] = 0x59u;
    m_auRxPayload[14u] = 0x02u;
    m_auRxPayload[15u] = 0x00u;
    m_auRxPayload[16u] = 0x00u;
    m_auRxPayload[17u] = 0x00u;
    m_auRxPayload[18u] = 0xCCu;
    m_auRxPayload[19u] = 0xC1u;
    m_auRxPayload[20u] = ECU_EOF;

    if( e_eFSP_MSGRX_RES_FRAMERESTART == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 3  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 3  -- FAIL \n");
    }

    if( ( 81u == l_tCtx.uRxBufFil) || ( 1u == l_tCtx.uRxBuffCntr ) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 4  -- FAIL \n");
    }

    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtx) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 5  -- FAIL \n");
    }

    if( ( 81u == l_tCtx.uRxBufFil) || ( 12u == l_tCtx.uRxBuffCntr ) )
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXTST_CornerCase6 6  -- FAIL \n");
    }
}


#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "MISRAC2012-Rule-11.5",  \
                            "MISRAC2012-Rule-8.9_a", "MISRAC2012-Rule-8.13", "MISRAC2012-Rule-2.2_b"
#endif