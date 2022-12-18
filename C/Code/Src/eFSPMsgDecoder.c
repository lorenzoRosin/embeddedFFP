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
static e_eFSP_MSGD_Res eFSP_MSGD_isMsgCorr(s_eCU_BUNSTF_Ctx* p_ctx, bool_t* isCorrect, cb_crc32_msgd cbCrcPtr, void* cbCrcCtx);
static e_eFSP_MSGD_Res eFSP_MSGD_isMsgCohe(s_eCU_BUNSTF_Ctx* p_ctx, bool_t* isCoherent);
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
	e_eFSP_MSGD_Res result;
	s_eCU_BUNSTF_Res resByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == a_memArea ) || ( NULL == f_Crc ) || ( NULL == p_clbCtx ) )
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
            p_ctx->cbCrcPtr = f_Crc;
            p_ctx->cbCrcCtx = p_clbCtx;

			/* initialize internal bytestuffer */
			resByStuff =  eCU_BUNSTF_InitCtx(&p_ctx->byteUStufferCtnx, a_memArea, memAreaSize);
			result = eFSP_MSGD_convertReturnFromBstf(resByStuff);
        }
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2012-Rule-10.3"
#endif

e_eFSP_MSGD_Res eFSP_MSGD_IsInit(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* p_isInit)
{
	/* Local variable */
	e_eFSP_MSGD_Res result;
	s_eCU_BUNSTF_Res resByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_isInit ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
        resByStuff = eCU_BUNSTF_IsInit(&p_ctx->byteUStufferCtnx, p_isInit);
        result = eFSP_MSGD_convertReturnFromBstf(resByStuff);
	}

	return result;
}

e_eFSP_MSGD_Res eFSP_MSGD_NewMsg(s_eFSP_MSGD_Ctx* const p_ctx)
{
	/* Local variable */
	e_eFSP_MSGD_Res result;
	s_eCU_BUNSTF_Res resByStuff;

	/* Check pointer validity */
	if( NULL == p_ctx )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Update index */
			resByStuff = eCU_BUNSTF_NewFrame(&p_ctx->byteUStufferCtnx);
			result = eFSP_MSGD_convertReturnFromBstf(resByStuff);
		}
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MEM-stack-param", "MISRAC2004-17.6_d", "MISRAC2012-Rule-1.3_s", \
                            "MISRAC2012-Rule-18.6_d", "CERT-DCL30-C_e"
    /* Suppressed for code clarity */
#endif

e_eFSP_MSGD_Res eFSP_MSGD_GetDecodedData(s_eFSP_MSGD_Ctx* const p_ctx, uint8_t** pp_data, uint32_t* const p_retrivedLen)
{
	/* Local variable */
	e_eFSP_MSGD_Res result;
	s_eCU_BUNSTF_Res resByStuff;
	uint8_t* dataPP;
	uint32_t dataSizeP;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == pp_data ) || ( NULL == p_retrivedLen ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            dataSizeP = 0u;
            dataPP = NULL;
			resByStuff = eCU_BUNSTF_GetUnstufData(&p_ctx->byteUStufferCtnx, &dataPP, &dataSizeP);
			result = eFSP_MSGD_convertReturnFromBstf(resByStuff);

			if( MSGD_RES_OK == result )
			{
                /* Starting point is always the same */
                *pp_data = &dataPP[EFSP_MSGDE_HEADERSIZE];

				/* Removed HADER from data ( CRC + DATA LEN ) */
                if( dataSizeP < EFSP_MIN_MSGDE_BUFFLEN )
                {
					/* No memory free */
                    *p_retrivedLen = 0u;
                }
                else
                {
                    /* Return reference of only the raw payload */
                    *p_retrivedLen = dataSizeP - EFSP_MSGDE_HEADERSIZE;
                }

				result = MSGD_RES_OK;
			}
		}
	}

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MEM-stack-param", "MISRAC2004-17.6_d", "MISRAC2012-Rule-1.3_s", \
                            "MISRAC2012-Rule-18.6_d", "CERT-DCL30-C_e"
#endif

