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
static bool_t eFSP_MSGE_IsStatusStillCoherent(const t_eFSP_MSGE_Ctx* p_ptCtx);
static e_eFSP_MSGE_RES eFSP_MSGE_ConvertRetFromBstf(const e_eCU_BSTF_RES p_eRetEvent);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_MSGE_RES eFSP_MSGE_InitCtx(t_eFSP_MSGE_Ctx* const p_ptCtx, uint8_t* p_puBuff, const uint32_t p_uBuffL,
								  f_eFSP_MSGE_CrcCb p_fCrc, t_eFSP_MSGE_CrcCtx* const p_ptFctx)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_eRes;
	e_eCU_BSTF_RES l_eResBTSTUFF;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) || ( NULL == p_fCrc ) || ( NULL == p_ptFctx ) )
	{
		l_eRes = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity, we need some len to store the data */
        if( p_uBuffL < EFSP_MIN_MSGEN_BUFFLEN )
        {
            l_eRes = e_eFSP_MSGE_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status clbck */
            p_ptCtx->fCrc = p_fCrc;
            p_ptCtx->ptCrcCtx = p_ptFctx;

			/* initialize internal bytestuffer */
			l_eResBTSTUFF =  eCU_BSTF_InitCtx(&p_ptCtx->tBSTFCtx, p_puBuff, p_uBuffL);
			l_eRes = eFSP_MSGE_ConvertRetFromBstf(l_eResBTSTUFF);
        }
	}

	return l_eRes;
}

e_eFSP_MSGE_RES eFSP_MSGE_IsInit(t_eFSP_MSGE_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_eRes;
    e_eCU_BSTF_RES l_eResBTSTUFF;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
        l_eResBTSTUFF = eCU_BSTF_IsInit(&p_ptCtx->tBSTFCtx, p_pbIsInit);
        l_eRes = eFSP_MSGE_ConvertRetFromBstf(l_eResBTSTUFF);
	}

	return l_eRes;
}

e_eFSP_MSGE_RES eFSP_MSGE_GetWherePutData(t_eFSP_MSGE_Ctx* const p_ptCtx, uint8_t** p_ppuDat, uint32_t* const p_puMaxL)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_eRes;
	e_eCU_BSTF_RES l_eResBTSTUFF;
	uint8_t* l_puData;
    uint8_t** l_ppuData;
	uint32_t l_uMaxDataL;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuDat ) || ( NULL == p_puMaxL ) )
	{
		l_eRes = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_IsStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            l_uMaxDataL = 0u;
            l_puData = NULL;
            l_ppuData = &l_puData;
			l_eResBTSTUFF = eCU_BSTF_GetWherePutData(&p_ptCtx->tBSTFCtx, l_ppuData, &l_uMaxDataL);
			l_eRes = eFSP_MSGE_ConvertRetFromBstf(l_eResBTSTUFF);

			if( e_eFSP_MSGE_RES_OK == l_eRes )
			{
                if( l_uMaxDataL < EFSP_MIN_MSGEN_BUFFLEN )
                {
                    l_eRes = e_eFSP_MSGE_RES_CORRUPTCTX;
                }
                else
                {
                    /* Return reference of only the raw payload */
                    *p_ppuDat = &(*l_ppuData)[EFSP_MSGEN_HEADERSIZE];
                    *p_puMaxL = l_uMaxDataL - EFSP_MSGEN_HEADERSIZE;
                }
			}
		}
	}

	return l_eRes;
}

