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
#include "eCUCrc.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t isMsgDecStatusStillCoherent(const s_eFSP_MsgDCtx* ctx);
static e_eFSP_MsgD_Res convertReturnFromBstfToMSGD(e_eCU_dBUStf_Res returnedEvent);
static e_eFSP_MsgD_Res isMsgCorrect(s_eCU_BUStuffCtx* ctx, bool_t* isCorrect, cb_crc32_msgd cbCrcPtr, void* cbCrcCtx);
static uint32_t composeU32LE(uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4);


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
            result = MSGD_RES_BADPARAM;
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

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-17.4_b",  "MEM-stack-param", "MISRAC2004-17.6_d", "MISRAC2012-Rule-1.3_s", \
                            "MISRAC2012-Rule-18.6_d", "CERT-DCL30-C_e"
    /* Suppressed for code clarity */
#endif

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
			result = convertReturnFromBstfToMSGD(resultByStuff);

			if( MSGD_RES_OK == result )
			{
                /* Starting point is always the same */
                *dataP = &dataPP[EFSP_MSGDE_HEADERSIZE];

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

				result = MSGD_RES_OK;
			}
		}
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-17.4_b",  "MEM-stack-param", "MISRAC2004-17.6_d", "MISRAC2012-Rule-1.3_s", \
                            "MISRAC2012-Rule-18.6_d", "CERT-DCL30-C_e"
#endif

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
			result = convertReturnFromBstfToMSGD(resultByStuff);

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

				result = MSGD_RES_OK;
			}
		}
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-17.4_b"
    /* Suppressed for code clarity */
#endif

e_eFSP_MsgD_Res msgDecoderIsAFullMsgUnstuff(s_eFSP_MsgDCtx* const ctx, bool_t* const isMsgDec)
{
	/* Local variable */
	e_eFSP_MsgD_Res result;
	e_eCU_dBUStf_Res resultByStuff;

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
			result = convertReturnFromBstfToMSGD(resultByStuff);
		}
	}

	return result;
}

e_eFSP_MsgD_Res msgDecoderGetMostEffDatLen(s_eFSP_MsgDCtx* const ctx, uint32_t* const mostEffPayload)
{
	/* Local variable */
	e_eFSP_MsgD_Res result;
	e_eCU_dBUStf_Res resultByStuff;
    bool_t isFullUnstuffed;
	uint32_t dataSizeP;
	uint8_t* dataPP;
    uint32_t dLenInMsg;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == mostEffPayload ) )
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
            isFullUnstuffed = false;
			resultByStuff = bUStufferIsAFullFrameUnstuff(&ctx->byteUStufferCtnx, &isFullUnstuffed);
			result = convertReturnFromBstfToMSGD(resultByStuff);

            if( MSGD_RES_OK == result )
            {
                if( true == isFullUnstuffed )
                {
                    /* Full unstuffed, done */
                    *mostEffPayload = 0u;

                }
                else
                {
                    /* How many byte are decoded? */
                    dataSizeP = 0u;
                    dataPP = NULL;
                    resultByStuff = bUStufferGetUnstufData(&ctx->byteUStufferCtnx, &dataPP, &dataSizeP);
                    result = convertReturnFromBstfToMSGD(resultByStuff);

                    if( MSGD_RES_OK == result )
                    {
                        /* Do we have enough data?  */
                        if( dataSizeP < EFSP_MSGDE_HEADERSIZE )
                        {
                            /* Need to receive all the eder before estimating data size */
                            *mostEffPayload = EFSP_MSGDE_HEADERSIZE - dataSizeP;
                        }
                        else
                        {
                            /* Enough data! Start remaining data estimation */
                            dLenInMsg = composeU32LE(dataPP[0x04u], dataPP[0x05u], dataPP[0x06u], dataPP[0x07u]);

                            if( ( dataSizeP - EFSP_MSGDE_HEADERSIZE ) < dLenInMsg)
                            {
                                *mostEffPayload = dLenInMsg - ( dataSizeP - EFSP_MSGDE_HEADERSIZE );
                            }
                            else
                            {
                                /* No more data but frame not ended, probably we are waiting EOF os some ESC
                                 * character */
                                *mostEffPayload  = 1u;
                            }
                        }
                    }
                }
            }
		}
	}

	return result;
}

