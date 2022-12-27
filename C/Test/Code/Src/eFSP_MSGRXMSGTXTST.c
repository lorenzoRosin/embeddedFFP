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
static bool_t c32SAdaptRx(t_eFSP_MSGD_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptTx(t_eFSP_MSGE_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);

static bool_t sendMsg( t_eFSP_MSGTX_TxCtx* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                       const uint32_t timeToSend );

static bool_t receiveMsg( t_eFSP_MSGRX_RxCtx* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                       const uint32_t timeToRx );

static bool_t timStart ( t_eFSP_MSGTX_TimCtx* cntx, const uint32_t timeoutVal );
static bool_t timGetRemaining ( t_eFSP_MSGTX_TimCtx* cntx, uint32_t* const remainings );
static bool_t timStartRx ( t_eFSP_MSGRX_TimCtx* cntx, const uint32_t timeoutVal );
static bool_t timGetRemainingRx ( t_eFSP_MSGRX_TimCtx* cntx, uint32_t* const remainings );

/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
struct t_eFSP_MSGD_CrcCtxUser
{
    e_eCU_CRC_RES lastError;
};

struct t_eFSP_MSGE_CrcCtxUser
{
    e_eCU_CRC_RES lastError;
};

struct t_eFSP_MSGRX_RxCtxUser
{
    bool sendIsError;
};

struct t_eFSP_MSGRX_TimCtxUser
{
    uint32_t tim_remainingTime;
    uint32_t tim_timCnt;
    uint32_t tim_timCntInc;
    bool sendIsError;
};

struct t_eFSP_MSGTX_TxCtxUser
{
    bool sendIsError;
};

struct t_eFSP_MSGTX_TimCtxUser
{
    uint32_t tim_remainingTime;
    uint32_t tim_timCnt;
    uint32_t tim_timCntInc;
    bool sendIsError;
};

static void eFSP_TEST_msgRxTransmitterCommon(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_TEST_msgRxTransmitter(void)
{
	(void)printf("\n\nMESSAGE RECEIVER TRANSMITTER TEST START \n\n");

    eFSP_TEST_msgRxTransmitterCommon();

    (void)printf("\n\nMESSAGE RECEIVER TRANSMITTER TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION IMPLEMENTATION
 **********************************************************************************************************************/
static uint8_t m_BufferCom[1000u];
static uint32_t m_BufferComCounterInsert;
static uint32_t m_BufferComCounterRetrive;

bool_t c32SAdaptRx(t_eFSP_MSGD_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val)
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

bool_t c32SAdaptTx(t_eFSP_MSGE_CrcCtx* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val)
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

        if( ( m_BufferComCounterInsert + dataToSendLen ) < sizeof( m_BufferCom ) )
        {
            (void)memcpy(&m_BufferCom[m_BufferComCounterInsert], dataToSend, dataToSendLen);
            m_BufferComCounterInsert += dataToSendLen;
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


bool_t receiveMsg( t_eFSP_MSGRX_RxCtx* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
{
    bool_t result;
    t_eFSP_MSGRX_RxCtx* ctxCur;
    uint32_t dataAvaiable;

    if( ( NULL == cntx ) || ( NULL == dataToRx ) || ( NULL == dataRecevd ) )
    {
        result = false;
    }
    else
    {
        if( m_BufferComCounterRetrive > m_BufferComCounterInsert)
        {
            /* Impossible */
            result = false;
            ctxCur->sendIsError = true;
        }
        else
        {
            result = true;
            ctxCur = (t_eFSP_MSGRX_RxCtx*)cntx;
            ctxCur->sendIsError = false;
            dataAvaiable = m_BufferComCounterInsert - m_BufferComCounterRetrive;

            if( 0u == dataAvaiable )
            {
                *dataRecevd = dataAvaiable;
            }
            else if( dataAvaiable > dataRxMax )
            {
                *dataRecevd = dataRxMax;
                (void)memcpy(dataToRx, &m_BufferCom[m_BufferComCounterRetrive], dataRxMax );
                m_BufferComCounterRetrive += dataRxMax;
            }
            else
            {
                *dataRecevd = dataAvaiable;
                (void)memcpy(dataToRx, &m_BufferCom[m_BufferComCounterRetrive], dataAvaiable );
                m_BufferComCounterRetrive += dataAvaiable;
            }

        }
    }

    return result;
}

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
        ctxCur->tim_timCnt = 0u;
        ctxCur->tim_timCntInc = 0u;
        ctxCur->tim_remainingTime = timeoutVal;
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

        if( ctxCur->tim_timCnt < MAX_UINT32VAL )
        {
            ctxCur->tim_timCnt++;
        }

        if( ctxCur->tim_remainingTime > 0u )
        {
            ctxCur->tim_remainingTime--;
        }

        *remainings = ctxCur->tim_remainingTime;
        result = true;
    }

    return result;
}


bool_t timStartRx ( t_eFSP_MSGRX_TimCtx* cntx, const uint32_t timeoutVal )
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
        ctxCur->tim_timCnt = 0u;
        ctxCur->tim_timCntInc = 0u;
        ctxCur->tim_remainingTime = timeoutVal;
        result = true;
    }

    return result;
}

bool_t timGetRemainingRx ( t_eFSP_MSGRX_TimCtx* cntx, uint32_t* const remainings )
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

        if( ctxCur->tim_timCnt < MAX_UINT32VAL )
        {
            ctxCur->tim_timCnt++;
        }

        if( ctxCur->tim_remainingTime > 0u )
        {
            ctxCur->tim_remainingTime--;
        }

        *remainings = ctxCur->tim_remainingTime;
        result = true;
    }

    return result;
}

