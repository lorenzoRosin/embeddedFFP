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
#include "eFSPMsgDecoder_Priv.h"
#include "eCUCrc.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t eFSP_MSGD_isStatusStillCoherent(const s_eFSP_MSGD_Ctx* p_ctx);
static e_eFSP_MSGD_Res eFSP_MSGD_convertReturnFromBstf(s_eCU_BUNSTF_Res returnedEvent);
static e_eFSP_MSGD_Res eFSP_MSGD_IsFullMsgDec(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* const p_isMsgDec);
static e_eFSP_MSGD_Res eFSP_MSGD_IsFrmBad(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* const p_isFrameBad);
static e_eFSP_MSGD_Res eFSP_MSGD_isMsgCorr(s_eCU_BUNSTF_Ctx* p_ctx, bool_t* p_isCorrect, cb_crc32_msgd f_Crc, void* p_cbCrcCtx);
static e_eFSP_MSGD_Res eFSP_MSGD_isMsgCohe(s_eCU_BUNSTF_Ctx* p_ctx, bool_t* p_isCoherent);
static uint32_t eFSP_MSGD_composeU32LE(uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed for code clarity */
#endif

e_eFSP_MSGD_Res eFSP_MSGD_InitCtx(s_eFSP_MSGD_Ctx* const p_ctx, uint8_t a_memArea[], const uint32_t memAreaSize,
								  cb_crc32_msgd f_Crc, void* const p_clbCtx)
{
	/* Local variable */
	e_eFSP_MSGD_Res l_result;
	s_eCU_BUNSTF_Res l_resByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == a_memArea ) || ( NULL == f_Crc ) || ( NULL == p_clbCtx ) )
	{
		l_result = MSGD_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity, we need some len to store the data */
        if( memAreaSize < EFSP_MIN_MSGDE_BUFFLEN )
        {
            l_result = MSGD_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status clbck */
            p_ctx->f_cbCrc = f_Crc;
            p_ctx->p_cbCrcCtx = p_clbCtx;

			/* initialize internal bytestuffer */
			l_resByStuff =  eCU_BUNSTF_InitCtx(&p_ctx->byteUStufferCtnx, a_memArea, memAreaSize);
			l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);
        }
	}

	return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3"
#endif

e_eFSP_MSGD_Res eFSP_MSGD_IsInit(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* p_isInit)
{
	/* Local variable */
	e_eFSP_MSGD_Res l_result;
	s_eCU_BUNSTF_Res l_resByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_isInit ) )
	{
		l_result = MSGD_RES_BADPOINTER;
	}
	else
	{
        l_resByStuff = eCU_BUNSTF_IsInit(&p_ctx->byteUStufferCtnx, p_isInit);
        l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);
	}

	return l_result;
}

e_eFSP_MSGD_Res eFSP_MSGD_NewMsg(s_eFSP_MSGD_Ctx* const p_ctx)
{
	/* Local variable */
	e_eFSP_MSGD_Res l_result;
	s_eCU_BUNSTF_Res l_resByStuff;

	/* Check pointer validity */
	if( NULL == p_ctx )
	{
		l_result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Update index */
			l_resByStuff = eCU_BUNSTF_NewFrame(&p_ctx->byteUStufferCtnx);
			l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);
		}
	}

	return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MEM-stack-param", "MISRAC2004-17.6_d", "MISRAC2012-Rule-1.3_s", \
                            "MISRAC2012-Rule-18.6_d", "CERT-DCL30-C_e"
    /* Suppressed for code clarity */
#endif

e_eFSP_MSGD_Res eFSP_MSGD_GetDecodedData(s_eFSP_MSGD_Ctx* const p_ctx, uint8_t** pp_data, uint32_t* const p_gettedLen)
{
	/* Local variable */
	e_eFSP_MSGD_Res l_result;
	s_eCU_BUNSTF_Res l_resByStuff;
	uint8_t* lp_dataP;
	uint32_t l_dataSizeP;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == pp_data ) || ( NULL == p_gettedLen ) )
	{
		l_result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            l_dataSizeP = 0u;
            lp_dataP = NULL;
			l_resByStuff = eCU_BUNSTF_GetUnstufData(&p_ctx->byteUStufferCtnx, &lp_dataP, &l_dataSizeP);
			l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

			if( MSGD_RES_OK == l_result )
			{
                /* Starting point is always the same */
                *pp_data = &lp_dataP[EFSP_MSGDE_HEADERSIZE];

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

				l_result = MSGD_RES_OK;
			}
		}
	}

	return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MEM-stack-param", "MISRAC2004-17.6_d", "MISRAC2012-Rule-1.3_s", \
                            "MISRAC2012-Rule-18.6_d", "CERT-DCL30-C_e"
