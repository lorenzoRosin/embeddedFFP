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
static e_eFSP_MSGD_RES eFSP_MSGD_convertReturnFromBstf(e_eCU_BUNSTF_RES p_eRetEvent);
static e_eFSP_MSGD_RES eFSP_MSGD_IsFullMsgDec(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_pbIsMsgDec);
static e_eFSP_MSGD_RES eFSP_MSGD_IsFrmBad(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_pbIsFrameBad);
static e_eFSP_MSGD_RES eFSP_MSGD_isMsgCorr(t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* p_pbIsCor, f_eFSP_MSGD_CrcCb p_fCrc,
                                           t_eFSP_MSGD_CrcCtx* p_ptCrcCtx);
static e_eFSP_MSGD_RES eFSP_MSGD_isMsgCohe(t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* p_pbIsCoh);
static uint32_t eFSP_MSGD_CreateU32LE(uint8_t p_uV1, uint8_t p_uV2, uint8_t p_uV3, uint8_t p_uV4);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_MSGD_RES eFSP_MSGD_InitCtx(t_eFSP_MSGD_Ctx* const p_ptCtx, uint8_t* p_puBuff, const uint32_t p_uBuffL,
								  f_eFSP_MSGD_CrcCb p_fCrc, t_eFSP_MSGD_CrcCtx* const p_ptFctx)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_eRes;
	e_eCU_BUNSTF_RES l_eResBSTF;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puBuff ) || ( NULL == p_fCrc ) || ( NULL == p_ptFctx ) )
	{
		l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity, we need some len to store the data */
        if( p_uBuffL < EFSP_MIN_MSGDE_BUFFLEN )
        {
            l_eRes = e_eFSP_MSGD_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status clbck */
            p_ptCtx->fCrc = p_fCrc;
            p_ptCtx->ptCrcCtx = p_ptFctx;

			/* initialize internal bytestuffer */
			l_eResBSTF =  eCU_BUNSTF_InitCtx(&p_ptCtx->tBUNSTFCtx, &p_puBuff[0u], p_uBuffL);
			l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);
        }
	}

	return l_eRes;
}

e_eFSP_MSGD_RES eFSP_MSGD_IsInit(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* p_pbIsInit)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_eRes;
	e_eCU_BUNSTF_RES l_eResBSTF;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsInit ) )
	{
		l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
        l_eResBSTF = eCU_BUNSTF_IsInit(&p_ptCtx->tBUNSTFCtx, p_pbIsInit);
        l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);
	}

	return l_eRes;
}

e_eFSP_MSGD_RES eFSP_MSGD_NewMsg(t_eFSP_MSGD_Ctx* const p_ptCtx)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_eRes;
	e_eCU_BUNSTF_RES l_eResBSTF;

	/* Check pointer validity */
	if( NULL == p_ptCtx )
	{
		l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Update index */
			l_eResBSTF = eCU_BUNSTF_NewFrame(&p_ptCtx->tBUNSTFCtx);
			l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);
		}
	}

	return l_eRes;
}

e_eFSP_MSGD_RES eFSP_MSGD_GetDecodedData(t_eFSP_MSGD_Ctx* const p_ptCtx, uint8_t** p_ppuDat, uint32_t* const p_puGetL)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_eRes;
	e_eCU_BUNSTF_RES l_eResBSTF;
	uint8_t*  l_puData;
    uint8_t** l_ppuData;
	uint32_t  l_uDataL;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_ppuDat ) || ( NULL == p_puGetL ) )
	{
		l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            l_uDataL = 0u;
            l_puData = NULL;
            l_ppuData = &l_puData;
			l_eResBSTF = eCU_BUNSTF_GetUnstufData(&p_ptCtx->tBUNSTFCtx, l_ppuData, &l_uDataL);
			l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);

			if( e_eFSP_MSGD_RES_OK == l_eRes )
			{
                /* Starting point is always the same */
                *p_ppuDat = &(*l_ppuData)[EFSP_MSGDE_HEADERSIZE];

				/* Removed HADER from data ( CRC + DATA LEN ) */
                if( l_uDataL < EFSP_MIN_MSGDE_BUFFLEN )
                {
					/* No memory free */
                    *p_puGetL = 0u;
                }
                else
                {
                    /* Return reference of only the raw payload */
                    *p_puGetL = l_uDataL - EFSP_MSGDE_HEADERSIZE;
                }

				l_eRes = e_eFSP_MSGD_RES_OK;
			}
		}
	}

	return l_eRes;
}

