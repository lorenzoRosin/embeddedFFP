/**
 * @file       eFSPMsgReceiver.h
 *
 * @brief      Message receiver utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgReceiver.h"
#include "eFSPMsgReceiver_Priv.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t isMsgReceStatusStillCoherent(const s_eFSP_MSGRX_Ctx* ctx);
static e_eFSP_MSGRX_Res convertReturnFromMSGDToMSGRX(e_eFSP_MSGD_Res returnedEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed because this warning is not working */
#endif

e_eFSP_MSGRX_Res eFSP_MSGRX_InitCtx(s_eFSP_MSGRX_Ctx* const ctx, const s_eFSP_MSGRX_InitData* initData)
{
	/* Local variable */
	e_eFSP_MSGRX_Res result;
	e_eFSP_MSGD_Res resultMsgD;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == initData ) )
	{
		result = MSGRX_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == initData->i_memArea ) || ( NULL == initData->i_receiveBuffArea ) ||
            ( NULL == initData->i_cbCrcP ) || ( NULL == initData->i_cbCrcCrx ) || ( NULL == initData->i_cbRxP ) ||
            ( NULL == initData->i_cbRxCtx ) || ( NULL == initData->i_rxTim.timerCtx ) ||
            ( NULL == initData->i_rxTim.tim_start ) || ( NULL == initData->i_rxTim.tim_getRemaining ) )
        {
            result = MSGRX_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity of data area */
            if( initData->i_receiveBuffAreaSize < 1u)
            {
                result = MSGRX_RES_BADPARAM;
            }
            else
            {
                /* Check data validity of time */
                if( ( initData->i_frameTimeoutMs < 1u ) || ( initData->i_timePerRecMs < 1u ) ||
                    ( initData->i_timePerRecMs > initData->i_frameTimeoutMs ) )
                {
                    result = MSGRX_RES_BADPARAM;
                }
                else
                {
                    /* Initialize internal status variable */
                    ctx->rxBuff = initData->i_receiveBuffArea;
                    ctx->rxBuffSize = initData->i_receiveBuffAreaSize;
                    ctx->rxBuffCntr = 0u;
                    ctx->rxBuffFill = 0u;
                    ctx->cbRxP = initData->i_cbRxP;
                    ctx->cbRxCtx = initData->i_cbRxCtx;
                    ctx->rxTim = initData->i_rxTim;
                    ctx->frameTimeoutMs = initData->i_frameTimeoutMs;
                    ctx->timePerRecMs = initData->i_timePerRecMs;
                    ctx->needWaitFrameStart = initData->i_needWaitFrameStart;

                    /* initialize internal bytestuffer */
                    resultMsgD =  eFSP_MSGD_InitCtx(&ctx->msgDecoderCtnx, initData->i_memArea, initData->i_memAreaSize,
                                               initData->i_cbCrcP, initData->i_cbCrcCrx);
                    result = convertReturnFromMSGDToMSGRX(resultMsgD);
                }
            }
        }
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3"
#endif

e_eFSP_MSGRX_Res eFSP_MSGRX_IsInit(s_eFSP_MSGRX_Ctx* const ctx, bool_t* isInit)
{
	/* Local variable */
	e_eFSP_MSGRX_Res result;
	e_eFSP_MSGD_Res resultMsgD;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == isInit ) )
	{
		result = MSGRX_RES_BADPOINTER;
	}
	else
	{
        resultMsgD = eFSP_MSGD_IsInit(&ctx->msgDecoderCtnx, isInit);
        result = convertReturnFromMSGDToMSGRX(resultMsgD);
	}

	return result;
}

