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
#include "eCUCrc.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSP_MSGE_isStatusStillCoherent(const s_eFSP_MSGE_Ctx* ctx);
static e_eFSP_MSGE_Res eFSP_MSGE_convertReturnFromBstf(e_eCU_BSTF_Res returnedEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed because this warning is not working */
#endif

e_eFSP_MSGE_Res eFSP_MSGE_InitCtx(s_eFSP_MSGE_Ctx* const ctx, uint8_t memArea[], const uint32_t memAreaSize,
								  cb_crc32_msge cbCrcP, void* const clbCtx)
{
	/* Local variable */
	e_eFSP_MSGE_Res result;
	e_eCU_BSTF_Res resultByStuff;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == memArea ) || ( NULL == cbCrcP ) || ( NULL == clbCtx ) )
	{
		result = MSGE_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity, we need some len to store the data */
        if( memAreaSize < EFSP_MIN_MSGEN_BUFFLEN )
        {
            result = MSGE_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status clbck */
            ctx->cbCrcPtr = cbCrcP;
            ctx->cbCrcCtx = clbCtx;

			/* initialize internal bytestuffer */
			resultByStuff =  eCU_BSTF_InitCtx(&ctx->byteStufferCtnx, memArea, memAreaSize);
			result = eFSP_MSGE_convertReturnFromBstf(resultByStuff);
        }
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3"
#endif

e_eFSP_MSGE_Res eFSP_MSGE_IsInit(s_eFSP_MSGE_Ctx* const ctx, bool_t* isInit)
{
	/* Local variable */
	e_eFSP_MSGE_Res result;
    e_eCU_BSTF_Res resultByStuff;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == isInit ) )
	{
		result = MSGE_RES_BADPOINTER;
	}
	else
	{
        resultByStuff = eCU_BSTF_IsInit(&ctx->byteStufferCtnx, isInit);
        result = eFSP_MSGE_convertReturnFromBstf(resultByStuff);
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MEM-stack-param", "MISRAC2004-17.6_d", "MISRAC2012-Rule-1.3_s", "MISRAC2012-Rule-18.6_d", \
                            "CERT-DCL30-C_e"
    /* Suppressed for code clarity */
#endif

e_eFSP_MSGE_Res eFSP_MSGE_GetWherePutData(s_eFSP_MSGE_Ctx* const ctx, uint8_t** dataP, uint32_t* const maxDataSize)
{
	/* Local variable */
	e_eFSP_MSGE_Res result;
	e_eCU_BSTF_Res resultByStuff;
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
		if( false == eFSP_MSGE_isStatusStillCoherent(ctx) )
		{
			result = MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            maxDataSizeP = 0u;
            dataPP = NULL;
			resultByStuff = eCU_BSTF_GetWherePutData(&ctx->byteStufferCtnx, &dataPP, &maxDataSizeP);
			result = eFSP_MSGE_convertReturnFromBstf(resultByStuff);

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

e_eFSP_MSGE_Res eFSP_MSGE_NewMessage(s_eFSP_MSGE_Ctx* const ctx, const uint32_t messageLen)
{
	/* Local variable */
	e_eFSP_MSGE_Res result;
	e_eCU_BSTF_Res resultByStuff;
	uint8_t* dataP;
	uint32_t maxDataSize;
    uint32_t cR32;
    bool_t crcRes;
    uint32_t nByteToSf;
	uint32_t nBToCrc;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGE_RES_BADPOINTER;
	}
	else
	{
        /* Check internal status validity */
        if( false == eFSP_MSGE_isStatusStillCoherent(ctx) )
        {
            result = MSGE_RES_CORRUPTCTX;
        }
		else
		{
            /* Check param validity, need at least 1 byte of paylaod */
            if( ( messageLen <= 0u ) || ( messageLen > ( MAX_UINT32VAL - EFSP_MSGEN_HEADERSIZE ) ) )
            {
                result = MSGE_RES_BADPARAM;
            }
            else
            {
                /* Data where already loaded in internal buffer */
                /* Get memory reference of CRC+LEN+DATA, so we can calculate data len and recalculate CRC */
                maxDataSize = 0u;
                dataP = NULL;
				resultByStuff = eCU_BSTF_GetWherePutData(&ctx->byteStufferCtnx, &dataP, &maxDataSize);
				result = eFSP_MSGE_convertReturnFromBstf(resultByStuff);

				if( MSGE_RES_OK == result )
				{
                    if( maxDataSize < EFSP_MIN_MSGEN_BUFFLEN )
                    {
                        result = MSGE_RES_CORRUPTCTX;
                    }
                    else
                    {
						if( ( messageLen + EFSP_MSGEN_HEADERSIZE ) > maxDataSize )
						{
							/* Data payload can not be greater that max payload size */
							result = MSGE_RES_BADPARAM;
						}
						else
						{
							/* we have now the memory reference  */
							/* Insert in the buffer the updated message size, in Little Endian */
							dataP[0x04u] = (uint8_t) ( ( messageLen        ) & 0x000000FFu );
							dataP[0x05u] = (uint8_t) ( ( messageLen >> 8u  ) & 0x000000FFu );
							dataP[0x06u] = (uint8_t) ( ( messageLen >> 16u ) & 0x000000FFu );
							dataP[0x07u] = (uint8_t) ( ( messageLen >> 24u ) & 0x000000FFu );

							/* Calculate the CRC of data payload and messageLen */
							nBToCrc = messageLen + 4u;
							crcRes = (*(ctx->cbCrcPtr))( ctx->cbCrcCtx, ECU_CRC_BASE_SEED, &dataP[4u], nBToCrc, &cR32 );

							if( true == crcRes )
							{
								/* Insert in the buffer the CRC32, in Little Endian */
								dataP[0x00u] = (uint8_t) ( ( cR32        ) & 0x000000FFu );
								dataP[0x01u] = (uint8_t) ( ( cR32 >> 8u  ) & 0x000000FFu );
								dataP[0x02u] = (uint8_t) ( ( cR32 >> 16u ) & 0x000000FFu );
								dataP[0x03u] = (uint8_t) ( ( cR32 >> 24u ) & 0x000000FFu );

								/* the message frame is ready, need to start the bytestuffer with size of crc + size
								* of len + real number of data */
								nByteToSf = ( EFSP_MSGEN_HEADERSIZE + messageLen );
								resultByStuff = eCU_BSTF_NewFrame(&ctx->byteStufferCtnx, nByteToSf);
								result = eFSP_MSGE_convertReturnFromBstf(resultByStuff);
							}
							else
							{
								result = MSGE_RES_CRCCLBKERROR;
							}
						}
                    }
				}
            }
		}
	}

	return result;
}

e_eFSP_MSGE_Res eFSP_MSGE_RestartMessage(s_eFSP_MSGE_Ctx* const ctx)
{
	/* Local variable */
	e_eFSP_MSGE_Res result;
	e_eCU_BSTF_Res resultByStuff;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_isStatusStillCoherent(ctx) )
		{
			result = MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Restart only the byte stuffer */
			resultByStuff = eCU_BSTF_RestartFrame(&ctx->byteStufferCtnx);
			result = eFSP_MSGE_convertReturnFromBstf(resultByStuff);
		}
	}

	return result;
}