e_eFSP_MSGD_RES eFSP_MSGD_GetDecodedLen(t_eFSP_MSGD_Ctx* const p_ptCtx, uint32_t* const p_puRetrivedL)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_eRes;
	e_eCU_BUNSTF_RES l_eResBSTF;
	uint32_t l_uDataL;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puRetrivedL ) )
	{
		l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            l_uDataL = 0u;
			l_eResBSTF = eCU_BUNSTF_GetUnstufLen(&p_ptCtx->tBUNSTFCtx, &l_uDataL);
			l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);

			if( e_eFSP_MSGD_RES_OK == l_eRes )
			{
				/* Removed HADER from data ( CRC + DATA LEN ) */
                if( l_uDataL < EFSP_MIN_MSGDE_BUFFLEN )
                {
					/* No memory free */
                    *p_puRetrivedL = 0u;
                }
                else
                {
                    /* Return reference of only the raw payload */
                    *p_puRetrivedL = l_uDataL - EFSP_MSGDE_HEADERSIZE;
                }

				l_eRes = e_eFSP_MSGD_RES_OK;
			}
		}
	}

	return l_eRes;
}

e_eFSP_MSGD_RES eFSP_MSGD_IsWaitingSof(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_pbIsWaitingSof)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_eRes;
	e_eCU_BUNSTF_RES l_eResBSTF;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsWaitingSof ) )
	{
		l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
			l_eResBSTF = eCU_BUNSTF_IsWaitingSof(&p_ptCtx->tBUNSTFCtx, p_pbIsWaitingSof);
			l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);
		}
	}

	return l_eRes;
}

e_eFSP_MSGD_RES eFSP_MSGD_IsAFullMsgDecoded(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_pbIsMsgDec)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsMsgDec ) )
	{
		l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
            l_eRes = eFSP_MSGD_IsFullMsgDec(p_ptCtx, p_pbIsMsgDec);
		}
	}

	return l_eRes;
}

e_eFSP_MSGD_RES eFSP_MSGD_IsFrameBad(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_pbIsFrameBad)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_eRes;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsFrameBad ) )
	{
		l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
            l_eRes = eFSP_MSGD_IsFrmBad(p_ptCtx, p_pbIsFrameBad);
		}
	}

	return l_eRes;
}

