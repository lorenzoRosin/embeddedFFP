/**
 * @file       eFSP_MSGTX.h
 *
 * @brief      Message transmitter utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_MSGTX.h"
#include "eFSP_MSGTXPRV.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSP_MSGTX_isStatusStillCoherent(const t_eFSP_MSGTX_Ctx* p_ptCtx);
static e_eFSP_MSGTX_RES eFSP_MSGTX_convertReturnFromMSGE(e_eFSP_MSGE_RES returnedEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_MSGTX_RES eFSP_MSGTX_InitCtx(t_eFSP_MSGTX_Ctx* const p_ptCtx, const t_eFSP_MSGTX_InitData* p_initData)
{
	/* Local variable */
	e_eFSP_MSGTX_RES l_eRes;
	e_eFSP_MSGE_RES l_resultMsgE;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_initData ) )
	{
		l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_initData->puIMemArea ) || ( NULL == p_initData->p_i_txBuffArea ) ||
            ( NULL == p_initData->fICrc ) || ( NULL == p_initData->ptICbCrcCtx ) || ( NULL == p_initData->f_i_Tx ) ||
            ( NULL == p_initData->p_i_cbTxCtx ) || ( NULL == p_initData->i_txTim.ptTimCtx ) ||
            ( NULL == p_initData->i_txTim.fTimStart ) || ( NULL == p_initData->i_txTim.fTimGetRemain ) )
        {
            l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity, we need some len to store the data */
            if( ( p_initData->i_txBuffAreaSize < 1u ) || ( p_initData->uITimeoutMs < 1u ) ||
                ( p_initData->i_timePerSendMs < 1u ) || ( p_initData->i_timePerSendMs > p_initData->uITimeoutMs ) )
            {
                l_eRes = e_eFSP_MSGTX_RES_BADPARAM;
            }
            else
            {
                /* Initialize internal status variable */
                p_ptCtx->p_rxBuff = p_initData->p_i_txBuffArea;
                p_ptCtx->txBuffSize = p_initData->i_txBuffAreaSize;
                p_ptCtx->txBuffCntr = 0u;
                p_ptCtx->txBuffFill = 0u;
                p_ptCtx->f_Tx = p_initData->f_i_Tx;
                p_ptCtx->p_TxCtx = p_initData->p_i_cbTxCtx;
                p_ptCtx->txTim =  p_initData->i_txTim;
                p_ptCtx->timeoutMs = p_initData->uITimeoutMs;
                p_ptCtx->timePerSendMs = p_initData->i_timePerSendMs;

                /* initialize internal message encoder */
                l_resultMsgE =  eFSP_MSGE_InitCtx(&p_ptCtx->msge_Ctx, p_initData->puIMemArea, p_initData->uIMemAreaL,
                                                   p_initData->fICrc, p_initData->ptICbCrcCtx);
                l_eRes = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);
            }
        }
	}

	return l_eRes;
}

e_eFSP_MSGTX_RES eFSP_MSGTX_IsInit(t_eFSP_MSGTX_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSP_MSGTX_RES l_eRes;
	e_eFSP_MSGE_RES l_resultMsgE;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
	}
	else
	{
        l_resultMsgE = eFSP_MSGE_IsInit(&p_ptCtx->msge_Ctx, p_pbIsInit);
        l_eRes = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);
	}

	return l_eRes;
}

e_eFSP_MSGTX_RES eFSP_MSGTX_GetWherePutData(t_eFSP_MSGTX_Ctx* const p_ptCtx, uint8_t** pp_data, uint32_t* const p_maxDL)
{
	/* Local variable */
	e_eFSP_MSGTX_RES l_eRes;
	e_eFSP_MSGE_RES l_resultMsgE;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == pp_data ) || ( NULL == p_maxDL ) )
	{
		l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGTX_isStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
		}
		else
		{
			/* Get only the payload data reference */
			l_resultMsgE = eFSP_MSGE_GetWherePutData(&p_ptCtx->msge_Ctx, pp_data, p_maxDL);
			l_eRes = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);
		}
	}

	return l_eRes;
}

