/**
 * @file       eFSPMsgRxTransmitterTest.h
 *
 * @brief      Message Tx Rx test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgRxTransmitterTest.h"
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
                            "CERT-INT30-C_b"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t* d, const uint32_t dLen, uint32_t* const c32Val);

static bool_t sendMsg( void* cntx, const uint8_t* dataToSend, const uint32_t dataToSendLen, uint32_t* const dataSended,
                       const uint32_t timeToSend );

static bool_t receiveMsg( void* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                       const uint32_t timeToRx );

static bool_t timStart ( void* cntx, const uint32_t timeoutVal );
static bool_t timGetRemaining ( void* cntx, uint32_t* const remainings );

/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
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
    uint32_t tim_remainingTime;
    uint32_t tim_timCnt;
    uint32_t tim_timCntInc;
    bool sendIsError;
}s_eCU_timerAdapterCtx;

static void msgRxTransmitterTestCommon(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void eFSP_TEST_msgRxTransmitter(void)
{
	(void)printf("\n\nMESSAGE RECEIVER TRANSMITTER TEST START \n\n");

    msgRxTransmitterTestCommon();

    (void)printf("\n\nMESSAGE RECEIVER TRANSMITTER TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION IMPLEMENTATION
 **********************************************************************************************************************/
static uint8_t m_BufferCom[1000u];
static uint32_t m_BufferComCounterInsert;
static uint32_t m_BufferComCounterRetrive;

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