e_eFSP_MSGD_RES eFSP_MSGD_GetMostEffDatLen(t_eFSP_MSGD_Ctx* const p_ptCtx, uint32_t* const p_puMostEffPayL)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_eRes;
	e_eCU_BUNSTF_RES l_eResBSTF;
    bool_t l_bIsFullUNSTF;
    bool_t l_bIsWaitingSof;
    bool_t l_bIsFrameBad;
    uint32_t l_uDataLRemaings;
	uint32_t l_uDPayRx;
    uint32_t l_uDataLRaw;
	uint8_t* l_puData;
    uint32_t l_uDPayTRx;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puMostEffPayL ) )
	{
		l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
            /* Check if the frame is already ended */
            l_bIsFullUNSTF = false;
            l_eResBSTF = eCU_BUNSTF_IsAFullFrameUnstuff(&p_ptCtx->tBUNSTFCtx, &l_bIsFullUNSTF);
            l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);

            if( e_eFSP_MSGD_RES_OK == l_eRes )
            {
                if( true == l_bIsFullUNSTF )
                {
                    /* Full unstuffed frame, could be wrong or rigth but no more data is needed */
                    *p_puMostEffPayL = 0u;
                }
                else
                {
                    /* Check for error */
                    l_bIsFrameBad = true;
                    l_eResBSTF = eCU_BUNSTF_IsFrameBad(&p_ptCtx->tBUNSTFCtx, &l_bIsFrameBad);
                    l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);

                    if( e_eFSP_MSGD_RES_OK == l_eRes )
                    {
                        if( true == l_bIsFrameBad )
                        {
                            /* Bad frame only for unstuffer level, dont request more data */
                            *p_puMostEffPayL = 0u;
                        }
                        else
                        {
                            /* How many byte do we have decoded? */
                            l_uDataLRaw = 0u;
                            l_puData = NULL;
                            l_eResBSTF = eCU_BUNSTF_GetUnstufData(&p_ptCtx->tBUNSTFCtx, &l_puData, &l_uDataLRaw);
                            l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);

                            if( e_eFSP_MSGD_RES_OK == l_eRes )
                            {
                                /* Do we have enough data?  */
                                if( 0u == l_uDataLRaw )
                                {
                                    /* No data, are we still waiting SOF? */
                                    l_bIsWaitingSof = false;
                                    l_eResBSTF = eCU_BUNSTF_IsWaitingSof(&p_ptCtx->tBUNSTFCtx, &l_bIsWaitingSof);
                                    l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);

                                    if( e_eFSP_MSGD_RES_OK == l_eRes )
                                    {
                                        if( true == l_bIsWaitingSof )
                                        {
                                            /* Header + SOF */
                                            *p_puMostEffPayL = EFSP_MSGDE_HEADERSIZE + 1u;
                                        }
                                        else
                                        {
                                            /* Header only */
                                            *p_puMostEffPayL = EFSP_MSGDE_HEADERSIZE;
                                        }
                                    }
                                }
                                else if( l_uDataLRaw < EFSP_MSGDE_HEADERSIZE )
                                {
                                    /* Need to receive all the header before estimating data size */
                                    *p_puMostEffPayL = EFSP_MSGDE_HEADERSIZE - l_uDataLRaw;
                                }
                                else
                                {
                                    /* Enough data! Start remaining data estimation */
                                    l_uDPayTRx = eFSP_MSGD_CreateU32LE(l_puData[0x04u], l_puData[0x05u],
                                                                        l_puData[0x06u], l_puData[0x07u]);

                                    /* How much payload do we have */
                                    l_uDPayRx = l_uDataLRaw - EFSP_MSGDE_HEADERSIZE;

                                    /* A correct frame payload must have less lenght than the size reported in frame
                                     * header */
                                    if( l_uDPayRx <= l_uDPayTRx )
                                    {
                                        l_uDataLRemaings = l_uDPayTRx - l_uDPayRx;

                                        /* Wait remaining data + EOF */
                                        if( l_uDataLRemaings < MAX_UINT32VAL )
                                        {
                                            /* l_uDataLRemaings != 0 -> need data + EOF
                                             * l_uDataLRemaings == 0 -> need EOF . Infact we are sure the frame is not
                                             * ended because we already have called the function
                                             * BUNSTF_IsAFullFrameUnstuff */
                                            *p_puMostEffPayL = l_uDataLRemaings + 1u;
                                        }
                                        else
                                        {
                                            *p_puMostEffPayL = l_uDataLRemaings;
                                        }
                                    }
                                    else
                                    {
                                        /* We have more data than expected -> bad frame -> no need to receive data */
                                        *p_puMostEffPayL = 0u;
                                    }
                                }
                            }
                        }
                    }
                }
            }
		}
	}

	return l_eRes;
}

