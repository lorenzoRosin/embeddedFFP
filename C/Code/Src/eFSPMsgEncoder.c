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
static bool_t eFSP_MSGE_isStatusStillCoherent(const s_eFSP_MSGE_Ctx* p_ctx);
static e_eFSP_MSGE_Res eFSP_MSGE_convertReturnFromBstf(e_eCU_BSTF_Res returnedEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed because this warning is not working */
#endif

e_eFSP_MSGE_Res eFSP_MSGE_InitCtx(s_eFSP_MSGE_Ctx* const p_ctx, uint8_t a_memArea[], const uint32_t memAreaSize,
								  cb_crc32_msge f_Crc, void* const p_clbCtx)
{
	/* Local variable */
	e_eFSP_MSGE_Res l_result;
	e_eCU_BSTF_Res l_resultByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == a_memArea ) || ( NULL == f_Crc ) || ( NULL == p_clbCtx ) )
	{
		l_result = MSGE_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity, we need some len to store the data */
        if( memAreaSize < EFSP_MIN_MSGEN_BUFFLEN )
        {
            l_result = MSGE_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status clbck */
            p_ctx->f_Crc = f_Crc;
            p_ctx->p_crcCtx = p_clbCtx;

			/* initialize internal bytestuffer */
			l_resultByStuff =  eCU_BSTF_InitCtx(&p_ctx->bstf_Ctx, a_memArea, memAreaSize);
			l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);
        }
	}

	return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3"
#endif

e_eFSP_MSGE_Res eFSP_MSGE_IsInit(s_eFSP_MSGE_Ctx* const p_ctx, bool_t* p_isInit)
{
	/* Local variable */
	e_eFSP_MSGE_Res l_result;
    e_eCU_BSTF_Res l_resultByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_isInit ) )
	{
		l_result = MSGE_RES_BADPOINTER;
	}
	else
	{
        l_resultByStuff = eCU_BSTF_IsInit(&p_ctx->bstf_Ctx, p_isInit);
        l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);
	}

	return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MEM-stack-param", "MISRAC2004-17.6_d", "MISRAC2012-Rule-1.3_s", "MISRAC2012-Rule-18.6_d", \
                            "CERT-DCL30-C_e"
    /* Suppressed for code clarity */
#endif

e_eFSP_MSGE_Res eFSP_MSGE_GetWherePutData(s_eFSP_MSGE_Ctx* const p_ctx, uint8_t** pp_data, uint32_t* const p_maxDataL)
{
	/* Local variable */
	e_eFSP_MSGE_Res l_result;
	e_eCU_BSTF_Res l_resultByStuff;
	uint8_t* lp_data;
	uint32_t l_maxDataSizeP;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == pp_data ) || ( NULL == p_maxDataL ) )
	{
		l_result = MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            l_maxDataSizeP = 0u;
            lp_data = NULL;
			l_resultByStuff = eCU_BSTF_GetWherePutData(&p_ctx->bstf_Ctx, &lp_data, &l_maxDataSizeP);
			l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);

			if( MSGE_RES_OK == l_result )
			{
                if( l_maxDataSizeP < EFSP_MIN_MSGEN_BUFFLEN )
                {
                    l_result = MSGE_RES_CORRUPTCTX;
                }
                else
                {
                    /* Return reference of only the raw payload */
                    *pp_data = &lp_data[EFSP_MSGEN_HEADERSIZE];
                    *p_maxDataL = l_maxDataSizeP - EFSP_MSGEN_HEADERSIZE;
                }
			}
		}
	}

	return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MEM-stack-param", "MISRAC2004-17.6_d", "MISRAC2012-Rule-1.3_s", "MISRAC2012-Rule-18.6_d", \
                            "CERT-DCL30-C_e"
#endif

