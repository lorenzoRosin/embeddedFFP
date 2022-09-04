/**
 * @file eFSPCircularQueue.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPCircularQueue.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t isQueueStatusStillCoherent(const s_eFSP_circQCtx* ctx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_Res circQInitCtx(s_eFSP_circQCtx* const ctx, uint8_t* memPool, const uint32_t memPoolSize)
{
	/* Local variable */
	e_eFSP_Res result;

	/* Check pointer validity */
	if( ( NULL == ctx) || ( NULL ==  memPool) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check data validity */
		if( memPoolSize <= 0u )
		{
			result = ECU_RES_BADPARAM;
		}
		else
		{
			/* Check Init */
			ctx->isInit = true;
			ctx->memPool = memPool;
			ctx->memPoolSize = memPoolSize;
			ctx->memPoolUsedSize = 0u;
			ctx->memPoolFrstFreeIdx = 0u;
			ctx->memPoolFrstOccIdx = 0u;

			result = ECU_RES_OK;
		}
    }

	return result;
}

e_eFSP_Res circQReset(s_eFSP_circQCtx* const ctx)
{
	/* Local variable */
	e_eFSP_Res result;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == ctx->isInit )
		{
			result = ECU_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == isQueueStatusStillCoherent(ctx) )
            {
                result = ECU_RES_BADPARAM;
            }
            else
            {
                /* Update index in order to discharge all saved data */
                ctx->memPoolUsedSize = 0u;
                ctx->memPoolFrstFreeIdx = 0u;
                ctx->memPoolFrstOccIdx = 0u;
                result = ECU_RES_OK;
            }
		}
    }

	return result;
}

e_eFSP_Res circQGetFreeSapce(s_eFSP_circQCtx* const ctx, uint32_t* const freeSpace)
{
	/* Local variable */
	e_eFSP_Res result;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == freeSpace ) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == ctx->isInit )
		{
			result = ECU_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == isQueueStatusStillCoherent(ctx) )
            {
                result = ECU_RES_BADPARAM;
            }
			else
			{
				*freeSpace = ctx->memPoolSize - ctx->memPoolUsedSize;
				result = ECU_RES_OK;
			}
		}
    }

	return result;
}

e_eFSP_Res circQGetOccupiedSapce(s_eFSP_circQCtx* const ctx, uint32_t* const usedSpace)
{
	/* Local variable */
	e_eFSP_Res result;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == usedSpace ) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == ctx->isInit )
		{
			result = ECU_RES_NOINITLIB;
		}
		else
		{
            /* Check data coherence */
            if( false == isQueueStatusStillCoherent(ctx) )
            {
                result = ECU_RES_BADPARAM;
            }
			else
			{
				*usedSpace = ctx->memPoolUsedSize;
				result = ECU_RES_OK;
			}
		}
    }

	return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-17.4_b"
    /* Suppressed for code clarity */
#endif

e_eFSP_Res circQInsertData(s_eFSP_circQCtx* const ctx, const uint8_t* data, const uint32_t datalen)
{
	/* Local variable */
	e_eFSP_Res result;
	uint32_t freeSpace;
	uint32_t firstTranshLen;
	uint32_t secondTranshLen;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == data ) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == ctx->isInit )
		{
			result = ECU_RES_NOINITLIB;
		}
		else
		{
			/* Check data validity */
			if( datalen <= 0u )
			{
				result = ECU_RES_BADPARAM;
			}
			else
			{
                /* Check data coherence */
                if( false == isQueueStatusStillCoherent(ctx) )
                {
                    result = ECU_RES_BADPARAM;
                }
                else
                {
                    /* Check for free memory */
                    result = circQGetFreeSapce(ctx, &freeSpace);

                    if( ECU_RES_OK == result )
                    {
                        if( datalen > freeSpace )
                        {
                            /* No memory avaiable */
                            result = ECU_RES_OUTOFMEM;
                        }
                        else
                        {
                            /* Can insert data */
                            if( ( datalen +  ctx->memPoolFrstFreeIdx ) <= ctx->memPoolSize )
                            {
                                /* Direct copy */
                                (void)memcpy(&ctx->memPool[ctx->memPoolFrstFreeIdx], data, datalen);

                                /* Update free index */
                                ctx->memPoolFrstFreeIdx += datalen;
                                if(ctx->memPoolFrstFreeIdx >= ctx->memPoolSize)
                                {
                                    ctx->memPoolFrstFreeIdx = 0u;
                                }
                            }
                            else
                            {
                                /* Multicopy */

                                /* First round */
                                firstTranshLen = ctx->memPoolSize - ctx->memPoolFrstFreeIdx;
                                (void)memcpy(&ctx->memPool[ctx->memPoolFrstFreeIdx], data, firstTranshLen);
                                ctx->memPoolFrstFreeIdx = 0u;

                                /* Second round */
                                secondTranshLen = datalen - firstTranshLen;
                                (void)memcpy(&ctx->memPool[ctx->memPoolFrstFreeIdx], &data[firstTranshLen], secondTranshLen);
                                ctx->memPoolFrstFreeIdx += secondTranshLen;
                            }

                            ctx->memPoolUsedSize += datalen;
                            result = ECU_RES_OK;
                        }
                    }
                }
			}
		}
    }

	return result;
}