e_eFSP_MSGD_Res eFSP_MSGD_GetDecodedLen(s_eFSP_MSGD_Ctx* const p_ctx, uint32_t* const p_retrivedLen)
{
	/* Local variable */
	e_eFSP_MSGD_Res result;
	s_eCU_BUNSTF_Res resByStuff;
	uint32_t dataSizeP;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_retrivedLen ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			/* Get memory reference of CRC+LEN+DATA, so we can calculate reference of only data payload */
            dataSizeP = 0u;
			resByStuff = eCU_BUNSTF_GetUnstufLen(&p_ctx->byteUStufferCtnx, &dataSizeP);
			result = eFSP_MSGD_convertReturnFromBstf(resByStuff);

			if( MSGD_RES_OK == result )
			{
				/* Removed HADER from data ( CRC + DATA LEN ) */
                if( dataSizeP < EFSP_MIN_MSGDE_BUFFLEN )
                {
					/* No memory free */
                    *p_retrivedLen = 0u;
                }
                else
                {
                    /* Return reference of only the raw payload */
                    *p_retrivedLen = dataSizeP - EFSP_MSGDE_HEADERSIZE;
                }

				result = MSGD_RES_OK;
			}
		}
	}

	return result;
}

e_eFSP_MSGD_Res eFSP_MSGD_IsWaitingSof(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* const p_isWaitingSof)
{
	/* Local variable */
	e_eFSP_MSGD_Res result;
	s_eCU_BUNSTF_Res resByStuff;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_isWaitingSof ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
			resByStuff = eCU_BUNSTF_IsWaitingSof(&p_ctx->byteUStufferCtnx, p_isWaitingSof);
			result = eFSP_MSGD_convertReturnFromBstf(resByStuff);
		}
	}

	return result;
}

e_eFSP_MSGD_Res eFSP_MSGD_IsAFullMsgDecoded(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* const p_isMsgDec)
{
	/* Local variable */
	e_eFSP_MSGD_Res result;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_isMsgDec ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
            result = eFSP_MSGD_IsFullMsgDec(p_ctx, p_isMsgDec);
		}
	}

	return result;
}

e_eFSP_MSGD_Res eFSP_MSGD_IsFrameBad(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* const p_isFrameBad)
{
	/* Local variable */
	e_eFSP_MSGD_Res result;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_isFrameBad ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
            result = eFSP_MSGD_IsFrmBad(p_ctx, p_isFrameBad);
		}
	}

	return result;
}