#endif

e_eFSP_MSGD_Res eFSP_MSGD_GetDecodedLen(s_eFSP_MSGD_Ctx* const p_ctx, uint32_t* const p_retrivedLen)
{
	/* Local variable */
	e_eFSP_MSGD_Res l_result;
	s_eCU_BUNSTF_Res l_resByStuff;
	uint32_t l_dataSizeP;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_retrivedLen ) )
	{
		l_result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            l_dataSizeP = 0u;
			l_resByStuff = eCU_BUNSTF_GetUnstufLen(&p_ctx->byteUStufferCtnx, &l_dataSizeP);
			l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

			if( MSGD_RES_OK == l_result )
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

				l_result = MSGD_RES_OK;
			}
		}
	}

	return l_result;
}

e_eFSP_MSGD_Res eFSP_MSGD_IsWaitingSof(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* const p_isWaitingSof)
{
	/* Local variable */
	e_eFSP_MSGD_Res l_result;
	s_eCU_BUNSTF_Res l_resByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_isWaitingSof ) )
	{
		l_result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			l_resByStuff = eCU_BUNSTF_IsWaitingSof(&p_ctx->byteUStufferCtnx, p_isWaitingSof);
			l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);
		}
	}

	return l_result;
}

e_eFSP_MSGD_Res eFSP_MSGD_IsAFullMsgDecoded(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* const p_isMsgDec)
{
	/* Local variable */
	e_eFSP_MSGD_Res l_result;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_isMsgDec ) )
	{
		l_result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
            l_result = eFSP_MSGD_IsFullMsgDec(p_ctx, p_isMsgDec);
		}
	}

	return l_result;
}

e_eFSP_MSGD_Res eFSP_MSGD_IsFrameBad(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* const p_isFrameBad)
{
	/* Local variable */
	e_eFSP_MSGD_Res l_result;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_isFrameBad ) )
	{
		l_result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
            l_result = eFSP_MSGD_IsFrmBad(p_ctx, p_isFrameBad);
		}
	}

	return l_result;
}

