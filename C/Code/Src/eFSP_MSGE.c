/**
 * @file       eFSP_MSGD.c
 *
 * @brief      Message encoder utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_MSGE.h"
#include "eCU_CRC.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSP_MSGE_isStatusStillCoherent(const t_eFSP_MSGE_Ctx* p_ptCtx);
static e_eFSP_MSGE_RES eFSP_MSGE_convertReturnFromBstf(e_eCU_BSTF_RES returnedEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_MSGE_RES eFSP_MSGE_InitCtx(t_eFSP_MSGE_Ctx* const p_ptCtx, uint8_t* p_memArea, const uint32_t memAreaSize,
								  f_eFSP_MSGE_CrcCb f_Crc, void* const p_clbCtx)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_result;
	e_eCU_BSTF_RES l_resultByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_memArea ) || ( NULL == f_Crc ) || ( NULL == p_clbCtx ) )
	{
		l_result = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity, we need some len to store the data */
        if( memAreaSize < EFSP_MIN_MSGEN_BUFFLEN )
        {
            l_result = e_eFSP_MSGE_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status clbck */
            p_ptCtx->fCrc = f_Crc;
            p_ptCtx->ptCrcCtx = p_clbCtx;

			/* initialize internal bytestuffer */
			l_resultByStuff =  eCU_BSTF_InitCtx(&p_ptCtx->tBSTFCtx, p_memArea, memAreaSize);
			l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);
        }
	}

	return l_result;
}

e_eFSP_MSGE_RES eFSP_MSGE_IsInit(t_eFSP_MSGE_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_result;
    e_eCU_BSTF_RES l_resultByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_result = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
        l_resultByStuff = eCU_BSTF_IsInit(&p_ptCtx->tBSTFCtx, p_pbIsInit);
        l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);
	}

	return l_result;
}

e_eFSP_MSGE_RES eFSP_MSGE_GetWherePutData(t_eFSP_MSGE_Ctx* const p_ptCtx, uint8_t** pp_data, uint32_t* const p_maxDataL)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_result;
	e_eCU_BSTF_RES l_resultByStuff;
	uint8_t* lp_data;
    uint8_t** lpp_dataP;
	uint32_t l_maxDataSizeP;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == pp_data ) || ( NULL == p_maxDataL ) )
	{
		l_result = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_isStatusStillCoherent(p_ptCtx) )
		{
			l_result = e_eFSP_MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            l_maxDataSizeP = 0u;
            lp_data = NULL;
            lpp_dataP = &lp_data;
			l_resultByStuff = eCU_BSTF_GetWherePutData(&p_ptCtx->tBSTFCtx, lpp_dataP, &l_maxDataSizeP);
			l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);

			if( e_eFSP_MSGE_RES_OK == l_result )
			{
                if( l_maxDataSizeP < EFSP_MIN_MSGEN_BUFFLEN )
                {
                    l_result = e_eFSP_MSGE_RES_CORRUPTCTX;
                }
                else
                {
                    /* Return reference of only the raw payload */
                    *pp_data = &(*lpp_dataP)[EFSP_MSGEN_HEADERSIZE];
                    *p_maxDataL = l_maxDataSizeP - EFSP_MSGEN_HEADERSIZE;
                }
			}
		}
	}

	return l_result;
}

e_eFSP_MSGE_RES eFSP_MSGE_NewMessage(t_eFSP_MSGE_Ctx* const p_ptCtx, const uint32_t messageLen)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_result;
	e_eCU_BSTF_RES l_resultByStuff;
	uint8_t* lp_data;
	uint32_t l_maxDataSize;
    uint32_t l_cR32;
    bool_t l_crcRes;
    uint32_t l_nByteToSf;
	uint32_t l_nBToCrc;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_result = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
        /* Check internal status validity */
        if( false == eFSP_MSGE_isStatusStillCoherent(p_ptCtx) )
        {
            l_result = e_eFSP_MSGE_RES_CORRUPTCTX;
        }
		else
		{
            /* Check param validity, need at least 1 byte of paylaod */
            if( ( messageLen <= 0u ) || ( messageLen > ( MAX_UINT32VAL - EFSP_MSGEN_HEADERSIZE ) ) )
            {
                l_result = e_eFSP_MSGE_RES_BADPARAM;
            }
            else
            {
                /* Data where already loaded in internal buffer */
                /* Get memory reference of CRC+LEN+DATA, so we can calculate data len and recalculate CRC */
                l_maxDataSize = 0u;
                lp_data = NULL;
				l_resultByStuff = eCU_BSTF_GetWherePutData(&p_ptCtx->tBSTFCtx, &lp_data, &l_maxDataSize);
				l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);

				if( e_eFSP_MSGE_RES_OK == l_result )
				{
                    if( l_maxDataSize < EFSP_MIN_MSGEN_BUFFLEN )
                    {
                        l_result = e_eFSP_MSGE_RES_CORRUPTCTX;
                    }
                    else
                    {
						if( ( messageLen + EFSP_MSGEN_HEADERSIZE ) > l_maxDataSize )
						{
							/* Data payload can not be greater that max payload size */
							l_result = e_eFSP_MSGE_RES_BADPARAM;
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
							l_crcRes = (*(p_ptCtx->fCrc))( p_ptCtx->ptCrcCtx, eCU_CRC_BASE_SEED, &lp_data[4u], l_nBToCrc,
                                                          &l_cR32 );

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
								l_resultByStuff = eCU_BSTF_NewFrame(&p_ptCtx->tBSTFCtx, l_nByteToSf);
								l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);
							}
							else
							{
								l_result = e_eFSP_MSGE_RES_CRCCLBKERROR;
							}
						}
                    }
				}
            }
		}
	}

	return l_result;
}

