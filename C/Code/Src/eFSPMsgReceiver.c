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
static bool_t eFSP_MSGRX_isStatusStillCoherent(const s_eFSP_MSGRX_Ctx* p_ctx);
static e_eFSP_MSGRX_Res eFSP_MSGRX_convertReturnFromMSGD(e_eFSP_MSGD_Res returnedEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed because this warning is not working */
#endif

e_eFSP_MSGRX_Res eFSP_MSGRX_InitCtx(s_eFSP_MSGRX_Ctx* const p_ctx, const s_eFSP_MSGRX_InitData* p_initData)
{
	/* Local variable */
	e_eFSP_MSGRX_Res l_result;
	e_eFSP_MSGD_Res l_resultMsgD;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_initData ) )
	{
		l_result = MSGRX_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_initData->p_i_memArea ) || ( NULL == p_initData->p_i_receiveBuffArea ) ||
            ( NULL == p_initData->f_i_Crc ) || ( NULL == p_initData->p_i_cbCrcCtx ) || ( NULL == p_initData->f_i_Rx ) ||
            ( NULL == p_initData->p_i_cbRxCtx ) || ( NULL == p_initData->i_rxTim.p_timerCtx ) ||
            ( NULL == p_initData->i_rxTim.f_tim_start ) || ( NULL == p_initData->i_rxTim.f_tim_getRemaining ) )
        {
            l_result = MSGRX_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity of data area */
            if( p_initData->i_receiveBuffAreaSize < 1u)
            {
                l_result = MSGRX_RES_BADPARAM;
            }
            else
            {
                /* Check data validity of time */
                if( ( p_initData->i_frameTimeoutMs < 1u ) || ( p_initData->i_timePerRecMs < 1u ) ||
                    ( p_initData->i_timePerRecMs > p_initData->i_frameTimeoutMs ) )
                {
                    l_result = MSGRX_RES_BADPARAM;
                }
                else
                {
                    /* Initialize internal status variable */
                    p_ctx->p_rxBuff = p_initData->p_i_receiveBuffArea;
                    p_ctx->rxBuffSize = p_initData->i_receiveBuffAreaSize;
                    p_ctx->rxBuffCntr = 0u;
                    p_ctx->rxBuffFill = 0u;
                    p_ctx->f_Rx = p_initData->f_i_Rx;
                    p_ctx->p_RxCtx = p_initData->p_i_cbRxCtx;
                    p_ctx->rxTim = p_initData->i_rxTim;
                    p_ctx->frameTimeoutMs = p_initData->i_frameTimeoutMs;
                    p_ctx->timePerRecMs = p_initData->i_timePerRecMs;
                    p_ctx->needWaitFrameStart = p_initData->i_needWaitFrameStart;

                    /* initialize internal bytestuffer */
                    l_resultMsgD =  eFSP_MSGD_InitCtx(&p_ctx->msgd_Ctx, p_initData->p_i_memArea, p_initData->i_memAreaSize,
                                               p_initData->f_i_Crc, p_initData->p_i_cbCrcCtx);
                    l_result = eFSP_MSGRX_convertReturnFromMSGD(l_resultMsgD);
                }
            }
        }
	}

	return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3"
#endif

e_eFSP_MSGRX_Res eFSP_MSGRX_IsInit(s_eFSP_MSGRX_Ctx* const p_ctx, bool_t* p_isInit)
{
	/* Local variable */
	e_eFSP_MSGRX_Res l_result;
	e_eFSP_MSGD_Res l_resultMsgD;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_isInit ) )
	{
		l_result = MSGRX_RES_BADPOINTER;
	}
	else
	{
        l_resultMsgD = eFSP_MSGD_IsInit(&p_ctx->msgd_Ctx, p_isInit);
        l_result = eFSP_MSGRX_convertReturnFromMSGD(l_resultMsgD);
	}

	return l_result;
}