e_eFSP_MSGD_Res eFSP_MSGD_GetMostEffDatLen(s_eFSP_MSGD_Ctx* const p_ctx, uint32_t* const p_mostEffPayload)
{
	/* Local variable */
	e_eFSP_MSGD_Res l_result;
	s_eCU_BUNSTF_Res l_resByStuff;
    bool_t l_isFullUnstuffed;
    bool_t l_isWaitingSof;
    bool_t l_isFrameBad;
    uint32_t l_dataSizeRemaings;
	uint32_t l_dPayRxed;
    uint32_t dataSizeRaw;
	uint8_t* lp_data;
    uint32_t l_dPayToRx;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_mostEffPayload ) )
	{
		l_result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
            /* Check if the frame is already ended */
            l_isFullUnstuffed = false;
            l_resByStuff = eCU_BUNSTF_IsAFullFrameUnstuff(&p_ctx->byteUStufferCtnx, &l_isFullUnstuffed);
            l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

            if( MSGD_RES_OK == l_result )
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
                    l_resByStuff = eCU_BUNSTF_IsFrameBad(&p_ctx->byteUStufferCtnx, &l_isFrameBad);
                    l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

                    if( MSGD_RES_OK == l_result )
                    {
                        if( true == l_isFrameBad )
                        {
                            /* Bad frame only for unstuffer level, dont request more data */
                            *p_mostEffPayload = 0u;
                        }
                        else
                        {
                            /* How many byte do we have decoded? */
                            dataSizeRaw = 0u;
                            lp_data = NULL;
                            l_resByStuff = eCU_BUNSTF_GetUnstufData(&p_ctx->byteUStufferCtnx, &lp_data, &dataSizeRaw);
                            l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

                            if( MSGD_RES_OK == l_result )
                            {
                                /* Do we have enough data?  */
                                if( 0u == dataSizeRaw )
                                {
                                    /* No data, are we still waiting SOF? */
                                    l_isWaitingSof = false;
                                    l_resByStuff = eCU_BUNSTF_IsWaitingSof(&p_ctx->byteUStufferCtnx, &l_isWaitingSof);
                                    l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

                                    if( MSGD_RES_OK == l_result )
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
                                else if( dataSizeRaw < EFSP_MSGDE_HEADERSIZE )
                                {
                                    /* Need to receive all the header before estimating data size */
                                    *p_mostEffPayload = EFSP_MSGDE_HEADERSIZE - dataSizeRaw;
                                }
                                else
                                {
                                    /* Enough data! Start remaining data estimation */
                                    l_dPayToRx = eFSP_MSGD_composeU32LE(lp_data[0x04u], lp_data[0x05u], lp_data[0x06u], lp_data[0x07u]);

                                    /* How much payload do we have */
                                    l_dPayRxed = dataSizeRaw - EFSP_MSGDE_HEADERSIZE;

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

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed for code clarity */
#endif

e_eFSP_MSGD_Res eFSP_MSGD_InsEncChunk(s_eFSP_MSGD_Ctx* const p_ctx, uint8_t a_encArea[], const uint32_t encLen,
                                      uint32_t* const p_consumedEncData)
{
	/* Local return  */
	e_eFSP_MSGD_Res l_result;
    e_eFSP_MSGD_Res l_resMsgCorrect;
    e_eFSP_MSGD_Res l_resultMsgCoherent;
	s_eCU_BUNSTF_Res l_resByStuff;

    /* Local coherence */
    bool_t l_isMCorrect;
    bool_t l_isMCoherent;

    /* Redo loop var */
    e_eFSP_MSGD_Priv_state l_insertEncState;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == a_encArea ) || ( NULL == p_consumedEncData ) )
	{
		l_result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			l_result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
            /* Init data */
            *p_consumedEncData = 0u;

            /* Elaborate */
            l_insertEncState = MSGD_PRV_PRECHECK;
            l_result = MSGD_RES_OK;

            while( l_insertEncState != MSGD_PRV_ELABDONE )
            {
                switch( l_insertEncState )
                {
                    case MSGD_PRV_PRECHECK:
                    {
                        /* Before inserting data check if the current message is coherent */
                        l_resultMsgCoherent = eFSP_MSGD_isMsgCohe(&p_ctx->byteUStufferCtnx, &l_isMCoherent);

                        if( MSGD_RES_OK == l_resultMsgCoherent )
                        {
                            /* no strange error found, check message correctness */
                            if( true != l_isMCoherent )
                            {
                                /* Message is not coherent */
                                l_insertEncState = MSGD_PRV_ELABDONE;
                                l_result = MSGD_RES_BADFRAME;
                            }
                            else
                            {
                                /* Message is correct but not ended */
                                l_insertEncState = MSGD_PRV_INSERTCHUNK;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_result = l_resultMsgCoherent;
                            l_insertEncState = MSGD_PRV_ELABDONE;
                        }

                        break;
                    }

                    case MSGD_PRV_INSERTCHUNK:
                    {
                        /* Insert data */
                        l_resByStuff = eCU_BUNSTF_InsStufChunk(&p_ctx->byteUStufferCtnx, a_encArea, encLen, p_consumedEncData);
                        l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

                        if( MSGD_RES_MESSAGEENDED == l_result )
                        {
                            /* Check if ended correclty */
                            l_insertEncState = MSGD_PRV_MSG_END_CHECK;
                        }
                        else if( MSGD_RES_OK == l_result )
                        {
                            /* Check if okied correclty */
                            l_insertEncState = MSGD_PRV_MSG_OK_CHECK;
                        }
                        else if( MSGD_RES_BADFRAME == l_result )
                        {
                            /* bad frame, return till somehome will restart elaboration */
                            l_insertEncState = MSGD_PRV_ELABDONE;
                        }
                        else if( MSGD_RES_FRAMERESTART == l_result )
                        {
                            /* Frame restarted, return */
                            l_insertEncState = MSGD_PRV_ELABDONE;
                        }
                        else
                        {
                            /* Some error in unstuffer process, return it */
                            l_insertEncState = MSGD_PRV_ELABDONE;
                        }

                        break;
                    }

                    case MSGD_PRV_MSG_END_CHECK:
                    {
                        /* Verify message integrity */
                        l_resMsgCorrect = eFSP_MSGD_isMsgCorr(&p_ctx->byteUStufferCtnx, &l_isMCorrect, p_ctx->f_cbCrc, p_ctx->p_cbCrcCtx);

                        if( MSGD_RES_OK == l_resMsgCorrect )
                        {
                            /* no strange error found, check message correctness */
                            if( true != l_isMCorrect )
                            {
                                /* Message ended but something about CRC or length is not rigth, restart a frame */
                                l_insertEncState = MSGD_PRV_ELABDONE;
                                l_result = MSGD_RES_BADFRAME;
                            }
                            else
                            {
                                /* Correct message */
                                l_insertEncState = MSGD_PRV_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_result = l_resMsgCorrect;
                            l_insertEncState = MSGD_PRV_ELABDONE;
                        }

                        break;
                    }

                    case MSGD_PRV_MSG_OK_CHECK:
                    {
                        /* Still parsing but we can check if data len is coherent */
                        l_resultMsgCoherent = eFSP_MSGD_isMsgCohe(&p_ctx->byteUStufferCtnx, &l_isMCoherent);

                        if( MSGD_RES_OK == l_resultMsgCoherent )
                        {
                            /* no strange error found, check message correctness */
                            if( true != l_isMCoherent )
                            {
                                l_insertEncState = MSGD_PRV_ELABDONE;
                                l_result = MSGD_RES_BADFRAME;
                            }
                            else
                            {
                                /* Message is correct but not ended, can return */
                                l_insertEncState = MSGD_PRV_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            l_result = l_resultMsgCoherent;
                            l_insertEncState = MSGD_PRV_ELABDONE;
                        }
                        break;
                    }

                    default:
                    {
                        /* Impossible end here */
                        l_insertEncState = MSGD_PRV_ELABDONE;
                        l_result = MSGD_RES_CORRUPTCTX;
                        break;
                    }
                }
            }
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
static bool_t eFSP_MSGD_isStatusStillCoherent(const s_eFSP_MSGD_Ctx* p_ctx)
{
    bool_t l_result;

	/* Check context validity */
	if( ( NULL == p_ctx->f_cbCrc ) || ( NULL == p_ctx->p_cbCrcCtx ) )
	{
		l_result = false;
	}
	else
	{
		l_result = true;
	}

    return l_result;
}

static e_eFSP_MSGD_Res eFSP_MSGD_convertReturnFromBstf(s_eCU_BUNSTF_Res returnedEvent)
{
	e_eFSP_MSGD_Res l_result;

	switch( returnedEvent )
	{
		case BUNSTF_RES_OK:
		{
			l_result = MSGD_RES_OK;
            break;
		}

		case BUNSTF_RES_BADPARAM:
		{
			l_result = MSGD_RES_BADPARAM;
            break;
		}

		case BUNSTF_RES_BADPOINTER:
		{
			l_result = MSGD_RES_BADPOINTER;
            break;
		}

		case BUNSTF_RES_CORRUPTCTX:
		{
			l_result = MSGD_RES_CORRUPTCTX;
            break;
		}

		case BUNSTF_RES_OUTOFMEM:
		{
			l_result = MSGD_RES_OUTOFMEM;
            break;
		}

		case BUNSTF_RES_BADFRAME:
		{
			l_result = MSGD_RES_BADFRAME;
            break;
		}

		case BUNSTF_RES_FRAMEENDED:
		{
			l_result = MSGD_RES_MESSAGEENDED;
            break;
		}

		case BUNSTF_RES_FRAMERESTART:
		{
			l_result = MSGD_RES_FRAMERESTART;
            break;
		}

		case BUNSTF_RES_NOINITLIB:
		{
			l_result = MSGD_RES_NOINITLIB;
            break;
		}

		default:
		{
            /* Impossible end here */
			l_result = MSGD_RES_CORRUPTCTX;
            break;
		}
	}

	return l_result;
}

static e_eFSP_MSGD_Res eFSP_MSGD_IsFullMsgDec(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* const p_isMsgDec)
{
	/* Local variable */
	e_eFSP_MSGD_Res l_result;
	s_eCU_BUNSTF_Res l_resByStuff;
    bool_t l_isFullUnstuffed;

    l_isFullUnstuffed = false;
    l_resByStuff = eCU_BUNSTF_IsAFullFrameUnstuff(&p_ctx->byteUStufferCtnx, &l_isFullUnstuffed);
    l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

    if( MSGD_RES_OK == l_result )
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
            l_result = eFSP_MSGD_isMsgCorr(&p_ctx->byteUStufferCtnx, p_isMsgDec, p_ctx->f_cbCrc, p_ctx->p_cbCrcCtx);
        }
    }

	return l_result;
}

static e_eFSP_MSGD_Res eFSP_MSGD_IsFrmBad(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* const p_isFrameBad)
{
	/* Local variable */
	e_eFSP_MSGD_Res l_result;
	s_eCU_BUNSTF_Res l_resByStuff;
    bool_t l_isFullUnstuffed;
    bool_t l_isCorrect;
    bool_t l_isFrameBadloc;

    l_isFrameBadloc = true;
    l_resByStuff = eCU_BUNSTF_IsFrameBad(&p_ctx->byteUStufferCtnx, &l_isFrameBadloc);
    l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

    if( MSGD_RES_OK == l_result )
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
            l_resByStuff = eCU_BUNSTF_IsAFullFrameUnstuff(&p_ctx->byteUStufferCtnx, &l_isFullUnstuffed);
            l_result = eFSP_MSGD_convertReturnFromBstf(l_resByStuff);

            if( MSGD_RES_OK == l_result )
            {
                if( false == l_isFullUnstuffed )
                {
                    /* Check ongoing coherence */
                    l_result = eFSP_MSGD_isMsgCohe(&p_ctx->byteUStufferCtnx, &l_isCorrect);

                    if( MSGD_RES_OK == l_result )
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
                    l_result = eFSP_MSGD_isMsgCorr(&p_ctx->byteUStufferCtnx, &l_isCorrect, p_ctx->f_cbCrc, p_ctx->p_cbCrcCtx);

                    if( MSGD_RES_OK == l_result )
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

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-8.13"
    /* Suppressed for code clarity */
#endif

static e_eFSP_MSGD_Res eFSP_MSGD_isMsgCorr(s_eCU_BUNSTF_Ctx* p_ctx, bool_t* p_isCorrect, cb_crc32_msgd f_Crc, void* p_cbCrcCtx)
{
    e_eFSP_MSGD_Res l_result;
    s_eCU_BUNSTF_Res l_byteUnstuffRes;
    uint32_t l_dPayToRx;
	uint32_t l_crcInMsg;
	uint32_t l_crcExp;
	uint32_t l_dataSize;
    bool_t l_crcRes;
	uint8_t* lp_data;

    /* Check NULL */
    if( ( NULL == p_ctx ) || ( NULL == p_isCorrect) || ( NULL == f_Crc) || ( NULL == p_cbCrcCtx ) )
    {
        l_result = MSGD_RES_BADPOINTER;
    }
    else
    {
        /* Ok the frame is complete, need to check if we have data size, data crc, crc rigth value */
        /* Init value */
        l_dataSize = 0u;
        lp_data = NULL;

        /* Get unstuffed data */
        l_byteUnstuffRes = eCU_BUNSTF_GetUnstufData(p_ctx, &lp_data, &l_dataSize);
        l_result = eFSP_MSGD_convertReturnFromBstf(l_byteUnstuffRes);

        if( MSGD_RES_OK == l_result )
        {
            /* Do we have enough data?  */
            if( l_dataSize < EFSP_MIN_MSGDE_BUFFLEN )
            {
                /* Too small frame, discharge */
                *p_isCorrect = false;
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
                    l_crcRes = (*(f_Crc))( p_cbCrcCtx, ECU_CRC_BASE_SEED, &lp_data[4u], l_dPayToRx + 4u,  &l_crcExp );

                    if( true == l_crcRes )
                    {
                        if( l_crcInMsg == l_crcExp )
                        {
                            /* All ok */
                            *p_isCorrect = true;
                        }
                        else
                        {
                            /* Data Crc is wrong, discharge */
                            *p_isCorrect = false;
                        }
                    }
                    else
                    {
                        l_result = MSGD_RES_CRCCLBKERROR;
                    }
                }
                else
                {
                    /* Data len is wrong, discharge */
                    *p_isCorrect = false;
                }
            }
        }
    }

    return l_result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-8.13"
#endif

static e_eFSP_MSGD_Res eFSP_MSGD_isMsgCohe(s_eCU_BUNSTF_Ctx* p_ctx, bool_t* p_isCoherent)
{
    /* Need to check coherence of the message during message receiving, how? Check if data len reported by payload
     * is lower than data payload received, if greater something is wrong  */
    e_eFSP_MSGD_Res l_result;
    s_eCU_BUNSTF_Res l_byteUnstuffRes;
    uint32_t l_dPayToRx;
	uint32_t l_dataSize;
	uint8_t* lp_data;

    /* Check NULL */
    if( ( NULL == p_ctx ) || ( NULL == p_isCoherent) )
    {
        l_result = MSGD_RES_BADPOINTER;
    }
    else
    {
        /* Check how much payload is received */
        /* Init value */
        l_dataSize = 0u;
        lp_data = NULL;

        /* Get unstuffed data */
        l_byteUnstuffRes = eCU_BUNSTF_GetUnstufData(p_ctx, &lp_data, &l_dataSize);
        l_result = eFSP_MSGD_convertReturnFromBstf(l_byteUnstuffRes);

        if( MSGD_RES_OK == l_result )
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