e_eFSP_MSGE_RES eFSP_MSGE_RestartMessage(t_eFSP_MSGE_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_result;
	e_eCU_BSTF_RES l_resultByStuff;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_result = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_isStatusStillCoherent(p_ptCtx) )
		{
			l_result = e_eFSP_MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Restart only the byte stuffer */
			l_resultByStuff = eCU_BSTF_RestartFrame(&p_ptCtx->tBSTFCtx);
			l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);
		}
	}

	return l_result;
}

e_eFSP_MSGE_RES eFSP_MSGE_GetRemByteToGet(t_eFSP_MSGE_Ctx* const p_ptCtx, uint32_t* const p_retrivedLen)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_result;
	e_eCU_BSTF_RES l_resultByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_retrivedLen ) )
	{
		l_result = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_isStatusStillCoherent(p_ptCtx) )
		{
			l_result = e_eFSP_MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			l_resultByStuff = eCU_BSTF_GetRemByteToGet(&p_ptCtx->tBSTFCtx, p_retrivedLen);
			l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);
		}
	}

	return l_result;
}

e_eFSP_MSGE_RES eFSP_MSGE_GetEncChunk(t_eFSP_MSGE_Ctx* const p_ptCtx, uint8_t* p_encodeDest, const uint32_t maxDestLen,
                                      uint32_t* const p_filledLen)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_result;
	e_eCU_BSTF_RES l_resultByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx )  || ( NULL == p_encodeDest ) || ( NULL == p_filledLen ) )
	{
		l_result = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_isStatusStillCoherent(p_ptCtx) )
		{
			l_result = e_eFSP_MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			l_resultByStuff = eCU_BSTF_GetStufChunk(&p_ptCtx->tBSTFCtx, p_encodeDest, maxDestLen, p_filledLen);
			l_result = eFSP_MSGE_convertReturnFromBstf(l_resultByStuff);
		}
	}

	return l_result;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSP_MSGE_isStatusStillCoherent(const t_eFSP_MSGE_Ctx* p_ptCtx)
{
    bool_t l_result;

	/* Check context validity */
	if( ( NULL == p_ptCtx->fCrc ) || ( NULL == p_ptCtx->ptCrcCtx ) )
	{
		l_result = false;
	}
	else
	{
		l_result = true;
	}

    return l_result;
}

static e_eFSP_MSGE_RES eFSP_MSGE_convertReturnFromBstf(e_eCU_BSTF_RES returnedEvent)
{
	e_eFSP_MSGE_RES l_result;

	switch( returnedEvent )
	{
		case e_eCU_BSTF_RES_OK:
		{
			l_result = e_eFSP_MSGE_RES_OK;
            break;
		}

		case e_eCU_BSTF_RES_BADPARAM:
		{
			l_result = e_eFSP_MSGE_RES_BADPARAM;
            break;
		}

		case e_eCU_BSTF_RES_BADPOINTER:
		{
			l_result = e_eFSP_MSGE_RES_BADPOINTER;
            break;
		}

		case e_eCU_BSTF_RES_CORRUPTCTX:
		{
			l_result = e_eFSP_MSGE_RES_CORRUPTCTX;
            break;
		}

		case e_eCU_BSTF_RES_FRAMEENDED:
		{
			l_result = e_eFSP_MSGE_RES_MESSAGEENDED;
            break;
		}

		case e_eCU_BSTF_RES_NOINITLIB:
		{
			l_result = e_eFSP_MSGE_RES_NOINITLIB;
            break;
		}

		case e_eCU_BSTF_RES_NOINITFRAME :
		{
			l_result = e_eFSP_MSGE_RES_NOINITMESSAGE;
            break;
		}

		default:
		{
            /* Impossible end here */
			l_result = e_eFSP_MSGE_RES_CORRUPTCTX;
            break;
		}
	}

	return l_result;
}