e_eFSP_MSGRX_Res eFSP_MSGRX_NewMsg(s_eFSP_MSGRX_Ctx* const p_ctx)
{
	/* Local variable */
	e_eFSP_MSGRX_Res l_result;
	e_eFSP_MSGD_Res l_resultMsgD;

	/* Check pointer validity */
	if( NULL == p_ctx )
	{
		l_result = MSGRX_RES_BADPOINTER;
	}
	else
	{
        /* Check internal status validity */
        if( false == eFSP_MSGRX_isStatusStillCoherent(p_ctx) )
        {
            l_result = MSGRX_RES_CORRUPTCTX;
        }
        else
        {
            /* Init message encoder */
            l_resultMsgD = eFSP_MSGD_NewMsg(&p_ctx->msgd_Ctx);
            l_result = eFSP_MSGRX_convertReturnFromMSGD(l_resultMsgD);

            if( MSGRX_RES_OK == l_result )
            {
                /* Start timer even if we need to wait SOF, this case is handled in the MSGRX_GetDecodedData */
                if( true != p_ctx->rxTim.f_tim_start( p_ctx->rxTim.p_timerCtx, p_ctx->frameTimeoutMs ) )
                {
                    l_result = MSGRX_RES_TIMCLBKERROR;
                }
            }
        }
	}

	return l_result;
}

e_eFSP_MSGRX_Res eFSP_MSGRX_NewMsgNClean(s_eFSP_MSGRX_Ctx* const p_ctx)
{
	/* Local variable for l_result */
	e_eFSP_MSGRX_Res l_result;
	e_eFSP_MSGD_Res l_resultMsgD;

	/* Check pointer validity */
	if( NULL == p_ctx )
	{
		l_result = MSGRX_RES_BADPOINTER;
	}
	else
	{
        /* Check internal status validity */
        if( false == eFSP_MSGRX_isStatusStillCoherent(p_ctx) )
        {
            l_result = MSGRX_RES_CORRUPTCTX;
        }
        else
        {
            /* Reset internal variable */
            p_ctx->rxBuffCntr = 0u;
            p_ctx->rxBuffFill = 0u;

            /* Init message encoder */
            l_resultMsgD = eFSP_MSGD_NewMsg(&p_ctx->msgd_Ctx);
            l_result = eFSP_MSGRX_convertReturnFromMSGD(l_resultMsgD);

            if( MSGRX_RES_OK == l_result )
            {
                /* Start timer even if we need to wait SOF, this case is handled in the MSGRX_GetDecodedData */
                if( true != p_ctx->rxTim.f_tim_start( p_ctx->rxTim.p_timerCtx, p_ctx->frameTimeoutMs ) )
                {
                    l_result = MSGRX_RES_TIMCLBKERROR;
                }
            }
        }
	}

	return l_result;
}

e_eFSP_MSGRX_Res eFSP_MSGRX_GetDecodedData(s_eFSP_MSGRX_Ctx* const p_ctx, uint8_t** pp_data, uint32_t* const p_GetLen)
{
	/* Local variable */
	e_eFSP_MSGRX_Res l_result;
	e_eFSP_MSGD_Res l_resultMsgD;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == pp_data ) || ( NULL == p_GetLen ) )
	{
		l_result = MSGRX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGRX_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGRX_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
			l_resultMsgD = eFSP_MSGD_GetDecodedData(&p_ctx->msgd_Ctx, pp_data, p_GetLen);
			l_result = eFSP_MSGRX_convertReturnFromMSGD(l_resultMsgD);
		}
	}

	return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed because this warning is not working */
#endif

