/**
 * @file       eFSP_MSGRXMSGTXTST.h
 *
 * @brief      Message Tx Rx test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_MSGRXMSGTXTST.h"
#include "eFSP_MSGTX.h"
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
                            "CERT-INT30-C_b"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSP_MSGRXMSGTXTST_c32SAdaptRx(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val);
static bool_t eFSP_MSGRXMSGTXTST_c32SAdaptTx(t_eFSP_MSGE_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val);

static bool_t eFSP_MSGRXMSGTXTST_sendMsg( t_eFSP_MSGTX_TxCtx* p_ptCtx, const uint8_t* p_puDataToTx, const uint32_t p_uDataToTxL, uint32_t* const p_uDataTxed,
                       const uint32_t p_uTimeToTx );

static bool_t eFSP_MSGRXMSGTXTST_receiveMsg( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                       const uint32_t p_uTimeToRx );

static bool_t eFSP_MSGRXMSGTXTST_timStart ( t_eFSP_MSGTX_TimCtx* p_ptCtx, const uint32_t p_uTimeoutVal );
static bool_t eFSP_MSGRXMSGTXTST_timGetRemaining ( t_eFSP_MSGTX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings );
static bool_t eFSP_MSGRXMSGTXTST_timStartRx ( t_eFSP_MSGRX_TimCtx* p_ptCtx, const uint32_t p_uTimeoutVal );
static bool_t eFSP_MSGRXMSGTXTST_timGetRemainingRx ( t_eFSP_MSGRX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings );

/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
struct t_eFSP_MSGD_CrcCtxUser
{
    e_eCU_CRC_RES eLastEr;
};

struct t_eFSP_MSGE_CrcCtxUser
{
    e_eCU_CRC_RES eLastEr;
};

struct t_eFSP_MSGRX_RxCtxUser
{
    bool bTxIsError;
};

struct t_eFSP_MSGRX_TimCtxUser
{
    uint32_t uTimRemainingT;
    uint32_t uTimTimCnt;
    uint32_t uTimTimCntInc;
    bool bTxIsError;
};

struct t_eFSP_MSGTX_TxCtxUser
{
    bool bTxIsError;
};

struct t_eFSP_MSGTX_TimCtxUser
{
    uint32_t uTimRemainingT;
    uint32_t uTimTimCnt;
    uint32_t uTimTimCntInc;
    bool bTxIsError;
};

static void eFSP_MSGRXMSGTXTST_Common(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_MSGRXMSGTXTST_ExeTest(void)
{
	(void)printf("\n\nMESSAGE RECEIVER TRANSMITTER TEST START \n\n");

    eFSP_MSGRXMSGTXTST_Common();

    (void)printf("\n\nMESSAGE RECEIVER TRANSMITTER TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION IMPLEMENTATION
 **********************************************************************************************************************/
static uint8_t m_auBufferCom[1000u];
static uint32_t m_uBufferComCounterInsert;
static uint32_t m_uBufferComCounterRetrive;

bool_t eFSP_MSGRXMSGTXTST_c32SAdaptRx(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t result;
    t_eFSP_MSGD_CrcCtx* ctxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGD_CrcCtx*)p_ptCtx;

        ctxCur->eLastEr = eCU_CRC_32Seed(p_uS, (const uint8_t*)p_puD, p_uDLen, p_puC32Val);
        if( e_eCU_CRC_RES_OK == ctxCur->eLastEr )
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

bool_t eFSP_MSGRXMSGTXTST_c32SAdaptTx(t_eFSP_MSGE_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t result;
    t_eFSP_MSGE_CrcCtx* ctxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGE_CrcCtx*)p_ptCtx;

        ctxCur->eLastEr = eCU_CRC_32Seed(p_uS, (const uint8_t*)p_puD, p_uDLen, p_puC32Val);
        if( e_eCU_CRC_RES_OK == ctxCur->eLastEr )
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

bool_t eFSP_MSGRXMSGTXTST_sendMsg( t_eFSP_MSGTX_TxCtx* p_ptCtx, const uint8_t* p_puDataToTx, const uint32_t p_uDataToTxL, uint32_t* const p_uDataTxed,
                const uint32_t p_uTimeToTx )
{
    bool_t result;
    t_eFSP_MSGTX_TxCtx* ctxCur;

    (void)p_uTimeToTx;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puDataToTx ) || ( NULL == p_uDataTxed ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TxCtx*)p_ptCtx;
        ctxCur->bTxIsError = true;

        if( ( m_uBufferComCounterInsert + p_uDataToTxL ) < sizeof( m_auBufferCom ) )
        {
            (void)memcpy(&m_auBufferCom[m_uBufferComCounterInsert], p_puDataToTx, p_uDataToTxL);
            m_uBufferComCounterInsert += p_uDataToTxL;
            *p_uDataTxed = p_uDataToTxL;

            result = true;
        }
        else
        {
            result = false;
        }
    }

    return result;
}


