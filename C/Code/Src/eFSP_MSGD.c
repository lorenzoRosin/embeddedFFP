/**
 * @file       eFSP_MSGD.c
 *
 * @brief      Message decoder utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_MSGD.h"
#include "eFSP_MSGDPRV.h"
#include "eCU_CRC.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSP_MSGD_isStatusStillCoherent(const t_eFSP_MSGD_Ctx* p_ptCtx);
static e_eFSP_MSGD_RES eFSP_MSGD_convertReturnFromBstf(e_eCU_BUNSTF_RES returnedEvent);
static e_eFSP_MSGD_RES eFSP_MSGD_IsFullMsgDec(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_isMsgDec);
static e_eFSP_MSGD_RES eFSP_MSGD_IsFrmBad(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_isFrameBad);
static e_eFSP_MSGD_RES eFSP_MSGD_isMsgCorr(t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* p_isCor, f_eFSP_MSGD_CrcCb f_Crc,
                                           void* p_crcCtx);
static e_eFSP_MSGD_RES eFSP_MSGD_isMsgCohe(t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* p_isCoherent);
static uint32_t eFSP_MSGD_composeU32LE(uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

e_eFSP_MSGD_RES eFSP_MSGD_InitCtx(t_eFSP_MSGD_Ctx* const p_ptCtx, uint8_t* p_memArea, const uint32_t memAreaSize,
								  f_eFSP_MSGD_CrcCb f_Crc, void* const p_clbCtx)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_result;
	e_eCU_BUNSTF_RES l_resByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_memArea ) || ( NULL == f_Crc ) || ( NULL == p_clbCtx ) )
	{
		l_result = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity, we need some len to store the data */
        if( memAreaSize < EFSP_MIN_MSGDE_BUFFLEN )
        {
            l_result = e_eFSP_MSGD_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status clbck */
            p_ptCtx->fCrc = f_Crc;
            p_ptCtx->ptCrcCtx = p_clbCtx;

			/* initialize internal bytestuffer */
			l_resByStuff =  eCU_BUNSTF_InitCtx(&p_ptCtx->tBUNSTFCtx, &p_memArea[0u], memAreaSize);
			l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);
        }
	}

	return l_result;
}

e_eFSP_MSGD_RES eFSP_MSGD_IsInit(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_result;
	e_eCU_BUNSTF_RES l_resByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_result = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
        l_resByStuff = eCU_BUNSTF_IsInit(&p_ptCtx->tBUNSTFCtx, p_pbIsInit);
        l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);
	}

	return l_result;
}

e_eFSP_MSGD_RES eFSP_MSGD_NewMsg(t_eFSP_MSGD_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_result;
	e_eCU_BUNSTF_RES l_resByStuff;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_result = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_result = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Update index */
			l_resByStuff = eCU_BUNSTF_NewFrame(&p_ptCtx->tBUNSTFCtx);
			l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);
		}
	}

	return l_result;
}

e_eFSP_MSGD_RES eFSP_MSGD_GetDecodedData(t_eFSP_MSGD_Ctx* const p_ptCtx, uint8_t** pp_data, uint32_t* const p_gettedLen)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_result;
	e_eCU_BUNSTF_RES l_resByStuff;
	uint8_t* lp_dataP;
    uint8_t** lpp_dataP;
	uint32_t l_dataSizeP;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == pp_data ) || ( NULL == p_gettedLen ) )
	{
		l_result = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_result = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            l_dataSizeP = 0u;
            lp_dataP = NULL;
            lpp_dataP = &lp_dataP;
			l_resByStuff = eCU_BUNSTF_GetUnstufData(&p_ptCtx->tBUNSTFCtx, lpp_dataP, &l_dataSizeP);
			l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

			if( e_eFSP_MSGD_RES_OK == l_result )
			{
                /* Starting point is always the same */
                *pp_data = &(*lpp_dataP)[EFSP_MSGDE_HEADERSIZE];

				/* Removed HADER from data ( CRC + DATA LEN ) */
                if( l_dataSizeP < EFSP_MIN_MSGDE_BUFFLEN )
                {
					/* No memory free */
                    *p_gettedLen = 0u;
                }
                else
                {
                    /* Return reference of only the raw payload */
                    *p_gettedLen = l_dataSizeP - EFSP_MSGDE_HEADERSIZE;
                }

				l_result = e_eFSP_MSGD_RES_OK;
			}
		}
	}

	return l_result;
}