bool_t receiveMsg( void* cntx, uint8_t* dataToRx, uint32_t* const dataRecevd, const uint32_t dataRxMax,
                const uint32_t timeToRx )
{
    bool_t result;
    s_eCU_msgSendAdapterCtx* ctxCur;
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
            ctxCur = (s_eCU_msgSendAdapterCtx*)cntx;
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
        ctxCur->tim_timCnt = 0u;
        ctxCur->tim_timCntInc = 0u;
        ctxCur->tim_remainingTime = timeoutVal;
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
void msgRxTransmitterTestCommon(void)
{
    /* Init static */
    m_BufferComCounterInsert = 0u;
    m_BufferComCounterRetrive = 0u;

    /* Local variable RECEIVER */
    s_eFSP_MSGRX_Ctx ctxRX;
    s_eFSP_MSGRX_InitData initDataRX;
    s_eCU_crcAdapterCtx ctxAdapterCrcRX;
    s_eCU_msgSendAdapterCtx ctxAdapterRx;
    s_eCU_timerAdapterCtx ctxAdapterTim;
    uint8_t  memAreaRX[20u];
    uint8_t  recBuff[1u];

    /* Local variable TRANSMITTER */
    s_eFSP_MSGTX_Ctx ctxTx;
    s_eFSP_MSGTX_InitData initDataTx;
    s_eCU_crcAdapterCtx ctxAdapterCrcTx;
    s_eCU_msgSendAdapterCtx ctxAdapterSend;
    s_eCU_timerAdapterCtx ctxAdapterTimTx;
    uint8_t  memAreaTx[20u];
    uint8_t  sendBuff[1u];

    /* Common var */
    uint8_t* dataP;
    uint32_t dataL;

    /* Clear */
    (void)memset(&ctxTx, 0, sizeof(s_eFSP_MSGTX_Ctx));
    (void)memset(&initDataTx, 0, sizeof(s_eFSP_MSGTX_InitData));
    (void)memset(&ctxRX, 0, sizeof(s_eFSP_MSGRX_Ctx));
    (void)memset(&initDataRX, 0, sizeof(s_eFSP_MSGRX_InitData));

    /* Init */
    initDataTx.p_i_memArea = memAreaTx;
    initDataTx.i_memAreaSize = sizeof(memAreaTx);
    initDataTx.p_i_txBuffArea = sendBuff;
    initDataTx.i_txBuffAreaSize = sizeof(sendBuff);
    initDataTx.f_i_Crc = &c32SAdapt;
    initDataTx.p_i_cbCrcCtx = &ctxAdapterCrcTx;
    initDataTx.f_i_Tx = &sendMsg;
    initDataTx.p_i_cbTxCtx = &ctxAdapterSend;
    initDataTx.i_txTim.p_timCtx = &ctxAdapterTimTx;
    initDataTx.i_txTim.f_timStart = &timStart;
    initDataTx.i_txTim.f_timGetRemaining = &timGetRemaining;
    initDataTx.i_timeoutMs = 1000u;
    initDataTx.i_timePerSendMs = 100u;

    if( MSGTX_RES_OK == eFSP_MSGTX_InitCtx(&ctxTx, &initDataTx) )
    {
        (void)printf("msgRxTransmitterTestCommon 1  -- OK \n");
    }
    else
    {
        (void)printf("msgRxTransmitterTestCommon 1  -- FAIL \n");
    }

    initDataRX.p_i_memArea = memAreaRX;
    initDataRX.i_memAreaSize = sizeof(memAreaRX);
    initDataRX.p_i_rxBuffArea = recBuff;
    initDataRX.i_rxBuffAreaSize = sizeof(recBuff);
    initDataRX.f_i_Crc = &c32SAdapt;
    initDataRX.p_i_cbCrcCtx = &ctxAdapterCrcRX;
    initDataRX.f_i_Rx = &receiveMsg;
    initDataRX.p_i_cbRxCtx = &ctxAdapterRx;
    initDataRX.i_rxTim.p_timCtx = &ctxAdapterTim;
    initDataRX.i_rxTim.f_timStart = &timStart;
    initDataRX.i_rxTim.f_timGetRemaining = &timGetRemaining;
    initDataRX.i_timeoutMs = 1000u;
    initDataRX.i_timePerRecMs = 100u;
    initDataRX.i_needWaitFrameStart = true;
    if( MSGRX_RES_OK == eFSP_MSGRX_InitCtx(&ctxRX, &initDataRX) )
    {
        (void)printf("msgRxTransmitterTestCommon 2  -- OK \n");
    }
    else
    {
        (void)printf("msgRxTransmitterTestCommon 2  -- FAIL \n");
    }

    /* Fill data */
    if( MSGTX_RES_OK == eFSP_MSGTX_GetWherePutData(&ctxTx, &dataP, &dataL) )
    {
        if( 12u == dataL )
        {
            (void)printf("msgRxTransmitterTestCommon 3  -- OK \n");
        }
        else
        {
            (void)printf("msgRxTransmitterTestCommon 3  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgRxTransmitterTestCommon 3  -- FAIL \n");
    }

    dataP[0u] = ECU_SOF;
    dataP[1u] = ECU_ESC;
    dataP[2u] = ECU_ESC;
    dataP[3u] = ECU_EOF;
    dataP[4u] = 0xFFu;
    dataP[5u] = 0xCCu;
    dataP[6u] = ECU_SOF;

    /* Start TX message */
    if( MSGTX_RES_OK == eFSP_MSGTX_NewMessage(&ctxTx, 7u) )
    {
        (void)printf("msgTransmitterTestCornerCase 4  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 4  -- FAIL \n");
    }

    /* Start RX message */
    if( MSGRX_RES_OK == eFSP_MSGRX_NewMsg(&ctxRX) )
    {
        (void)printf("msgTransmitterTestCornerCase 5  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 5  -- FAIL \n");
    }

    /* Send data  */
    if( MSGTX_RES_MESSAGESENDED == eFSP_MSGTX_SendChunk(&ctxTx) )
    {
        (void)printf("msgTransmitterTestCornerCase 6  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 6  -- FAIL \n");
    }

    /* Receive data */
    if( MSGRX_RES_MESSAGERECEIVED == eFSP_MSGRX_ReceiveChunk(&ctxRX) )
    {
        (void)printf("msgTransmitterTestCornerCase 7  -- OK \n");
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 7  -- FAIL \n");
    }

    /* Get RX data */
    if( MSGRX_RES_OK == eFSP_MSGRX_GetDecodedData(&ctxRX, &dataP, &dataL) )
    {
        if( 7u == dataL )
        {
            if( (  ECU_SOF == dataP[0u] ) || (  ECU_ESC == dataP[1u] ) || (  ECU_ESC == dataP[2u] ) ||
                (  ECU_EOF == dataP[3u] ) || (  0xFFu == dataP[4u] ) || (  0xCCu == dataP[5u] ) ||
                (  ECU_SOF == dataP[6u] ) )
            {
                (void)printf("msgTransmitterTestCornerCase 8  -- OK \n");
            }
            else
            {
                (void)printf("msgTransmitterTestCornerCase 8  -- FAIL \n");
            }
        }
        else
        {
            (void)printf("msgTransmitterTestCornerCase 8  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("msgTransmitterTestCornerCase 8  -- FAIL \n");
    }
}



#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3", "CERT-STR32-C", "MISRAC2012-Rule-11.5", "CERT-EXP36-C_b", \
                            "CERT-INT30-C_b"
#endif