e_eFSP_MSGRX_Res eFSP_MSGRX_ReceiveChunk(s_eFSP_MSGRX_Ctx* const p_ctx)
{
	/* Local variable of the operation l_result */
	e_eFSP_MSGRX_Res l_result;
	e_eFSP_MSGD_Res l_resultMsgD;

    /* Local variable to keep track of the current state machine state */
    e_eFSP_MSGRX_Priv_state l_stateM;

    /* Local variable used for the current time calculation */
    uint32_t l_sRemRxTime;
    uint32_t l_cRemainRxTime;
    uint32_t l_elapsedFromStart;
    uint32_t l_sSessionRemanTime;
    uint32_t l_cSessionRemanTime;

    bool_t l_isMsgDec;
    bool_t l_isInit;

    /* Local variable usend for the current data calculation */
    uint8_t *lp_cDToRxP;
    uint32_t l_cDToRxLen;
    uint32_t l_cDRxed;

    /* Other local variable */
    uint32_t l_rxMostEff;
    bool_t l_isWaitingSof;

	/* Check pointer validity */
	if( NULL == p_ctx )
	{
		l_result = MSGRX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGRX_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGRX_RES_CORRUPTCTX;
		}
		else
		{
            /* Init time frame counter */
            l_result = MSGRX_RES_OK;
            l_stateM = MSGRX_PRV_CHECKINIT;

            /* Init data */
            l_sRemRxTime = 0u;
            l_cSessionRemanTime = 0u;
            l_sSessionRemanTime = 0u;
            l_rxMostEff = 0u;
            l_cDRxed = 0u;
            l_isMsgDec = false;
            l_isWaitingSof = false;

            /* wait end elaboration or end for timeout */
            while( l_stateM != MSGRX_PRV_ELABDONE )
            {
                switch( l_stateM )
                {
                    case MSGRX_PRV_CHECKINIT:
                    {
                        /* Check if lib is initialized */
                        l_isInit = false;
                        l_resultMsgD = eFSP_MSGD_IsInit(&p_ctx->msgd_Ctx, &l_isInit);
                        l_result = eFSP_MSGRX_convertReturnFromMSGD(l_resultMsgD);

                        /* Check if any error is returned */
                        if( MSGRX_RES_OK == l_result )
                        {
                            if( true == l_isInit )
                            {
                                /* All ok */
                                l_stateM = MSGRX_PRV_CHECKINITTIMEOUT;
                            }
                            else
                            {
                                /* Need to init the lib before */
                                l_result = MSGRX_RES_NOINITLIB;
                                l_stateM = MSGRX_PRV_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_stateM = MSGRX_PRV_ELABDONE;
                        }
                        break;
                    }

                    case MSGRX_PRV_CHECKINITTIMEOUT:
                    {
                        /* Check if frame timeout is eplased */
                        if( true == p_ctx->rxTim.f_tim_getRemaining(p_ctx->rxTim.p_timerCtx, &l_sRemRxTime) )
                        {
                            /* Check also if we are still waiting start of frame to be received */
                            l_resultMsgD =  eFSP_MSGD_IsWaitingSof(&p_ctx->msgd_Ctx, &l_isWaitingSof);
                            l_result = eFSP_MSGRX_convertReturnFromMSGD(l_resultMsgD);

                            if( MSGRX_RES_OK == l_result )
                            {
                                if( true == p_ctx->needWaitFrameStart )
                                {
                                    /* Need to wait SOF */
                                    if( true == l_isWaitingSof )
                                    {
                                        /* We are waiting start of frame, reset timer */
                                        if( true == p_ctx->rxTim.f_tim_start( p_ctx->rxTim.p_timerCtx, p_ctx->frameTimeoutMs ) )
                                        {
                                            /* Ok restarted the timer */
                                            l_sRemRxTime = p_ctx->frameTimeoutMs;
                                            l_cSessionRemanTime = p_ctx->timePerRecMs;
                                            l_sSessionRemanTime = p_ctx->timePerRecMs;

                                            /* check if we have some data to receive in RX buffer */
                                            l_stateM = MSGRX_PRV_CHECKHOWMANYDATA;
                                        }
                                        else
                                        {
                                            /* Some error */
                                            l_result = MSGRX_RES_TIMCLBKERROR;
                                            l_stateM = MSGRX_PRV_ELABDONE;
                                        }
                                    }
                                    else
                                    {
                                        /* Frame already started */
                                        if( l_sRemRxTime <= 0u )
                                        {
                                            /* Time elapsed */
                                            l_result = MSGRX_RES_MESSAGETIMEOUT;
                                            l_stateM = MSGRX_PRV_ELABDONE;
                                        }
                                        else
                                        {
                                            /* Frame timeout is not elapsed, calculate frame session timeout */
                                            if( l_sRemRxTime >= p_ctx->timePerRecMs )
                                            {
                                                /* Time elapsed */
                                                l_cSessionRemanTime = p_ctx->timePerRecMs;
                                                l_sSessionRemanTime = p_ctx->timePerRecMs;
                                            }
                                            else
                                            {
                                                /* Session timeout not elapsed, can receive data */
                                                l_cSessionRemanTime = l_sRemRxTime;
                                                l_sSessionRemanTime = l_sRemRxTime;
                                            }

                                            /* check if we have some data to receive in RX buffer */
                                            l_stateM = MSGRX_PRV_CHECKHOWMANYDATA;
                                        }
                                    }
                                }
                                else
                                {
                                    /* No need to wait SOF, timer start con receiveStart function call */
                                    if( l_sRemRxTime <= 0u )
                                    {
                                        /* Time elapsed */
                                        l_result = MSGRX_RES_MESSAGETIMEOUT;
                                        l_stateM = MSGRX_PRV_ELABDONE;
                                    }
                                    else
                                    {
                                        /* Frame timeout is not elapsed, calculate frame session timeout */
                                        if( l_sRemRxTime >= p_ctx->timePerRecMs )
                                        {
                                            /* Time elapsed */
                                            l_cSessionRemanTime = p_ctx->timePerRecMs;
                                            l_sSessionRemanTime = p_ctx->timePerRecMs;
                                        }
                                        else
                                        {
                                            /* Session timeout not elapsed, can receive data */
                                            l_cSessionRemanTime = l_sRemRxTime;
                                            l_sSessionRemanTime = l_sRemRxTime;
                                        }

                                        /* check if we have some data to receive in RX buffer */
                                        l_stateM = MSGRX_PRV_CHECKHOWMANYDATA;
                                    }
                                }
                            }
                            else
                            {
                                /* Some error */
                                l_stateM = MSGRX_PRV_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error on timer */
                            l_result = MSGRX_RES_TIMCLBKERROR;
                            l_stateM = MSGRX_PRV_ELABDONE;
                        }
                        break;
                    }

                    case MSGRX_PRV_CHECKHOWMANYDATA:
                    {
                        /* How many byte do we need to receive? */
                        l_resultMsgD = eFSP_MSGD_GetMostEffDatLen(&p_ctx->msgd_Ctx, &l_rxMostEff);
                        l_result = eFSP_MSGRX_convertReturnFromMSGD(l_resultMsgD);
                        if( MSGRX_RES_OK == l_result )
                        {
                            if( l_rxMostEff > 0u )
                            {
                                /* Ok we need some data to be retrived */
                                l_stateM = MSGRX_PRV_CHECKIFBUFFERRX;

                                /* Check compatibility with rx buffer dimension */
                                if( l_rxMostEff > p_ctx->rxBuffSize )
                                {
                                    /* In this way we dont' have any overflow */
                                    l_rxMostEff = p_ctx->rxBuffSize;
                                }
                            }
                            else
                            {
                                /* Could be because the message is received or an error occoured */
                                l_resultMsgD = eFSP_MSGD_IsAFullMsgDecoded(&p_ctx->msgd_Ctx, &l_isMsgDec);
                                l_result = eFSP_MSGRX_convertReturnFromMSGD(l_resultMsgD);

                                if( MSGRX_RES_OK == l_result )
                                {
                                    if( true == l_isMsgDec )
                                    {
                                        /* Ok the message is correct */
                                        l_stateM = MSGRX_PRV_ELABDONE;
                                        l_result = MSGRX_RES_MESSAGERECEIVED;
                                    }
                                    else
                                    {
                                        /* Umm the message is not correct */
                                        l_stateM = MSGRX_PRV_ELABDONE;
                                        l_result = MSGRX_RES_BADFRAME;
                                    }
                                }
                                else
                                {
                                    /* Some error */
                                    l_stateM = MSGRX_PRV_ELABDONE;
                                }
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_stateM = MSGRX_PRV_ELABDONE;
                        }
                        break;
                    }

                    case MSGRX_PRV_CHECKIFBUFFERRX:
                    {
                        /* Is needed data already present in the receive buffer? */
                        l_cDToRxLen = p_ctx->rxBuffFill - p_ctx->rxBuffCntr;

                        if( l_cDToRxLen > 0u )
                        {
                            /* Can parse already received data in msg buffer */
                            l_stateM = MSGRX_PRV_INSERTCHUNK;
                        }
                        else
                        {
                            /* No data in msg buffer, retrive some other chunk of data */
                            p_ctx->rxBuffCntr = 0u;
                            p_ctx->rxBuffFill = 0u;
                            l_stateM = MSGRX_PRV_RECEIVE_BUFF;
                        }
                        break;
                    }

                    case MSGRX_PRV_RECEIVE_BUFF:
                    {
                        /* Ok, rx buffer is empty and need to be filled with data that we can parse */
                        if( true == (*p_ctx->f_Rx)(p_ctx->p_RxCtx, p_ctx->p_rxBuff, &p_ctx->rxBuffFill, l_rxMostEff,
                                                  l_cSessionRemanTime) )
                        {
                            /* Check for some strangeness */
                            if( p_ctx->rxBuffFill > l_rxMostEff )
                            {
                                l_result = MSGRX_RES_CORRUPTCTX;
                                l_stateM = MSGRX_PRV_ELABDONE;
                            }
                            else
                            {
                                /* Update received counter */
                                p_ctx->rxBuffCntr = 0u;

                                /* Go next state */
                                l_stateM = MSGRX_PRV_INSERTCHUNK;
                            }
                        }
                        else
                        {
                            /* Error sending data */
                            l_result = MSGRX_RES_RXCLBKERROR;
                            l_stateM = MSGRX_PRV_ELABDONE;
                        }

                        break;
                    }

                    case MSGRX_PRV_INSERTCHUNK:
                    {
                        /* Ok, we have some data in RX buffer */
                        lp_cDToRxP = &p_ctx->p_rxBuff[p_ctx->rxBuffCntr];
                        l_cDToRxLen = p_ctx->rxBuffFill - p_ctx->rxBuffCntr;

                        if( l_cDToRxLen > 0u )
                        {
                            /* We can try to decode data event if we already finished cuz the function
                            * MSGD_InsEncChunk is well maden */
                            l_resultMsgD = eFSP_MSGD_InsEncChunk(&p_ctx->msgd_Ctx, lp_cDToRxP, l_cDToRxLen, &l_cDRxed );
                            l_result = eFSP_MSGRX_convertReturnFromMSGD(l_resultMsgD);

                            if( MSGRX_RES_OK == l_result )
                            {
                                /* Retrived some data, by design if MSGD_InsEncChunk return MSGE_RES_OK this
                                 * means that the value of loaded data inside send buffer is equals to it's size */
                                p_ctx->rxBuffCntr = 0u;
                                p_ctx->rxBuffFill = 0u;

                                /* Check for timeout */
                                l_stateM = MSGRX_PRV_CHECKTIMEOUTAFTERRX;
                            }
                            else if( MSGRX_RES_MESSAGERECEIVED == l_result )
                            {
                                /* Ok we retrived all the possible data */
                                /* Update RX buffer */
                                p_ctx->rxBuffCntr += l_cDRxed;

                                /* Check for timeout */
                                l_stateM = MSGRX_PRV_CHECKTIMEOUTAFTERRX;
                            }
                            else if( MSGRX_RES_BADFRAME == l_result )
                            {
                                /* Update RX buffer */
                                p_ctx->rxBuffCntr += l_cDRxed;

                                /* Bad frame, but check timeout also */
                                l_stateM = MSGRX_PRV_CHECKTIMEOUTAFTERRX;
                            }
                            else if( MSGRX_RES_FRAMERESTART == l_result )
                            {
                                /* Update RX buffer */
                                p_ctx->rxBuffCntr += l_cDRxed;

                                 /* ok frame restarted, but check timeout also */
                                l_stateM = MSGRX_PRV_CHECKTIMEOUTAFTERRX;
                            }
                            else
                            {
                                /* Update RX buffer */
                                p_ctx->rxBuffCntr += l_cDRxed;

                                /* Some error, can return */
                                l_stateM = MSGRX_PRV_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Didn't receive data in the previous MSGRX_PRV_RECEIVE_BUFF, check timeout */
                            l_result = MSGRX_RES_OK;
                            l_stateM = MSGRX_PRV_CHECKTIMEOUTAFTERRX;
                        }

                        break;
                    }

                    case MSGRX_PRV_CHECKTIMEOUTAFTERRX:
                    {
                        /* Check if frame timeout is eplased */
                        if( true == p_ctx->rxTim.f_tim_getRemaining(p_ctx->rxTim.p_timerCtx, &l_cRemainRxTime) )
                        {
                            /* Check time validity */
                            if( l_cRemainRxTime > l_sRemRxTime )
                            {
                                /* It's not possible to have more time to send the frame now than during
                                * the begining */
                                l_result = MSGRX_RES_CORRUPTCTX;
                                l_stateM = MSGRX_PRV_ELABDONE;
                            }
                            else
                            {
                                /* Calc elapsed time */
                                l_elapsedFromStart = l_sRemRxTime - l_cRemainRxTime;

                                /* Are we waiting SOF? */
                                l_resultMsgD =  eFSP_MSGD_IsWaitingSof(&p_ctx->msgd_Ctx, &l_isWaitingSof);
                                if( MSGD_RES_OK == l_resultMsgD )
                                {
                                    /* Do we need to wait start of frame? */
                                    if( ( true == p_ctx->needWaitFrameStart ) && ( MSGRX_RES_FRAMERESTART == l_result ) )
                                    {
                                        /* Timeout dosent occour in this situation */
                                        l_stateM = MSGRX_PRV_ELABDONE;

                                        /* Frame restarted, restart the timer */
                                        if( true == p_ctx->rxTim.f_tim_start( p_ctx->rxTim.p_timerCtx, p_ctx->frameTimeoutMs ) )
                                        {
                                            /* Ok restarted the timer */
                                            l_sRemRxTime = p_ctx->frameTimeoutMs;

                                            /* Dosent care about session imeout because we are returning in any case */
                                        }
                                        else
                                        {
                                            /* Some error */
                                            l_result = MSGRX_RES_TIMCLBKERROR;
                                        }
                                    }
                                    else if( ( true == p_ctx->needWaitFrameStart ) && ( true == l_isWaitingSof ) &&
                                             ( MSGRX_RES_OK == l_result ) )
                                    {
                                        /* In this case total time dosen't need to be decreased, only the session */
                                        /* Frame restarted, restart the timer */
                                        if( true == p_ctx->rxTim.f_tim_start( p_ctx->rxTim.p_timerCtx, p_ctx->frameTimeoutMs ) )
                                        {
                                            /* Ok restarted the timer */
                                            l_sRemRxTime = p_ctx->frameTimeoutMs;
                                            l_cRemainRxTime = p_ctx->frameTimeoutMs;

                                            /* Update session timeout */
                                            if( l_elapsedFromStart >= l_sSessionRemanTime )
                                            {
                                                /* No more time */
                                                l_stateM = MSGRX_PRV_ELABDONE;
                                            }
                                            else
                                            {
                                                /* Update remaining session time */
                                                l_sSessionRemanTime = l_sSessionRemanTime - l_elapsedFromStart;
                                                l_cSessionRemanTime = l_sSessionRemanTime;

                                                /* Can retrive more data */
                                                l_stateM = MSGRX_PRV_CHECKHOWMANYDATA;
                                            }
                                        }
                                        else
                                        {
                                            /* Some error */
                                            l_result = MSGRX_RES_TIMCLBKERROR;
											l_stateM = MSGRX_PRV_ELABDONE;
                                        }
                                    }
                                    else
                                    {
                                        /* Check for timeout */
                                        if( l_cRemainRxTime <= 0u )
                                        {
                                            /* Time elapsed */
                                            l_result = MSGRX_RES_MESSAGETIMEOUT;
                                            l_stateM = MSGRX_PRV_ELABDONE;
                                        }
                                        else
                                        {
                                            /* Frame timeout is not elapsed, check current session if expired */
                                            if( l_elapsedFromStart >= l_sSessionRemanTime )
                                            {
                                                /* Session time is elapsed, can return the previous l_result */
                                                l_stateM = MSGRX_PRV_ELABDONE;
                                            }
                                            else
                                            {
                                                /* Session timeout not elapsed, if needed can do more elaboration */
                                                l_cSessionRemanTime = l_sSessionRemanTime - l_elapsedFromStart;

                                                /* Check what to do looking at the previous state l_result */
                                                if( MSGRX_RES_OK == l_result )
                                                {
                                                    /* Can retrive more data */
                                                    l_stateM = MSGRX_PRV_CHECKHOWMANYDATA;
                                                }
                                                else if( MSGRX_RES_MESSAGERECEIVED == l_result )
                                                {
                                                    /* Ok we retrived all the possible data */
                                                    l_stateM = MSGRX_PRV_ELABDONE;
                                                }
                                                else if( MSGRX_RES_BADFRAME == l_result )
                                                {
                                                    /* Bad frame */
                                                    l_stateM = MSGRX_PRV_ELABDONE;
                                                }
                                                else if( MSGRX_RES_FRAMERESTART == l_result )
                                                {
                                                    /* ok frame restarted */
                                                    l_stateM = MSGRX_PRV_ELABDONE;
                                                }
                                                else
                                                {
                                                    /* Some error, But was already handled by previous state */
                                                    l_result = MSGRX_RES_CORRUPTCTX;
                                                    l_stateM = MSGRX_PRV_ELABDONE;
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    /* Some error */
                                    l_stateM = MSGRX_PRV_ELABDONE;
                                    l_result = eFSP_MSGRX_convertReturnFromMSGD(l_resultMsgD);
                                }
                            }
                        }
                        else
                        {
                            /* Some error on timer */
                            l_result = MSGRX_RES_TIMCLBKERROR;
                            l_stateM = MSGRX_PRV_ELABDONE;
                        }
                        break;
                    }

                    default:
                    {
                        /* Impossible end here */
                        l_stateM = MSGRX_PRV_ELABDONE;
                        l_result = MSGRX_RES_CORRUPTCTX;
                        break;
                    }
                }
            }
		}
	}

	return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3"
#endif

/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSP_MSGRX_isStatusStillCoherent(const s_eFSP_MSGRX_Ctx* p_ctx)
{
    bool_t l_result;

	/* Check pointer validity */
	if( ( NULL == p_ctx->p_rxBuff ) || ( NULL == p_ctx->f_Rx ) || ( NULL == p_ctx->p_RxCtx ) ||
        ( NULL == p_ctx->rxTim.p_timerCtx ) || ( NULL == p_ctx->rxTim.f_tim_start ) ||
        ( NULL == p_ctx->rxTim.f_tim_getRemaining ) )
	{
		l_result = false;
	}
	else
	{
        /* Check send buffer validity */
        if( ( p_ctx->rxBuffSize < 1u ) || ( p_ctx->rxBuffFill > p_ctx->rxBuffSize )  ||
            ( p_ctx->rxBuffCntr > p_ctx->rxBuffFill ) )
        {
            l_result =  false;
        }
        else
        {
            /* Check timings validity */
            if( ( p_ctx->frameTimeoutMs < 1u ) || ( p_ctx->timePerRecMs < 1u ) ||
                ( p_ctx->timePerRecMs > p_ctx->frameTimeoutMs ) )
            {
                l_result = false;
            }
            else
            {
                l_result = true;
            }
        }
	}

    return l_result;
}

static e_eFSP_MSGRX_Res eFSP_MSGRX_convertReturnFromMSGD(e_eFSP_MSGD_Res returnedEvent)
{
	e_eFSP_MSGRX_Res l_result;

	switch( returnedEvent )
	{
		case MSGD_RES_OK:
		{
			l_result = MSGRX_RES_OK;
            break;
		}

		case MSGD_RES_BADPARAM:
		{
			l_result = MSGRX_RES_BADPARAM;
            break;
		}

		case MSGD_RES_BADPOINTER:
		{
			l_result = MSGRX_RES_BADPOINTER;
            break;
		}

		case MSGD_RES_CORRUPTCTX:
		{
			l_result = MSGRX_RES_CORRUPTCTX;
            break;
		}

        case MSGD_RES_OUTOFMEM:
        {
			l_result = MSGRX_RES_OUTOFMEM;
            break;
        }

        case MSGD_RES_BADFRAME:
        {
			l_result = MSGRX_RES_BADFRAME;
            break;
        }

		case MSGD_RES_MESSAGEENDED:
		{
			l_result = MSGRX_RES_MESSAGERECEIVED;
            break;
		}

		case MSGD_RES_FRAMERESTART:
		{
			l_result = MSGRX_RES_FRAMERESTART;
            break;
		}

		case MSGD_RES_NOINITLIB:
		{
			l_result = MSGRX_RES_NOINITLIB;
            break;
		}

		case MSGD_RES_CRCCLBKERROR :
		{
			l_result = MSGRX_RES_CRCCLBKERROR;
            break;
		}

		default:
		{
            /* Impossible end here */
			l_result = MSGRX_RES_CORRUPTCTX;
            break;
		}
	}

	return l_result;
}