e_eFSP_MSGD_RES eFSP_MSGD_GetDecodedLen(t_eFSP_MSGD_Ctx* const p_ptCtx, uint32_t* const p_retrivedLen)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_result;
	e_eCU_BUNSTF_RES l_resByStuff;
	uint32_t l_dataSizeP;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_retrivedLen ) )
	{
		l_result = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_result = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            l_dataSizeP = 0u;
			l_resByStuff = eCU_BUNSTF_GetUnstufLen(&p_ptCtx->tBUNSTFCtx, &l_dataSizeP);
			l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

			if( e_eFSP_MSGD_RES_OK == l_result )
			{
				/* Removed HADER from data ( CRC + DATA LEN ) */
                if( l_dataSizeP < EFSP_MIN_MSGDE_BUFFLEN )
                {
					/* No memory free */
                    *p_retrivedLen = 0u;
                }
                else
                {
                    /* Return reference of only the raw payload */
                    *p_retrivedLen = l_dataSizeP - EFSP_MSGDE_HEADERSIZE;
                }

				l_result = e_eFSP_MSGD_RES_OK;
			}
		}
	}

	return l_result;
}

e_eFSP_MSGD_RES eFSP_MSGD_IsWaitingSof(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_isWaitingSof)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_result;
	e_eCU_BUNSTF_RES l_resByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_isWaitingSof ) )
	{
		l_result = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_result = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
			l_resByStuff = eCU_BUNSTF_IsWaitingSof(&p_ptCtx->tBUNSTFCtx, p_isWaitingSof);
			l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);
		}
	}

	return l_result;
}

e_eFSP_MSGD_RES eFSP_MSGD_IsAFullMsgDecoded(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_isMsgDec)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_result;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_isMsgDec ) )
	{
		l_result = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_result = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
            l_result = eFSP_MSGD_IsFullMsgDec(p_ptCtx, p_isMsgDec);
		}
	}

	return l_result;
}

e_eFSP_MSGD_RES eFSP_MSGD_IsFrameBad(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_isFrameBad)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_result;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_isFrameBad ) )
	{
		l_result = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_result = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
            l_result = eFSP_MSGD_IsFrmBad(p_ptCtx, p_isFrameBad);
		}
	}

	return l_result;
}

