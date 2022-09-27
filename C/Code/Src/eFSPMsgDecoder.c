/**
 * @file       eFSPMsgDecoder.c
 *
 * @brief      Message decoder utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgDecoder.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t isMsgDecStatusStillCoherent(const s_eFSP_MsgDCtx* ctx);
static e_eFSP_MsgD_Res convertReturnFromBstfToMSGD(e_eCU_dBUStf_Res returnedEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_MsgD_Res msgDecoderInitCtx(s_eFSP_MsgDCtx* const ctx, uint8_t* const memArea, const uint32_t memAreaSize,
								 cb_crc32_msgd cbCrcP, void* const clbCtx)
{
	/* Local variable */
	e_eFSP_MsgD_Res result;
	e_eCU_dBUStf_Res resultByStuff;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == memArea ) || ( NULL == cbCrcP ) || ( NULL == clbCtx ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity, we need some len to store the data */
        if( memAreaSize < EFSP_MIN_MSGDE_BUFFLEN )
        {
            result = MSGE_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status clbck */
            ctx->cbCrcPtr = cbCrcP;
            ctx->cbCrcCtx = clbCtx;

			/* initialize internal bytestuffer */
			resultByStuff =  bUStufferInitCtx(&ctx->byteUStufferCtnx, memArea, memAreaSize);
			result = convertReturnFromBstfToMSGD(resultByStuff);
        }
	}

	return result;
}

e_eFSP_MsgD_Res msgDecoderStartNewMsg(s_eFSP_MsgDCtx* const ctx)
{
	/* Local variable */
	e_eFSP_MsgD_Res result;
	e_eCU_dBUStf_Res resultByStuff;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgDecStatusStillCoherent(ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Update index */
			resultByStuff = bUStufferStartNewFrame(&ctx->byteUStufferCtnx);
			result = convertReturnFromBstfToMSGD(resultByStuff);
		}
	}

	return result;
}

e_eFSP_MsgD_Res msgDecoderGetDecodedData(s_eFSP_MsgDCtx* const ctx, uint8_t** dataP, uint32_t* const retrivedLen)
{
	/* Local variable */
	e_eFSP_MsgD_Res result;
	e_eCU_dBUStf_Res resultByStuff;
	uint8_t* dataPP;
	uint32_t dataSizeP;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == dataP ) || ( NULL == retrivedLen ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgDecStatusStillCoherent(ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            dataSizeP = 0u;
            dataPP = NULL;
			resultByStuff = bUStufferGetUnstufData(&ctx->byteUStufferCtnx, &dataPP, &dataSizeP);
			result = convertReturnFromBstfToMSGE(resultByStuff);

			if( MSGD_RES_OK == result )
			{
				/* Removed HADER from data ( CRC + DATA LEN ) */
                if( dataSizeP < EFSP_MIN_MSGDE_BUFFLEN )
                {
					/* No memory free */
					*dataP = &dataPP[8u];
                    *retrivedLen = 0u;
                }
                else
                {
                    /* Return reference of only the raw payload */
                    *dataP = &dataPP[8u];
                    *retrivedLen = dataSizeP - EFSP_MSGDE_HEADERSIZE;
                }
				
				result = MSGE_RES_OK;
			}
		}
	}

	return result;
}

e_eFSP_MsgD_Res msgDecoderGetDecodedLen(s_eFSP_MsgDCtx* const ctx, uint32_t* const retrivedLen)
{
	/* Local variable */
	e_eFSP_MsgD_Res result;
	e_eCU_dBUStf_Res resultByStuff;
	uint32_t dataSizeP;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == retrivedLen ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgDecStatusStillCoherent(ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            dataSizeP = 0u;
			resultByStuff = bUStufferGetUnstufLen(&ctx->byteUStufferCtnx, &dataSizeP);
			result = convertReturnFromBstfToMSGE(resultByStuff);

			if( MSGD_RES_OK == result )
			{
				/* Removed HADER from data ( CRC + DATA LEN ) */
                if( dataSizeP < EFSP_MIN_MSGDE_BUFFLEN )
                {
					/* No memory free */
                    *retrivedLen = 0u;
                }
                else
                {
                    /* Return reference of only the raw payload */
                    *retrivedLen = dataSizeP - EFSP_MSGDE_HEADERSIZE;
                }
				
				result = MSGE_RES_OK;
			}
		}
	}

	return result;
}

e_eFSP_MsgD_Res msgDecoderIsAFullMsgUnstuff(s_eFSP_MsgDCtx* const ctx, bool_t* const isMsgDec)
{
	/* Local variable */
	e_eFSP_MsgD_Res result;
	e_eCU_dBUStf_Res resultByStuff;
	uint32_t dataSizeP;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == isMsgDec ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgDecStatusStillCoherent(ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			resultByStuff = bUStufferIsAFullFrameUnstuff(&ctx->byteUStufferCtnx, isMsgDec);
			result = convertReturnFromBstfToMSGE(resultByStuff);
		}
	}

	return result;
}

