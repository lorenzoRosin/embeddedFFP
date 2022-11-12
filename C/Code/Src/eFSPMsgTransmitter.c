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
static bool_t isMsgTransStatusStillCoherent(const s_eFSP_MSGTX_Ctx* ctx);
static e_eFSP_MSGTX_Res convertReturnFromMSGEToMSGTX(e_eFSP_MSGE_Res returnedEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_MSGTX_Res MSGTX_InitCtx(s_eFSP_MSGTX_Ctx* const ctx, const s_eFSP_MSGTX_InitData* initData)
{
	/* Local variable */
	e_eFSP_MSGTX_Res result;
	e_eFSP_MSGE_Res resultMsgE;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == initData ) )
	{
		result = MSGTX_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == initData->i_memArea ) || ( NULL == initData->i_sendBuffArea ) || ( NULL == initData->i_cbCrcP ) ||
            ( NULL == initData->i_cbCrcCtx ) || ( NULL == initData->i_cbTxP ) || ( NULL == initData->i_cbTxCtx ) )
        {
            result = MSGTX_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity, we need some len to store the data */
            if( ( initData->i_sendBuffAreaSize < 1u ) || ( initData->i_frameTimeoutMs < 1u ) ||
                ( initData->i_timePerSendMs < 1u ) || ( initData->i_timePerSendMs > initData->i_frameTimeoutMs ) )
            {
                result = MSGTX_RES_BADPARAM;
            }
            else
            {
                /* Initialize internal status variable */
                ctx->sendBuff = initData->i_sendBuffArea;
                ctx->sendBuffSize = initData->i_sendBuffAreaSize;
                ctx->sendBuffCntr = 0u;
                ctx->sendBuffFill = 0u;
                ctx->cbTxP = initData->i_cbTxP;
                ctx->cbTxCtx = initData->i_cbTxCtx;
                ctx->timeCounterMs = 0u;
                ctx->frameTimeoutMs = initData->i_frameTimeoutMs;
                ctx->timePerSendMs = initData->i_timePerSendMs;

                /* initialize internal message encoder */
                resultMsgE =  MSGE_InitCtx(&ctx->msgEncoderCtnx, initData->i_memArea, initData->i_memAreaSize,
                                                initData->i_cbCrcP, initData->i_cbCrcCtx);
                result = convertReturnFromMSGEToMSGTX(resultMsgE);
            }
        }
	}

	return result;
}

e_eFSP_MSGTX_Res MSGTX_StartNewMessage(s_eFSP_MSGTX_Ctx* const ctx, const uint32_t messageLen)
{
	/* Local variable */
	e_eFSP_MSGTX_Res result;
	e_eFSP_MSGE_Res resultMsgE;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGTX_RES_BADPOINTER;
	}
	else
	{
        /* Check param validity, need at least 1 byte of paylaod */
        if( messageLen <= 0u )
        {
            result = MSGTX_RES_BADPARAM;
        }
		else
		{
            /* Check internal status validity */
            if( false == isMsgTransStatusStillCoherent(ctx) )
            {
                result = MSGTX_RES_CORRUPTCTX;
            }
            else
            {
                /* Reset internal variable */
                ctx->sendBuffCntr = 0u;
                ctx->sendBuffFill = 0u;
                ctx->timeCounterMs = 0u;

                /* Init message encoder */
                resultMsgE = MSGE_StartNewMessage(&ctx->msgEncoderCtnx, messageLen);
                result = convertReturnFromMSGEToMSGTX(resultMsgE);
            }
		}
	}

	return result;
}

e_eFSP_MSGTX_Res MSGTX_GetPayloadLocation(s_eFSP_MSGTX_Ctx* const ctx, uint8_t** dataP, uint32_t* const maxDataSize)
{
	/* Local variable */
	e_eFSP_MSGTX_Res result;
	e_eFSP_MSGE_Res resultMsgE;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == dataP ) || ( NULL == maxDataSize ) )
	{
		result = MSGTX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgTransStatusStillCoherent(ctx) )
		{
			result = MSGTX_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
			resultMsgE = MSGE_GetPayloadLocation(&ctx->msgEncoderCtnx, dataP, maxDataSize);
			result = convertReturnFromMSGEToMSGTX(resultMsgE);
		}
	}

	return result;
}