e_eFSP_MSGD_RES eFSP_MSGD_GetMostEffDatLen(t_eFSP_MSGD_Ctx* const p_ptCtx, uint32_t* const p_mostEffPayload)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_result;
	e_eCU_BUNSTF_RES l_resByStuff;
    bool_t l_isFullUnstuffed;
    bool_t l_isWaitingSof;
    bool_t l_isFrameBad;
    uint32_t l_dataSizeRemaings;
	uint32_t l_dPayRxed;
    uint32_t l_dataSizeRaw;
	uint8_t* lp_data;
    uint32_t l_dPayToRx;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_mostEffPayload ) )
	{
		l_result = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_result = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
            /* Check if the frame is already ended */
            l_isFullUnstuffed = false;
            l_resByStuff = eCU_BUNSTF_IsAFullFrameUnstuff(&p_ptCtx->tBUNSTFCtx, &l_isFullUnstuffed);
            l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

            if( e_eFSP_MSGD_RES_OK == l_result )
            {
                if( true == l_isFullUnstuffed )
                {
                    /* Full unstuffed frame, could be wrong or rigth but no more data is needed */
                    *p_mostEffPayload = 0u;
                }
                else
                {
                    /* Check for error */
                    l_isFrameBad = true;
                    l_resByStuff = eCU_BUNSTF_IsFrameBad(&p_ptCtx->tBUNSTFCtx, &l_isFrameBad);
                    l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

                    if( e_eFSP_MSGD_RES_OK == l_result )
                    {
                        if( true == l_isFrameBad )
                        {
                            /* Bad frame only for unstuffer level, dont request more data */
                            *p_mostEffPayload = 0u;
                        }
                        else
                        {
                            /* How many byte do we have decoded? */
                            l_dataSizeRaw = 0u;
                            lp_data = NULL;
                            l_resByStuff = eCU_BUNSTF_GetUnstufData(&p_ptCtx->tBUNSTFCtx, &lp_data, &l_dataSizeRaw);
                            l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

                            if( e_eFSP_MSGD_RES_OK == l_result )
                            {
                                /* Do we have enough data?  */
                                if( 0u == l_dataSizeRaw )
                                {
                                    /* No data, are we still waiting SOF? */
                                    l_isWaitingSof = false;
                                    l_resByStuff = eCU_BUNSTF_IsWaitingSof(&p_ptCtx->tBUNSTFCtx, &l_isWaitingSof);
                                    l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

                                    if( e_eFSP_MSGD_RES_OK == l_result )
                                    {
                                        if( true == l_isWaitingSof )
                                        {
                                            /* Header + SOF */
                                            *p_mostEffPayload = EFSP_MSGDE_HEADERSIZE + 1u;
                                        }
                                        else
                                        {
                                            /* Header only */
                                            *p_mostEffPayload = EFSP_MSGDE_HEADERSIZE;
                                        }
                                    }
                                }
                                else if( l_dataSizeRaw < EFSP_MSGDE_HEADERSIZE )
                                {
                                    /* Need to receive all the header before estimating data size */
                                    *p_mostEffPayload = EFSP_MSGDE_HEADERSIZE - l_dataSizeRaw;
                                }
                                else
                                {
                                    /* Enough data! Start remaining data estimation */
                                    l_dPayToRx = eFSP_MSGD_composeU32LE(lp_data[0x04u], lp_data[0x05u],
                                                                        lp_data[0x06u], lp_data[0x07u]);

                                    /* How much payload do we have */
                                    l_dPayRxed = l_dataSizeRaw - EFSP_MSGDE_HEADERSIZE;

                                    /* A correct frame payload must have less lenght than the size reported in frame
                                     * header */
                                    if( l_dPayRxed <= l_dPayToRx )
                                    {
                                        l_dataSizeRemaings = l_dPayToRx - l_dPayRxed;

                                        /* Wait remaining data + EOF */
                                        if( l_dataSizeRemaings < MAX_UINT32VAL )
                                        {
                                            /* l_dataSizeRemaings != 0 -> need data + EOF
                                             * l_dataSizeRemaings == 0 -> need EOF . Infact we are sure the frame is not
                                             * ended because we already have called the function
                                             * BUNSTF_IsAFullFrameUnstuff */
                                            *p_mostEffPayload = l_dataSizeRemaings + 1u;
                                        }
                                        else
                                        {
                                            *p_mostEffPayload = l_dataSizeRemaings;
                                        }
                                    }
                                    else
                                    {
                                        /* We have more data than expected -> bad frame -> no need to receive data */
                                        *p_mostEffPayload = 0u;
                                    }
                                }
                            }
                        }
                    }
                }
            }
		}
	}

	return l_result;
}

