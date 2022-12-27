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
static bool_t eFSP_MSGTX_IsStatusStillCoherent(const t_eFSP_MSGTX_Ctx* p_ptCtx);
static e_eFSP_MSGTX_RES eFSP_MSGTX_ConvertRetFromMSGE(e_eFSP_MSGE_RES p_eRetEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_MSGTX_RES eFSP_MSGTX_InitCtx(t_eFSP_MSGTX_Ctx* const p_ptCtx, const t_eFSP_MSGTX_InitData* p_ptInitData)
{
	/* Local variable */
	e_eFSP_MSGTX_RES l_eRes;
	e_eFSP_MSGE_RES l_eResMsgE;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ptInitData ) )
	{
		l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == p_ptInitData->puIMemArea ) || ( NULL == p_ptInitData->puITxBuffArea ) ||
            ( NULL == p_ptInitData->fICrc ) || ( NULL == p_ptInitData->ptICbCrcCtx ) || ( NULL == p_ptInitData->fITx ) ||
            ( NULL == p_ptInitData->ptICbTxCtx ) || ( NULL == p_ptInitData->tITxTim.ptTimCtx ) ||
            ( NULL == p_ptInitData->tITxTim.fTimStart ) || ( NULL == p_ptInitData->tITxTim.fTimGetRemain ) )
        {
            l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity, we need some len to store the data */
            if( ( p_ptInitData->uITxBuffAreaL < 1u ) || ( p_ptInitData->uITimeoutMs < 1u ) ||
                ( p_ptInitData->uITimePerSendMs < 1u ) || ( p_ptInitData->uITimePerSendMs > p_ptInitData->uITimeoutMs ) )
            {
                l_eRes = e_eFSP_MSGTX_RES_BADPARAM;
            }
            else
            {
                /* Initialize internal status variable */
                p_ptCtx->p_rxBuff = p_ptInitData->puITxBuffArea;
                p_ptCtx->txBuffSize = p_ptInitData->uITxBuffAreaL;
                p_ptCtx->txBuffCntr = 0u;
                p_ptCtx->txBuffFill = 0u;
                p_ptCtx->f_Tx = p_ptInitData->fITx;
                p_ptCtx->p_TxCtx = p_ptInitData->ptICbTxCtx;
                p_ptCtx->txTim =  p_ptInitData->tITxTim;
                p_ptCtx->timeoutMs = p_ptInitData->uITimeoutMs;
                p_ptCtx->timePerSendMs = p_ptInitData->uITimePerSendMs;

                /* initialize internal message encoder */
                l_eResMsgE =  eFSP_MSGE_InitCtx(&p_ptCtx->msge_Ctx, p_ptInitData->puIMemArea, p_ptInitData->uIMemAreaL,
                                                   p_ptInitData->fICrc, p_ptInitData->ptICbCrcCtx);
                l_eRes = eFSP_MSGTX_ConvertRetFromMSGE(l_eResMsgE);
            }
        }
	}

	return l_eRes;
}

e_eFSP_MSGTX_RES eFSP_MSGTX_IsInit(t_eFSP_MSGTX_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSP_MSGTX_RES l_eRes;
	e_eFSP_MSGE_RES l_eResMsgE;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
	}
	else
	{
        l_eResMsgE = eFSP_MSGE_IsInit(&p_ptCtx->msge_Ctx, p_pbIsInit);
        l_eRes = eFSP_MSGTX_ConvertRetFromMSGE(l_eResMsgE);
	}

	return l_eRes;
}

e_eFSP_MSGTX_RES eFSP_MSGTX_GetWherePutData(t_eFSP_MSGTX_Ctx* const p_ptCtx, uint8_t** p_ppuData, uint32_t* const p_puMaxDL)
{
	/* Local variable */
	e_eFSP_MSGTX_RES l_eRes;
	e_eFSP_MSGE_RES l_eResMsgE;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuData ) || ( NULL == p_puMaxDL ) )
	{
		l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGTX_IsStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
		}
		else
		{
			/* Get only the payload data reference */
			l_eResMsgE = eFSP_MSGE_GetWherePutData(&p_ptCtx->msge_Ctx, p_ppuData, p_puMaxDL);
			l_eRes = eFSP_MSGTX_ConvertRetFromMSGE(l_eResMsgE);
		}
	}

	return l_eRes;
}