e_eFSP_MSGD_RES eFSP_MSGD_InsEncChunk(t_eFSP_MSGD_Ctx* const p_ptCtx, uint8_t* p_puEncArea, const uint32_t p_uEncL,
                                      uint32_t* const p_puUsedEncB)
{
	/* Local return  */
	e_eFSP_MSGD_RES l_eRes;
    e_eFSP_MSGD_RES l_eResMsgCor;
    e_eFSP_MSGD_RES l_eResMsgCoh;
	e_eCU_BUNSTF_RES l_eResBSTF;

    /* Local coherence */
    bool_t l_bIsMCor;
    bool_t l_bIsMCoh;

    /* Redo loop var */
    e_eFSP_MSGDPRV_SM l_eInsEncState;

	/* Check pointer validity */
	if( ( NULL == p_ptCtx ) || ( NULL == p_puEncArea ) || ( NULL == p_puUsedEncB ) )
	{
		l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ptCtx) )
		{
			l_eRes = e_eFSP_MSGD_RES_CORRUPTCTX;
		}
		else
		{
            /* Init data */
            *p_puUsedEncB = 0u;

            /* Elaborate */
            l_eInsEncState = e_eFSP_MSGDPRV_SM_PRECHECK;
            l_eRes = e_eFSP_MSGD_RES_OK;

            while( l_eInsEncState != e_eFSP_MSGDPRV_SM_ELABDONE )
            {
                switch( l_eInsEncState )
                {
                    case e_eFSP_MSGDPRV_SM_PRECHECK:
                    {
                        /* Before inserting data check if the current message is coherent */
                        l_eResMsgCoh = eFSP_MSGD_isMsgCohe(&p_ptCtx->tBUNSTFCtx, &l_bIsMCoh);

                        if( e_eFSP_MSGD_RES_OK == l_eResMsgCoh )
                        {
                            /* no strange error found, check message correctness */
                            if( true != l_bIsMCoh )
                            {
                                /* Message is not coherent */
                                l_eInsEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                                l_eRes = e_eFSP_MSGD_RES_BADFRAME;
                            }
                            else
                            {
                                /* Message is correct but not ended */
                                l_eInsEncState = e_eFSP_MSGDPRV_SM_INSERTCHUNK;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_eRes = l_eResMsgCoh;
                            l_eInsEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        }

                        break;
                    }

                    case e_eFSP_MSGDPRV_SM_INSERTCHUNK:
                    {
                        /* Insert data */
                        l_eResBSTF = eCU_BUNSTF_InsStufChunk(&p_ptCtx->tBUNSTFCtx, p_puEncArea, p_uEncL, p_puUsedEncB);
                        l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);

                        if( e_eFSP_MSGD_RES_MESSAGEENDED == l_eRes )
                        {
                            /* Check if ended correclty */
                            l_eInsEncState = e_eFSP_MSGDPRV_SM_ENDCHECK;
                        }
                        else if( e_eFSP_MSGD_RES_OK == l_eRes )
                        {
                            /* Check if okied correclty */
                            l_eInsEncState = e_eFSP_MSGDPRV_SM_OKCHECK;
                        }
                        else if( e_eFSP_MSGD_RES_BADFRAME == l_eRes )
                        {
                            /* bad frame, return till somehome will restart elaboration */
                            l_eInsEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        }
                        else if( e_eFSP_MSGD_RES_FRAMERESTART == l_eRes )
                        {
                            /* Frame restarted, return */
                            l_eInsEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        }
                        else
                        {
                            /* Some error in unstuffer process, return it */
                            l_eInsEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        }

                        break;
                    }

                    case e_eFSP_MSGDPRV_SM_ENDCHECK:
                    {
                        /* Verify message integrity */
                        l_eResMsgCor = eFSP_MSGD_isMsgCorr(&p_ptCtx->tBUNSTFCtx, &l_bIsMCor, p_ptCtx->fCrc, 
                                                           p_ptCtx->ptCrcCtx);

                        if( e_eFSP_MSGD_RES_OK == l_eResMsgCor )
                        {
                            /* no strange error found, check message correctness */
                            if( true != l_bIsMCor )
                            {
                                /* Message ended but something about CRC or length is not rigth, restart a frame */
                                l_eInsEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                                l_eRes = e_eFSP_MSGD_RES_BADFRAME;
                            }
                            else
                            {
                                /* Correct message */
                                l_eInsEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_eRes = l_eResMsgCor;
                            l_eInsEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        }

                        break;
                    }

                    case e_eFSP_MSGDPRV_SM_OKCHECK:
                    {
                        /* Still parsing but we can check if data len is coherent */
                        l_eResMsgCoh = eFSP_MSGD_isMsgCohe(&p_ptCtx->tBUNSTFCtx, &l_bIsMCoh);

                        if( e_eFSP_MSGD_RES_OK == l_eResMsgCoh )
                        {
                            /* no strange error found, check message correctness */
                            if( true != l_bIsMCoh )
                            {
                                l_eInsEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                                l_eRes = e_eFSP_MSGD_RES_BADFRAME;
                            }
                            else
                            {
                                /* Message is correct but not ended, can return */
                                l_eInsEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_eRes = l_eResMsgCoh;
                            l_eInsEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        }
                        break;
                    }

                    default:
                    {
                        /* Impossible end here */
                        l_eInsEncState = e_eFSP_MSGDPRV_SM_ELABDONE;
                        l_eRes = e_eFSP_MSGD_RES_CORRUPTCTX;
                        break;
                    }
                }
            }
		}
	}

	return l_eRes;
}