e_eFSP_MSGD_RES eFSP_MSGD_InsEncChunk(t_eFSP_MSGD_Ctx* const p_ptCtx, uint8_t* p_encArea, const uint32_t encLen,
                                      uint32_t* const p_usedEncByt)
{
	/* Local return  */
	e_eFSP_MSGD_RES l_result;
    e_eFSP_MSGD_RES l_resMsgCor;
    e_eFSP_MSGD_RES l_resultMsgCoherent;
	e_eCU_BUNSTF_RES l_resBStuf;

    /* Local coherence */
    bool_t l_isMCor;
    bool_t l_isMCoherent;

    /* Redo loop var */
    e_eFSP_MSGDPRV_SM l_insertEncState;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_encArea ) || ( NULL == p_usedEncByt ) )
	{
		l_result = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_result = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
            /* Init data */
            *p_usedEncByt = 0u;

            /* Elaborate */
            l_insertEncState = e_eFSP_MSGDPRV_SM_PRECHECK;
            l_result = e_eFSP_MSGD_RES_OK;

            while( l_insertEncState != e_eFSP_MSGDPRV_SM_ELABDONE )
            {
                switch( l_insertEncState )
                {
                    case e_eFSP_MSGDPRV_SM_PRECHECK:
                    {
                        /* Before inserting data check if the current message is coherent */
                        l_resultMsgCoherent = eFSP_MSGD_isMsgCohe(&p_ptCtx->tBUNSTFCtx, &l_isMCoherent);

                        if( e_eFSP_MSGD_RES_OK == l_resultMsgCoherent )
                        {
                            /* no strange error found, check message correctness */
                            if( true != l_isMCoherent )
                            {
                                /* Message is not coherent */
                                l_insertEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                                l_result = e_eFSP_MSGD_RES_BADFRAME;
                            }
                            else
                            {
                                /* Message is correct but not ended */
                                l_insertEncState = e_eFSP_MSGDPRV_SM_INSERTCHUNK;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_result = l_resultMsgCoherent;
                            l_insertEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        }

                        break;
                    }

                    case e_eFSP_MSGDPRV_SM_INSERTCHUNK:
                    {
                        /* Insert data */
                        l_resBStuf = eCU_BUNSTF_InsStufChunk(&p_ptCtx->tBUNSTFCtx, p_encArea, encLen, p_usedEncByt);
                        l_result = eFSP_MSGD_convertReturnFromBstf(l_resBStuf);

                        if( e_eFSP_MSGD_RES_MESSAGEENDED == l_result )
                        {
                            /* Check if ended correclty */
                            l_insertEncState = e_eFSP_MSGDPRV_SM_ENDCHECK;
                        }
                        else if( e_eFSP_MSGD_RES_OK == l_result )
                        {
                            /* Check if okied correclty */
                            l_insertEncState = e_eFSP_MSGDPRV_SM_OKCHECK;
                        }
                        else if( e_eFSP_MSGD_RES_BADFRAME == l_result )
                        {
                            /* bad frame, return till somehome will restart elaboration */
                            l_insertEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        }
                        else if( e_eFSP_MSGD_RES_FRAMERESTART == l_result )
                        {
                            /* Frame restarted, return */
                            l_insertEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        }
                        else
                        {
                            /* Some error in unstuffer process, return it */
                            l_insertEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        }

                        break;
                    }

                    case e_eFSP_MSGDPRV_SM_ENDCHECK:
                    {
                        /* Verify message integrity */
                        l_resMsgCor = eFSP_MSGD_isMsgCorr(&p_ptCtx->tBUNSTFCtx, &l_isMCor, p_ptCtx->fCrc, p_ptCtx->ptCrcCtx);

                        if( e_eFSP_MSGD_RES_OK == l_resMsgCor )
                        {
                            /* no strange error found, check message correctness */
                            if( true != l_isMCor )
                            {
                                /* Message ended but something about CRC or length is not rigth, restart a frame */
                                l_insertEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                                l_result = e_eFSP_MSGD_RES_BADFRAME;
                            }
                            else
                            {
                                /* Correct message */
                                l_insertEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_result = l_resMsgCor;
                            l_insertEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        }

                        break;
                    }

                    case e_eFSP_MSGDPRV_SM_OKCHECK:
                    {
                        /* Still parsing but we can check if data len is coherent */
                        l_resultMsgCoherent = eFSP_MSGD_isMsgCohe(&p_ptCtx->tBUNSTFCtx, &l_isMCoherent);

                        if( e_eFSP_MSGD_RES_OK == l_resultMsgCoherent )
                        {
                            /* no strange error found, check message correctness */
                            if( true != l_isMCoherent )
                            {
                                l_insertEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                                l_result = e_eFSP_MSGD_RES_BADFRAME;
                            }
                            else
                            {
                                /* Message is correct but not ended, can return */
                                l_insertEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_result = l_resultMsgCoherent;
                            l_insertEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        }
                        break;
                    }

                    default:
                    {
                        /* Impossible end here */
                        l_insertEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        l_result = e_eFSP_MSGD_RES_CORRUPTCTX;
                        break;
                    }
                }
            }
		}
	}

	return l_result;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSP_MSGD_isStatusStillCoherent(const t_eFSP_MSGD_Ctx* p_ptCtx)
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

static e_eFSP_MSGD_RES eFSP_MSGD_convertReturnFromBstf(e_eCU_BUNSTF_RES returnedEvent)
{
	e_eFSP_MSGD_RES l_result;

	switch( returnedEvent )
	{
		case e_eCU_BUNSTF_RES_OK:
		{
			l_result = e_eFSP_MSGD_RES_OK;
            break;
		}

		case e_eCU_BUNSTF_RES_BADPARAM:
		{
			l_result = e_eFSP_MSGD_RES_BADPARAM;
            break;
		}

		case e_eCU_BUNSTF_RES_BADPOINTER:
		{
			l_result = e_eFSP_MSGD_RES_BADPOINTER;
            break;
		}

		case e_eCU_BUNSTF_RES_CORRUPTCTX:
		{
			l_result = e_eFSP_MSGD_RES_CORRUPTCTX;
            break;
		}

		case e_eCU_BUNSTF_RES_OUTOFMEM:
		{
			l_result = e_eFSP_MSGD_RES_OUTOFMEM;
            break;
		}

		case e_eCU_BUNSTF_RES_BADFRAME:
		{
			l_result = e_eFSP_MSGD_RES_BADFRAME;
            break;
		}

		case e_eCU_BUNSTF_RES_FRAMEENDED:
		{
			l_result = e_eFSP_MSGD_RES_MESSAGEENDED;
            break;
		}

		case e_eCU_BUNSTF_RES_FRAMERESTART:
		{
			l_result = e_eFSP_MSGD_RES_FRAMERESTART;
            break;
		}

		case e_eCU_BUNSTF_RES_NOINITLIB:
		{
			l_result = e_eFSP_MSGD_RES_NOINITLIB;
            break;
		}

		default:
		{
            /* Impossible end here */
			l_result = e_eFSP_MSGD_RES_CORRUPTCTX;
            break;
		}
	}

	return l_result;
}

static e_eFSP_MSGD_RES eFSP_MSGD_IsFullMsgDec(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_isMsgDec)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_result;
	e_eCU_BUNSTF_RES l_resByStuff;
    bool_t l_isFullUnstuffed;

    l_isFullUnstuffed = false;
    l_resByStuff = eCU_BUNSTF_IsAFullFrameUnstuff(&p_ptCtx->tBUNSTFCtx, &l_isFullUnstuffed);
    l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

    if( e_eFSP_MSGD_RES_OK == l_result )
    {
        if( false == l_isFullUnstuffed )
        {
            /* Not full unstuffed so itisnt full decoded. Even if the a bad frame is received its not
             * fully decoded */
            *p_isMsgDec = false;
        }
        else
        {
            /* Full frame received at unstuffer level, check if it is valid message at this level */
            l_result = eFSP_MSGD_isMsgCorr(&p_ptCtx->tBUNSTFCtx, p_isMsgDec, p_ptCtx->fCrc, p_ptCtx->ptCrcCtx);
        }
    }

	return l_result;
}

static e_eFSP_MSGD_RES eFSP_MSGD_IsFrmBad(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_isFrameBad)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_result;
	e_eCU_BUNSTF_RES l_resByStuff;
    bool_t l_isFullUnstuffed;
    bool_t l_isCorrect;
    bool_t l_isFrameBadloc;

    l_isFrameBadloc = true;
    l_resByStuff = eCU_BUNSTF_IsFrameBad(&p_ptCtx->tBUNSTFCtx, &l_isFrameBadloc);
    l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

    if( e_eFSP_MSGD_RES_OK == l_result )
    {
        if( true == l_isFrameBadloc )
        {
            /* Not correct at bytestuffer level, no need to check CRC or data len */
            *p_isFrameBad = true;
        }
        else
        {
            /* Frame seems ok, no error found. It could be ended on in receiving state. If ended check CRC,
             * if still in receiving state check coherence with datalen */
            l_isFullUnstuffed = false;
            l_resByStuff = eCU_BUNSTF_IsAFullFrameUnstuff(&p_ptCtx->tBUNSTFCtx, &l_isFullUnstuffed);
            l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

            if( e_eFSP_MSGD_RES_OK == l_result )
            {
                if( false == l_isFullUnstuffed )
                {
                    /* Check ongoing coherence */
                    l_result = eFSP_MSGD_isMsgCohe(&p_ptCtx->tBUNSTFCtx, &l_isCorrect);

                    if( e_eFSP_MSGD_RES_OK == l_result )
                    {
                        /* no strange error found, check message correctness */
                        if( true != l_isCorrect )
                        {
                            /* Message not ended but something about it's coherence is wrong */
                            *p_isFrameBad = true;
                        }
                        else
                        {
                            /* Correct message, till now */
                            *p_isFrameBad = false;
                        }
                    }
                }
                else
                {
                    /* Check complete frame coherence */
                    l_result = eFSP_MSGD_isMsgCorr(&p_ptCtx->tBUNSTFCtx, &l_isCorrect, p_ptCtx->fCrc, p_ptCtx->ptCrcCtx);

                    if( e_eFSP_MSGD_RES_OK == l_result )
                    {
                        /* no strange error found, check message correctness */
                        if( true != l_isCorrect )
                        {
                            /* Message ended but something about CRC or length is not rigth, restart a frame */
                            *p_isFrameBad = true;
                        }
                        else
                        {
                            /* Correct message */
                            *p_isFrameBad = false;
                        }
                    }
                }
            }
        }
    }

	return l_result;
}

