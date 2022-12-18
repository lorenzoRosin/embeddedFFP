/**
 * @file       eFSPMsgTransmitter.h
 *
 * @brief      Message transmitter utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgTransmitter.h"
#include "eFSPMsgTransmitter_Priv.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSP_MSGTX_isStatusStillCoherent(const s_eFSP_MSGTX_Ctx* p_ctx);
static e_eFSP_MSGTX_Res eFSP_MSGTX_convertReturnFromMSGE(e_eFSP_MSGE_Res returnedEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed because this warning is not working */
#endif

e_eFSP_MSGTX_Res eFSP_MSGTX_InitCtx(s_eFSP_MSGTX_Ctx* const p_ctx, const s_eFSP_MSGTX_InitData* p_initData)
{
	/* Local variable */
	e_eFSP_MSGTX_Res l_result;
	e_eFSP_MSGE_Res l_resultMsgE;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_initData ) )
	{
		l_result = MSGTX_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_initData->p_i_memArea ) || ( NULL == p_initData->p_i_sendBuffArea ) || ( NULL == p_initData->f_i_Crc ) ||
            ( NULL == p_initData->p_i_cbCrcCtx ) || ( NULL == p_initData->f_i_Tx ) || ( NULL == p_initData->p_i_cbTxCtx ) ||
            ( NULL == p_initData->i_txTim.p_timerCtx ) || ( NULL == p_initData->i_txTim.f_tim_start ) ||
            ( NULL == p_initData->i_txTim.f_tim_getRemaining ) )
        {
            l_result = MSGTX_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity, we need some len to store the data */
            if( ( p_initData->i_sendBuffAreaSize < 1u ) || ( p_initData->i_frameTimeoutMs < 1u ) ||
                ( p_initData->i_timePerSendMs < 1u ) || ( p_initData->i_timePerSendMs > p_initData->i_frameTimeoutMs ) )
            {
                l_result = MSGTX_RES_BADPARAM;
            }
            else
            {
                /* Initialize internal status variable */
                p_ctx->p_rxBuff = p_initData->p_i_sendBuffArea;
                p_ctx->txBuffSize = p_initData->i_sendBuffAreaSize;
                p_ctx->txBuffCntr = 0u;
                p_ctx->txBuffFill = 0u;
                p_ctx->f_Tx = p_initData->f_i_Tx;
                p_ctx->p_TxCtx = p_initData->p_i_cbTxCtx;
                p_ctx->txTim =  p_initData->i_txTim;
                p_ctx->frameTimeoutMs = p_initData->i_frameTimeoutMs;
                p_ctx->timePerSendMs = p_initData->i_timePerSendMs;

                /* initialize internal message encoder */
                l_resultMsgE =  eFSP_MSGE_InitCtx(&p_ctx->msge_Ctx, p_initData->p_i_memArea, p_initData->i_memAreaSize,
                                           p_initData->f_i_Crc, p_initData->p_i_cbCrcCtx);
                l_result = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);
            }
        }
	}

	return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3"
#endif

e_eFSP_MSGTX_Res eFSP_MSGTX_IsInit(s_eFSP_MSGTX_Ctx* const p_ctx, bool_t* p_isInit)
{
	/* Local variable */
	e_eFSP_MSGTX_Res l_result;
	e_eFSP_MSGE_Res l_resultMsgE;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_isInit ) )
	{
		l_result = MSGTX_RES_BADPOINTER;
	}
	else
	{
        l_resultMsgE = eFSP_MSGE_IsInit(&p_ctx->msge_Ctx, p_isInit);
        l_result = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);
	}

	return l_result;
}