e_eFSP_MsgD_Res msgDecoderInsEncChunk(s_eFSP_MsgDCtx* const ctx, const uint8_t* encArea, const uint32_t encLen,
                                      uint32_t* const consumedEncData, uint32_t* errSofRec)
{
	/* Local variable */
	e_eFSP_MsgD_Res result;
    e_eFSP_MsgD_Res resultMsgCorrect;
	e_eCU_dBUStf_Res resultByStuff;
    bool_t isMCorrect;
    const uint8_t *currentArea;
    uint32_t currentEncLen;
    uint32_t currentCosumed;
    uint32_t totalCosumed;
    uint32_t currentErrSofRec;
    uint32_t totalErrSofRec;
    bool_t needToParseRemainingData;

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
            /* Init return value before while */
            *consumedEncData = 0u;
            *errSofRec = 0u;

            needToParseRemainingData = false;

            /* Init var before while */
            currentArea = encArea;
            currentEncLen = encLen;
            totalCosumed = 0u;
            totalErrSofRec = 0u;

            /* Elaborate */
            do
            {
                /* Init partial coutner  */
                currentCosumed = 0u;
                currentErrSofRec = 0u;

                /* Insert data */
                resultByStuff = bUStufferInsStufChunk(&ctx->byteUStufferCtnx, currentArea, currentEncLen, &currentCosumed, &currentErrSofRec);
                result = convertReturnFromBstfToMSGD(resultByStuff);

                /* update total counter */
                if( totalErrSofRec < ( 0xFFFFFFFFu - currentErrSofRec ) )
                {
                    totalErrSofRec += currentErrSofRec;
                }
                else
                {
                    totalErrSofRec = 0xFFFFFFFFu;
                    result = MSGD_RES_OUTOFMEM;
                }

                /* update total counter */
                if( totalCosumed < ( 0xFFFFFFFFu - currentCosumed ) )
                {
                    totalCosumed += currentCosumed;
                }
                else
                {
                    totalCosumed = 0xFFFFFFFFu;
                    result = MSGD_RES_OUTOFMEM;
                }

                if( MSGD_RES_FRAMEENDED == result )
                {
                    /* Verify msg integrity */
                    resultMsgCorrect = isMsgCorrect(&ctx->byteUStufferCtnx, &isMCorrect, ctx->cbCrcPtr, ctx->cbCrcCtx);

                    if( MSGD_RES_OK == resultMsgCorrect )
                    {
                        /* no strange error found, check message correctness */
                        if( true != isMCorrect )
                        {
                            /* Too small frame or bad cr found, discharge and continue parse data if present */
                            /* Increase error counter if frame is wrong */
                            if( totalErrSofRec < ( 0xFFFFFFFFu - currentErrSofRec ) )
                            {
                                totalErrSofRec += currentErrSofRec;
                            }
                            else
                            {
                                totalErrSofRec = 0xFFFFFFFFu;
                                result = MSGD_RES_OUTOFMEM;
                            }

                            if( MSGD_RES_FRAMEENDED == result )
                            {
                                resultByStuff = bUStufferStartNewFrame(&ctx->byteUStufferCtnx);
                                result = convertReturnFromBstfToMSGD(resultByStuff);

                                if( MSGD_RES_OK == result )
                                {
                                    /* retrigger and update counter if we have some space */
                                    if( totalCosumed < encLen )
                                    {
                                        /* retrigger */
                                        needToParseRemainingData = true;

                                        /* Update pointer */
                                        currentArea = &encArea[totalCosumed];
                                        currentEncLen = encLen - totalCosumed;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        /* Some error */
                        result = resultMsgCorrect;
                    }
                }

            }while( true == needToParseRemainingData );

            /* Update return variable */
            *consumedEncData = totalCosumed;
            *errSofRec = totalErrSofRec;
		}
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-17.4_b"
#endif

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
	e_eFSP_MsgD_Res result;

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
			result = MSGD_RES_CORRUPTCTX;
            break;
		}
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-17.4_b", "MISRAC2012-Rule-8.13"
    /* Suppressed for code clarity */
#endif

e_eFSP_MsgD_Res isMsgCorrect(s_eCU_BUStuffCtx* ctx, bool_t* isCorrect, cb_crc32_msgd cbCrcPtr, void* cbCrcCtx)
{
    e_eFSP_MsgD_Res result;
    e_eCU_dBUStf_Res byteUnstuffRes;
    uint32_t dLenInMsg;
	uint32_t crcInMsg;
	uint32_t crcExp;
	uint32_t dataSizeP;
    bool_t crcRes;
	uint8_t* dataPP;

    /* Check NULL */
    if( ( NULL == ctx ) || ( NULL == isCorrect) || ( NULL == cbCrcPtr) || ( NULL == cbCrcCtx ) )
    {
        result = MSGD_RES_BADPOINTER;
    }
    else
    {
        /* Ok the frame is complete, need to check if we have data size, data crc, crc rigth value */

        /* Init value */
        dataSizeP = 0u;
        dataPP = NULL;

        /* Get unstuffed data */
        byteUnstuffRes = bUStufferGetUnstufData(ctx, &dataPP, &dataSizeP);
        result = convertReturnFromBstfToMSGD(byteUnstuffRes);

        if( MSGD_RES_OK == result )
        {
            /* Do we have enough data?  */
            if( dataSizeP < EFSP_MIN_MSGDE_BUFFLEN )
            {
                /* Too small frame, discharge */
                *isCorrect = false;
            }
            else
            {
                /* Enough data! Is data len in frame coherent?  */
                dLenInMsg = composeU32LE(dataPP[0x04u], dataPP[0x05u], dataPP[0x06u], dataPP[0x07u]);

                if( ( dataSizeP - EFSP_MSGDE_HEADERSIZE ) == dLenInMsg)
                {
                    /* Data len is coherent! Is crc rigth? */
                    crcExp = 0u;

                    /* Estrapolate CRC in Little Endian */
                    crcInMsg = composeU32LE(dataPP[0x00u], dataPP[0x01u], dataPP[0x02u], dataPP[0x03u]);

                    /* Calculate CRC */
                    crcRes = (*(cbCrcPtr))( cbCrcCtx, ECU_CRC_BASE_SEED, &dataPP[4u], dLenInMsg + 4u,  &crcExp );

                    if( true == crcRes )
                    {
                        if( crcInMsg == crcExp )
                        {
                            /* All ok */
                            *isCorrect = true;
                        }
                        else
                        {
                            /* Data Crc is wrong, discharge */
                            *isCorrect = false;
                        }
                    }
                    else
                    {
                        result = MSGD_RES_CRCCLBKERROR;
                    }
                }
                else
                {
                    /* Data len is wrong, discharge */
                    *isCorrect = false;
                }
            }
        }
    }

    return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-17.4_b", "MISRAC2012-Rule-8.13"
#endif

uint32_t composeU32LE(uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4)
{
    uint32_t tempS;
    uint32_t result;

    /* Init var */
    result = 0u;

    /* Calculate */
    tempS =                 (uint32_t) v1;
    result |= ( tempS & 0x000000FFu );
    tempS =  (uint32_t) ( ( (uint32_t) v2 ) << 8u  );
    result |= ( tempS & 0x0000FF00u );
    tempS =  (uint32_t) ( ( (uint32_t) v3 ) << 16u  );
    result |= ( tempS & 0x00FF0000u );
    tempS =  (uint32_t) ( ( (uint32_t) v4 ) << 24u  );
    result |= ( tempS & 0xFF000000u );

    return result;
}