static e_eFSP_MSGD_RES eFSP_MSGD_isMsgCorr(t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* p_isCor, f_eFSP_MSGD_CrcCb f_Crc,
                                           void* p_crcCtx)
{
    e_eFSP_MSGD_RES l_result;
    e_eCU_BUNSTF_RES l_byteUnstuffRes;
    uint32_t l_dPayToRx;
	uint32_t l_crcInMsg;
	uint32_t l_crcExp;
	uint32_t l_dataSize;
    bool_t l_crcRes;
	uint8_t* lp_data;

    /* Check NULL */
    if( ( NULL == p_ptCtx ) || ( NULL == p_isCor) || ( NULL == f_Crc) || ( NULL == p_crcCtx ) )
    {
        l_result = e_eFSP_MSGD_RES_BADPOINTER;
    }
    else
    {
        /* Ok the frame is complete, need to check if we have data size, data crc, crc rigth value */
        /* Init value */
        l_dataSize = 0u;
        lp_data = NULL;

        /* Get unstuffed data */
        l_byteUnstuffRes = eCU_BUNSTF_GetUnstufData(p_ptCtx, &lp_data, &l_dataSize);
        l_result = eFSP_MSGD_convertReturnFromBstf(l_byteUnstuffRes);

        if( e_eFSP_MSGD_RES_OK == l_result )
        {
            /* Do we have enough data?  */
            if( l_dataSize < EFSP_MIN_MSGDE_BUFFLEN )
            {
                /* Too small frame, discharge */
                *p_isCor = false;
            }
            else
            {
                /* Enough data! Is data len in frame coherent?  */
                l_dPayToRx = eFSP_MSGD_composeU32LE(lp_data[0x04u], lp_data[0x05u], lp_data[0x06u], lp_data[0x07u]);

                if( ( l_dataSize - EFSP_MSGDE_HEADERSIZE ) == l_dPayToRx )
                {
                    /* Data len is coherent! Is crc rigth? */
                    l_crcExp = 0u;

                    /* Estrapolate CRC in Little Endian */
                    l_crcInMsg = eFSP_MSGD_composeU32LE(lp_data[0x00u], lp_data[0x01u], lp_data[0x02u], lp_data[0x03u]);

                    /* Calculate CRC */
                    l_crcRes = (*(f_Crc))( p_crcCtx, eCU_CRC_BASE_SEED, &lp_data[4u], l_dPayToRx + 4u,  &l_crcExp );

                    if( true == l_crcRes )
                    {
                        if( l_crcInMsg == l_crcExp )
                        {
                            /* All ok */
                            *p_isCor = true;
                        }
                        else
                        {
                            /* Data Crc is wrong, discharge */
                            *p_isCor = false;
                        }
                    }
                    else
                    {
                        l_result = e_eFSP_MSGD_RES_CRCCLBKERROR;
                    }
                }
                else
                {
                    /* Data len is wrong, discharge */
                    *p_isCor = false;
                }
            }
        }
    }

    return l_result;
}