/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
static bool_t eFSP_MSGD_isStatusStillCoherent(const t_eFSP_MSGD_Ctx* p_ptCtx)
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

static e_eFSP_MSGD_RES eFSP_MSGD_convertReturnFromBstf(e_eCU_BUNSTF_RES p_eRetEvent)
{
	e_eFSP_MSGD_RES l_eRes;

	switch( p_eRetEvent )
	{
		case e_eCU_BUNSTF_RES_OK:
		{
			l_eRes = e_eFSP_MSGD_RES_OK;
            break;
		}

		case e_eCU_BUNSTF_RES_BADPARAM:
		{
			l_eRes = e_eFSP_MSGD_RES_BADPARAM;
            break;
		}

		case e_eCU_BUNSTF_RES_BADPOINTER:
		{
			l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
            break;
		}

		case e_eCU_BUNSTF_RES_CORRUPTCTX:
		{
			l_eRes = e_eFSP_MSGD_RES_CORRUPTCTX;
            break;
		}

		case e_eCU_BUNSTF_RES_OUTOFMEM:
		{
			l_eRes = e_eFSP_MSGD_RES_OUTOFMEM;
            break;
		}

		case e_eCU_BUNSTF_RES_BADFRAME:
		{
			l_eRes = e_eFSP_MSGD_RES_BADFRAME;
            break;
		}

		case e_eCU_BUNSTF_RES_FRAMEENDED:
		{
			l_eRes = e_eFSP_MSGD_RES_MESSAGEENDED;
            break;
		}

		case e_eCU_BUNSTF_RES_FRAMERESTART:
		{
			l_eRes = e_eFSP_MSGD_RES_FRAMERESTART;
            break;
		}

		case e_eCU_BUNSTF_RES_NOINITLIB:
		{
			l_eRes = e_eFSP_MSGD_RES_NOINITLIB;
            break;
		}

		default:
		{
            /* Impossible end here */
			l_eRes = e_eFSP_MSGD_RES_CORRUPTCTX;
            break;
		}
	}

	return l_eRes;
}

static e_eFSP_MSGD_RES eFSP_MSGD_IsFullMsgDec(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_pbIsMsgDec)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_eRes;
	e_eCU_BUNSTF_RES l_eResBSTF;
    bool_t l_bIsFullUNSTF;

    l_bIsFullUNSTF = false;
    l_eResBSTF = eCU_BUNSTF_IsAFullFrameUnstuff(&p_ptCtx->tBUNSTFCtx, &l_bIsFullUNSTF);
    l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);

    if( e_eFSP_MSGD_RES_OK == l_eRes )
    {
        if( false == l_bIsFullUNSTF )
        {
            /* Not full unstuffed so itisnt full decoded. Even if the a bad frame is received its not
             * fully decoded */
            *p_pbIsMsgDec = false;
        }
        else
        {
            /* Full frame received at unstuffer level, check if it is valid message at this level */
            l_eRes = eFSP_MSGD_isMsgCorr(&p_ptCtx->tBUNSTFCtx, p_pbIsMsgDec, p_ptCtx->fCrc, p_ptCtx->ptCrcCtx);
        }
    }

	return l_eRes;
}

