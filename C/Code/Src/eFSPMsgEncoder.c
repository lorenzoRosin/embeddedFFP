/**
 * @file       eFSPMsgDecoder.c
 *
 * @brief      Message encoder utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgEncoder.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t isMsgEncStatusStillCoherent(const s_eFSP_MsgECtx* ctx);
static e_eFSP_MsgE_Res convertReturnFromBstfToMSGE(e_eCU_dBStf_Res returnedEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_MsgE_Res msgEncoderInitCtx(s_eFSP_MsgECtx* const ctx, const uint8_t* memArea, const uint32_t memAreaSize,
								  cb_crc32_msge cbCrcP, void* const clbCtx)
{
	/* Local variable */
	e_eFSP_MsgE_Res result;
	e_eCU_dBStf_Res resultByteStuff;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == memArea )  || ( NULL == cbCrcP ) || ( NULL == clbCtx ) )
	{
		result = MSGE_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( memAreaSize <= EFSP_MIN_MSGEN_BUFFLEN )
        {
            result = MSGE_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status clbck */
            ctx->cbCrcPointer = cbCrcP;
            ctx->cbCrcCtx = clbCtx;
			
			/* initialize internal bytestuffer */
			resultByteStuff =  bStufferInitCtx(&ctx->byteStufferCtnx, memArea, memAreaSize);
			result = convertReturnFromBstfToMSGE(resultByteStuff);
        }
	}

	return result;
}
 
e_eFSP_MsgE_Res msgEncoderStartNewMessage(s_eFSP_MsgECtx* const ctx, const uint32_t  messageLen)
{
	/* Local variable */
	e_eFSP_MsgE_Res result;
	e_eCU_dBStf_Res resultByteStuff;
	uint8_t* dataP;
	uint32_t maxDataSize;
    uint32_t cR32;
    bool_t crcRes;
	
	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = DBSTF_RES_BADPOINTER;
	}
	else
	{
        /* Check param validity */
        if( frameLen <= 0u )
        {
            result = DBSTF_RES_BADPARAM;
        }
		else
		{
            /* Check internal status validity */
            if( false == isMsgEncStatusStillCoherent(ctx) )
            {
                result = DBSTF_RES_CORRUPTCTX;
            }
            else
            {
				/* Get memory reference */
				resultByteStuff = bStufferGetUnStufDataLocation(&ctx->byteStufferCtnx, &dataP, &maxDataSize);
				result = convertReturnFromBstfToMSGE(resultByteStuff);
				
				if( MSGE_RES_OK == result )
				{
					/* we have now the memory reference  */
					/* Insert in the buffer the message size and after the CRC32, in Little Endian */
					dataP[0x04u] = (uint8_t) ( ( messageLen        ) & 0x000000FFu ); 
					dataP[0x05u] = (uint8_t) ( ( messageLen >> 8u  ) & 0x000000FFu ); 
					dataP[0x06u] = (uint8_t) ( ( messageLen >> 16u ) & 0x000000FFu ); 
					dataP[0x07u] = (uint8_t) ( ( messageLen >> 24u ) & 0x000000FFu ); 
					
					/* Calculate the CRC of data payload and messageLen */
                    crcRes = (*(ctx->cbCrcPointer))( ctx->cbCrcCtx, ECU_CRC_BASE_SEED, &dataP[4u], messageLen + 4u, &cR32 );

                    if( true == crcRes )
                    {
						/* Insert in the buffer the CRC32, in Little Endian */
						dataP[0x00u] = (uint8_t) ( ( cR32        ) & 0x000000FFu ); 
						dataP[0x01u] = (uint8_t) ( ( cR32 >> 8u  ) & 0x000000FFu ); 
						dataP[0x02u] = (uint8_t) ( ( cR32 >> 16u ) & 0x000000FFu ); 
						dataP[0x03u] = (uint8_t) ( ( cR32 >> 24u ) & 0x000000FFu ); 
						
						/* the message frame is ready, need to start the bytestuffer */
						resultByteStuff = bStufferStartNewFrame(&ctx->byteStufferCtnx, ( EFSP_MSGEN_HEADERSIZE + messageLen ));
						result = convertReturnFromBstfToMSGE(resultByteStuff);
                    }
					else
					{
						result = MSGE_RES_CRCCLBKERROR;
					}
				}				
            }
		}
	}

	return result;
}