static e_eFSP_MSGD_RES eFSP_MSGD_isMsgCohe(t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* p_isCoherent)
{
    /* Need to check coherence of the message during message receiving, how? Check if data len reported by payload
     * is lower than data payload received, if greater something is wrong  */
    e_eFSP_MSGD_RES l_result;
    e_eCU_BUNSTF_RES l_byteUnstuffRes;
    uint32_t l_dPayToRx;
	uint32_t l_dataSize;
	uint8_t* lp_data;

    /* Check NULL */
    if( ( NULL == p_ptCtx ) || ( NULL == p_isCoherent) )
    {
        l_result = e_eFSP_MSGD_RES_BADPOINTER;
    }
    else
    {
        /* Check how much payload is received */
        /* Init value */
        l_dataSize = 0u;
        lp_data = NULL;

        /* Get unstuffed data */
        l_byteUnstuffRes = eCU_BUNSTF_GetUnstufData(p_ptCtx, &lp_data, &l_dataSize);
        l_result = eFSP_MSGD_convertReturnFromBstf(l_byteUnstuffRes);

        if( e_eFSP_MSGD_RES_OK == l_result )
        {
            /* Do we have enough data?  */
            if( l_dataSize < EFSP_MSGDE_HEADERSIZE )
            {
                /* Not enoght data to make a check */
                *p_isCoherent = true;
            }
            else
            {
                /* Enough data! Is data len in frame coherent?  */
                l_dPayToRx = eFSP_MSGD_composeU32LE(lp_data[0x04u], lp_data[0x05u], lp_data[0x06u], lp_data[0x07u]);

                if( ( l_dataSize - EFSP_MSGDE_HEADERSIZE ) <= l_dPayToRx )
                {
                    /* Data len is coherent! */
                    *p_isCoherent = true;
                }
                else
                {
                    /* Data len is wrong, discharge */
                    *p_isCoherent = false;
                }
            }
        }
    }

    return l_result;
}

static uint32_t eFSP_MSGD_composeU32LE(uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4)
{
    uint32_t l_tempS;
    uint32_t l_result;

    /* Init var */
    l_result = 0u;

    /* Calculate */
    l_tempS =                 (uint32_t) v1;
    l_result |= ( l_tempS & 0x000000FFu );
    l_tempS =  (uint32_t) ( ( (uint32_t) v2 ) << 8u  );
    l_result |= ( l_tempS & 0x0000FF00u );
    l_tempS =  (uint32_t) ( ( (uint32_t) v3 ) << 16u  );
    l_result |= ( l_tempS & 0x00FF0000u );
    l_tempS =  (uint32_t) ( ( (uint32_t) v4 ) << 24u  );
    l_result |= ( l_tempS & 0xFF000000u );

    return l_result;
}