e_eFSP_MSGE_RES eFSP_MSGE_NewMessage(t_eFSP_MSGE_Ctx* const p_ptCtx, const uint32_t p_uMsgLen)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_eRes;
	e_eCU_BSTF_RES l_eResBTSTUFF;
	uint8_t* l_puData;
	uint32_t l_uMaxDataL;
    uint32_t l_uC32;
    bool_t l_bResC;
    uint32_t l_uNBToSf;
	uint32_t l_uNBToC;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
        /* Check internal status validity */
        if( false == eFSP_MSGE_IsStatusStillCoherent(p_ptCtx) )
        {
            l_eRes = e_eFSP_MSGE_RES_CORRUPTCTX;
        }
		else
		{
            /* Check param validity, need at least 1 byte of paylaod */
            if( ( p_uMsgLen <= 0u ) || ( p_uMsgLen > ( MAX_UINT32VAL - EFSP_MSGEN_HEADERSIZE ) ) )
            {
                l_eRes = e_eFSP_MSGE_RES_BADPARAM;
            }
            else
            {
                /* Data where already loaded in internal buffer */
                /* Get memory reference of CRC+LEN+DATA, so we can calculate data len and recalculate CRC */
                l_uMaxDataL = 0u;
                l_puData = NULL;
				l_eResBTSTUFF = eCU_BSTF_GetWherePutData(&p_ptCtx->tBSTFCtx, &l_puData, &l_uMaxDataL);
				l_eRes = eFSP_MSGE_ConvertRetFromBstf(l_eResBTSTUFF);

				if( e_eFSP_MSGE_RES_OK == l_eRes )
				{
                    if( l_uMaxDataL < EFSP_MIN_MSGEN_BUFFLEN )
                    {
                        l_eRes = e_eFSP_MSGE_RES_CORRUPTCTX;
                    }
                    else
                    {
						if( ( p_uMsgLen + EFSP_MSGEN_HEADERSIZE ) > l_uMaxDataL )
						{
							/* Data payload can not be greater that max payload size */
							l_eRes = e_eFSP_MSGE_RES_BADPARAM;
						}
						else
						{
							/* we have now the memory reference  */
							/* Insert in the buffer the updated message size, in Little Endian */
							l_puData[0x04u] = (uint8_t) ( ( p_uMsgLen        ) & 0x000000FFu );
							l_puData[0x05u] = (uint8_t) ( ( p_uMsgLen >> 8u  ) & 0x000000FFu );
							l_puData[0x06u] = (uint8_t) ( ( p_uMsgLen >> 16u ) & 0x000000FFu );
							l_puData[0x07u] = (uint8_t) ( ( p_uMsgLen >> 24u ) & 0x000000FFu );

							/* Calculate the CRC of data payload and p_uMsgLen */
							l_uNBToC = p_uMsgLen + 4u;
							l_bResC = (*(p_ptCtx->fCrc))( p_ptCtx->ptCrcCtx, eCU_CRC_BASE_SEED, &l_puData[4u], l_uNBToC,
                                                          &l_uC32 );

							if( true == l_bResC )
							{
								/* Insert in the buffer the CRC32, in Little Endian */
								l_puData[0x00u] = (uint8_t) ( ( l_uC32        ) & 0x000000FFu );
								l_puData[0x01u] = (uint8_t) ( ( l_uC32 >> 8u  ) & 0x000000FFu );
								l_puData[0x02u] = (uint8_t) ( ( l_uC32 >> 16u ) & 0x000000FFu );
								l_puData[0x03u] = (uint8_t) ( ( l_uC32 >> 24u ) & 0x000000FFu );

								/* the message frame is ready, need to start the bytestuffer with size of crc + size
								* of len + real number of data */
								l_uNBToSf = ( EFSP_MSGEN_HEADERSIZE + p_uMsgLen );
								l_eResBTSTUFF = eCU_BSTF_NewFrame(&p_ptCtx->tBSTFCtx, l_uNBToSf);
								l_eRes = eFSP_MSGE_ConvertRetFromBstf(l_eResBTSTUFF);
							}
							else
							{
								l_eRes = e_eFSP_MSGE_RES_CRCCLBKERROR;
							}
						}
                    }
				}
            }
		}
	}

	return l_eRes;
}