e_eFSP_MsgE_Res msgEncoderGetPayloadLocation(s_eFSP_MsgECtx* const ctx, uint8_t** dataP, uint32_t* const maxDataSize)
{
	/* Local variable */
	e_eFSP_MsgE_Res result;
	e_eCU_dBStf_Res resultByteStuff;
	uint8_t* dataPP;
	uint32_t maxDataSizeP;
    uint32_t cR32;
    bool_t crcRes;
	
	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = DBSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgEncStatusStillCoherent(ctx) )
		{
			result = DBSTF_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			resultByteStuff = bStufferGetUnStufDataLocation(&ctx->byteStufferCtnx, &dataPP, &maxDataSizeP);
			result = convertReturnFromBstfToMSGE(resultByteStuff);
			
			if( MSGE_RES_OK == result )
			{
				*dataP = &dataPP[8u];
				*maxDataSize = maxDataSizeP - EFSP_MSGEN_HEADERSIZE;
			}				
		}
	}

	return result;
}

e_eFSP_MsgE_Res msgEncoderRestartCurrentMessage(s_eFSP_MsgECtx* const ctx)
{
	/* Local variable */
	e_eFSP_MsgE_Res result;
	e_eCU_dBStf_Res resultByteStuff;
	uint8_t* dataPP;
	uint32_t maxDataSizeP;
    uint32_t cR32;
    bool_t crcRes;
	
	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = DBSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgEncStatusStillCoherent(ctx) )
		{
			result = DBSTF_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			resultByteStuff = bStufferRestartCurrentFrame(&ctx->byteStufferCtnx);
			result = convertReturnFromBstfToMSGE(resultByteStuff);			
		}
	}

	return result;
}

e_eFSP_MsgE_Res msgEncoderGetRemToRetrive(s_eFSP_MsgECtx* const ctx, uint32_t* const retrivedLen)
{
	/* Local variable */
	e_eFSP_MsgE_Res result;
	e_eCU_dBStf_Res resultByteStuff;
	uint8_t* dataPP;
	uint32_t maxDataSizeP;
    uint32_t cR32;
    bool_t crcRes;
	
	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = DBSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgEncStatusStillCoherent(ctx) )
		{
			result = DBSTF_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			resultByteStuff = bStufferGetRemToRetrive(&ctx->byteStufferCtnx, retrivedLen);
			result = convertReturnFromBstfToMSGE(resultByteStuff);			
		}
	}

	return result;
}

e_eFSP_MsgE_Res msgEncoderRetriveEChunk(s_eFSP_MsgECtx* const ctx, uint8_t* const encodeDest,
									   const uint32_t maxDestLen, uint32_t* const filledLen)
{
	/* Local variable */
	e_eFSP_MsgE_Res result;
	e_eCU_dBStf_Res resultByteStuff;
	uint8_t* dataPP;
	uint32_t maxDataSizeP;
    uint32_t cR32;
    bool_t crcRes;
	
	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = DBSTF_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgEncStatusStillCoherent(ctx) )
		{
			result = DBSTF_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			resultByteStuff = bStufferRetriStufChunk(&ctx->byteStufferCtnx, encodeDest, maxDestLen, filledLen);
			result = convertReturnFromBstfToMSGE(resultByteStuff);			
		}
	}

	return result;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
bool_t isMsgEncStatusStillCoherent(const s_eFSP_MsgECtx* ctx)
{
    bool_t result;

	/* Check context validity */
	if( ( ctx->memAreaSize <= 0u ) || ( NULL == ctx->memArea ) || ( ctx->memAreaCntr > ctx->memAreaSize ) )
	{
		result = false;
	}
	else
	{
		/* Check data coherence */
		if( ( ( true == ctx->needSof ) && ( false == ctx->needEof ) ) ||
            ( ( true == ctx->needSof ) && ( true == ctx->precedentWasEsc ) ) ||
            ( ( false == ctx->needEof ) && ( true == ctx->precedentWasEsc ) ) )
		{
            result = false;
		}
		else
		{
            if( ( true == ctx->needSof ) && ( 0u != ctx->memAreaCntr ) )
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

e_eFSP_MsgE_Res convertReturnFromBstfToMSGE(e_eCU_dBStf_Res returnedEvent)
{
	e_eFSP_MsgE_Res result;
	
	switch()
	{
		case(DBSTF_RES_OK):
		{
			result = MSGE_RES_OK;
		}break;
		
		case(DBSTF_RES_BADPARAM):
		{
			result = MSGE_RES_BADPARAM;
		}break;
		
		case(DBSTF_RES_BADPOINTER):
		{
			result = MSGE_RES_BADPOINTER;
		}break;
		
		case(DBSTF_RES_CORRUPTCTX):
		{
			result = MSGE_RES_CORRUPTCTX;
		}break;
		
		case(DBSTF_RES_FRAMEENDED):
		{
			result = MSGE_RES_MESSAGEENDED;
		}break;
		
		case(DBSTF_RES_NOINITLIB):
		{
			result = MSGE_RES_NOINITLIB;
		}break;
		
		case(DBSTF_RES_NOINITFRAME):
		{
			result = MSGE_RES_NOINITMESSAGE;
		}break;
		
		default
		{
			result = MSGE_RES_BADPARAM;
		}
	}
	
	return result;
}