e_eFSP_MSGTX_RES eFSP_MSGTX_NewMessage(t_eFSP_MSGTX_Ctx* const p_ptCtx, const uint32_t messageLen)
{
	/* Local variable */
	e_eFSP_MSGTX_RES l_eRes;
	e_eFSP_MSGE_RES l_resultMsgE;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
	}
	else
	{
        /* Check internal status validity */
        if( false == eFSP_MSGTX_isStatusStillCoherent(p_ptCtx) )
        {
            l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
        }
		else
		{
            /* Check param validity, need at least 1 byte of paylaod */
            if( messageLen <= 0u )
            {
                l_eRes = e_eFSP_MSGTX_RES_BADPARAM;
            }
            else
            {
                /* Reset internal variable */
                p_ptCtx->txBuffCntr = 0u;
                p_ptCtx->txBuffFill = 0u;

                /* Init message encoder */
                l_resultMsgE = eFSP_MSGE_NewMessage(&p_ptCtx->msge_Ctx, messageLen);
                l_eRes = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);

                /* Start timer */
                if( e_eFSP_MSGTX_RES_OK == l_eRes )
                {
                    if( true != p_ptCtx->txTim.fTimStart( p_ptCtx->txTim.ptTimCtx, p_ptCtx->timeoutMs ) )
                    {
                        l_eRes = e_eFSP_MSGTX_RES_TIMCLBKERROR;
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSP_MSGTX_RES eFSP_MSGTX_RestartMessage(t_eFSP_MSGTX_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSP_MSGTX_RES l_eRes;
	e_eFSP_MSGE_RES l_resultMsgE;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGTX_isStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
		}
		else
		{
            /* Reset internal variable */
            p_ptCtx->txBuffCntr = 0u;
            p_ptCtx->txBuffFill = 0u;

			/* Restart only the byte stuffer */
			l_resultMsgE = eFSP_MSGE_RestartMessage(&p_ptCtx->msge_Ctx);
			l_eRes = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);

            /* Start timer */
            if( e_eFSP_MSGTX_RES_OK == l_eRes )
            {
                if( true != p_ptCtx->txTim.fTimStart( p_ptCtx->txTim.ptTimCtx, p_ptCtx->timeoutMs ) )
                {
                    l_eRes = e_eFSP_MSGTX_RES_TIMCLBKERROR;
                }
            }
		}
	}

	return l_eRes;
}

e_eFSP_MSGTX_RES eFSP_MSGTX_SendChunk(t_eFSP_MSGTX_Ctx* const p_ptCtx)
{
	/* Local variable of the operation l_eRes */
	e_eFSP_MSGTX_RES l_eRes;
	e_eFSP_MSGE_RES l_resultMsgE;
    bool_t l_isInit;

    /* Local variable to keep track of the current state machine state */
    e_eFSP_MSGTXPRV_SM l_stateM;

    /* Local variable to keep track of the start, current etc remaining time */
    uint32_t l_sRemainTxTime;
    uint32_t l_cRemainTxTime;
    uint32_t l_sessionRem;
    uint32_t l_elapsedFromStart;

    /* Local variable usend for the current data calculation */
    const uint8_t *lp_cDToTx;
    uint32_t l_cDToTxLen;
    uint32_t l_cDTxed;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGTX_isStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
		}
		else
		{
            /* Init state machine value */
            l_sessionRem = 0u;
            l_sRemainTxTime = 0u;
            l_eRes = e_eFSP_MSGTX_RES_OK;
            l_stateM = e_eFSP_MSGTXPRV_SM_CHECKINIT;

            /* wait end elaboration or end for timeout */
            while( l_stateM != e_eFSP_MSGTXPRV_SM_ELABDONE )
            {
                switch( l_stateM )
                {
                    case e_eFSP_MSGTXPRV_SM_CHECKINIT:
                    {
                        /* Check if lib is initialized */
                        l_isInit = false;
                        l_resultMsgE = eFSP_MSGE_IsInit(&p_ptCtx->msge_Ctx, &l_isInit);
                        l_eRes = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);

                        /* Check if frame timeout is eplased */
                        if( e_eFSP_MSGTX_RES_OK == l_eRes )
                        {
                            if( true == l_isInit )
                            {
                                /* All ok */
                                l_stateM = e_eFSP_MSGTXPRV_SM_CHECKINITTIMEOUT;
                            }
                            else
                            {
                                /* Need to init the lib before */
                                l_eRes = e_eFSP_MSGTX_RES_NOINITLIB;
                                l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                        }
                        break;
                    }

                    case e_eFSP_MSGTXPRV_SM_CHECKINITTIMEOUT:
                    {
                        /* Check if frame timeout is eplased */
                        if( true == p_ptCtx->txTim.fTimGetRemain(p_ptCtx->txTim.ptTimCtx, &l_sRemainTxTime) )
                        {
                            if( l_sRemainTxTime <= 0u )
                            {
                                /* Time elapsed */
                                l_eRes = e_eFSP_MSGTX_RES_MESSAGETIMEOUT;
                                l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                            }
                            else
                            {
                                /* Frame timeout is not elapsed, calculate frame session timeout */
                                if( l_sRemainTxTime >= p_ptCtx->timePerSendMs )
                                {
                                    /* Time elapsed */
                                    l_sessionRem = p_ptCtx->timePerSendMs;
                                }
                                else
                                {
                                    /* Session timeout not elapsed, can send data */
                                    l_sessionRem = l_sRemainTxTime;
                                }

                                /* check if we have some data to send in TX buffer */
                                l_stateM = e_eFSP_MSGTXPRV_SM_CHECKIFBUFFERTX;
                            }
                        }
                        else
                        {
                            /* Some error on timer */
                            l_eRes = e_eFSP_MSGTX_RES_TIMCLBKERROR;
                            l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                        }
                        break;
                    }

                    case e_eFSP_MSGTXPRV_SM_CHECKIFBUFFERTX:
                    {
                        /* Is data present in send buffer? */
                        l_cDToTxLen = p_ptCtx->txBuffFill - p_ptCtx->txBuffCntr;

                        if( l_cDToTxLen > 0u )
                        {
                            /* Can send data in msg buffer */
                            l_stateM = e_eFSP_MSGTXPRV_SM_SENDBUFF;
                        }
                        else
                        {
                            /* No data in msg buffer, retrive some other chunk of data, only if the message send is
                             * not completed of course */
                            p_ptCtx->txBuffFill = 0u;
                            p_ptCtx->txBuffCntr = 0u;
                            l_stateM = e_eFSP_MSGTXPRV_SM_RETRIVECHUNK;
                        }
                        break;
                    }

                    case e_eFSP_MSGTXPRV_SM_RETRIVECHUNK:
                    {
                        /* Ok, the send buffer is empty, need to load remainings data */
                        p_ptCtx->txBuffCntr = 0u;
                        p_ptCtx->txBuffFill = 0u;

                        /* Is data present in message encoder buffer? */
                        l_resultMsgE = eFSP_MSGE_GetEncChunk(&p_ptCtx->msge_Ctx, p_ptCtx->p_rxBuff, p_ptCtx->txBuffSize,
                                                        &p_ptCtx->txBuffFill);
                        l_eRes = eFSP_MSGTX_convertReturnFromMSGE(l_resultMsgE);

                        if( e_eFSP_MSGTX_RES_OK == l_eRes )
                        {
                            /* Retrived some data to send, by design if MSGE_RetriveEChunk return e_eFSP_MSGE_RES_OK this
                             * means that the value of loaded data inside send buffer is equals to it's size */

                            /* Can go to send loaded data now */
                            l_stateM = e_eFSP_MSGTXPRV_SM_SENDBUFF;
                        }
                        else if( e_eFSP_MSGTX_RES_MESSAGESENDED == l_eRes )
                        {
                            /* Ok we retrived all the possible data */
                            if( 0u == p_ptCtx->txBuffFill )
                            {
                                /* No more data to send or retrive */
                                l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                            }
                            else
                            {
                                /* Ok retrived all data but need to send the remaining data present in sendBuff */
                                l_stateM = e_eFSP_MSGTXPRV_SM_SENDBUFF;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                        }

                        break;
                    }

                    case e_eFSP_MSGTXPRV_SM_SENDBUFF:
                    {
                        /* Get data to send */
                        lp_cDToTx = &p_ptCtx->p_rxBuff[p_ptCtx->txBuffCntr];
                        l_cDToTxLen = p_ptCtx->txBuffFill - p_ptCtx->txBuffCntr;

                        /* Can send data from send buffer */
                        if( true == (*p_ptCtx->f_Tx)(p_ptCtx->p_TxCtx, lp_cDToTx, l_cDToTxLen, &l_cDTxed, l_sessionRem) )
                        {
                            /* Check for some strangeness */
                            if( l_cDTxed > l_cDToTxLen )
                            {
                                l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
                                l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                            }
                            else
                            {
                                /* Update sended counter */
                                p_ptCtx->txBuffCntr += l_cDTxed;

                                /* Check if time is elapsed */
                                l_stateM = e_eFSP_MSGTXPRV_SM_CHECKTIMEOUTAFTERTX;
                            }
                        }
                        else
                        {
                            /* Error sending data */
                            l_eRes = e_eFSP_MSGTX_RES_TXCLBKERROR;
                            l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                        }
                        break;
                    }

                    case e_eFSP_MSGTXPRV_SM_CHECKTIMEOUTAFTERTX:
                    {
                        /* Check if frame timeout is eplased */
                        if( true == p_ptCtx->txTim.fTimGetRemain(p_ptCtx->txTim.ptTimCtx, &l_cRemainTxTime) )
                        {
                            /* Check time validity */
                            if( l_cRemainTxTime > l_sRemainTxTime )
                            {
                                /* It's not possible to have more time to send the frame now than during
                                 * the begining */
                                l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
                                l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                            }
                            else
                            {
                                if( l_cRemainTxTime <= 0u )
                                {
                                    /* Time elapsed */
                                    l_eRes = e_eFSP_MSGTX_RES_MESSAGETIMEOUT;
                                    l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                                }
                                else
                                {
                                    /* During start do we had the whole time slice? */
                                    if( l_sRemainTxTime >= p_ptCtx->timePerSendMs )
                                    {
                                        /* Started with a whole time slice */
                                        l_elapsedFromStart = l_sRemainTxTime - l_cRemainTxTime;

                                        /* Frame timeout is not elapsed, check current session if expired */
                                        if( l_elapsedFromStart >= p_ptCtx->timePerSendMs )
                                        {
                                            /* Time elapsed */
                                            l_eRes = e_eFSP_MSGTX_RES_OK;
                                            l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                                        }
                                        else
                                        {
                                            /* Session timeout not elapsed, can send data */
                                            l_sessionRem = p_ptCtx->timePerSendMs - l_elapsedFromStart;

                                            /* Check if something to send is present */
                                            l_stateM = e_eFSP_MSGTXPRV_SM_CHECKIFBUFFERTX;
                                        }
                                    }
                                    else
                                    {
                                        /* Started only with remaining time */
                                        l_sessionRem = l_cRemainTxTime;

                                        /* Check if something to send is present */
                                        l_stateM = e_eFSP_MSGTXPRV_SM_CHECKIFBUFFERTX;
                                    }
                                }
                            }
                        }
                        else
                        {
                            /* Some error on timer */
                            l_eRes = e_eFSP_MSGTX_RES_TIMCLBKERROR;
                            l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                        }
                        break;
                    }

                    default:
                    {
                        /* Impossible end here */
                        l_stateM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                        l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
                        break;
                    }
                }
            }
        }
	}

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSP_MSGTX_isStatusStillCoherent(const t_eFSP_MSGTX_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx->p_rxBuff ) || ( NULL == p_ptCtx->f_Tx ) || ( NULL == p_ptCtx->p_TxCtx ) ||
        ( NULL == p_ptCtx->txTim.ptTimCtx ) || ( NULL == p_ptCtx->txTim.fTimStart ) ||
        ( NULL == p_ptCtx->txTim.fTimGetRemain ) )
	{
		l_eRes = false;
	}
	else
	{
        /* Check send buffer validity */
        if( p_ptCtx->txBuffSize < 1u )
        {
            l_eRes =  false;
        }
        else
        {
            /* Check send buffer validity */
            if( ( p_ptCtx->txBuffFill > p_ptCtx->txBuffSize )  || ( p_ptCtx->txBuffCntr > p_ptCtx->txBuffFill ) )
            {
                l_eRes =  false;
            }
            else
            {
                /* Check timings validity */
                if( ( p_ptCtx->timeoutMs < 1u ) || ( p_ptCtx->timePerSendMs < 1u ) )
                {
                    l_eRes = false;
                }
                else
                {
                    /* Check timings validity */
                    if( p_ptCtx->timePerSendMs > p_ptCtx->timeoutMs )
                    {
                        l_eRes = false;
                    }
                    else
                    {
                        l_eRes = true;
                    }
                }
            }
        }
	}

    return l_eRes;
}