e_eFSP_MSGTX_Res eFSP_MSGTX_GetWherePutData(s_eFSP_MSGTX_Ctx* const p_ctx, uint8_t** pp_data, uint32_t* const p_maxDataL)
{
	/* Local variable */
	e_eFSP_MSGTX_Res l_result;
	e_eFSP_MSGE_Res l_resultMsgE;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == pp_data ) || ( NULL == p_maxDataL ) )
	{
		l_result = MSGTX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGTX_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGTX_RES_CORRUPTCTX;
		}
		else
		{
			/* Get only the payload data reference */
			l_resultMsgE = eFSP_MSGE_GetWherePutData(&p_ctx->msge_Ctx, pp_data, p_maxDataL);
			l_result = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);
		}
	}

	return l_result;
}

e_eFSP_MSGTX_Res eFSP_MSGTX_NewMessage(s_eFSP_MSGTX_Ctx* const p_ctx, const uint32_t messageLen)
{
	/* Local variable */
	e_eFSP_MSGTX_Res l_result;
	e_eFSP_MSGE_Res l_resultMsgE;

	/* Check pointer validity */
	if( NULL == p_ctx )
	{
		l_result = MSGTX_RES_BADPOINTER;
	}
	else
	{
        /* Check internal status validity */
        if( false == eFSP_MSGTX_isStatusStillCoherent(p_ctx) )
        {
            l_result = MSGTX_RES_CORRUPTCTX;
        }
		else
		{
            /* Check param validity, need at least 1 byte of paylaod */
            if( messageLen <= 0u )
            {
                l_result = MSGTX_RES_BADPARAM;
            }
            else
            {
                /* Reset internal variable */
                p_ctx->txBuffCntr = 0u;
                p_ctx->txBuffFill = 0u;

                /* Init message encoder */
                l_resultMsgE = eFSP_MSGE_NewMessage(&p_ctx->msge_Ctx, messageLen);
                l_result = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);

                /* Start timer */
                if( MSGTX_RES_OK == l_result )
                {
                    if( true != p_ctx->txTim.f_tim_start( p_ctx->txTim.p_timerCtx, p_ctx->frameTimeoutMs ) )
                    {
                        l_result = MSGTX_RES_TIMCLBKERROR;
                    }
                }
            }
		}
	}

	return l_result;
}

e_eFSP_MSGTX_Res eFSP_MSGTX_RestartMessage(s_eFSP_MSGTX_Ctx* const p_ctx)
{
	/* Local variable */
	e_eFSP_MSGTX_Res l_result;
	e_eFSP_MSGE_Res l_resultMsgE;

	/* Check pointer validity */
	if( NULL == p_ctx )
	{
		l_result = MSGTX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGTX_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGTX_RES_CORRUPTCTX;
		}
		else
		{
            /* Reset internal variable */
            p_ctx->txBuffCntr = 0u;
            p_ctx->txBuffFill = 0u;

			/* Restart only the byte stuffer */
			l_resultMsgE = eFSP_MSGE_RestartMessage(&p_ctx->msge_Ctx);
			l_result = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);

            /* Start timer */
            if( MSGTX_RES_OK == l_result )
            {
                if( true != p_ctx->txTim.f_tim_start( p_ctx->txTim.p_timerCtx, p_ctx->frameTimeoutMs ) )
                {
                    l_result = MSGTX_RES_TIMCLBKERROR;
                }
            }
		}
	}

	return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed because this warning is not working */
#endif