e_eFSP_MSGE_Res eFSP_MSGE_GetRemByteToGet(s_eFSP_MSGE_Ctx* const ctx, uint32_t* const retrivedLen)
{
	/* Local variable */
	e_eFSP_MSGE_Res result;
	e_eCU_BSTF_Res resultByStuff;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == retrivedLen ) )
	{
		result = MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_isStatusStillCoherent(ctx) )
		{
			result = MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			resultByStuff = eCU_BSTF_GetRemByteToGet(&ctx->byteStufferCtnx, retrivedLen);
			result = eFSP_MSGE_convertReturnFromBstf(resultByStuff);
		}
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed because this warning is not working */
#endif

e_eFSP_MSGE_Res eFSP_MSGE_GetEncChunk(s_eFSP_MSGE_Ctx* const ctx, uint8_t encodeDest[], const uint32_t maxDestLen,
                                   uint32_t* const filledLen)
{
	/* Local variable */
	e_eFSP_MSGE_Res result;
	e_eCU_BSTF_Res resultByStuff;

	/* Check pointer validity */
	if( ( NULL == ctx )  || ( NULL == encodeDest ) || ( NULL == filledLen ) )
	{
		result = MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_isStatusStillCoherent(ctx) )
		{
			result = MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			resultByStuff = eCU_BSTF_GetStufChunk(&ctx->byteStufferCtnx, encodeDest, maxDestLen, filledLen);
			result = eFSP_MSGE_convertReturnFromBstf(resultByStuff);
		}
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3"
#endif



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSP_MSGE_isStatusStillCoherent(const s_eFSP_MSGE_Ctx* ctx)
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

static e_eFSP_MSGE_Res eFSP_MSGE_convertReturnFromBstf(e_eCU_BSTF_Res returnedEvent)
{
	e_eFSP_MSGE_Res result;

	switch( returnedEvent )
	{
		case BSTF_RES_OK:
		{
			result = MSGE_RES_OK;
            break;
		}

		case BSTF_RES_BADPARAM:
		{
			result = MSGE_RES_BADPARAM;
            break;
		}

		case BSTF_RES_BADPOINTER:
		{
			result = MSGE_RES_BADPOINTER;
            break;
		}

		case BSTF_RES_CORRUPTCTX:
		{
			result = MSGE_RES_CORRUPTCTX;
            break;
		}

		case BSTF_RES_FRAMEENDED:
		{
			result = MSGE_RES_MESSAGEENDED;
            break;
		}

		case BSTF_RES_NOINITLIB:
		{
			result = MSGE_RES_NOINITLIB;
            break;
		}

		case BSTF_RES_NOINITFRAME :
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