bool_t eFSP_MSGRXMSGTXTST_receiveMsg( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                const uint32_t p_uTimeToRx )
{
    bool_t result;
    t_eFSP_MSGRX_RxCtx* ctxCur;
    uint32_t dataAvaiable;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puDataToRx ) || ( NULL == p_puDataRxedL ) )
    {
        result = false;
    }
    else
    {
        if( m_uBufferComCounterRetrive > m_uBufferComCounterInsert)
        {
            /* Impossible */
            result = false;
            ctxCur->bTxIsError = true;
        }
        else
        {
            result = true;
            ctxCur = (t_eFSP_MSGRX_RxCtx*)p_ptCtx;
            ctxCur->bTxIsError = false;
            dataAvaiable = m_uBufferComCounterInsert - m_uBufferComCounterRetrive;

            if( 0u == dataAvaiable )
            {
                *p_puDataRxedL = dataAvaiable;
            }
            else if( dataAvaiable > p_uDataRxMax )
            {
                *p_puDataRxedL = p_uDataRxMax;
                (void)memcpy(p_puDataToRx, &m_auBufferCom[m_uBufferComCounterRetrive], p_uDataRxMax );
                m_uBufferComCounterRetrive += p_uDataRxMax;
            }
            else
            {
                *p_puDataRxedL = dataAvaiable;
                (void)memcpy(p_puDataToRx, &m_auBufferCom[m_uBufferComCounterRetrive], dataAvaiable );
                m_uBufferComCounterRetrive += dataAvaiable;
            }

        }
    }

    return result;
}

bool_t eFSP_MSGRXMSGTXTST_timStart ( t_eFSP_MSGTX_TimCtx* p_ptCtx, const uint32_t p_uTimeoutVal )
{
    bool_t result;
    t_eFSP_MSGTX_TimCtx* ctxCur;

    if( NULL == p_ptCtx )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TimCtx*)p_ptCtx;
        ctxCur->bTxIsError = true;
        ctxCur->uTimTimCnt = 0u;
        ctxCur->uTimTimCntInc = 0u;
        ctxCur->uTimRemainingT = p_uTimeoutVal;
        result = true;
    }

    return result;
}

bool_t eFSP_MSGRXMSGTXTST_timGetRemaining ( t_eFSP_MSGTX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings )
{
    bool_t result;
    t_eFSP_MSGTX_TimCtx* ctxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puRemainings ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TimCtx*)p_ptCtx;
        ctxCur->bTxIsError = true;

        if( ctxCur->uTimTimCnt < MAX_UINT32VAL )
        {
            ctxCur->uTimTimCnt++;
        }

        if( ctxCur->uTimRemainingT > 0u )
        {
            ctxCur->uTimRemainingT--;
        }

        *p_puRemainings = ctxCur->uTimRemainingT;
        result = true;
    }

    return result;
}


bool_t eFSP_MSGRXMSGTXTST_timStartRx ( t_eFSP_MSGRX_TimCtx* p_ptCtx, const uint32_t p_uTimeoutVal )
{
    bool_t result;
    t_eFSP_MSGTX_TimCtx* ctxCur;

    if( NULL == p_ptCtx )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TimCtx*)p_ptCtx;
        ctxCur->bTxIsError = true;
        ctxCur->uTimTimCnt = 0u;
        ctxCur->uTimTimCntInc = 0u;
        ctxCur->uTimRemainingT = p_uTimeoutVal;
        result = true;
    }

    return result;
}

bool_t eFSP_MSGRXMSGTXTST_timGetRemainingRx ( t_eFSP_MSGRX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings )
{
    bool_t result;
    t_eFSP_MSGTX_TimCtx* ctxCur;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puRemainings ) )
    {
        result = false;
    }
    else
    {
        ctxCur = (t_eFSP_MSGTX_TimCtx*)p_ptCtx;
        ctxCur->bTxIsError = true;

        if( ctxCur->uTimTimCnt < MAX_UINT32VAL )
        {
            ctxCur->uTimTimCnt++;
        }

        if( ctxCur->uTimRemainingT > 0u )
        {
            ctxCur->uTimRemainingT--;
        }

        *p_puRemainings = ctxCur->uTimRemainingT;
        result = true;
    }

    return result;
}

