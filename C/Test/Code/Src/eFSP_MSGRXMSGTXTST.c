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
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE TEST VARIABLE DECLARATION
 **********************************************************************************************************************/
static uint8_t m_auBufferCom[1000u];
static uint32_t m_uBufferComCounterInsert;
static uint32_t m_uBufferComCounterRetrive;



/***********************************************************************************************************************
 *   PRIVATE TEST STRUCT DECLARATION
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
bool_t eFSP_MSGRXMSGTXTST_c32SAdaptRx(t_eFSP_MSGD_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t l_bRes;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->eLastEr = eCU_CRC_32Seed(p_uS, (const uint8_t*)p_puD, p_uDLen, p_puC32Val);
        if( e_eCU_CRC_RES_OK == p_ptCtx->eLastEr )
        {
            l_bRes = true;
        }
        else
        {
            l_bRes = false;
        }
    }

    return l_bRes;
}

bool_t eFSP_MSGRXMSGTXTST_c32SAdaptTx(t_eFSP_MSGE_CrcCtx* p_ptCtx, const uint32_t p_uS, const uint8_t* p_puD, const uint32_t p_uDLen, uint32_t* const p_puC32Val)
{
    bool_t l_bRes;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puC32Val ) )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->eLastEr = eCU_CRC_32Seed(p_uS, (const uint8_t*)p_puD, p_uDLen, p_puC32Val);
        if( e_eCU_CRC_RES_OK == p_ptCtx->eLastEr )
        {
            l_bRes = true;
        }
        else
        {
            l_bRes = false;
        }
    }

    return l_bRes;
}

bool_t eFSP_MSGRXMSGTXTST_sendMsg( t_eFSP_MSGTX_TxCtx* p_ptCtx, const uint8_t* p_puDataToTx, const uint32_t p_uDataToTxL, uint32_t* const p_uDataTxed,
                const uint32_t p_uTimeToTx )
{
    bool_t l_bRes;

    (void)p_uTimeToTx;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puDataToTx ) || ( NULL == p_uDataTxed ) )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->bTxIsError = true;

        if( ( m_uBufferComCounterInsert + p_uDataToTxL ) < sizeof( m_auBufferCom ) )
        {
            (void)memcpy(&m_auBufferCom[m_uBufferComCounterInsert], p_puDataToTx, p_uDataToTxL);
            m_uBufferComCounterInsert += p_uDataToTxL;
            *p_uDataTxed = p_uDataToTxL;

            l_bRes = true;
        }
        else
        {
            l_bRes = false;
        }
    }

    return l_bRes;
}


bool_t eFSP_MSGRXMSGTXTST_receiveMsg( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataToRx, uint32_t* const p_puDataRxedL, const uint32_t p_uDataRxMax,
                const uint32_t p_uTimeToRx )
{
    bool_t l_bRes;
    uint32_t l_uDataAvaiable;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puDataToRx ) || ( NULL == p_puDataRxedL ) )
    {
        l_bRes = false;
    }
    else
    {
        if( m_uBufferComCounterRetrive > m_uBufferComCounterInsert)
        {
            /* Impossible */
            l_bRes = false;
            p_ptCtx->bTxIsError = true;
        }
        else
        {
            l_bRes = true;
            p_ptCtx->bTxIsError = false;
            l_uDataAvaiable = m_uBufferComCounterInsert - m_uBufferComCounterRetrive;

            if( 0u == l_uDataAvaiable )
            {
                *p_puDataRxedL = l_uDataAvaiable;
            }
            else if( l_uDataAvaiable > p_uDataRxMax )
            {
                *p_puDataRxedL = p_uDataRxMax;
                (void)memcpy(p_puDataToRx, &m_auBufferCom[m_uBufferComCounterRetrive], p_uDataRxMax );
                m_uBufferComCounterRetrive += p_uDataRxMax;
            }
            else
            {
                *p_puDataRxedL = l_uDataAvaiable;
                (void)memcpy(p_puDataToRx, &m_auBufferCom[m_uBufferComCounterRetrive], l_uDataAvaiable );
                m_uBufferComCounterRetrive += l_uDataAvaiable;
            }

        }
    }

    return l_bRes;
}