e_eFSP_MSGTX_Res eFSP_MSGTX_SendChunk(s_eFSP_MSGTX_Ctx* const p_ctx)
{
	/* Local variable of the operation l_result */
	e_eFSP_MSGTX_Res l_result;
	e_eFSP_MSGE_Res l_resultMsgE;
    bool_t l_isInit;

    /* Local variable to keep track of the current state machine state */
    e_eFSP_MSGTX_Priv_state l_stateM;

    /* Local variable to keep track of the start, current etc remaining time */
    uint32_t l_sRemainTxTime;
    uint32_t l_cRemainTxTime;
    uint32_t l_sessionRemaining;
    uint32_t l_elapsedFromStart;

    /* Local variable usend for the current data calculation */
    const uint8_t *lp_cDToTx;
    uint32_t l_cDToTxLen;
    uint32_t l_cDTxed;

	/* Check pointer validity */
	if( NULL == p_ctx )
	{
		l_result = MSGTX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGTX_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGTX_RES_CORRUPTCTX;
		}
		else
		{
            /* Init state machine value */
            l_sessionRemaining = 0u;
            l_sRemainTxTime = 0u;
            l_result = MSGTX_RES_OK;
            l_stateM = MSGTX_PRV_CHECKINIT;

            /* wait end elaboration or end for timeout */
            while( l_stateM != MSGTX_PRV_ELABDONE )
            {
                switch( l_stateM )
                {
                    case MSGTX_PRV_CHECKINIT:
                    {
                        /* Check if lib is initialized */
                        l_isInit = false;
                        l_resultMsgE = eFSP_MSGE_IsInit(&p_ctx->msge_Ctx, &l_isInit);
                        l_result = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);

                        /* Check if frame timeout is eplased */
                        if( MSGTX_RES_OK == l_result )
                        {
                            if( true == l_isInit )
                            {
                                /* All ok */
                                l_stateM = MSGTX_PRV_CHECKINITTIMEOUT;
                            }
                            else
                            {
                                /* Need to init the lib before */
                                l_result = MSGTX_RES_NOINITLIB;
                                l_stateM = MSGTX_PRV_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_stateM = MSGTX_PRV_ELABDONE;
                        }
                        break;
                    }

                    case MSGTX_PRV_CHECKINITTIMEOUT:
                    {
                        /* Check if frame timeout is eplased */
                        if( true == p_ctx->txTim.f_tim_getRemaining(p_ctx->txTim.p_timerCtx, &l_sRemainTxTime) )
                        {
                            if( l_sRemainTxTime <= 0u )
                            {
                                /* Time elapsed */
                                l_result = MSGTX_RES_MESSAGETIMEOUT;
                                l_stateM = MSGTX_PRV_ELABDONE;
                            }
                            else
                            {
                                /* Frame timeout is not elapsed, calculate frame session timeout */
                                if( l_sRemainTxTime >= p_ctx->timePerSendMs )
                                {
                                    /* Time elapsed */
                                    l_sessionRemaining = p_ctx->timePerSendMs;
                                }
                                else
                                {
                                    /* Session timeout not elapsed, can send data */
                                    l_sessionRemaining = l_sRemainTxTime;
                                }

                                /* check if we have some data to send in TX buffer */
                                l_stateM = MSGTX_PRV_CHECKIFBUFFERTX;
                            }
                        }
                        else
                        {
                            /* Some error on timer */
                            l_result = MSGTX_RES_TIMCLBKERROR;
                            l_stateM = MSGTX_PRV_ELABDONE;
                        }
                        break;
                    }

                    case MSGTX_PRV_CHECKIFBUFFERTX:
                    {
                        /* Is data present in send buffer? */
                        l_cDToTxLen = p_ctx->txBuffFill - p_ctx->txBuffCntr;

                        if( l_cDToTxLen > 0u )
                        {
                            /* Can send data in msg buffer */
                            l_stateM = MSGTX_PRV_SENDBUFF;
                        }
                        else
                        {
                            /* No data in msg buffer, retrive some other chunk of data, only if the message send is
                             * not completed of course */
                            p_ctx->txBuffFill = 0u;
                            p_ctx->txBuffCntr = 0u;
                            l_stateM = MSGTX_PRV_RETRIVECHUNK;
                        }
                        break;
                    }

                    case MSGTX_PRV_RETRIVECHUNK:
                    {
                        /* Ok, the send buffer is empty, need to load remainings data */
                        p_ctx->txBuffCntr = 0u;
                        p_ctx->txBuffFill = 0u;

                        /* Is data present in message encoder buffer? */
                        l_resultMsgE = eFSP_MSGE_GetEncChunk(&p_ctx->msge_Ctx, p_ctx->p_rxBuff, p_ctx->txBuffSize,
                                                        &p_ctx->txBuffFill);
                        l_result = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);

                        if( MSGTX_RES_OK == l_result )
                        {
                            /* Retrived some data to send, by design if MSGE_RetriveEChunk return MSGE_RES_OK this
                             * means that the value of loaded data inside send buffer is equals to it's size */

                            /* Can go to send loaded data now */
                            l_stateM = MSGTX_PRV_SENDBUFF;
                        }
                        else if( MSGTX_RES_MESSAGESENDED == l_result )
                        {
                            /* Ok we retrived all the possible data */
                            if( 0u == p_ctx->txBuffFill )
                            {
                                /* No more data to send or retrive */
                                l_stateM = MSGTX_PRV_ELABDONE;
                            }
                            else
                            {
                                /* Ok retrived all data but need to send the remaining data present in sendBuff */
                                l_stateM = MSGTX_PRV_SENDBUFF;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_stateM = MSGTX_PRV_ELABDONE;
                        }

                        break;
                    }

                    case MSGTX_PRV_SENDBUFF:
                    {
                        /* Get data to send */
                        lp_cDToTx = &p_ctx->p_rxBuff[p_ctx->txBuffCntr];
                        l_cDToTxLen = p_ctx->txBuffFill - p_ctx->txBuffCntr;

                        /* Can send data from send buffer */
                        if( true == (*p_ctx->f_Tx)(p_ctx->p_TxCtx, lp_cDToTx, l_cDToTxLen, &l_cDTxed, l_sessionRemaining) )
                        {
                            /* Check for some strangeness */
                            if( l_cDTxed > l_cDToTxLen )
                            {
                                l_result = MSGTX_RES_CORRUPTCTX;
                                l_stateM = MSGTX_PRV_ELABDONE;
                            }
                            else
                            {
                                /* Update sended counter */
                                p_ctx->txBuffCntr += l_cDTxed;

                                /* Check if time is elapsed */
                                l_stateM = MSGTX_PRV_CHECKTIMEOUTAFTERTX;
                            }
                        }
                        else
                        {
                            /* Error sending data */
                            l_result = MSGTX_RES_TXCLBKERROR;
                            l_stateM = MSGTX_PRV_ELABDONE;
                        }
                        break;
                    }

                    case MSGTX_PRV_CHECKTIMEOUTAFTERTX:
                    {
                        /* Check if frame timeout is eplased */
                        if( true == p_ctx->txTim.f_tim_getRemaining(p_ctx->txTim.p_timerCtx, &l_cRemainTxTime) )
                        {
                            /* Check time validity */
                            if( l_cRemainTxTime > l_sRemainTxTime )
                            {
                                /* It's not possible to have more time to send the frame now than during
                                 * the begining */
                                l_result = MSGTX_RES_CORRUPTCTX;
                                l_stateM = MSGTX_PRV_ELABDONE;
                            }
                            else
                            {
                                if( l_cRemainTxTime <= 0u )
                                {
                                    /* Time elapsed */
                                    l_result = MSGTX_RES_MESSAGETIMEOUT;
                                    l_stateM = MSGTX_PRV_ELABDONE;
                                }
                                else
                                {
                                    /* During start do we had the whole time slice? */
                                    if( l_sRemainTxTime >= p_ctx->timePerSendMs )
                                    {
                                        /* Started with a whole time slice */
                                        l_elapsedFromStart = l_sRemainTxTime - l_cRemainTxTime;

                                        /* Frame timeout is not elapsed, check current session if expired */
                                        if( l_elapsedFromStart >= p_ctx->timePerSendMs )
                                        {
                                            /* Time elapsed */
                                            l_result = MSGTX_RES_OK;
                                            l_stateM = MSGTX_PRV_ELABDONE;
                                        }
                                        else
                                        {
                                            /* Session timeout not elapsed, can send data */
                                            l_sessionRemaining = p_ctx->timePerSendMs - l_elapsedFromStart;

                                            /* Check if something to send is present */
                                            l_stateM = MSGTX_PRV_CHECKIFBUFFERTX;
                                        }
                                    }
                                    else
                                    {
                                        /* Started only with remaining time */
                                        l_sessionRemaining = l_cRemainTxTime;

                                        /* Check if something to send is present */
                                        l_stateM = MSGTX_PRV_CHECKIFBUFFERTX;
                                    }
                                }
                            }
                        }
                        else
                        {
                            /* Some error on timer */
                            l_result = MSGTX_RES_TIMCLBKERROR;
                            l_stateM = MSGTX_PRV_ELABDONE;
                        }
                        break;
                    }

                    default:
                    {
                        /* Impossible end here */
                        l_stateM = MSGTX_PRV_ELABDONE;
                        l_result = MSGTX_RES_CORRUPTCTX;
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
static bool_t eFSP_MSGTX_isStatusStillCoherent(const s_eFSP_MSGTX_Ctx* p_ctx)
{
    bool_t l_result;

	/* Check pointer validity */
	if( ( NULL == p_ctx->p_rxBuff ) || ( NULL == p_ctx->f_Tx ) || ( NULL == p_ctx->p_TxCtx ) ||
        ( NULL == p_ctx->txTim.p_timerCtx ) || ( NULL == p_ctx->txTim.f_tim_start ) ||
        ( NULL == p_ctx->txTim.f_tim_getRemaining ) )
	{
		l_result = false;
	}
	else
	{
        /* Check send buffer validity */
        if( p_ctx->txBuffSize < 1u )
        {
            l_result =  false;
        }
        else
        {
            /* Check send buffer validity */
            if( ( p_ctx->txBuffFill > p_ctx->txBuffSize )  || ( p_ctx->txBuffCntr > p_ctx->txBuffFill ) )
            {
                l_result =  false;
            }
            else
            {
                /* Check timings validity */
                if( ( p_ctx->frameTimeoutMs < 1u ) || ( p_ctx->timePerSendMs < 1u ) )
                {
                    l_result = false;
                }
                else
                {
                    /* Check timings validity */
                    if( p_ctx->timePerSendMs > p_ctx->frameTimeoutMs )
                    {
                        l_result = false;
                    }
                    else
                    {
                        l_result = true;
                    }
                }
            }
        }
	}

    return l_result;
}

static e_eFSP_MSGTX_Res eFSP_MSGTX_convertReturnFromMSGE(e_eFSP_MSGE_Res returnedEvent)
{
	e_eFSP_MSGTX_Res l_result;

	switch( returnedEvent )
	{
		case MSGE_RES_OK:
		{
			l_result = MSGTX_RES_OK;
            break;
		}

		case MSGE_RES_BADPARAM:
		{
			l_result = MSGTX_RES_BADPARAM;
            break;
		}

		case MSGE_RES_BADPOINTER:
		{
			l_result = MSGTX_RES_BADPOINTER;
            break;
		}

		case MSGE_RES_CORRUPTCTX:
		{
			l_result = MSGTX_RES_CORRUPTCTX;
            break;
		}

		case MSGE_RES_MESSAGEENDED:
		{
			l_result = MSGTX_RES_MESSAGESENDED;
            break;
		}

		case MSGE_RES_NOINITLIB:
		{
			l_result = MSGTX_RES_NOINITLIB;
            break;
		}

		case MSGE_RES_NOINITMESSAGE :
		{
			l_result = MSGTX_RES_NOINITMESSAGE;
            break;
		}

		case MSGE_RES_CRCCLBKERROR :
		{
			l_result = MSGTX_RES_CRCCLBKERROR;
            break;
		}

		default:
		{
            /* Impossible end here */
			l_result = MSGTX_RES_CORRUPTCTX;
            break;
		}
	}

	return l_result;
}