e_eFSP_MSGTX_RES eFSP_MSGTX_NewMessage(t_eFSP_MSGTX_Ctx* const p_ptCtx, const uint32_t p_uMsgL)
{
	/* Local variable */
	e_eFSP_MSGTX_RES l_eRes;
	e_eFSP_MSGE_RES l_eResMsgE;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
	}
	else
	{
        /* Check internal status validity */
        if( false == eFSP_MSGTX_IsStatusStillCoherent(p_ptCtx) )
        {
            l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
        }
		else
		{
            /* Check param validity, need at least 1 byte of paylaod */
            if( p_uMsgL <= 0u )
            {
                l_eRes = e_eFSP_MSGTX_RES_BADPARAM;
            }
            else
            {
                /* Reset internal variable */
                p_ptCtx->txBuffCntr = 0u;
                p_ptCtx->txBuffFill = 0u;

                /* Init message encoder */
                l_eResMsgE = eFSP_MSGE_NewMessage(&p_ptCtx->msge_Ctx, p_uMsgL);
                l_eRes = eFSP_MSGTX_ConvertRetFromMSGE(l_eResMsgE);

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
	e_eFSP_MSGE_RES l_eResMsgE;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGTX_IsStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
		}
		else
		{
            /* Reset internal variable */
            p_ptCtx->txBuffCntr = 0u;
            p_ptCtx->txBuffFill = 0u;

			/* Restart only the byte stuffer */
			l_eResMsgE = eFSP_MSGE_RestartMessage(&p_ptCtx->msge_Ctx);
			l_eRes = eFSP_MSGTX_ConvertRetFromMSGE(l_eResMsgE);

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
	e_eFSP_MSGE_RES l_eResMsgE;
    bool_t l_bIsInit;

    /* Local variable to keep track of the current state machine state */
    e_eFSP_MSGTXPRV_SM l_eSM;

    /* Local variable to keep track of the start, current etc remaining time */
    uint32_t l_uSRemainTxT;
    uint32_t l_uCRemainTxT;
    uint32_t l_uSessionRem;
    uint32_t l_uElapFromStart;

    /* Local variable usend for the current data calculation */
    const uint8_t *l_puCDToTx;
    uint32_t l_uCDToTxL;
    uint32_t l_uCDTxed;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSP_MSGTX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGTX_IsStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
		}
		else
		{
            /* Init state machine value */
            l_uSessionRem = 0u;
            l_uSRemainTxT = 0u;
            l_eRes = e_eFSP_MSGTX_RES_OK;
            l_eSM = e_eFSP_MSGTXPRV_SM_CHECKINIT;

            /* wait end elaboration or end for timeout */
            while( l_eSM != e_eFSP_MSGTXPRV_SM_ELABDONE )
            {
                switch( l_eSM )
                {
                    case e_eFSP_MSGTXPRV_SM_CHECKINIT:
                    {
                        /* Check if lib is initialized */
                        l_bIsInit = false;
                        l_eResMsgE = eFSP_MSGE_IsInit(&p_ptCtx->msge_Ctx, &l_bIsInit);
                        l_eRes = eFSP_MSGTX_ConvertRetFromMSGE(l_eResMsgE);

                        /* Check if frame timeout is eplased */
                        if( e_eFSP_MSGTX_RES_OK == l_eRes )
                        {
                            if( true == l_bIsInit )
                            {
                                /* All ok */
                                l_eSM = e_eFSP_MSGTXPRV_SM_CHECKINITTIMEOUT;
                            }
                            else
                            {
                                /* Need to init the lib before */
                                l_eRes = e_eFSP_MSGTX_RES_NOINITLIB;
                                l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                        }
                        break;
                    }

                    case e_eFSP_MSGTXPRV_SM_CHECKINITTIMEOUT:
                    {
                        /* Check if frame timeout is eplased */
                        if( true == p_ptCtx->txTim.fTimGetRemain(p_ptCtx->txTim.ptTimCtx, &l_uSRemainTxT) )
                        {
                            if( l_uSRemainTxT <= 0u )
                            {
                                /* Time elapsed */
                                l_eRes = e_eFSP_MSGTX_RES_MESSAGETIMEOUT;
                                l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                            }
                            else
                            {
                                /* Frame timeout is not elapsed, calculate frame session timeout */
                                if( l_uSRemainTxT >= p_ptCtx->timePerSendMs )
                                {
                                    /* Time elapsed */
                                    l_uSessionRem = p_ptCtx->timePerSendMs;
                                }
                                else
                                {
                                    /* Session timeout not elapsed, can send data */
                                    l_uSessionRem = l_uSRemainTxT;
                                }

                                /* check if we have some data to send in TX buffer */
                                l_eSM = e_eFSP_MSGTXPRV_SM_CHECKIFBUFFERTX;
                            }
                        }
                        else
                        {
                            /* Some error on timer */
                            l_eRes = e_eFSP_MSGTX_RES_TIMCLBKERROR;
                            l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                        }
                        break;
                    }

                    case e_eFSP_MSGTXPRV_SM_CHECKIFBUFFERTX:
                    {
                        /* Is data present in send buffer? */
                        l_uCDToTxL = p_ptCtx->txBuffFill - p_ptCtx->txBuffCntr;

                        if( l_uCDToTxL > 0u )
                        {
                            /* Can send data in msg buffer */
                            l_eSM = e_eFSP_MSGTXPRV_SM_SENDBUFF;
                        }
                        else
                        {
                            /* No data in msg buffer, retrive some other chunk of data, only if the message send is
                             * not completed of course */
                            p_ptCtx->txBuffFill = 0u;
                            p_ptCtx->txBuffCntr = 0u;
                            l_eSM = e_eFSP_MSGTXPRV_SM_RETRIVECHUNK;
                        }
                        break;
                    }

                    case e_eFSP_MSGTXPRV_SM_RETRIVECHUNK:
                    {
                        /* Ok, the send buffer is empty, need to load remainings data */
                        p_ptCtx->txBuffCntr = 0u;
                        p_ptCtx->txBuffFill = 0u;

                        /* Is data present in message encoder buffer? */
                        l_eResMsgE = eFSP_MSGE_GetEncChunk(&p_ptCtx->msge_Ctx, p_ptCtx->p_rxBuff, p_ptCtx->txBuffSize,
                                                        &p_ptCtx->txBuffFill);
                        l_eRes = eFSP_MSGTX_ConvertRetFromMSGE(l_eResMsgE);

                        if( e_eFSP_MSGTX_RES_OK == l_eRes )
                        {
                            /* Retrived some data to send, by design if MSGE_RetriveEChunk return e_eFSP_MSGE_RES_OK this
                             * means that the value of loaded data inside send buffer is equals to it's size */

                            /* Can go to send loaded data now */
                            l_eSM = e_eFSP_MSGTXPRV_SM_SENDBUFF;
                        }
                        else if( e_eFSP_MSGTX_RES_MESSAGESENDED == l_eRes )
                        {
                            /* Ok we retrived all the possible data */
                            if( 0u == p_ptCtx->txBuffFill )
                            {
                                /* No more data to send or retrive */
                                l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                            }
                            else
                            {
                                /* Ok retrived all data but need to send the remaining data present in sendBuff */
                                l_eSM = e_eFSP_MSGTXPRV_SM_SENDBUFF;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                        }

                        break;
                    }

                    case e_eFSP_MSGTXPRV_SM_SENDBUFF:
                    {
                        /* Get data to send */
                        l_puCDToTx = &p_ptCtx->p_rxBuff[p_ptCtx->txBuffCntr];
                        l_uCDToTxL = p_ptCtx->txBuffFill - p_ptCtx->txBuffCntr;

                        /* Can send data from send buffer */
                        if( true == (*p_ptCtx->f_Tx)(p_ptCtx->p_TxCtx, l_puCDToTx, l_uCDToTxL, &l_uCDTxed, l_uSessionRem) )
                        {
                            /* Check for some strangeness */
                            if( l_uCDTxed > l_uCDToTxL )
                            {
                                l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
                                l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                            }
                            else
                            {
                                /* Update sended counter */
                                p_ptCtx->txBuffCntr += l_uCDTxed;

                                /* Check if time is elapsed */
                                l_eSM = e_eFSP_MSGTXPRV_SM_CHECKTIMEOUTAFTERTX;
                            }
                        }
                        else
                        {
                            /* Error sending data */
                            l_eRes = e_eFSP_MSGTX_RES_TXCLBKERROR;
                            l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                        }
                        break;
                    }

                    case e_eFSP_MSGTXPRV_SM_CHECKTIMEOUTAFTERTX:
                    {
                        /* Check if frame timeout is eplased */
                        if( true == p_ptCtx->txTim.fTimGetRemain(p_ptCtx->txTim.ptTimCtx, &l_uCRemainTxT) )
                        {
                            /* Check time validity */
                            if( l_uCRemainTxT > l_uSRemainTxT )
                            {
                                /* It's not possible to have more time to send the frame now than during
                                 * the begining */
                                l_eRes = e_eFSP_MSGTX_RES_CORRUPTCTX;
                                l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                            }
                            else
                            {
                                if( l_uCRemainTxT <= 0u )
                                {
                                    /* Time elapsed */
                                    l_eRes = e_eFSP_MSGTX_RES_MESSAGETIMEOUT;
                                    l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                                }
                                else
                                {
                                    /* During start do we had the whole time slice? */
                                    if( l_uSRemainTxT >= p_ptCtx->timePerSendMs )
                                    {
                                        /* Started with a whole time slice */
                                        l_uElapFromStart = l_uSRemainTxT - l_uCRemainTxT;

                                        /* Frame timeout is not elapsed, check current session if expired */
                                        if( l_uElapFromStart >= p_ptCtx->timePerSendMs )
                                        {
                                            /* Time elapsed */
                                            l_eRes = e_eFSP_MSGTX_RES_OK;
                                            l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                                        }
                                        else
                                        {
                                            /* Session timeout not elapsed, can send data */
                                            l_uSessionRem = p_ptCtx->timePerSendMs - l_uElapFromStart;

                                            /* Check if something to send is present */
                                            l_eSM = e_eFSP_MSGTXPRV_SM_CHECKIFBUFFERTX;
                                        }
                                    }
                                    else
                                    {
                                        /* Started only with remaining time */
                                        l_uSessionRem = l_uCRemainTxT;

                                        /* Check if something to send is present */
                                        l_eSM = e_eFSP_MSGTXPRV_SM_CHECKIFBUFFERTX;
                                    }
                                }
                            }
                        }
                        else
                        {
                            /* Some error on timer */
                            l_eRes = e_eFSP_MSGTX_RES_TIMCLBKERROR;
                            l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
                        }
                        break;
                    }

                    default:
                    {
                        /* Impossible end here */
                        l_eSM = e_eFSP_MSGTXPRV_SM_ELABDONE;
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
static bool_t eFSP_MSGTX_IsStatusStillCoherent(const t_eFSP_MSGTX_Ctx* p_ptCtx)
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

static e_eFSP_MSGTX_RES eFSP_MSGTX_ConvertRetFromMSGE(e_eFSP_MSGE_RES p_eRetEvent)
{
	e_eFSP_MSGTX_RES l_eRes;

	switch( p_eRetEvent )
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