e_eFSP_MSGRX_Res eFSP_MSGRX_StartNewMsg(s_eFSP_MSGRX_Ctx* const ctx)
{
	/* Local variable */
	e_eFSP_MSGRX_Res result;
	e_eFSP_MSGD_Res resultMsgD;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGRX_RES_BADPOINTER;
	}
	else
	{
        /* Check internal status validity */
        if( false == isMsgReceStatusStillCoherent(ctx) )
        {
            result = MSGRX_RES_CORRUPTCTX;
        }
        else
        {
            /* Init message encoder */
            resultMsgD = eFSP_MSGD_StartNewMsg(&ctx->msgDecoderCtnx);
            result = convertReturnFromMSGDToMSGRX(resultMsgD);

            if( MSGRX_RES_OK == result )
            {
                /* Start timer even if we need to wait SOF, this case is handled in the MSGRX_GetDecodedData */
                if( true != ctx->rxTim.tim_start( ctx->rxTim.timerCtx, ctx->frameTimeoutMs ) )
                {
                    result = MSGRX_RES_TIMCLBKERROR;
                }
            }
        }
	}

	return result;
}

e_eFSP_MSGRX_Res eFSP_MSGRX_StartNewMsgNClean(s_eFSP_MSGRX_Ctx* const ctx)
{
	/* Local variable for result */
	e_eFSP_MSGRX_Res result;
	e_eFSP_MSGD_Res resultMsgD;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGRX_RES_BADPOINTER;
	}
	else
	{
        /* Check internal status validity */
        if( false == isMsgReceStatusStillCoherent(ctx) )
        {
            result = MSGRX_RES_CORRUPTCTX;
        }
        else
        {
            /* Reset internal variable */
            ctx->rxBuffCntr = 0u;
            ctx->rxBuffFill = 0u;

            /* Init message encoder */
            resultMsgD = eFSP_MSGD_StartNewMsg(&ctx->msgDecoderCtnx);
            result = convertReturnFromMSGDToMSGRX(resultMsgD);

            if( MSGRX_RES_OK == result )
            {
                /* Start timer even if we need to wait SOF, this case is handled in the MSGRX_GetDecodedData */
                if( true != ctx->rxTim.tim_start( ctx->rxTim.timerCtx, ctx->frameTimeoutMs ) )
                {
                    result = MSGRX_RES_TIMCLBKERROR;
                }
            }
        }
	}

	return result;
}

e_eFSP_MSGRX_Res eFSP_MSGRX_GetDecodedData(s_eFSP_MSGRX_Ctx* const ctx, uint8_t** dataP, uint32_t* const retrivedLen)
{
	/* Local variable */
	e_eFSP_MSGRX_Res result;
	e_eFSP_MSGD_Res resultMsgD;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == dataP ) || ( NULL == retrivedLen ) )
	{
		result = MSGRX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgReceStatusStillCoherent(ctx) )
		{
			result = MSGRX_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
			resultMsgD = eFSP_MSGD_GetDecodedData(&ctx->msgDecoderCtnx, dataP, retrivedLen);
			result = convertReturnFromMSGDToMSGRX(resultMsgD);
		}
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed because this warning is not working */
#endif