e_eFSP_MSGTX_Res MSGTX_RestartCurrentMessage(s_eFSP_MSGTX_Ctx* const ctx)
{
	/* Local variable */
	e_eFSP_MSGTX_Res result;
	e_eFSP_MSGE_Res resultMsgE;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGTX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgTransStatusStillCoherent(ctx) )
		{
			result = MSGTX_RES_CORRUPTCTX;
		}
		else
		{
            /* Reset internal variable */
            ctx->sendBuffCntr = 0u;
            ctx->sendBuffFill = 0u;
            ctx->timeCounterMs = 0u;

			/* Restart only the byte stuffer */
			resultMsgE = MSGE_RestartCurrentMessage(&ctx->msgEncoderCtnx);
			result = convertReturnFromMSGEToMSGTX(resultMsgE);
		}
	}

	return result;
}

e_eFSP_MSGTX_Res MSGTX_SendChunk(s_eFSP_MSGTX_Ctx* const ctx)
{
	/* Local variable of the operation result */
	e_eFSP_MSGTX_Res result;
	e_eFSP_MSGE_Res resultMsgE;

    /* Local variable to keep track of the current state machine state */
    e_eFSP_MSGTX_Priv_state stateM;

    /* Local variable to keep track of the total time spended in the function */
    uint32_t totalTxTime;

    /* Local variable usend for the current time calculation */
    uint32_t cTxTime;
    uint32_t cRemainTxTime;

    /* Local variable usend for the current data calculation */
    const uint8_t *cDToTxP;
    uint32_t cDToTxLen;
    uint32_t cDTxed;


	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGTX_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgTransStatusStillCoherent(ctx) )
		{
			result = MSGTX_RES_CORRUPTCTX;
		}
		else
		{
            /* Init time frame counter */
            totalTxTime = 0u;
            cTxTime = 0u;
            result = MSGTX_RES_OK;
            stateM = MSGTX_PRV_CHECKIFBUFFERTX;

            /* wait end elaboration or end for timeout */
            while( ( ctx->timeCounterMs < ctx->frameTimeoutMs ) && ( totalTxTime < ctx->timePerSendMs ) &&
                   ( stateM != MSGTX_PRV_ELABDONE ) )
            {
                switch( stateM )
                {
                    case MSGTX_PRV_CHECKIFBUFFERTX:
                    {
                        /* Is data present in send buffer? */
                        cDToTxLen = ctx->sendBuffFill - ctx->sendBuffCntr;

                        if( cDToTxLen > 0u )
                        {
                            /* Can send data in msg buffer */
                            stateM = MSGTX_PRV_SENDBUFF;
                        }
                        else
                        {
                            /* No data in msg buffer, retrive some other chunk of data, only if the message send is not
                             * completed of course */
                            stateM = MSGTX_PRV_RETRIVECHUNK;
                        }
                        break;
                    }

                    case MSGTX_PRV_SENDBUFF:
                    {
                        /* Ok, we need to send data present in the send buffer */
                        /* Refresh Total remaining time */
                        cRemainTxTime = ctx->frameTimeoutMs - ctx->timeCounterMs;

                        /* If this session timeout is lower thant total remaining time ocntinue using the session
                         * remaining time */
                        if( ( ctx->timePerSendMs - totalTxTime ) < cRemainTxTime )
                        {
                            cRemainTxTime = ctx->timePerSendMs - totalTxTime;
                        }

                        /* Get data to send */
                        cDToTxP = &ctx->sendBuff[ctx->sendBuffCntr];
                        cDToTxLen = ctx->sendBuffFill - ctx->sendBuffCntr;

                        /* Can send data from send buffer */
                        if( true == (*ctx->cbTxP)(ctx->cbTxCtx, cDToTxP, cDToTxLen, &cDTxed, cRemainTxTime, &cTxTime) )
                        {
                            /* Check for some strangeness */
                            if( cDTxed > cDToTxLen)
                            {
                                result = MSGTX_RES_CORRUPTCTX;
                                stateM = MSGTX_PRV_ELABDONE;
                            }
                            else
                            {
                                /* Update sended counter */
                                ctx->sendBuffCntr += cDTxed;

                                /* Update timings */
                                totalTxTime += cTxTime;
                                ctx->timeCounterMs += cTxTime;

                                /* Go next state */
                                stateM = MSGTX_PRV_CHECKIFBUFFERTX;
                            }
                        }
                        else
                        {
                            /* Error sending data */
                            result = MSGTX_RES_TXCLBKERROR;
                            stateM = MSGTX_PRV_ELABDONE;
                        }
                        break;
                    }

                    case MSGTX_PRV_RETRIVECHUNK:
                    {
                        /* Ok, the send buffer is empty, need to load remainings data */
                        ctx->sendBuffCntr = 0u;
                        ctx->sendBuffFill = 0u;

                        /* Is data present in message encoder buffer? */
                        resultMsgE = MSGE_RetriveEChunk(&ctx->msgEncoderCtnx, ctx->sendBuff, ctx->sendBuffSize,
                                                        &ctx->sendBuffFill);
                        result = convertReturnFromMSGEToMSGTX(resultMsgE);

                        if( MSGTX_RES_OK == result )
                        {
                            /* Retrived some data to send, by design if MSGE_RetriveEChunk return MSGE_RES_OK this
                             * means that the value of loaded data inside send buffer is equals to it's size */

                            /* Can go to send loaded data now */
                            stateM = MSGTX_PRV_CHECKIFBUFFERTX;
                        }
                        else if( MSGTX_RES_MESSAGESENDED == result )
                        {
                            /* Ok we retrived all the possible data */
                            if( 0u == ctx->sendBuffFill )
                            {
                                /* No more data to send or retrive */
                                stateM = MSGTX_PRV_ELABDONE;
                            }
                            else
                            {
                                /* Ok retrived all data but need to send the remaining present in sendBuff */
                                stateM = MSGTX_PRV_CHECKIFBUFFERTX;
                            }
                        }
                        else
                        {
                            /* Some error */
                            stateM = MSGTX_PRV_ELABDONE;
                        }

                        break;
                    }

                    default:
                    {
                        /* Impossible end here */
                        stateM = MSGTX_PRV_ELABDONE;
                        result = MSGTX_RES_CORRUPTCTX;
                        break;
                    }
                }
            }

            /* Check for timeout */
            if( ctx->timeCounterMs >= ctx->frameTimeoutMs )
            {
                if( ( MSGTX_RES_OK == result ) || ( MSGTX_RES_MESSAGESENDED == result ) )
                {
                    /* Frame can go in timeout when we are not finished (RES_OK) or when all the message is sended
                     * ( RES_MESSAGESENDED ) */
                    result = MSGTX_RES_MESSAGETIMEOUT;
                }
            }
		}
	}

	return result;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