static e_eFSP_MSGTX_RES eFSP_MSGTX_convertReturnFromMSGE(e_eFSP_MSGE_RES returnedEvent)
{
	e_eFSP_MSGTX_RES l_eRes;

	switch( returnedEvent )
	{
		case e_eFSP_MSGE_RES_OK:
		{
			l_eRes = e_eFSP_MSGTX_RES_OK;
            break;
		}

		case e_eFSP_MSGE_RES_BADPARAM:
		{
			l_eRes = e_eFSP_MSGTX_RES_BADPARAM;
            break;
		}

		case e_eFSP_MSGE_RES_BADPOINTER:
		{
			l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
            break;
		}

		case e_eFSP_MSGE_RES_CORRUPTCTX:
		{
			l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
            break;
		}

		case e_eFSP_MSGE_RES_MESSAGEENDED:
		{
			l_eRes = e_eFSP_MSGTX_RES_MESSAGESENDED;
            break;
		}

		case e_eFSP_MSGE_RES_NOINITLIB:
		{
			l_eRes = e_eFSP_MSGTX_RES_NOINITLIB;
            break;
		}

		case e_eFSP_MSGE_RES_NOINITMESSAGE :
		{
			l_eRes = e_eFSP_MSGTX_RES_NOINITMESSAGE;
            break;
		}

		case e_eFSP_MSGE_RES_CRCCLBKERROR :
		{
			l_eRes = e_eFSP_MSGTX_RES_CRCCLBKERROR;
            break;
		}

		default:
		{
            /* Impossible end here */
			l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
            break;
		}
	}

	return l_eRes;
}