e_eFSP_MSGRX_Res eFSP_MSGRX_ReceiveChunk(s_eFSP_MSGRX_Ctx* const ctx)
{
	/* Local variable of the operation result */
	e_eFSP_MSGRX_Res result;
	e_eFSP_MSGD_Res resultMsgD;

    /* Local variable to keep track of the current state machine state */
    e_eFSP_MSGRX_Priv_state stateM;

    /* Local variable used for the current time calculation */
    uint32_t sRemRxTime;
    uint32_t cRemainRxTime;
    uint32_t elapsedFromStart;
    uint32_t sSessionRemanTime;
    uint32_t cSessionRemanTime;

    bool_t isMsgDec;
    bool_t isInit;

    /* Local variable usend for the current data calculation */
    uint8_t *cDToRxP;
    uint32_t cDToRxLen;
    uint32_t cDRxed;

    /* Other local variable */
    uint32_t rxMostEff;
    bool_t isWaitingSof;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGRX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgReceStatusStillCoherent(ctx) )
		{
			result = MSGRX_RES_CORRUPTCTX;
		}
		else
		{
            /* Init time frame counter */
            result = MSGRX_RES_OK;
            stateM = MSGRX_PRV_CHECKINIT;

            /* Init data */
            sRemRxTime = 0u;
            cSessionRemanTime = 0u;
            sSessionRemanTime = 0u;
            rxMostEff = 0u;
            cDRxed = 0u;
            isMsgDec = false;
            isWaitingSof = false;

            /* wait end elaboration or end for timeout */
            while( stateM != MSGRX_PRV_ELABDONE )
            {
                switch( stateM )
                {
                    case MSGRX_PRV_CHECKINIT:
                    {
                        /* Check if lib is initialized */
                        isInit = false;
                        resultMsgD = eFSP_MSGD_IsInit(&ctx->msgDecoderCtnx, &isInit);
                        result = convertReturnFromMSGDToMSGRX(resultMsgD);

                        /* Check if any error is returned */
                        if( MSGRX_RES_OK == result )
                        {
                            if( true == isInit )
                            {
                                /* All ok */
                                stateM = MSGRX_PRV_CHECKINITTIMEOUT;
                            }
                            else
                            {
                                /* Need to init the lib before */
                                result = MSGRX_RES_NOINITLIB;
                                stateM = MSGRX_PRV_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            stateM = MSGRX_PRV_ELABDONE;
                        }
                        break;
                    }

                    case MSGRX_PRV_CHECKINITTIMEOUT:
                    {
                        /* Check if frame timeout is eplased */
                        if( true == ctx->rxTim.tim_getRemaining(ctx->rxTim.timerCtx, &sRemRxTime) )
                        {
                            /* Check also if we are still waiting start of frame to be received */
                            resultMsgD =  eFSP_MSGD_IsWaitingSof(&ctx->msgDecoderCtnx, &isWaitingSof);
                            result = convertReturnFromMSGDToMSGRX(resultMsgD);

                            if( MSGRX_RES_OK == result )
                            {
                                if( true == ctx->needWaitFrameStart )
                                {
                                    /* Need to wait SOF */
                                    if( true == isWaitingSof )
                                    {
                                        /* We are waiting start of frame, reset timer */
                                        if( true == ctx->rxTim.tim_start( ctx->rxTim.timerCtx, ctx->frameTimeoutMs ) )
                                        {
                                            /* Ok restarted the timer */
                                            sRemRxTime = ctx->frameTimeoutMs;
                                            cSessionRemanTime = ctx->timePerRecMs;
                                            sSessionRemanTime = ctx->timePerRecMs;

                                            /* check if we have some data to receive in RX buffer */
                                            stateM = MSGRX_PRV_CHECKHOWMANYDATA;
                                        }
                                        else
                                        {
                                            /* Some error */
                                            result = MSGRX_RES_TIMCLBKERROR;
                                            stateM = MSGRX_PRV_ELABDONE;
                                        }
                                    }
                                    else
                                    {
                                        /* Frame already started */
                                        if( sRemRxTime <= 0u )
                                        {
                                            /* Time elapsed */
                                            result = MSGRX_RES_MESSAGETIMEOUT;
                                            stateM = MSGRX_PRV_ELABDONE;
                                        }
                                        else
                                        {
                                            /* Frame timeout is not elapsed, calculate frame session timeout */
                                            if( sRemRxTime >= ctx->timePerRecMs )
                                            {
                                                /* Time elapsed */
                                                cSessionRemanTime = ctx->timePerRecMs;
                                                sSessionRemanTime = ctx->timePerRecMs;
                                            }
                                            else
                                            {
                                                /* Session timeout not elapsed, can receive data */
                                                cSessionRemanTime = sRemRxTime;
                                                sSessionRemanTime = sRemRxTime;
                                            }

                                            /* check if we have some data to receive in RX buffer */
                                            stateM = MSGRX_PRV_CHECKHOWMANYDATA;
                                        }
                                    }
                                }
                                else
                                {
                                    /* No need to wait SOF, timer start con receiveStart function call */
                                    if( sRemRxTime <= 0u )
                                    {
                                        /* Time elapsed */
                                        result = MSGRX_RES_MESSAGETIMEOUT;
                                        stateM = MSGRX_PRV_ELABDONE;
                                    }
                                    else
                                    {
                                        /* Frame timeout is not elapsed, calculate frame session timeout */
                                        if( sRemRxTime >= ctx->timePerRecMs )
                                        {
                                            /* Time elapsed */
                                            cSessionRemanTime = ctx->timePerRecMs;
                                            sSessionRemanTime = ctx->timePerRecMs;
                                        }
                                        else
                                        {
                                            /* Session timeout not elapsed, can receive data */
                                            cSessionRemanTime = sRemRxTime;
                                            sSessionRemanTime = sRemRxTime;
                                        }

                                        /* check if we have some data to receive in RX buffer */
                                        stateM = MSGRX_PRV_CHECKHOWMANYDATA;
                                    }
                                }
                            }
                            else
                            {
                                /* Some error */
                                stateM = MSGRX_PRV_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error on timer */
                            result = MSGRX_RES_TIMCLBKERROR;
                            stateM = MSGRX_PRV_ELABDONE;
                        }
                        break;
                    }

                    case MSGRX_PRV_CHECKHOWMANYDATA:
                    {
                        /* How many byte do we need to receive? */
                        resultMsgD = eFSP_MSGD_GetMostEffDatLen(&ctx->msgDecoderCtnx, &rxMostEff);
                        result = convertReturnFromMSGDToMSGRX(resultMsgD);
                        if( MSGRX_RES_OK == result )
                        {
                            if( rxMostEff > 0u )
                            {
                                /* Ok we need some data to be retrived */
                                stateM = MSGRX_PRV_CHECKIFBUFFERRX;

                                /* Check compatibility with rx buffer dimension */
                                if( rxMostEff > ctx->rxBuffSize )
                                {
                                    /* In this way we dont' have any overflow */
                                    rxMostEff = ctx->rxBuffSize;
                                }
                            }
                            else
                            {
                                /* Could be because the message is received or an error occoured */
                                resultMsgD = eFSP_MSGD_IsAFullMsgDecoded(&ctx->msgDecoderCtnx, &isMsgDec);
                                result = convertReturnFromMSGDToMSGRX(resultMsgD);

                                if( MSGRX_RES_OK == result )
                                {
                                    if( true == isMsgDec )
                                    {
                                        /* Ok the message is correct */
                                        stateM = MSGRX_PRV_ELABDONE;
                                        result = MSGRX_RES_MESSAGERECEIVED;
                                    }
                                    else
                                    {
                                        /* Umm the message is not correct */
                                        stateM = MSGRX_PRV_ELABDONE;
                                        result = MSGRX_RES_BADFRAME;
                                    }
                                }
                                else
                                {
                                    /* Some error */
                                    stateM = MSGRX_PRV_ELABDONE;
                                }
                            }
                        }
                        else
                        {
                            /* Some error */
                            stateM = MSGRX_PRV_ELABDONE;
                        }
                        break;
                    }

                    case MSGRX_PRV_CHECKIFBUFFERRX:
                    {
                        /* Is needed data already present in the receive buffer? */
                        cDToRxLen = ctx->rxBuffFill - ctx->rxBuffCntr;

                        if( cDToRxLen > 0u )
                        {
                            /* Can parse already received data in msg buffer */
                            stateM = MSGRX_PRV_INSERTCHUNK;
                        }
                        else
                        {
                            /* No data in msg buffer, retrive some other chunk of data */
                            ctx->rxBuffCntr = 0u;
                            ctx->rxBuffFill = 0u;
                            stateM = MSGRX_PRV_RECEIVE_BUFF;
                        }
                        break;
                    }

                    case MSGRX_PRV_RECEIVE_BUFF:
                    {
                        /* Ok, rx buffer is empty and need to be filled with data that we can parse */
                        if( true == (*ctx->cbRxP)(ctx->cbRxCtx, ctx->rxBuff, &ctx->rxBuffFill, rxMostEff,
                                                  cSessionRemanTime) )
                        {
                            /* Check for some strangeness */
                            if( ctx->rxBuffFill > rxMostEff )
                            {
                                result = MSGRX_RES_CORRUPTCTX;
                                stateM = MSGRX_PRV_ELABDONE;
                            }
                            else
                            {
                                /* Update received counter */
                                ctx->rxBuffCntr = 0u;

                                /* Go next state */
                                stateM = MSGRX_PRV_INSERTCHUNK;
                            }
                        }
                        else
                        {
                            /* Error sending data */
                            result = MSGRX_RES_RXCLBKERROR;
                            stateM = MSGRX_PRV_ELABDONE;
                        }

                        break;
                    }

                    case MSGRX_PRV_INSERTCHUNK:
                    {
                        /* Ok, we have some data in RX buffer */
                        cDToRxP = &ctx->rxBuff[ctx->rxBuffCntr];
                        cDToRxLen = ctx->rxBuffFill - ctx->rxBuffCntr;

                        if( cDToRxLen > 0u )
                        {
                            /* We can try to decode data event if we already finished cuz the function
                            * MSGD_InsEncChunk is well maden */
                            resultMsgD = eFSP_MSGD_InsEncChunk(&ctx->msgDecoderCtnx, cDToRxP, cDToRxLen, &cDRxed );
                            result = convertReturnFromMSGDToMSGRX(resultMsgD);

                            if( MSGRX_RES_OK == result )
                            {
                                /* Retrived some data, by design if MSGD_InsEncChunk return MSGE_RES_OK this
                                 * means that the value of loaded data inside send buffer is equals to it's size */
                                ctx->rxBuffCntr = 0u;
                                ctx->rxBuffFill = 0u;

                                /* Check for timeout */
                                stateM = MSGRX_PRV_CHECKTIMEOUTAFTERRX;
                            }
                            else if( MSGRX_RES_MESSAGERECEIVED == result )
                            {
                                /* Ok we retrived all the possible data */
                                /* Update RX buffer */
                                ctx->rxBuffCntr += cDRxed;

                                /* Check for timeout */
                                stateM = MSGRX_PRV_CHECKTIMEOUTAFTERRX;
                            }
                            else if( MSGRX_RES_BADFRAME == result )
                            {
                                /* Update RX buffer */
                                ctx->rxBuffCntr += cDRxed;

                                /* Bad frame, but check timeout also */
                                stateM = MSGRX_PRV_CHECKTIMEOUTAFTERRX;
                            }
                            else if( MSGRX_RES_FRAMERESTART == result )
                            {
                                /* Update RX buffer */
                                ctx->rxBuffCntr += cDRxed;

                                 /* ok frame restarted, but check timeout also */
                                stateM = MSGRX_PRV_CHECKTIMEOUTAFTERRX;
                            }
                            else
                            {
                                /* Update RX buffer */
                                ctx->rxBuffCntr += cDRxed;

                                /* Some error, can return */
                                stateM = MSGRX_PRV_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Didn't receive data in the previous MSGRX_PRV_RECEIVE_BUFF, check timeout */
                            result = MSGRX_RES_OK;
                            stateM = MSGRX_PRV_CHECKTIMEOUTAFTERRX;
                        }

                        break;
                    }

                    case MSGRX_PRV_CHECKTIMEOUTAFTERRX:
                    {
                        /* Check if frame timeout is eplased */
                        if( true == ctx->rxTim.tim_getRemaining(ctx->rxTim.timerCtx, &cRemainRxTime) )
                        {
                            /* Check time validity */
                            if( cRemainRxTime > sRemRxTime )
                            {
                                /* It's not possible to have more time to send the frame now than during
                                * the begining */
                                result = MSGRX_RES_CORRUPTCTX;
                                stateM = MSGRX_PRV_ELABDONE;
                            }
                            else
                            {
                                /* Calc elapsed time */
                                elapsedFromStart = sRemRxTime - cRemainRxTime;

                                /* Are we waiting SOF? */
                                resultMsgD =  eFSP_MSGD_IsWaitingSof(&ctx->msgDecoderCtnx, &isWaitingSof);
                                if( MSGD_RES_OK == resultMsgD )
                                {
                                    /* Do we need to wait start of frame? */
                                    if( ( true == ctx->needWaitFrameStart ) && ( MSGRX_RES_FRAMERESTART == result ) )
                                    {
                                        /* Timeout dosent occour in this situation */
                                        stateM = MSGRX_PRV_ELABDONE;

                                        /* Frame restarted, restart the timer */
                                        if( true == ctx->rxTim.tim_start( ctx->rxTim.timerCtx, ctx->frameTimeoutMs ) )
                                        {
                                            /* Ok restarted the timer */
                                            sRemRxTime = ctx->frameTimeoutMs;

                                            /* Dosent care about session imeout because we are returning in any case */
                                        }
                                        else
                                        {
                                            /* Some error */
                                            result = MSGRX_RES_TIMCLBKERROR;
                                        }
                                    }
                                    else if( ( true == ctx->needWaitFrameStart ) && ( true == isWaitingSof ) &&
                                             ( MSGRX_RES_OK == result ) )
                                    {
                                        /* In this case total time dosen't need to be decreased, only the session */
                                        /* Frame restarted, restart the timer */
                                        if( true == ctx->rxTim.tim_start( ctx->rxTim.timerCtx, ctx->frameTimeoutMs ) )
                                        {
                                            /* Ok restarted the timer */
                                            sRemRxTime = ctx->frameTimeoutMs;
                                            cRemainRxTime = ctx->frameTimeoutMs;

                                            /* Update session timeout */
                                            if( elapsedFromStart >= sSessionRemanTime )
                                            {
                                                /* No more time */
                                                stateM = MSGRX_PRV_ELABDONE;
                                            }
                                            else
                                            {
                                                /* Update remaining session time */
                                                sSessionRemanTime = sSessionRemanTime - elapsedFromStart;
                                                cSessionRemanTime = sSessionRemanTime;

                                                /* Can retrive more data */
                                                stateM = MSGRX_PRV_CHECKHOWMANYDATA;
                                            }
                                        }
                                        else
                                        {
                                            /* Some error */
                                            result = MSGRX_RES_TIMCLBKERROR;
											stateM = MSGRX_PRV_ELABDONE;
                                        }
                                    }
                                    else
                                    {
                                        /* Check for timeout */
                                        if( cRemainRxTime <= 0u )
                                        {
                                            /* Time elapsed */
                                            result = MSGRX_RES_MESSAGETIMEOUT;
                                            stateM = MSGRX_PRV_ELABDONE;
                                        }
                                        else
                                        {
                                            /* Frame timeout is not elapsed, check current session if expired */
                                            if( elapsedFromStart >= sSessionRemanTime )
                                            {
                                                /* Session time is elapsed, can return the previous result */
                                                stateM = MSGRX_PRV_ELABDONE;
                                            }
                                            else
                                            {
                                                /* Session timeout not elapsed, if needed can do more elaboration */
                                                cSessionRemanTime = sSessionRemanTime - elapsedFromStart;

                                                /* Check what to do looking at the previous state result */
                                                if( MSGRX_RES_OK == result )
                                                {
                                                    /* Can retrive more data */
                                                    stateM = MSGRX_PRV_CHECKHOWMANYDATA;
                                                }
                                                else if( MSGRX_RES_MESSAGERECEIVED == result )
                                                {
                                                    /* Ok we retrived all the possible data */
                                                    stateM = MSGRX_PRV_ELABDONE;
                                                }
                                                else if( MSGRX_RES_BADFRAME == result )
                                                {
                                                    /* Bad frame */
                                                    stateM = MSGRX_PRV_ELABDONE;
                                                }
                                                else if( MSGRX_RES_FRAMERESTART == result )
                                                {
                                                    /* ok frame restarted */
                                                    stateM = MSGRX_PRV_ELABDONE;
                                                }
                                                else
                                                {
                                                    /* Some error, But was already handled by previous state */
                                                    result = MSGRX_RES_CORRUPTCTX;
                                                    stateM = MSGRX_PRV_ELABDONE;
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    /* Some error */
                                    stateM = MSGRX_PRV_ELABDONE;
                                    result = convertReturnFromMSGDToMSGRX(resultMsgD);
                                }
                            }
                        }
                        else
                        {
                            /* Some error on timer */
                            result = MSGRX_RES_TIMCLBKERROR;
                            stateM = MSGRX_PRV_ELABDONE;
                        }
                        break;
                    }

                    default:
                    {
                        /* Impossible end here */
                        stateM = MSGRX_PRV_ELABDONE;
                        result = MSGRX_RES_CORRUPTCTX;
                        break;
                    }
                }
            }
		}
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3"
#endif

/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
bool_t isMsgReceStatusStillCoherent(const s_eFSP_MSGRX_Ctx* ctx)
{
    bool_t result;

	/* Check pointer validity */
	if( ( NULL == ctx->rxBuff ) || ( NULL == ctx->cbRxP ) || ( NULL == ctx->cbRxCtx ) ||
        ( NULL == ctx->rxTim.timerCtx ) || ( NULL == ctx->rxTim.tim_start ) ||
        ( NULL == ctx->rxTim.tim_getRemaining ) )
	{
		result = false;
	}
	else
	{
        /* Check send buffer validity */
        if( ( ctx->rxBuffSize < 1u ) || ( ctx->rxBuffFill > ctx->rxBuffSize )  ||
            ( ctx->rxBuffCntr > ctx->rxBuffFill ) )
        {
            result =  false;
        }
        else
        {
            /* Check timings validity */
            if( ( ctx->frameTimeoutMs < 1u ) || ( ctx->timePerRecMs < 1u ) ||
                ( ctx->timePerRecMs > ctx->frameTimeoutMs ) )
            {
                result = false;
            }
            else
            {
                result = true;
            }
        }
	}

    return result;
}

e_eFSP_MSGRX_Res convertReturnFromMSGDToMSGRX(e_eFSP_MSGD_Res returnedEvent)
{
	e_eFSP_MSGRX_Res result;

	switch( returnedEvent )
	{
		case MSGD_RES_OK:
		{
			result = MSGRX_RES_OK;
            break;
		}

		case MSGD_RES_BADPARAM:
		{
			result = MSGRX_RES_BADPARAM;
            break;
		}

		case MSGD_RES_BADPOINTER:
		{
			result = MSGRX_RES_BADPOINTER;
            break;
		}

		case MSGD_RES_CORRUPTCTX:
		{
			result = MSGRX_RES_CORRUPTCTX;
            break;
		}

        case MSGD_RES_OUTOFMEM:
        {
			result = MSGRX_RES_OUTOFMEM;
            break;
        }

        case MSGD_RES_BADFRAME:
        {
			result = MSGRX_RES_BADFRAME;
            break;
        }

		case MSGD_RES_MESSAGEENDED:
		{
			result = MSGRX_RES_MESSAGERECEIVED;
            break;
		}

		case MSGD_RES_FRAMERESTART:
		{
			result = MSGRX_RES_FRAMERESTART;
            break;
		}

		case MSGD_RES_NOINITLIB:
		{
			result = MSGRX_RES_NOINITLIB;
            break;
		}

		case MSGD_RES_CRCCLBKERROR :
		{
			result = MSGRX_RES_CRCCLBKERROR;
            break;
		}

		default:
		{
            /* Impossible end here */
			result = MSGRX_RES_CORRUPTCTX;
            break;
		}
	}

	return result;
}