bool_t isMsgTransStatusStillCoherent(const s_eFSP_MSGTX_Ctx* ctx)
{
    bool_t result;

	/* Check pointer validity */
	if( ( NULL == ctx->sendBuff ) || ( NULL == ctx->cbTxP ) || ( NULL == ctx->cbTxCtx ) )
	{
		result = false;
	}
	else
	{
        /* Check send buffer validity */
        if( ctx->sendBuffSize < 1u )
        {
            result =  false;
        }
        else
        {
            /* Check send buffer validity */
            if( ( ctx->sendBuffFill > ctx->sendBuffSize )  || ( ctx->sendBuffCntr > ctx->sendBuffFill ) )
            {
                result =  false;
            }
            else
            {
                /* Check timings validity */
                if( ( ctx->frameTimeoutMs < 1u ) || ( ctx->timePerSendMs < 1u ) )
                {
                    result = false;
                }
                else
                {
                    /* Check timings validity */
                    if( ctx->timePerSendMs > ctx->frameTimeoutMs )
                    {
                        result = false;
                    }
                    else
                    {
                        result = true;
                    }
                }
            }
        }
	}

    return result;
}

e_eFSP_MSGTX_Res convertReturnFromMSGEToMSGTX(e_eFSP_MSGE_Res returnedEvent)
{
	e_eFSP_MSGTX_Res result;

	switch( returnedEvent )
	{
		case MSGE_RES_OK:
		{
			result = MSGTX_RES_OK;
            break;
		}

		case MSGE_RES_BADPARAM:
		{
			result = MSGTX_RES_BADPARAM;
            break;
		}

		case MSGE_RES_BADPOINTER:
		{
			result = MSGTX_RES_BADPOINTER;
            break;
		}

		case MSGE_RES_CORRUPTCTX:
		{
			result = MSGTX_RES_CORRUPTCTX;
            break;
		}

		case MSGE_RES_MESSAGEENDED:
		{
			result = MSGTX_RES_MESSAGESENDED;
            break;
		}

		case MSGE_RES_NOINITLIB:
		{
			result = MSGTX_RES_NOINITLIB;
            break;
		}

		case MSGE_RES_NOINITMESSAGE :
		{
			result = MSGTX_RES_NOINITMESSAGE;
            break;
		}

		case MSGE_RES_CRCCLBKERROR :
		{
			result = MSGTX_RES_CRCCLBKERROR;
            break;
		}

		default:
		{
            /* Impossible end here */
			result = MSGTX_RES_CORRUPTCTX;
            break;
		}
	}

	return result;
}