bool_t eFSP_MSGRXMSGTXTST_timStart ( t_eFSP_MSGTX_TimCtx* p_ptCtx, const uint32_t p_uTimeoutVal )
{
    bool_t l_bRes;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->bTxIsError = true;
        p_ptCtx->uTimTimCnt = 0u;
        p_ptCtx->uTimTimCntInc = 0u;
        p_ptCtx->uTimRemainingT = p_uTimeoutVal;
        l_bRes = true;
    }

    return l_bRes;
}

bool_t eFSP_MSGRXMSGTXTST_timGetRemaining ( t_eFSP_MSGTX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings )
{
    bool_t l_bRes;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puRemainings ) )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->bTxIsError = true;

        if( p_ptCtx->uTimTimCnt < MAX_UINT32VAL )
        {
            p_ptCtx->uTimTimCnt++;
        }

        if( p_ptCtx->uTimRemainingT > 0u )
        {
            p_ptCtx->uTimRemainingT--;
        }

        *p_puRemainings = p_ptCtx->uTimRemainingT;
        l_bRes = true;
    }

    return l_bRes;
}


bool_t eFSP_MSGRXMSGTXTST_timStartRx ( t_eFSP_MSGRX_TimCtx* p_ptCtx, const uint32_t p_uTimeoutVal )
{
    bool_t l_bRes;

    if( NULL == p_ptCtx )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->bTxIsError = true;
        p_ptCtx->uTimTimCnt = 0u;
        p_ptCtx->uTimTimCntInc = 0u;
        p_ptCtx->uTimRemainingT = p_uTimeoutVal;
        l_bRes = true;
    }

    return l_bRes;
}