e_eFSP_MsgD_Res bUStufferInsStufChunk(s_eFSP_MsgDCtx* const ctx, const uint8_t* encArea, const uint32_t encLen,
                                      uint32_t* const consumedEncData, uint32_t* errSofRec)
{
	/* Local variable */
	e_eFSP_MsgD_Res result;
	e_eCU_dBUStf_Res resultByStuff;
	uint32_t dataSizeP;
	uint8_t* dataPP;
	uint32_t dLenInMsg;
	uint32_t crcInMsg;
	uint32_t crcExp;
	uint32_t tempS;
	bool_t crcRes;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == encArea ) || ( NULL == consumedEncData ) || ( NULL == errSofRec ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == isMsgDecStatusStillCoherent(ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			resultByStuff = bUStufferInsStufChunk(&ctx->byteUStufferCtnx, encArea, encLen, consumedEncData, errSofRec);
			result = convertReturnFromBstfToMSGE(resultByStuff);
			
			if( MSGD_RES_FRAMEENDED == result)
			{
				/* Ok the frame is complete, need to check iv we have data size, data crc, crc rigth value */
				dataSizeP = 0u;
				dataPP = NULL;
				resultByStuff = bUStufferGetUnstufData(&ctx->byteUStufferCtnx, &dataPP, &dataSizeP);
				result = convertReturnFromBstfToMSGE(resultByStuff);
	
				if( MSGD_RES_OK == result )
				{
					/* Do we have enough data?  */
					if( dataSizeP < EFSP_MIN_MSGDE_BUFFLEN )
					{
						/* Too small frame, discharge */
						*errSofRec = *errSofRec + 1u;
						
						/* Restart */
						resultByStuff = bUStufferStartNewFrame(&ctx->byteUStufferCtnx);
						result = convertReturnFromBstfToMSGD(resultByStuff);
					}
					else
					{
						/* Enough data! Is data len in frame coherent?  */
						dLenInMsg = 0u;
						tempS = 0;
						
						/* Estrapolate data len in Little Endian */
                        tempS = (uint32_t) dataPP[0x04u];
                        dLenInMsg |= ( tempS & 0x000000FFu );
                        tempS =  (uint32_t) ( ( (uint32_t) dataPP[0x05u] ) << 8u  );
                        dLenInMsg |= ( tempS & 0x0000FF00u );
                        tempS =  (uint32_t) ( ( (uint32_t) dataPP[0x06u] ) << 16u  );
                        dLenInMsg |= ( tempS & 0x00FF0000u );
                        tempS =  (uint32_t) ( ( (uint32_t) dataPP[0x07u] ) << 24u  );
                        dLenInMsg |= ( tempS & 0xFF000000u );
						
						if( ( dataSizeP - EFSP_MIN_MSGDE_BUFFLEN ) == dLenInMsg)
						{
							/* Data len is coherent! Is crc rigth? */
							crcInMsg = 0u;
							crcExp = 0u;
							tempS = 0;
							
							/* Estrapolate CRC in Little Endian */
							tempS = (uint32_t) dataPP[0x00u];
							crcInMsg |= ( tempS & 0x000000FFu );
							tempS =  (uint32_t) ( ( (uint32_t) dataPP[0x01u] ) << 8u  );
							crcInMsg |= ( tempS & 0x0000FF00u );
							tempS =  (uint32_t) ( ( (uint32_t) dataPP[0x02u] ) << 16u  );
							crcInMsg |= ( tempS & 0x00FF0000u );
							tempS =  (uint32_t) ( ( (uint32_t) dataPP[0x03u] ) << 24u  );
							crcInMsg |= ( tempS & 0xFF000000u );
							
							/* Calculate CRC */
							crcRes = (*(ctx->cbCrcPtr))( ctx->cbCrcCtx, ECU_CRC_BASE_SEED, &dataPP[4u], dLenInMsg + 4u, 
														 &crcExp );
	
							if( true == crcRes )
							{
								if( crcInMsg == crcExp )
								{
									/* All ok */
								}
								else
								{
									/* Data Crc is wrong, discharge */
									*errSofRec = *errSofRec + 1u;
									
									/* Restart */
									resultByStuff = bUStufferStartNewFrame(&ctx->byteUStufferCtnx);
									result = convertReturnFromBstfToMSGD(resultByStuff);
								}
							}
							else
							{
								result = MSGE_RES_CRCCLBKERROR;
							}							
						}
						else
						{
							/* Data len is wrong, discharge */
							*errSofRec = *errSofRec + 1u;
							
							/* Restart */
							resultByStuff = bUStufferStartNewFrame(&ctx->byteUStufferCtnx);
							result = convertReturnFromBstfToMSGD(resultByStuff);
						}
					}
					
					result = MSGE_RES_OK;
				}
			}
		}
	}

	return result;
}

/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
bool_t isMsgDecStatusStillCoherent(const s_eFSP_MsgDCtx* ctx)
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

e_eFSP_MsgD_Res convertReturnFromBstfToMSGD(e_eCU_dBUStf_Res returnedEvent)
{
	e_eFSP_MsgE_Res result;

	switch( returnedEvent )
	{
		case DBUSTF_RES_OK:
		{
			result = MSGD_RES_OK;
            break;
		}

		case DBUSTF_RES_BADPARAM:
		{
			result = MSGD_RES_BADPARAM;
            break;
		}

		case DBUSTF_RES_BADPOINTER:
		{
			result = MSGD_RES_BADPOINTER;
            break;
		}

		case DBUSTF_RES_CORRUPTCTX:
		{
			result = MSGD_RES_CORRUPTCTX;
            break;
		}

		case DBUSTF_RES_OUTOFMEM:
		{
			result = MSGD_RES_OUTOFMEM;
            break;
		}

		case DBUSTF_RES_FRAMEENDED:
		{
			result = MSGD_RES_FRAMEENDED;
            break;
		}

		case DBUSTF_RES_NOINITLIB:
		{
			result = MSGD_RES_NOINITLIB;
            break;
		}
		
		default:
		{
            /* Impossible end here */
			result = MSGE_RES_BADPARAM;
            break;
		}
	}

	return result;
}