static e_eFSP_MSGD_RES eFSP_MSGD_IsFrmBad(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_pbIsFrameBad)
{
	/* Local variable */
	e_eFSP_MSGD_RES l_eRes;
	e_eCU_BUNSTF_RES l_eResBSTF;
    bool_t l_bIsFullUNSTF;
    bool_t l_bIsCor;
    bool_t l_bIsFrmBadloc;

    l_bIsFrmBadloc = true;
    l_eResBSTF = eCU_BUNSTF_IsFrameBad(&p_ptCtx->tBUNSTFCtx, &l_bIsFrmBadloc);
    l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);

    if( e_eFSP_MSGD_RES_OK == l_eRes )
    {
        if( true == l_bIsFrmBadloc )
        {
            /* Not correct at bytestuffer level, no need to check CRC or data len */
            *p_pbIsFrameBad = true;
        }
        else
        {
            /* Frame seems ok, no error found. It could be ended on in receiving state. If ended check CRC,
             * if still in receiving state check coherence with datalen */
            l_bIsFullUNSTF = false;
            l_eResBSTF = eCU_BUNSTF_IsAFullFrameUnstuff(&p_ptCtx->tBUNSTFCtx, &l_bIsFullUNSTF);
            l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBSTF);

            if( e_eFSP_MSGD_RES_OK == l_eRes )
            {
                if( false == l_bIsFullUNSTF )
                {
                    /* Check ongoing coherence */
                    l_eRes = eFSP_MSGD_isMsgCohe(&p_ptCtx->tBUNSTFCtx, &l_bIsCor);

                    if( e_eFSP_MSGD_RES_OK == l_eRes )
                    {
                        /* no strange error found, check message correctness */
                        if( true != l_bIsCor )
                        {
                            /* Message not ended but something about it's coherence is wrong */
                            *p_pbIsFrameBad = true;
                        }
                        else
                        {
                            /* Correct message, till now */
                            *p_pbIsFrameBad = false;
                        }
                    }
                }
                else
                {
                    /* Check complete frame coherence */
                    l_eRes = eFSP_MSGD_isMsgCorr(&p_ptCtx->tBUNSTFCtx, &l_bIsCor, p_ptCtx->fCrc, p_ptCtx->ptCrcCtx);

                    if( e_eFSP_MSGD_RES_OK == l_eRes )
                    {
                        /* no strange error found, check message correctness */
                        if( true != l_bIsCor )
                        {
                            /* Message ended but something about CRC or length is not rigth, restart a frame */
                            *p_pbIsFrameBad = true;
                        }
                        else
                        {
                            /* Correct message */
                            *p_pbIsFrameBad = false;
                        }
                    }
                }
            }
        }
    }

	return l_eRes;
}

static e_eFSP_MSGD_RES eFSP_MSGD_isMsgCorr(t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* p_pbIsCor, f_eFSP_MSGD_CrcCb p_fCrc,
                                           t_eFSP_MSGD_CrcCtx* p_ptCrcCtx)
{
    e_eFSP_MSGD_RES l_eRes;
    e_eCU_BUNSTF_RES l_eResBUNSTF;
    uint32_t l_uDPayTRx;
	uint32_t l_uCMsg;
	uint32_t l_uCExp;
	uint32_t l_uDataL;
    bool_t l_bResCrc;
	uint8_t* l_puData;

    /* Check NULL */
    if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsCor) || ( NULL == p_fCrc) || ( NULL == p_ptCrcCtx ) )
    {
        l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
    }
    else
    {
        /* Ok the frame is complete, need to check if we have data size, data crc, crc rigth value */
        /* Init value */
        l_uDataL = 0u;
        l_puData = NULL;

        /* Get unstuffed data */
        l_eResBUNSTF = eCU_BUNSTF_GetUnstufData(p_ptCtx, &l_puData, &l_uDataL);
        l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBUNSTF);

        if( e_eFSP_MSGD_RES_OK == l_eRes )
        {
            /* Do we have enough data?  */
            if( l_uDataL < EFSP_MIN_MSGDE_BUFFLEN )
            {
                /* Too small frame, discharge */
                *p_pbIsCor = false;
            }
            else
            {
                /* Enough data! Is data len in frame coherent?  */
                l_uDPayTRx = eFSP_MSGD_CreateU32LE(l_puData[0x04u], l_puData[0x05u], l_puData[0x06u], l_puData[0x07u]);

                if( ( l_uDataL - EFSP_MSGDE_HEADERSIZE ) == l_uDPayTRx )
                {
                    /* Data len is coherent! Is crc rigth? */
                    l_uCExp = 0u;

                    /* Estrapolate CRC in Little Endian */
                    l_uCMsg = eFSP_MSGD_CreateU32LE(l_puData[0x00u], l_puData[0x01u], l_puData[0x02u], l_puData[0x03u]);

                    /* Calculate CRC */
                    l_bResCrc = (*(p_fCrc))( p_ptCrcCtx, eCU_CRC_BASE_SEED, &l_puData[4u], l_uDPayTRx + 4u,  &l_uCExp );

                    if( true == l_bResCrc )
                    {
                        if( l_uCMsg == l_uCExp )
                        {
                            /* All ok */
                            *p_pbIsCor = true;
                        }
                        else
                        {
                            /* Data Crc is wrong, discharge */
                            *p_pbIsCor = false;
                        }
                    }
                    else
                    {
                        l_eRes = e_eFSP_MSGD_RES_CRCCLBKERROR;
                    }
                }
                else
                {
                    /* Data len is wrong, discharge */
                    *p_pbIsCor = false;
                }
            }
        }
    }

    return l_eRes;
}

