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



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t isMsgTransStatusStillCoherent(const s_eFSP_MsgECtx* ctx);
static e_eFSP_MsgTx_Res convertReturnFromMSGEToMSGTX(e_eFSP_MsgE_Res returnedEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_MsgTx_Res msgTransmInitCtx(s_eFSP_MsgTxCtx* const ctx, const s_eFSP_MsgTxInitData* initData)
{
	/* Local variable */
	e_eFSP_MsgTx_Res result;
	e_eFSP_MsgE_Res resultMsgE;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == initData ) )
	{
		result = MSGTTX_RES_BADPOINTER;
	}
	else
	{
        /* Check pointer validity */
        if( ( NULL == initData->i_memArea ) || ( NULL == initData->i_sendBuffArea ) || ( NULL == initData->i_cbCrcP ) ||
            ( NULL == initData->i_cbCrcCtx ) || ( NULL == initData->i_cbTxP ) || ( NULL == initData->i_cbTxCtx ) )
        {
            result = MSGTTX_RES_BADPOINTER;
        }
        else
        {
            /* Check data validity, we need some len to store the data */
            if( ( initData->i_sendBuffAreaSize < 1u ) || ( initData->i_frameTimeoutMs < initData->i_timePerSendMs ) )
            {
                result = MSGTTX_RES_BADPARAM;
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

                /* initialize internal bytestuffer */
                resultMsgE =  msgEncoderInitCtx(&ctx->msgEncoderCtnx, initData->i_memArea, initData->i_memAreaSize, initData->i_cbCrcP, initData->i_cbCrcCtx);
                result = convertReturnFromMSGEToMSGTX(resultMsgE);
            }
        }
	}

	return result;
}

e_eFSP_MsgTx_Res msgTransmStartNewMessage(s_eFSP_MsgTxCtx* const ctx, const uint32_t messageLen)
{
	/* Local variable */
	e_eFSP_MsgTx_Res result;
	e_eFSP_MsgE_Res resultMsgE;
	uint8_t* dataP;
	uint32_t maxDataSize;
    uint32_t cR32;
    bool_t crcRes;
    uint32_t nByteToSf;
	uint32_t nBToCrc;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGTTX_RES_BADPOINTER;
	}
	else
	{
        /* Check param validity, need at least 1 byte of paylaod */
        if( messageLen <= 0u )
        {
            result = MSGTTX_RES_BADPARAM;
        }
		else
		{
            /* Check internal status validity */
            if( false == isMsgTransStatusStillCoherent(ctx) )
            {
                result = MSGTTX_RES_CORRUPTCTX;
            }
            else
            {
                /* Reset internal variable */
                ctx->sendBuffCntr = 0u;
                ctx->sendBuffFill = 0u;
                ctx->timeCounterMs = 0u;

                /* Init message encoder */
                resultMsgE = msgEncoderStartNewMessage(&ctx->msgEncoderCtnx, messageLen);
                result = convertReturnFromMSGEToMSGTX(resultMsgE);
            }
		}
	}

	return result;
}
























/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_MsgTx_Res msgTransmGetPayloadLocation(s_eFSP_MsgTxCtx* const ctx, uint8_t** dataP, uint32_t* const maxDataSize)
{
	/* Local variable */
	e_eFSP_MsgTx_Res result;
	e_eFSP_MsgE_Res resultMsgE;
	uint8_t* dataPP;
	uint32_t maxDataSizeP;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == dataP ) || ( NULL == maxDataSize ) )
	{
		result = MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgTransStatusStillCoherent(ctx) )
		{
			result = MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            maxDataSizeP = 0u;
            dataPP = NULL;
			resultMsgE = bStufferGetUnStufDataLocation(&ctx->byteStufferCtnx, &dataPP, &maxDataSizeP);
			result = convertReturnFromMSGEToMSGTX(resultMsgE);

			if( MSGE_RES_OK == result )
			{
                if( maxDataSizeP < EFSP_MIN_MSGEN_BUFFLEN )
                {
                    result = MSGE_RES_CORRUPTCTX;
                }
                else
                {
                    /* Return reference of only the raw payload */
                    *dataP = &dataPP[EFSP_MSGEN_HEADERSIZE];
                    *maxDataSize = maxDataSizeP - EFSP_MSGEN_HEADERSIZE;
                }
			}
		}
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MEM-stack-param", "MISRAC2004-17.6_d", "MISRAC2012-Rule-1.3_s", "MISRAC2012-Rule-18.6_d", \
                            "CERT-DCL30-C_e"
#endif

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-17.4_b"
#endif

e_eFSP_MsgTx_Res msgTransmRestartCurrentMessage(s_eFSP_MsgTxCtx* const ctx)
{
	/* Local variable */
	e_eFSP_MsgTx_Res result;
	e_eFSP_MsgE_Res resultMsgE;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgTransStatusStillCoherent(ctx) )
		{
			result = MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Restart only the byte stuffer */
			resultMsgE = bStufferRestartCurrentFrame(&ctx->byteStufferCtnx);
			result = convertReturnFromMSGEToMSGTX(resultMsgE);
		}
	}

	return result;
}

e_eFSP_MsgTx_Res msgTransmSendChunk(s_eFSP_MsgTxCtx* const ctx)
{
	/* Local variable */
	e_eFSP_MsgTx_Res result;
	e_eFSP_MsgE_Res resultMsgE;

	/* Check pointer validity */
	if( ( NULL == ctx )  || ( NULL == encodeDest ) || ( NULL == filledLen ) )
	{
		result = MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgTransStatusStillCoherent(ctx) )
		{
			result = MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			resultMsgE = bStufferRetriStufChunk(&ctx->byteStufferCtnx, encodeDest, maxDestLen, filledLen);
			result = convertReturnFromMSGEToMSGTX(resultMsgE);
		}
	}

	return result;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
bool_t isMsgTransStatusStillCoherent(const s_eFSP_MsgECtx* ctx)
{
    bool_t result;

	/* Check context validity */
	if( ( NULL == ctx->cbCrcPtr ) || ( NULL == ctx->cbCrcCtx ) )
	{
		result = false;
	}
	else
	{
		result = true;
	}

    return result;
}

e_eFSP_MsgTx_Res convertReturnFromMSGEToMSGTX(e_eFSP_MsgE_Res returnedEvent)
{
	e_eFSP_MsgTx_Res result;

	switch( returnedEvent )
	{
		case DBSTF_RES_OK:
		{
			result = MSGE_RES_OK;
            break;
		}

		case DBSTF_RES_BADPARAM:
		{
			result = MSGE_RES_BADPARAM;
            break;
		}

		case DBSTF_RES_BADPOINTER:
		{
			result = MSGE_RES_BADPOINTER;
            break;
		}

		case DBSTF_RES_CORRUPTCTX:
		{
			result = MSGE_RES_CORRUPTCTX;
            break;
		}

		case DBSTF_RES_FRAMEENDED:
		{
			result = MSGE_RES_MESSAGEENDED;
            break;
		}

		case DBSTF_RES_NOINITLIB:
		{
			result = MSGE_RES_NOINITLIB;
            break;
		}

		case DBSTF_RES_NOINITFRAME :
		{
			result = MSGE_RES_NOINITMESSAGE;
            break;
		}

		default:
		{
            /* Impossible end here */
			result = MSGE_RES_CORRUPTCTX;
            break;
		}
	}

	return result;
}


