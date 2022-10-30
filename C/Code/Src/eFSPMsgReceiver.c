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
static bool_t isMsgReceStatusStillCoherent(const s_eFSP_MsgRxCtx* ctx);
static e_eFSP_MsgRx_Res convertReturnFromMSGDToMSGRX(e_eFSP_MsgD_Res returnedEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_MsgRx_Res msgReceiverInitCtx(s_eFSP_MsgRxCtx* const ctx, const s_eFSP_MsgRxInitData* initData)
{
	/* Local variable */
	e_eFSP_MsgRx_Res result;
	e_eFSP_MsgD_Res resultMsgE;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == initData ) )
	{
		result = MSGTRX_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == initData->i_memArea ) || ( NULL == initData->i_receiveBuffArea ) || ( NULL == initData->i_cbCrcP ) ||
            ( NULL == initData->i_cbCrcCrx ) || ( NULL == initData->i_cbRxP ) || ( NULL == initData->i_cbRxCtx ) )
        {
            result = MSGTRX_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity, we need some len to store the data */
            if( ( initData->i_receiveBuffAreaSize < 1u ) || ( initData->i_frameTimeoutMs < 1u ) ||
                ( initData->i_timePerRecMs < 1u ) || ( initData->i_timePerRecMs > initData->i_frameTimeoutMs ) )
            {
                result = MSGTRX_RES_BADPARAM;
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
                ctx->timeCounterMs = 0u;
                ctx->frameTimeoutMs = initData->i_frameTimeoutMs;
                ctx->timePerRecMs = initData->i_timePerRecMs;
                ctx->needWaitFrameStart = initData->i_needWaitFrameStart;
                ctx->waitingFrameStart = true;

                /* initialize internal bytestuffer */
                resultMsgE =  msgDecoderInitCtx(&ctx->msgDecoderCtnx, initData->i_memArea, initData->i_memAreaSize,
                                                initData->i_cbCrcP, initData->i_cbCrcCrx);
                result = convertReturnFromMSGDToMSGRX(resultMsgE);
            }
        }
	}

	return result;
}

e_eFSP_MsgRx_Res msgReceiverStartNewMsg(s_eFSP_MsgRxCtx* const ctx)
{
	/* Local variable */
	e_eFSP_MsgRx_Res result;
	e_eFSP_MsgD_Res resultMsgE;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGTRX_RES_BADPOINTER;
	}
	else
	{
        /* Check internal status validity */
        if( false == isMsgReceStatusStillCoherent(ctx) )
        {
            result = MSGTRX_RES_CORRUPTCTX;
        }
        else
        {
            /* Reset internal variable */
            ctx->rxBuffCntr = 0u;
            ctx->rxBuffFill = 0u;
            ctx->timeCounterMs = 0u;

            /* Init message encoder */
            resultMsgE = msgDecoderStartNewMsg(&ctx->msgDecoderCtnx);
            result = convertReturnFromMSGDToMSGRX(resultMsgE);
        }
	}

	return result;
}

e_eFSP_MsgRx_Res msgReceiverGetDecodedData(s_eFSP_MsgRxCtx* const ctx, uint8_t** dataP, uint32_t* const retrivedLen)
{
	/* Local variable */
	e_eFSP_MsgRx_Res result;
	e_eFSP_MsgD_Res resultMsgE;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == dataP ) || ( NULL == retrivedLen ) )
	{
		result = MSGTRX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgReceStatusStillCoherent(ctx) )
		{
			result = MSGTRX_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
			resultMsgE = msgDecoderGetDecodedData(&ctx->msgDecoderCtnx, dataP, retrivedLen);
			result = convertReturnFromMSGDToMSGRX(resultMsgE);
		}
	}

	return result;
}