e_eFSP_MSGD_Res eFSP_MSGD_GetMostEffDatLen(s_eFSP_MSGD_Ctx* const p_ctx, uint32_t* const p_mostEffPayload)
{
	/* Local variable */
	e_eFSP_MSGD_Res result;
	s_eCU_BUNSTF_Res resByStuff;
    bool_t isFullUnstuffed;
    bool_t p_isWaitingSof;
    bool_t p_isFrameBad;
    uint32_t dataSizeRemaings;
	uint32_t dPayRxed;
    uint32_t dataSizeRaw;
	uint8_t* dataPP;
    uint32_t dPayToRx;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == p_mostEffPayload ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
            /* Check if the frame is already ended */
            isFullUnstuffed = false;
            resByStuff = eCU_BUNSTF_IsAFullFrameUnstuff(&p_ctx->byteUStufferCtnx, &isFullUnstuffed);
            result = eFSP_MSGD_convertReturnFromBstf(resByStuff);

            if( MSGD_RES_OK == result )
            {
                if( true == isFullUnstuffed )
                {
                    /* Full unstuffed frame, could be wrong or rigth but no more data is needed */
                    *p_mostEffPayload = 0u;
                }
                else
                {
                    /* Check for error */
                    p_isFrameBad = true;
                    resByStuff = eCU_BUNSTF_IsFrameBad(&p_ctx->byteUStufferCtnx, &p_isFrameBad);
                    result = eFSP_MSGD_convertReturnFromBstf(resByStuff);

                    if( MSGD_RES_OK == result )
                    {
                        if( true == p_isFrameBad )
                        {
                            /* Bad frame only for unstuffer level, dont request more data */
                            *p_mostEffPayload = 0u;
                        }
                        else
                        {
                            /* How many byte do we have decoded? */
                            dataSizeRaw = 0u;
                            dataPP = NULL;
                            resByStuff = eCU_BUNSTF_GetUnstufData(&p_ctx->byteUStufferCtnx, &dataPP, &dataSizeRaw);
                            result = eFSP_MSGD_convertReturnFromBstf(resByStuff);

                            if( MSGD_RES_OK == result )
                            {
                                /* Do we have enough data?  */
                                if( 0u == dataSizeRaw )
                                {
                                    /* No data, are we still waiting SOF? */
                                    p_isWaitingSof = false;
                                    resByStuff = eCU_BUNSTF_IsWaitingSof(&p_ctx->byteUStufferCtnx, &p_isWaitingSof);
                                    result = eFSP_MSGD_convertReturnFromBstf(resByStuff);

                                    if( MSGD_RES_OK == result )
                                    {
                                        if( true == p_isWaitingSof )
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
                                    dPayToRx = eFSP_MSGD_composeU32LE(dataPP[0x04u], dataPP[0x05u], dataPP[0x06u], dataPP[0x07u]);

                                    /* How much payload do we have */
                                    dPayRxed = dataSizeRaw - EFSP_MSGDE_HEADERSIZE;

                                    /* A correct frame payload must have less lenght than the size reported in frame
                                     * header */
                                    if( dPayRxed <= dPayToRx )
                                    {
                                        dataSizeRemaings = dPayToRx - dPayRxed;

                                        /* Wait remaining data + EOF */
                                        if( dataSizeRemaings < MAX_UINT32VAL )
                                        {
                                            /* dataSizeRemaings != 0 -> need data + EOF
                                             * dataSizeRemaings == 0 -> need EOF . Infact we are sure the frame is not
                                             * ended because we already have called the function
                                             * BUNSTF_IsAFullFrameUnstuff */
                                            *p_mostEffPayload = dataSizeRemaings + 1u;
                                        }
                                        else
                                        {
                                            *p_mostEffPayload = dataSizeRemaings;
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

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-10.3"
    /* Suppressed for code clarity */
#endif

e_eFSP_MSGD_Res eFSP_MSGD_InsEncChunk(s_eFSP_MSGD_Ctx* const p_ctx, uint8_t a_encArea[], const uint32_t encLen,
                                 uint32_t* const p_consumedEncData)
{
	/* Local return  */
	e_eFSP_MSGD_Res result;
    e_eFSP_MSGD_Res resMsgCorrect;
    e_eFSP_MSGD_Res resultMsgCoherent;
	s_eCU_BUNSTF_Res resByStuff;

    /* Local coherence */
    bool_t isMCorrect;
    bool_t isMCoherent;

    /* Redo loop var */
    e_eFSP_MSGD_Priv_state insertEncState;

	/* Check pointer validity */
	if( ( NULL == p_ctx ) || ( NULL == a_encArea ) || ( NULL == p_consumedEncData ) )
	{
		result = MSGD_RES_BADPOINTER;
	}
	else
	{
		/* Check internal status validity */
		if( false == eFSP_MSGD_isStatusStillCoherent(p_ctx) )
		{
			result = MSGD_RES_CORRUPTCTX;
		}
		else
		{
            /* Init data */
            *p_consumedEncData = 0u;

            /* Elaborate */
            insertEncState = MSGD_PRV_PRECHECK;
            result = MSGD_RES_OK;

            while( insertEncState != MSGD_PRV_ELABDONE )
            {
                switch( insertEncState )
                {
                    case MSGD_PRV_PRECHECK:
                    {
                        /* Before inserting data check if the current message is coherent */
                        resultMsgCoherent = eFSP_MSGD_isMsgCohe(&p_ctx->byteUStufferCtnx, &isMCoherent);

                        if( MSGD_RES_OK == resultMsgCoherent )
                        {
                            /* no strange error found, check message correctness */
                            if( true != isMCoherent )
                            {
                                /* Message is not coherent */
                                insertEncState = MSGD_PRV_ELABDONE;
                                result = MSGD_RES_BADFRAME;
                            }
                            else
                            {
                                /* Message is correct but not ended */
                                insertEncState = MSGD_PRV_INSERTCHUNK;
                            }
                        }
                        else
                        {
                            /* Some error */
                            result = resultMsgCoherent;
                            insertEncState = MSGD_PRV_ELABDONE;
                        }

                        break;
                    }

                    case MSGD_PRV_INSERTCHUNK:
                    {
                        /* Insert data */
                        resByStuff = eCU_BUNSTF_InsStufChunk(&p_ctx->byteUStufferCtnx, a_encArea, encLen, p_consumedEncData);
                        result = eFSP_MSGD_convertReturnFromBstf(resByStuff);

                        if( MSGD_RES_MESSAGEENDED == result )
                        {
                            /* Check if ended correclty */
                            insertEncState = MSGD_PRV_MSG_END_CHECK;
                        }
                        else if( MSGD_RES_OK == result )
                        {
                            /* Check if okied correclty */
                            insertEncState = MSGD_PRV_MSG_OK_CHECK;
                        }
                        else if( MSGD_RES_BADFRAME == result )
                        {
                            /* bad frame, return till somehome will restart elaboration */
                            insertEncState = MSGD_PRV_ELABDONE;
                        }
                        else if( MSGD_RES_FRAMERESTART == result )
                        {
                            /* Frame restarted, return */
                            insertEncState = MSGD_PRV_ELABDONE;
                        }
                        else
                        {
                            /* Some error in unstuffer process, return it */
                            insertEncState = MSGD_PRV_ELABDONE;
                        }

                        break;
                    }

                    case MSGD_PRV_MSG_END_CHECK:
                    {
                        /* Verify message integrity */
                        resMsgCorrect = eFSP_MSGD_isMsgCorr(&p_ctx->byteUStufferCtnx, &isMCorrect, p_ctx->cbCrcPtr, p_ctx->cbCrcCtx);

                        if( MSGD_RES_OK == resMsgCorrect )
                        {
                            /* no strange error found, check message correctness */
                            if( true != isMCorrect )
                            {
                                /* Message ended but something about CRC or length is not rigth, restart a frame */
                                insertEncState = MSGD_PRV_ELABDONE;
                                result = MSGD_RES_BADFRAME;
                            }
                            else
                            {
                                /* Correct message */
                                insertEncState = MSGD_PRV_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            result = resMsgCorrect;
                            insertEncState = MSGD_PRV_ELABDONE;
                        }

                        break;
                    }

                    case MSGD_PRV_MSG_OK_CHECK:
                    {
                        /* Still parsing but we can check if data len is coherent */
                        resultMsgCoherent = eFSP_MSGD_isMsgCohe(&p_ctx->byteUStufferCtnx, &isMCoherent);

                        if( MSGD_RES_OK == resultMsgCoherent )
                        {
                            /* no strange error found, check message correctness */
                            if( true != isMCoherent )
                            {
                                insertEncState = MSGD_PRV_ELABDONE;
                                result = MSGD_RES_BADFRAME;
                            }
                            else
                            {
                                /* Message is correct but not ended, can return */
                                insertEncState = MSGD_PRV_ELABDONE;
                            }
                        }
                        else
                        {
                            /* Some error */
                            result = resultMsgCoherent;
                            insertEncState = MSGD_PRV_ELABDONE;
                        }
                        break;
                    }

                    default:
                    {
                        /* Impossible end here */
                        insertEncState = MSGD_PRV_ELABDONE;
                        result = MSGD_RES_CORRUPTCTX;
                        break;
                    }
                }
            }
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
static bool_t eFSP_MSGD_isStatusStillCoherent(const s_eFSP_MSGD_Ctx* p_ctx)
{
    bool_t result;

	/* Check context validity */
	if( ( NULL == p_ctx->cbCrcPtr ) || ( NULL == p_ctx->cbCrcCtx ) )
	{
		result = false;
	}
	else
	{
		result = true;
	}

    return result;
}

static e_eFSP_MSGD_Res eFSP_MSGD_convertReturnFromBstf(s_eCU_BUNSTF_Res returnedEvent)
{
	e_eFSP_MSGD_Res result;

	switch( returnedEvent )
	{
		case BUNSTF_RES_OK:
		{
			result = MSGD_RES_OK;
            break;
		}

		case BUNSTF_RES_BADPARAM:
		{
			result = MSGD_RES_BADPARAM;
            break;
		}

		case BUNSTF_RES_BADPOINTER:
		{
			result = MSGD_RES_BADPOINTER;
            break;
		}

		case BUNSTF_RES_CORRUPTCTX:
		{
			result = MSGD_RES_CORRUPTCTX;
            break;
		}

		case BUNSTF_RES_OUTOFMEM:
		{
			result = MSGD_RES_OUTOFMEM;
            break;
		}

		case BUNSTF_RES_BADFRAME:
		{
			result = MSGD_RES_BADFRAME;
            break;
		}

		case BUNSTF_RES_FRAMEENDED:
		{
			result = MSGD_RES_MESSAGEENDED;
            break;
		}

		case BUNSTF_RES_FRAMERESTART:
		{
			result = MSGD_RES_FRAMERESTART;
            break;
		}

		case BUNSTF_RES_NOINITLIB:
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

static e_eFSP_MSGD_Res eFSP_MSGD_IsFullMsgDec(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* const p_isMsgDec)
{
	/* Local variable */
	e_eFSP_MSGD_Res result;
	s_eCU_BUNSTF_Res resByStuff;
    bool_t isFullUnstuffed;

    isFullUnstuffed = false;
    resByStuff = eCU_BUNSTF_IsAFullFrameUnstuff(&p_ctx->byteUStufferCtnx, &isFullUnstuffed);
    result = eFSP_MSGD_convertReturnFromBstf(resByStuff);

    if( MSGD_RES_OK == result )
    {
        if( false == isFullUnstuffed )
        {
            /* Not full unstuffed so itisnt full decoded. Even if the a bad frame is received its not
             * fully decoded */
            *p_isMsgDec = false;
        }
        else
        {
            /* Full frame received at unstuffer level, check if it is valid message at this level */
            result = eFSP_MSGD_isMsgCorr(&p_ctx->byteUStufferCtnx, p_isMsgDec, p_ctx->cbCrcPtr, p_ctx->cbCrcCtx);
        }
    }

	return result;
}

static e_eFSP_MSGD_Res eFSP_MSGD_IsFrmBad(s_eFSP_MSGD_Ctx* const p_ctx, bool_t* const p_isFrameBad)
{
	/* Local variable */
	e_eFSP_MSGD_Res result;
	s_eCU_BUNSTF_Res resByStuff;
    bool_t isFullUnstuffed;
    bool_t isCorrect;
    bool_t isFrameBadloc;

    isFrameBadloc = true;
    resByStuff = eCU_BUNSTF_IsFrameBad(&p_ctx->byteUStufferCtnx, &isFrameBadloc);
    result = eFSP_MSGD_convertReturnFromBstf(resByStuff);

    if( MSGD_RES_OK == result )
    {
        if( true == isFrameBadloc )
        {
            /* Not correct at bytestuffer level, no need to check CRC or data len */
            *p_isFrameBad = true;
        }
        else
        {
            /* Frame seems ok, no error found. It could be ended on in receiving state. If ended check CRC,
             * if still in receiving state check coherence with datalen */
            isFullUnstuffed = false;
            resByStuff = eCU_BUNSTF_IsAFullFrameUnstuff(&p_ctx->byteUStufferCtnx, &isFullUnstuffed);
            result = eFSP_MSGD_convertReturnFromBstf(resByStuff);

            if( MSGD_RES_OK == result )
            {
                if( false == isFullUnstuffed )
                {
                    /* Check ongoing coherence */
                    result = eFSP_MSGD_isMsgCohe(&p_ctx->byteUStufferCtnx, &isCorrect);

                    if( MSGD_RES_OK == result )
                    {
                        /* no strange error found, check message correctness */
                        if( true != isCorrect )
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
                    result = eFSP_MSGD_isMsgCorr(&p_ctx->byteUStufferCtnx, &isCorrect, p_ctx->cbCrcPtr, p_ctx->cbCrcCtx);

                    if( MSGD_RES_OK == result )
                    {
                        /* no strange error found, check message correctness */
                        if( true != isCorrect )
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

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2012-Rule-8.13"
    /* Suppressed for code clarity */
#endif

static e_eFSP_MSGD_Res eFSP_MSGD_isMsgCorr(s_eCU_BUNSTF_Ctx* p_ctx, bool_t* isCorrect, cb_crc32_msgd cbCrcPtr, void* cbCrcCtx)
{
    e_eFSP_MSGD_Res result;
    s_eCU_BUNSTF_Res byteUnstuffRes;
    uint32_t dPayToRx;
	uint32_t crcInMsg;
	uint32_t crcExp;
	uint32_t dataSizeP;
    bool_t crcRes;
	uint8_t* dataPP;

    /* Check NULL */
    if( ( NULL == p_ctx ) || ( NULL == isCorrect) || ( NULL == cbCrcPtr) || ( NULL == cbCrcCtx ) )
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
        byteUnstuffRes = eCU_BUNSTF_GetUnstufData(p_ctx, &dataPP, &dataSizeP);
        result = eFSP_MSGD_convertReturnFromBstf(byteUnstuffRes);

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
                dPayToRx = eFSP_MSGD_composeU32LE(dataPP[0x04u], dataPP[0x05u], dataPP[0x06u], dataPP[0x07u]);

                if( ( dataSizeP - EFSP_MSGDE_HEADERSIZE ) == dPayToRx )
                {
                    /* Data len is coherent! Is crc rigth? */
                    crcExp = 0u;

                    /* Estrapolate CRC in Little Endian */
                    crcInMsg = eFSP_MSGD_composeU32LE(dataPP[0x00u], dataPP[0x01u], dataPP[0x02u], dataPP[0x03u]);

                    /* Calculate CRC */
                    crcRes = (*(cbCrcPtr))( cbCrcCtx, ECU_CRC_BASE_SEED, &dataPP[4u], dPayToRx + 4u,  &crcExp );

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
    #pragma cstat_restore = "MISRAC2012-Rule-8.13"
#endif

static e_eFSP_MSGD_Res eFSP_MSGD_isMsgCohe(s_eCU_BUNSTF_Ctx* p_ctx, bool_t* isCoherent)
{
    /* Need to check coherence of the message during message receiving, how? Check if data len reported by payload
     * is lower than data payload received, if greater something is wrong  */
    e_eFSP_MSGD_Res result;
    s_eCU_BUNSTF_Res byteUnstuffRes;
    uint32_t dPayToRx;
	uint32_t dataSizeP;
	uint8_t* dataPP;

    /* Check NULL */
    if( ( NULL == p_ctx ) || ( NULL == isCoherent) )
    {
        result = MSGD_RES_BADPOINTER;
    }
    else
    {
        /* Check how much payload is received */
        /* Init value */
        dataSizeP = 0u;
        dataPP = NULL;

        /* Get unstuffed data */
        byteUnstuffRes = eCU_BUNSTF_GetUnstufData(p_ctx, &dataPP, &dataSizeP);
        result = eFSP_MSGD_convertReturnFromBstf(byteUnstuffRes);

        if( MSGD_RES_OK == result )
        {
            /* Do we have enough data?  */
            if( dataSizeP < EFSP_MSGDE_HEADERSIZE )
            {
                /* Not enoght data to make a check */
                *isCoherent = true;
            }
            else
            {
                /* Enough data! Is data len in frame coherent?  */
                dPayToRx = eFSP_MSGD_composeU32LE(dataPP[0x04u], dataPP[0x05u], dataPP[0x06u], dataPP[0x07u]);

                if( ( dataSizeP - EFSP_MSGDE_HEADERSIZE ) <= dPayToRx )
                {
                    /* Data len is coherent! */
                    *isCoherent = true;
                }
                else
                {
                    /* Data len is wrong, discharge */
                    *isCoherent = false;
                }
            }
        }
    }

    return result;
}

static uint32_t eFSP_MSGD_composeU32LE(uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4)
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