static e_eFSP_MSGD_RES eFSP_MSGD_isMsgCohe(t_eCU_BUNSTF_Ctx* p_ptCtx, bool_t* p_pbIsCoh)
{
    /* Need to check coherence of the message during message receiving, how? Check if data len reported by payload
     * is lower than data payload received, if greater something is wrong  */
    e_eFSP_MSGD_RES l_eRes;
    e_eCU_BUNSTF_RES l_eResBUNSTF;
    uint32_t l_uDPayTRx;
	uint32_t l_uDataL;
	uint8_t* l_puData;

    /* Check NULL */
    if( ( NULL == p_ptCtx ) || ( NULL == p_pbIsCoh) )
    {
        l_eRes = e_eFSP_MSGD_RES_BADPOINTER;
    }
    else
    {
        /* Check how much payload is received */
        /* Init value */
        l_uDataL = 0u;
        l_puData = NULL;

        /* Get unstuffed data */
        l_eResBUNSTF = eCU_BUNSTF_GetUnstufData(p_ptCtx, &l_puData, &l_uDataL);
        l_eRes = eFSP_MSGD_convertReturnFromBstf(l_eResBUNSTF);

        if( e_eFSP_MSGD_RES_OK == l_eRes )
        {
            /* Do we have enough data?  */
            if( l_uDataL < EFSP_MSGDE_HEADERSIZE )
            {
                /* Not enoght data to make a check */
                *p_pbIsCoh = true;
            }
            else
            {
                /* Enough data! Is data len in frame coherent?  */
                l_uDPayTRx = eFSP_MSGD_CreateU32LE(l_puData[0x04u], l_puData[0x05u], l_puData[0x06u], l_puData[0x07u]);

                if( ( l_uDataL - EFSP_MSGDE_HEADERSIZE ) <= l_uDPayTRx )
                {
                    /* Data len is coherent! */
                    *p_pbIsCoh = true;
                }
                else
                {
                    /* Data len is wrong, discharge */
                    *p_pbIsCoh = false;
                }
            }
        }
    }

    return l_eRes;
}

static uint32_t eFSP_MSGD_CreateU32LE(uint8_t p_uV1, uint8_t p_uV2, uint8_t p_uV3, uint8_t p_uV4)
{
    uint32_t l_uTemp;
    uint32_t l_uRes;

    /* Init var */
    l_uRes = 0u;

    /* Calculate */
    l_uTemp =                 (uint32_t) p_uV1;
    l_uRes |= ( l_uTemp & 0x000000FFu );
    l_uTemp =  (uint32_t) ( ( (uint32_t) p_uV2 ) << 8u  );
    l_uRes |= ( l_uTemp & 0x0000FF00u );
    l_uTemp =  (uint32_t) ( ( (uint32_t) p_uV3 ) << 16u  );
    l_uRes |= ( l_uTemp & 0x00FF0000u );
    l_uTemp =  (uint32_t) ( ( (uint32_t) p_uV4 ) << 24u  );
    l_uRes |= ( l_uTemp & 0xFF000000u );

    return l_uRes;
}