e_eFSP_Res circQRetriveData(s_eFSP_circQCtx* const ctx, uint8_t* const data, const uint32_t datalen)
{
	/* Local variable */
	e_eFSP_Res result;
	uint32_t usedSpace;
	uint32_t firstTranshLen;
	uint32_t secondTranshLen;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == data ) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == ctx->isInit )
		{
			result = ECU_RES_NOINITLIB;
		}
		else
		{
			/* Check data validity an queue integrity */
			if( datalen <= 0u )
			{
				result = ECU_RES_BADPARAM;
			}
			else
			{
                /* Check data coherence */
                if( false == isQueueStatusStillCoherent(ctx) )
                {
                    result = ECU_RES_BADPARAM;
                }
                else
                {
                    result = circQGetOccupiedSapce(ctx, &usedSpace);
                    if( ECU_RES_OK == result )
                    {
                        if( datalen > usedSpace)
                        {
                            /* No enoght data in the queue */
                            result = ECU_RES_OUTOFMEM;
                        }
                        else
                        {
                            /* Can retrive data */
                            if( ( datalen +  ctx->memPoolFrstOccIdx ) <= ctx->memPoolSize )
                            {
                                /* Direct copy */
                                (void)memcpy(data, &ctx->memPool[ctx->memPoolFrstOccIdx], datalen);

                                /* Update used index */
                                ctx->memPoolFrstOccIdx += datalen;
                                if(ctx->memPoolFrstOccIdx >= ctx->memPoolSize)
                                {
                                    ctx->memPoolFrstOccIdx = 0u;
                                }
                            }
                            else
                            {
                                /* Multicopy */

                                /* First round */
                                firstTranshLen = ctx->memPoolSize - ctx->memPoolFrstOccIdx;
                                (void)memcpy(data, &ctx->memPool[ctx->memPoolFrstOccIdx], firstTranshLen);
                                ctx->memPoolFrstOccIdx = 0u;

                                /* Second round */
                                secondTranshLen = datalen - firstTranshLen;
                                (void)memcpy(&data[firstTranshLen], &ctx->memPool[ctx->memPoolFrstOccIdx], secondTranshLen);
                                ctx->memPoolFrstOccIdx += secondTranshLen;
                            }

                            ctx->memPoolUsedSize -= datalen;
                            result = ECU_RES_OK;
                        }
                    }
                }
			}
		}
    }

	return result;
}

e_eFSP_Res circQPeekData(s_eFSP_circQCtx* const ctx, uint8_t* const data, const uint32_t datalen)
{
	/* Local variable */
	e_eFSP_Res result;
	uint32_t usedSpace;
	uint32_t firstTranshLen;
	uint32_t secondTranshLen;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == data ) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == ctx->isInit )
		{
			result = ECU_RES_NOINITLIB;
		}
		else
		{
			/* Check data validity an queue integrity */
			if( datalen <= 0u )
			{
				result = ECU_RES_BADPARAM;
			}
			else
			{
                /* Check data coherence */
                if( false == isQueueStatusStillCoherent(ctx) )
                {
                    result = ECU_RES_BADPARAM;
                }
                else
                {
                    result = circQGetOccupiedSapce(ctx, &usedSpace);
                    if( ECU_RES_OK == result )
                    {
                        if( datalen > usedSpace)
                        {
                            /* No enoght data in the queue */
                            result = ECU_RES_OUTOFMEM;
                        }
                        else
                        {
                            /* Can retrive data */
                            if( ( datalen +  ctx->memPoolFrstOccIdx ) <= ctx->memPoolSize )
                            {
                                /* Direct copy */
                                (void)memcpy(data, &ctx->memPool[ctx->memPoolFrstOccIdx], datalen);
                            }
                            else
                            {
                                /* Multicopy */

                                /* First round */
                                firstTranshLen = ctx->memPoolSize - ctx->memPoolFrstOccIdx;
                                (void)memcpy(data, &ctx->memPool[ctx->memPoolFrstOccIdx], firstTranshLen);

                                /* Second round */
                                secondTranshLen = datalen - firstTranshLen;
                                (void)memcpy(&data[firstTranshLen], &ctx->memPool[0u], secondTranshLen);
                            }

                            result = ECU_RES_OK;
                        }
                    }
                }
			}
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
bool_t isQueueStatusStillCoherent(const s_eFSP_circQCtx* ctx)
{
    bool_t result;

	/* Check context validity */
	if( ( ctx->memPoolSize <= 0u ) || ( NULL == ctx->memPool ) )
	{
		result = false;
	}
	else
	{
		/* Check queue data coherence */
		if( ( ctx->memPoolFrstFreeIdx >= ctx->memPoolSize ) || ( ctx->memPoolFrstOccIdx >= ctx->memPoolSize ) ||
		    ( ctx->memPoolSize < ctx->memPoolUsedSize ) )
		{
			result = false;
		}
		else
		{
            result = true;
		}
	}

    return result;
}