e_eFSP_MSGE_Res eFSP_MSGE_NewMessage(s_eFSP_MSGE_Ctx* const p_ctx, const uint32_t messageLen)
{
	/* Local variable */
	e_eFSP_MSGE_Res l_result;
	e_eCU_BSTF_Res l_resultByStuff;
	uint8_t* lp_data;
	uint32_t l_maxDataSize;
    uint32_t l_cR32;
    bool_t l_crcRes;
    uint32_t l_nByteToSf;
	uint32_t l_nBToCrc;

	/* Check pointer validity */
	if( NULL == p_ctx )
	{
		l_result = MSGE_RES_BADPOINTER;
	}
	else
	{
        /* Check internal status validity */
        if( false == eFSP_MSGE_isStatusStillCoherent(p_ctx) )
        {
            l_result = MSGE_RES_CORRUPTCTX;
        }
		else
		{
            /* Check param validity, need at least 1 byte of paylaod */
            if( ( messageLen <= 0u ) || ( messageLen > ( MAX_UINT32VAL - EFSP_MSGEN_HEADERSIZE ) ) )
            {
                l_result = MSGE_RES_BADPARAM;
            }
            else
            {
                /* Data where already loaded in internal buffer */
                /* Get memory reference of CRC+LEN+DATA, so we can calculate data len and recalculate CRC */
                l_maxDataSize = 0u;
                lp_data = NULL;
				l_resultByStuff = eCU_BSTF_GetWherePutData(&p_ctx->bstf_Ctx, &lp_data, &l_maxDataSize);
				l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);

				if( MSGE_RES_OK == l_result )
				{
                    if( l_maxDataSize < EFSP_MIN_MSGEN_BUFFLEN )
                    {
                        l_result = MSGE_RES_CORRUPTCTX;
                    }
                    else
                    {
						if( ( messageLen + EFSP_MSGEN_HEADERSIZE ) > l_maxDataSize )
						{
							/* Data payload can not be greater that max payload size */
							l_result = MSGE_RES_BADPARAM;
						}
						else
						{
							/* we have now the memory reference  */
							/* Insert in the buffer the updated message size, in Little Endian */
							lp_data[0x04u] = (uint8_t) ( ( messageLen        ) & 0x000000FFu );
							lp_data[0x05u] = (uint8_t) ( ( messageLen >> 8u  ) & 0x000000FFu );
							lp_data[0x06u] = (uint8_t) ( ( messageLen >> 16u ) & 0x000000FFu );
							lp_data[0x07u] = (uint8_t) ( ( messageLen >> 24u ) & 0x000000FFu );

							/* Calculate the CRC of data payload and messageLen */
							l_nBToCrc = messageLen + 4u;
							l_crcRes = (*(p_ctx->f_Crc))( p_ctx->p_crcCtx, ECU_CRC_BASE_SEED, &lp_data[4u], l_nBToCrc, &l_cR32 );

							if( true == l_crcRes )
							{
								/* Insert in the buffer the CRC32, in Little Endian */
								lp_data[0x00u] = (uint8_t) ( ( l_cR32        ) & 0x000000FFu );
								lp_data[0x01u] = (uint8_t) ( ( l_cR32 >> 8u  ) & 0x000000FFu );
								lp_data[0x02u] = (uint8_t) ( ( l_cR32 >> 16u ) & 0x000000FFu );
								lp_data[0x03u] = (uint8_t) ( ( l_cR32 >> 24u ) & 0x000000FFu );

								/* the message frame is ready, need to start the bytestuffer with size of crc + size
								* of len + real number of data */
								l_nByteToSf = ( EFSP_MSGEN_HEADERSIZE + messageLen );
								l_resultByStuff = eCU_BSTF_NewFrame(&p_ctx->bstf_Ctx, l_nByteToSf);
								l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);
							}
							else
							{
								l_result = MSGE_RES_CRCCLBKERROR;
							}
						}
                    }
				}
            }
		}
	}

	return l_result;
}

e_eFSP_MSGE_Res eFSP_MSGE_RestartMessage(s_eFSP_MSGE_Ctx* const p_ctx)
{
	/* Local variable */
	e_eFSP_MSGE_Res l_result;
	e_eCU_BSTF_Res l_resultByStuff;

	/* Check pointer validity */
	if( NULL == p_ctx )
	{
		l_result = MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Restart only the byte stuffer */
			l_resultByStuff = eCU_BSTF_RestartFrame(&p_ctx->bstf_Ctx);
			l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);
		}
	}

	return l_result;
}

e_eFSP_MSGE_Res eFSP_MSGE_GetRemByteToGet(s_eFSP_MSGE_Ctx* const p_ctx, uint32_t* const p_retrivedLen)
{
	/* Local variable */
	e_eFSP_MSGE_Res l_result;
	e_eCU_BSTF_Res l_resultByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_retrivedLen ) )
	{
		l_result = MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			l_resultByStuff = eCU_BSTF_GetRemByteToGet(&p_ctx->bstf_Ctx, p_retrivedLen);
			l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);
		}
	}

	return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed because this warning is not working */
#endif

e_eFSP_MSGE_Res eFSP_MSGE_GetEncChunk(s_eFSP_MSGE_Ctx* const p_ctx, uint8_t p_encodeDest[], const uint32_t maxDestLen,
                                      uint32_t* const p_filledLen)
{
	/* Local variable */
	e_eFSP_MSGE_Res l_result;
	e_eCU_BSTF_Res l_resultByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ctx )  || ( NULL == p_encodeDest ) || ( NULL == p_filledLen ) )
	{
		l_result = MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			l_resultByStuff = eCU_BSTF_GetStufChunk(&p_ctx->bstf_Ctx, p_encodeDest, maxDestLen, p_filledLen);
			l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);
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
static bool_t eFSP_MSGE_isStatusStillCoherent(const s_eFSP_MSGE_Ctx* p_ctx)
{
    bool_t l_result;

	/* Check context validity */
	if( ( NULL == p_ctx->f_Crc ) || ( NULL == p_ctx->p_crcCtx ) )
	{
		l_result = false;
	}
	else
	{
		l_result = true;
	}

    return l_result;
}

static e_eFSP_MSGE_Res eFSP_MSGE_convertReturnFromBstf(e_eCU_BSTF_Res returnedEvent)
{
	e_eFSP_MSGE_Res l_result;

	switch( returnedEvent )
	{
		case BSTF_RES_OK:
		{
			l_result = MSGE_RES_OK;
            break;
		}

		case BSTF_RES_BADPARAM:
		{
			l_result = MSGE_RES_BADPARAM;
            break;
		}

		case BSTF_RES_BADPOINTER:
		{
			l_result = MSGE_RES_BADPOINTER;
            break;
		}

		case BSTF_RES_CORRUPTCTX:
		{
			l_result = MSGE_RES_CORRUPTCTX;
            break;
		}

		case BSTF_RES_FRAMEENDED:
		{
			l_result = MSGE_RES_MESSAGEENDED;
            break;
		}

		case BSTF_RES_NOINITLIB:
		{
			l_result = MSGE_RES_NOINITLIB;
            break;
		}

		case BSTF_RES_NOINITFRAME :
		{
			l_result = MSGE_RES_NOINITMESSAGE;
            break;
		}

		default:
		{
            /* Impossible end here */
			l_result = MSGE_RES_CORRUPTCTX;
            break;
		}
	}

	return l_result;
}