e_eFSP_MsgRx_Res msgReceiverReceiveChunk(s_eFSP_MsgRxCtx* const ctx)
{
	/* Local variable */
	e_eFSP_MsgRx_Res result;
	e_eFSP_MsgD_Res resultMsgE;

    /* Local variable of data to send */
    e_eFSP_rece_Priv_state stateM;
    uint32_t remainingTimeMs;
    uint32_t sendFramTimeTotal;
    uint32_t sendFramTimeSingle;
    const uint8_t *cDataToSendP;
    uint32_t cDataToSendLen;
    uint32_t cDataSendedSingle;


	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGTRX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgReceStatusStillCoherent(ctx) )
		{
			result = MSGTRX_RES_CORRUPTCTX;
		}
		else
		{
            /* Init time frame counter */
            sendFramTimeTotal = 0u;
            sendFramTimeSingle = 0u;
            result = MSGTRX_RES_OK;
            stateM = MSGRX_PRV_CHECKIFBUFFERTX;

            /* wait end elaboration or end for timeout */
            while( ( ctx->timeCounterMs < ctx->frameTimeoutMs ) && ( sendFramTimeTotal < ctx->timePerRecMs ) &&
                   ( stateM != MSGRX_PRV_ELABDONE ) )
            {
                switch( stateM )
                {
                    case MSGRX_PRV_CHECKIFBUFFERTX:
                    {
                        /* Is data present in send buffer? */
                        cDataToSendLen = ctx->rxBuffFill - ctx->rxBuffCntr;

                        if( cDataToSendLen > 0u )
                        {
                            /* Can send data in msg buffer */
                            stateM = MSGRX_PRV_SEND_BUFF;
                        }
                        else
                        {
                            /* No data in msg buffer */
                            stateM = MSGRX_PRV_CHECK_RETRIVECHUNK;
                        }
                        break;
                    }

                    case MSGRX_PRV_SEND_BUFF:
                    {
                        /* Refresh remaining time */
                        remainingTimeMs = ctx->frameTimeoutMs - ctx->timeCounterMs;

                        if( ( ctx->timePerRecMs - sendFramTimeTotal ) < remainingTimeMs )
                        {
                            remainingTimeMs = ctx->timePerRecMs - sendFramTimeTotal;
                        }

                        /* Get data to send */
                        cDataToSendP = &ctx->rxBuff[ctx->rxBuffCntr];
                        cDataToSendLen = ctx->rxBuffFill - ctx->rxBuffCntr;

                        /* Can send data from send buffer */
                        if( true == (*ctx->cbRxP)(ctx->cbRxCtx, cDataToSendP, cDataToSendLen, &cDataSendedSingle, remainingTimeMs, &sendFramTimeSingle) )
                        {
                            /* Check for some strangeness */
                            if( cDataSendedSingle > cDataToSendLen)
                            {
                                result = MSGTRX_RES_CORRUPTCTX;
                                stateM = MSGRX_PRV_ELABDONE;
                            }
                            else
                            {
                                /* Update sended counter */
                                ctx->rxBuffCntr += cDataSendedSingle;

                                /* Update timings */
                                sendFramTimeTotal += sendFramTimeSingle;
                                ctx->timeCounterMs += sendFramTimeSingle;

                                /* Go next state */
                                stateM = MSGRX_PRV_CHECKIFBUFFERTX;
                            }
                        }
                        else
                        {
                            /* Error sending data */
                            result = MSGTRX_RES_TXCLBKERROR;
                            stateM = MSGRX_PRV_ELABDONE;
                        }
                        break;
                    }

                    case MSGRX_PRV_CHECK_RETRIVECHUNK:
                    {
                        /* Reset counter */
                        ctx->rxBuffCntr = 0u;

                        /* Is data present in message encoder buffer? */
                        resultMsgE = msgEncoderRetriveEChunk(&ctx->msgDecoderCtnx, ctx->rxBuff, ctx->rxBuffSize, &ctx->rxBuffFill);
                        result = convertReturnFromMSGDToMSGRX(resultMsgE);

                        if( MSGTRX_RES_OK == result )
                        {
                            /* Retrived some data to send */
                            stateM = MSGRX_PRV_CHECKIFBUFFERTX;
                        }
                        else if( MSGTRX_RES_MESSAGESENDED == result )
                        {
                            if( 0u == ctx->rxBuffFill )
                            {
                                /* More data to send  */
                                stateM = MSGRX_PRV_ELABDONE;
                            }
                            else
                            {
                                /* No more data to retrive */
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

                    default:
                    {
                        /* Impossible end here */
                        stateM = MSGRX_PRV_ELABDONE;
                        result = MSGTRX_RES_CORRUPTCTX;
                        break;
                    }
                }
            }

            /* Check for timeout */
            if( ctx->timeCounterMs >= ctx->frameTimeoutMs )
            {
                if( ( MSGTRX_RES_OK == result ) || ( MSGTRX_RES_MESSAGESENDED == result ) )
                {
                    /* Frame can go in timeout when we are not finished (RES_OK) or when all the message is sended
                     * ( RES_MESSAGESENDED ) */
                    result = MSGTRX_RES_MESSAGETIMEOUT;
                }
            }
		}
	}

	return result;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
bool_t isMsgReceStatusStillCoherent(const s_eFSP_MsgRxCtx* ctx)
{
    bool_t result;

	/* Check pointer validity */
	if( ( NULL == ctx->rxBuff ) || ( NULL == ctx->cbRxP ) || ( NULL == ctx->cbRxCtx ) )
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

e_eFSP_MsgRx_Res convertReturnFromMSGDToMSGRX(e_eFSP_MsgD_Res returnedEvent)
{
	e_eFSP_MsgRx_Res result;

	switch( returnedEvent )
	{
		case MSGD_RES_OK:
		{
			result = MSGTRX_RES_OK;
            break;
		}

		case MSGD_RES_BADPARAM:
		{
			result = MSGTRX_RES_BADPARAM;
            break;
		}

		case MSGD_RES_BADPOINTER:
		{
			result = MSGTRX_RES_BADPOINTER;
            break;
		}

		case MSGD_RES_CORRUPTCTX:
		{
			result = MSGTRX_RES_CORRUPTCTX;
            break;
		}

        case MSGD_RES_OUTOFMEM:
        {
			result = MSGTRX_RES_OUTOFMEM;
            break;
        }

		case MSGD_RES_MESSAGEENDED:
		{
			result = MSGTRX_RES_MESSAGERECEIVED;
            break;
		}

		case MSGD_RES_NOINITLIB:
		{
			result = MSGTRX_RES_NOINITLIB;
            break;
		}

		case MSGD_RES_CRCCLBKERROR :
		{
			result = MSGTRX_RES_CRCCLBKERROR;
            break;
		}

		default:
		{
            /* Impossible end here */
			result = MSGTRX_RES_CORRUPTCTX;
            break;
		}
	}

	return result;
}