/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void eFSP_TEST_msgRxTransmitterCommon(void)
{
    /* Init static */
    m_BufferComCounterInsert = 0u;
    m_BufferComCounterRetrive = 0u;

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
    initDataTx.fICrc = &c32SAdaptTx;
    initDataTx.ptICbCrcCtx = &ctxAdapterCrcTx;
    initDataTx.fITx = &sendMsg;
    initDataTx.ptICbTxCtx = &ctxAdapterSend;
    initDataTx.tITxTim.ptTimCtx = &ctxAdapterTimTx;
    initDataTx.tITxTim.fTimStart = &timStart;
    initDataTx.tITxTim.fTimGetRemain = &timGetRemaining;
    initDataTx.uITimeoutMs = 1000u;
    initDataTx.uITimePerSendMs = 100u;

    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctxTx, &initDataTx) )
    {
        (void)printf("eFSP_TEST_msgRxTransmitterCommon 1  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgRxTransmitterCommon 1  -- FAIL \n");
    }

    initDataRX.puIMemArea = memAreaRX;
    initDataRX.uIMemAreaL = sizeof(memAreaRX);
    initDataRX.puIRxBuffArea = recBuff;
    initDataRX.uIRxBuffAreaL = sizeof(recBuff);
    initDataRX.fICrc = &c32SAdaptRx;
    initDataRX.ptICbCrcCtx = &ctxAdapterCrcRX;
    initDataRX.fIRx = &receiveMsg;
    initDataRX.ptICbRxCtx = &ctxAdapterRx;
    initDataRX.tIRxTim.ptTimCtx = &ctxAdapterTim;
    initDataRX.tIRxTim.fTimStart = &timStartRx;
    initDataRX.tIRxTim.fTimGetRemain = &timGetRemainingRx;
    initDataRX.uITimeoutMs = 1000u;
    initDataRX.uITimePerRecMs = 100u;
    initDataRX.bINeedWaitFrameStart = true;
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctxRX, &initDataRX) )
    {
        (void)printf("eFSP_TEST_msgRxTransmitterCommon 2  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgRxTransmitterCommon 2  -- FAIL \n");
    }

    /* Fill data */
    if( e_eFSP_MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctxTx, &dataP, &dataL) )
    {
        if( 12u == dataL )
        {
            (void)printf("eFSP_TEST_msgRxTransmitterCommon 3  -- OK \n");
        }
        else
        {
            (void)printf("eFSP_TEST_msgRxTransmitterCommon 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgRxTransmitterCommon 3  -- FAIL \n");
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
        (void)printf("eFSP_TEST_msgTransmitterCornerCase 4  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase 4  -- FAIL \n");
    }

    /* Start RX message */
    if( e_eFSP_MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctxRX) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase 5  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase 5  -- FAIL \n");
    }

    /* Send data  */
    if( e_eFSP_MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctxTx) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase 6  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase 6  -- FAIL \n");
    }

    /* Receive data */
    if( e_eFSP_MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctxRX) )
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase 7  -- OK \n");
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase 7  -- FAIL \n");
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
                (void)printf("eFSP_TEST_msgTransmitterCornerCase 8  -- OK \n");
            }
            else
            {
                (void)printf("eFSP_TEST_msgTransmitterCornerCase 8  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("eFSP_TEST_msgTransmitterCornerCase 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("eFSP_TEST_msgTransmitterCornerCase 8  -- FAIL \n");
    }
}



#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "CERT-INT30-C_b"
#endif