bool_t eFSP_MSGRXMSGTXTST_timGetRemainingRx ( t_eFSP_MSGRX_TimCtx* p_ptCtx, uint32_t* const p_puRemainings )
{
    bool_t l_bRes;

    if( ( NULL == p_ptCtx ) || ( NULL == p_puRemainings ) )
    {
        l_bRes = false;
    }
    else
    {
        p_ptCtx->bTxIsError = true;

        if( p_ptCtx->uTimTimCnt < MAX_UINT32VAL )
        {
            p_ptCtx->uTimTimCnt++;
        }

        if( p_ptCtx->uTimRemainingT > 0u )
        {
            p_ptCtx->uTimRemainingT--;
        }

        *p_puRemainings = p_ptCtx->uTimRemainingT;
        l_bRes = true;
    }

    return l_bRes;
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
    t_eFSP_MSGRX_Ctx l_tCtxRX;
    t_eFSP_MSGRX_InitData l_tInitDataRX;
    t_eFSP_MSGD_CrcCtx l_tCtxAdapterCrcRX;
    t_eFSP_MSGRX_RxCtx l_tCtxAdapterRx;
    t_eFSP_MSGRX_TimCtx l_tCtxAdapterTim;
    uint8_t  l_auMemAreaRX[20u];
    uint8_t  l_auRecBuff[1u];

    /* Local variable TRANSMITTER */
    t_eFSP_MSGTX_Ctx l_tCtxTx;
    t_eFSP_MSGTX_InitData l_tInitDataTx;
    t_eFSP_MSGE_CrcCtx l_tCtxAdapterCrcTx;
    t_eFSP_MSGTX_TxCtx l_tCtxAdapterSend;
    t_eFSP_MSGTX_TimCtx l_tCtxAdapterTimTx;
    uint8_t  l_auMemAreaTx[20u];
    uint8_t  l_auSendBuff[1u];

    /* Common var */
    uint8_t* l_puData;
    uint32_t l_uDataL;

    /* Clear */
    (void)memset(&l_tCtxTx, 0, sizeof(t_eFSP_MSGTX_Ctx));
    (void)memset(&l_tInitDataTx, 0, sizeof(t_eFSP_MSGTX_InitData));
    (void)memset(&l_tCtxRX, 0, sizeof(t_eFSP_MSGRX_Ctx));
    (void)memset(&l_tInitDataRX, 0, sizeof(t_eFSP_MSGRX_InitData));
    (void)memset(&l_auMemAreaRX, 0, sizeof(l_auMemAreaRX));
    (void)memset(&l_auRecBuff, 0, sizeof(l_auRecBuff));
    (void)memset(&l_auMemAreaTx, 0, sizeof(l_auMemAreaTx));
    (void)memset(&l_auSendBuff, 0, sizeof(l_auSendBuff));

    /* Init */
    l_tInitDataTx.puIMemArea = l_auMemAreaTx;
    l_tInitDataTx.uIMemAreaL = sizeof(l_auMemAreaTx);
    l_tInitDataTx.puITxBuffArea = l_auSendBuff;
    l_tInitDataTx.uITxBuffAreaL = sizeof(l_auSendBuff);
    l_tInitDataTx.fICrc = &eFSP_MSGRXMSGTXTST_c32SAdaptTx;
    l_tInitDataTx.ptICbCrcCtx = &l_tCtxAdapterCrcTx;
    l_tInitDataTx.fITx = &eFSP_MSGRXMSGTXTST_sendMsg;
    l_tInitDataTx.ptICbTxCtx = &l_tCtxAdapterSend;
    l_tInitDataTx.tITxTim.ptTimCtx = &l_tCtxAdapterTimTx;
    l_tInitDataTx.tITxTim.fTimStart = &eFSP_MSGRXMSGTXTST_timStart;
    l_tInitDataTx.tITxTim.fTimGetRemain = &eFSP_MSGRXMSGTXTST_timGetRemaining;
    l_tInitDataTx.uITimeoutMs = 1000u;
    l_tInitDataTx.uITimePerSendMs = 100u;

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&l_tCtxTx, &l_tInitDataTx) )
    {
        (void)printf("eFSP_MSGRXMSGTXTST_Common 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXMSGTXTST_Common 1  -- FAIL \n");
    }

    l_tInitDataRX.puIMemArea = l_auMemAreaRX;
    l_tInitDataRX.uIMemAreaL = sizeof(l_auMemAreaRX);
    l_tInitDataRX.puIRxBuffArea = l_auRecBuff;
    l_tInitDataRX.uIRxBuffAreaL = sizeof(l_auRecBuff);
    l_tInitDataRX.fICrc = &eFSP_MSGRXMSGTXTST_c32SAdaptRx;
    l_tInitDataRX.ptICbCrcCtx = &l_tCtxAdapterCrcRX;
    l_tInitDataRX.fIRx = &eFSP_MSGRXMSGTXTST_receiveMsg;
    l_tInitDataRX.ptICbRxCtx = &l_tCtxAdapterRx;
    l_tInitDataRX.tIRxTim.ptTimCtx = &l_tCtxAdapterTim;
    l_tInitDataRX.tIRxTim.fTimStart = &eFSP_MSGRXMSGTXTST_timStartRx;
    l_tInitDataRX.tIRxTim.fTimGetRemain = &eFSP_MSGRXMSGTXTST_timGetRemainingRx;
    l_tInitDataRX.uITimeoutMs = 1000u;
    l_tInitDataRX.uITimePerRecMs = 100u;
    l_tInitDataRX.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&l_tCtxRX, &l_tInitDataRX) )
    {
        (void)printf("eFSP_MSGRXMSGTXTST_Common 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGRXMSGTXTST_Common 2  -- FAIL \n");
    }

    /* Fill data */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&l_tCtxTx, &l_puData, &l_uDataL) )
    {
        if( 12u == l_uDataL )
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

    l_puData[0u] = ECU_SOF;
    l_puData[1u] = ECU_ESC;
    l_puData[2u] = ECU_ESC;
    l_puData[3u] = ECU_EOF;
    l_puData[4u] = 0xFFu;
    l_puData[5u] = 0xCCu;
    l_puData[6u] = ECU_SOF;

    /* Start TX message */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&l_tCtxTx, 7u) )
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 4  -- FAIL \n");
    }

    /* Start RX message */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&l_tCtxRX) )
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 5  -- FAIL \n");
    }

    /* Send data  */
    if( e_eFSP_MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&l_tCtxTx) )
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 6  -- FAIL \n");
    }

    /* Receive data */
    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&l_tCtxRX) )
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_MSGTXTST_CornerCase 7  -- FAIL \n");
    }

    /* Get RX data */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&l_tCtxRX, &l_puData, &l_uDataL) )
    {
        if( 7u == l_uDataL )
        {
            if( (  ECU_SOF == l_puData[0u] ) || (  ECU_ESC == l_puData[1u] ) || (  ECU_ESC == l_puData[2u] ) ||
                (  ECU_EOF == l_puData[3u] ) || (  0xFFu == l_puData[4u] ) || (  0xCCu == l_puData[5u] ) ||
                (  ECU_SOF == l_puData[6u] ) )
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