e_eFSP_MSGE_RES eFSP_MSGE_RestartMessage(t_eFSP_MSGE_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_eRes;
	e_eCU_BSTF_RES l_eResBTSTUFF;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_IsStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Restart only the byte stuffer */
			l_eResBTSTUFF = eCU_BSTF_RestartFrame(&p_ptCtx->tBSTFCtx);
			l_eRes = eFSP_MSGE_ConvertRetFromBstf(l_eResBTSTUFF);
		}
	}

	return l_eRes;
}

e_eFSP_MSGE_RES eFSP_MSGE_GetRemByteToGet(t_eFSP_MSGE_Ctx* const p_ptCtx, uint32_t* const p_puRetrivedLen)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_eRes;
	e_eCU_BSTF_RES l_eResBTSTUFF;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puRetrivedLen ) )
	{
		l_eRes = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_IsStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			l_eResBTSTUFF = eCU_BSTF_GetRemByteToGet(&p_ptCtx->tBSTFCtx, p_puRetrivedLen);
			l_eRes = eFSP_MSGE_ConvertRetFromBstf(l_eResBTSTUFF);
		}
	}

	return l_eRes;
}

e_eFSP_MSGE_RES eFSP_MSGE_GetEncChunk(t_eFSP_MSGE_Ctx* const p_ptCtx, uint8_t* p_puEncBuff, const uint32_t p_uMaxBufL,
                                      uint32_t* const p_puGettedL)
{
	/* Local variable */
	e_eFSP_MSGE_RES l_eRes;
	e_eCU_BSTF_RES l_eResBTSTUFF;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx )  || ( NULL == p_puEncBuff ) || ( NULL == p_puGettedL ) )
	{
		l_eRes = e_eFSP_MSGE_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGE_IsStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGE_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference */
			l_eResBTSTUFF = eCU_BSTF_GetStufChunk(&p_ptCtx->tBSTFCtx, p_puEncBuff, p_uMaxBufL, p_puGettedL);
			l_eRes = eFSP_MSGE_ConvertRetFromBstf(l_eResBTSTUFF);
		}
	}

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSP_MSGE_IsStatusStillCoherent(const t_eFSP_MSGE_Ctx* p_ptCtx)
{
    bool_t l_eRes;

	/* Check context validity */
	if( ( NULL == p_ptCtx->fCrc ) || ( NULL == p_ptCtx->ptCrcCtx ) )
	{
		l_eRes = false;
	}
	else
	{
		l_eRes = true;
	}

    return l_eRes;
}

static e_eFSP_MSGE_RES eFSP_MSGE_ConvertRetFromBstf(const e_eCU_BSTF_RES p_eRetEvent)
{
	e_eFSP_MSGE_RES l_eRes;

	switch( p_eRetEvent )
	{
		case e_eCU_BSTF_RES_OK:
		{
			l_eRes = e_eFSP_MSGE_RES_OK;
            break;
		}

		case e_eCU_BSTF_RES_BADPARAM:
		{
			l_eRes = e_eFSP_MSGE_RES_BADPARAM;
            break;
		}

		case e_eCU_BSTF_RES_BADPOINTER:
		{
			l_eRes = e_eFSP_MSGE_RES_BADPOINTER;
            break;
		}

		case e_eCU_BSTF_RES_CORRUPTCTX:
		{
			l_eRes = e_eFSP_MSGE_RES_CORRUPTCTX;
            break;
		}

		case e_eCU_BSTF_RES_FRAMEENDED:
		{
			l_eRes = e_eFSP_MSGE_RES_MESSAGEENDED;
            break;
		}

		case e_eCU_BSTF_RES_NOINITLIB:
		{
			l_eRes = e_eFSP_MSGE_RES_NOINITLIB;
            break;
		}

		case e_eCU_BSTF_RES_NOINITFRAME :
		{
			l_eRes = e_eFSP_MSGE_RES_NOINITMESSAGE;
            break;
		}

		default:
		{
            /* Impossible end here */
			l_eRes = e_eFSP_MSGE_RES_CORRUPTCTX;
            break;
		}
	}

	return l_eRes;
}