/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSP_MSGRXMSGTXTST_Common(void)
{
    /* Init static */
    m_uBufferComCounterInsert = 0u;
    m_uBufferComCounterRetrive = 0u;

    /* Local variable RECEIVER */
    t_eFSP_MSGRX_Ctx ctxRX;
    t_eFSP_MSGRX_InitData initDataRX;
    t_eFSP_MSGD_CrcCtx ctxAdapterCrcRX;
    t_eFSP_MSGRX_RxCtx ctxAdapterRx;
    t_eFSP_MSGRX_TimCtx ctxAdapterTim;
    uint8_t  memAreaRX[20u];
    uint8_t  recBuff[1u];

    /* Local variable TRANSMITTER */
    t_eFSP_MSGTX_Ctx ctxTx;
    t_eFSP_MSGTX_InitData initDataTx;
    t_eFSP_MSGE_CrcCtx ctxAdapterCrcTx;
    t_eFSP_MSGTX_TxCtx ctxAdapterSend;
    t_eFSP_MSGTX_TimCtx ctxAdapterTimTx;
    uint8_t  memAreaTx[20u];
    uint8_t  sendBuff[1u];

    /* Common var */
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctxTx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&initDataTx, 0, sizeof(t_eFSP_MSGTX_InitData));
    (void)memset(&ctxRX, 0, sizeof(t_eFSP_MSGRX_Ctx));
    (void)memset(&initDataRX, 0, sizeof(t_eFSP_MSGRX_InitData));

    /* Init */
    initDataTx.puIMemArea = memAreaTx;
    initDataTx.uIMemAreaL = sizeof(memAreaTx);
    initDataTx.puITxBuffArea = sendBuff;
    initDataTx.uITxBuffAreaL = sizeof(sendBuff);
    initDataTx.fICrc = &eFSP_MSGRXMSGTXTST_c32SAdaptTx;
    initDataTx.ptICbCrcCtx = &ctxAdapterCrcTx;
    initDataTx.fITx = &eFSP_MSGRXMSGTXTST_sendMsg;
    initDataTx.ptICbTxCtx = &ctxAdapterSend;
    initDataTx.tITxTim.ptTimCtx = &ctxAdapterTimTx;
    initDataTx.tITxTim.fTimStart = &eFSP_MSGRXMSGTXTST_timStart;
    initDataTx.tITxTim.fTimGetRemain = &eFSP_MSGRXMSGTXTST_timGetRemaining;
    initDataTx.uITimeoutMs = 1000u;
    initDataTx.uITimePerSendMs = 100u;

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctxTx, &initDataTx) )
    {
        (void)printf("eFSP_MSGRXMSGTXTST_Common 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXMSGTXTST_Common 1  -- FAIL \n");
    }

    initDataRX.puIMemArea = memAreaRX;
    initDataRX.uIMemAreaL = sizeof(memAreaRX);
    initDataRX.puIRxBuffArea = recBuff;
    initDataRX.uIRxBuffAreaL = sizeof(recBuff);
    initDataRX.fICrc = &eFSP_MSGRXMSGTXTST_c32SAdaptRx;
    initDataRX.ptICbCrcCtx = &ctxAdapterCrcRX;
    initDataRX.fIRx = &eFSP_MSGRXMSGTXTST_receiveMsg;
    initDataRX.ptICbRxCtx = &ctxAdapterRx;
    initDataRX.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initDataRX.tIRxTim.fTimStart = &eFSP_MSGRXMSGTXTST_timStartRx;
    initDataRX.tIRxTim.fTimGetRemain = &eFSP_MSGRXMSGTXTST_timGetRemainingRx;
    initDataRX.uITimeoutMs = 1000u;
    initDataRX.uITimePerRecMs = 100u;
    initDataRX.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctxRX, &initDataRX) )
    {
        (void)printf("eFSP_MSGRXMSGTXTST_Common 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXMSGTXTST_Common 2  -- FAIL \n");
    }

    /* Fill data */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctxTx, &dataP, &dataL) )
    {
        if( 12u == dataL )
        {
            (void)printf("eFSP_MSGRXMSGTXTST_Common 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_MSGRXMSGTXTST_Common 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGRXMSGTXTST_Common 3  -- FAIL \n");
    }

    dataP[0u] = ECU_SOF;
    dataP[1u] = ECU_ESC;
    dataP[2u] = ECU_ESC;
    dataP[3u] = ECU_EOF;
    dataP[4u] = 0xFFu;
    dataP[5u] = 0xCCu;
    dataP[6u] = ECU_SOF;

    /* Start TX message */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctxTx, 7u) )
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 4  -- FAIL \n");
    }

    /* Start RX message */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctxRX) )
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 5  -- FAIL \n");
    }

    /* Send data  */
    if( e_eFSP_MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctxTx) )
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 6  -- FAIL \n");
    }

    /* Receive data */
    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctxRX) )
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 7  -- FAIL \n");
    }

    /* Get RX data */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctxRX, &dataP, &dataL) )
    {
        if( 7u == dataL )
        {
            if( (  ECU_SOF == dataP[0u] ) || (  ECU_ESC == dataP[1u] ) || (  ECU_ESC == dataP[2u] ) ||
                (  ECU_EOF == dataP[3u] ) || (  0xFFu == dataP[4u] ) || (  0xCCu == dataP[5u] ) ||
                (  ECU_SOF == dataP[6u] ) )
            {
                (void)printf("eFSP_MSGTXTST_CornerCase 8  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_MSGTXTST_CornerCase 8  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_MSGTXTST_CornerCase 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 8  -- FAIL \n");
    }
}



#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "CERT-INT30-C_b"
#endif