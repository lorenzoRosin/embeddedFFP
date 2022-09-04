/**
 * @file eFSPDataUnPack.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPDataUnPack.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t isUnPackStatusStillCoherent(const s_eFSP_DataUnPackCtx* ctx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_Res dataUnPackinitCtx(s_eFSP_DataUnPackCtx* const ctx, uint8_t* const memUPKA, const uint32_t memUPKASize,
                         const bool_t isLEnd)
{
	/* Local variable */
	e_eFSP_Res result;

	/* Check pointer validity */
	if( ( NULL == ctx) || ( NULL ==  memUPKA) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check data validity */
		if( memUPKASize <= 0u )
		{
			result = ECU_RES_BADPARAM;
		}
		else
		{
            ctx->isInit = true;
            ctx->isLE = isLEnd;
            ctx->memUPKA = memUPKA;
            ctx->memUPKASize = memUPKASize;
            ctx->memUPKACntr = 0u;

            result = ECU_RES_OK;
		}
    }

	return result;
}

e_eFSP_Res dataUnPackReset(s_eFSP_DataUnPackCtx* const ctx)
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
			/* Update index */
			ctx->memUPKACntr = 0u;

			result = ECU_RES_OK;
		}
    }

	return result;
}

e_eFSP_Res dataUnPackGetRemToPop(s_eFSP_DataUnPackCtx* const ctx, uint32_t* const retrivedLen)
{
	/* Local variable */
	e_eFSP_Res result;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == retrivedLen ) )
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
			/* Check internal status validity */
			if( false == isUnPackStatusStillCoherent(ctx) )
			{
				/* We have removed more data that we had */
				result = ECU_RES_BADPARAM;
			}
			else
			{
				*retrivedLen = ctx->memUPKASize - ctx->memUPKACntr;
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

e_eFSP_Res dataUnPackPopArray(s_eFSP_DataUnPackCtx* const ctx, uint8_t* const dataDest, uint32_t const retrivedLen)
{
	/* Local variable */
	e_eFSP_Res result;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == dataDest ) )
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
            if( retrivedLen <= 0u )
            {
                /* We have removed more data that we had */
                result = ECU_RES_BADPARAM;
            }
            else
            {
                /* Check internal status validity */
                if( false == isUnPackStatusStillCoherent(ctx) )
                {
                    result = ECU_RES_BADPARAM;
                }
                else
                {
                    /* Check if we can pop that amount */
                    if( ( ctx->memUPKACntr + retrivedLen ) > ctx->memUPKASize )
                    {
                        result = ECU_RES_OUTOFMEM;
                    }
                    else
                    {
                        /* Copy data */
                        (void)memcpy(dataDest, &ctx->memUPKA[ctx->memUPKACntr], retrivedLen);

                        /* Update index */
                        ctx->memUPKACntr += retrivedLen;
                        result = ECU_RES_OK;
                    }
                }
            }
		}
    }

	return result;
}

e_eFSP_Res dataUnPackPopU8(s_eFSP_DataUnPackCtx* const ctx, uint8_t *dataP)
{
	/* Local variable */
	e_eFSP_Res result;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == dataP ) )
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
            /* Check internal status validity */
            if( false == isUnPackStatusStillCoherent(ctx) )
            {
                result = ECU_RES_BADPARAM;
            }
			else
			{
				/* Check if we can pop that amount */
				if( ( ctx->memUPKACntr + sizeof(uint8_t) ) > ctx->memUPKASize )
				{
					result = ECU_RES_OUTOFMEM;
				}
				else
				{
					/* Copy data */
					*dataP = ctx->memUPKA[ctx->memUPKACntr];

					/* Update index */
					ctx->memUPKACntr++;
					result = ECU_RES_OK;
				}
			}
		}
    }

	return result;
}

e_eFSP_Res dataUnPackPopU16(s_eFSP_DataUnPackCtx* const ctx, uint16_t* dataP)
{
	/* Local variable */
	e_eFSP_Res result;
    uint16_t tempS;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == dataP ) )
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
            /* Check internal status validity */
            if( false == isUnPackStatusStillCoherent(ctx) )
            {
                result = ECU_RES_BADPARAM;
            }
			else
			{
				/* Check if we can pop that amount */
				if( ( ctx->memUPKACntr + sizeof(uint16_t) ) > ctx->memUPKASize )
				{
					result = ECU_RES_OUTOFMEM;
				}
				else
				{
                    *dataP = 0u;

					if( true == ctx->isLE)
					{
                        /* Copy data Little endian */
                        tempS = (uint16_t) ctx->memUPKA[ctx->memUPKACntr];
                        *dataP |= ( tempS & 0x00FFu );
                        ctx->memUPKACntr++;

                        tempS =  (uint16_t) ( ( (uint16_t) ctx->memUPKA[ctx->memUPKACntr] ) << 8u  );
						*dataP |= ( tempS & 0xFF00u );
						ctx->memUPKACntr++;
					}
					else
					{
						/* Copy data big endian */
                        tempS =  (uint16_t) ( ( (uint16_t) ctx->memUPKA[ctx->memUPKACntr] ) << 8u  );
						*dataP |= ( tempS & 0xFF00u );
						ctx->memUPKACntr++;

                        tempS = (uint16_t) ctx->memUPKA[ctx->memUPKACntr];
                        *dataP |= ( tempS & 0x00FFu );
                        ctx->memUPKACntr++;
					}

					result = ECU_RES_OK;
				}
			}
		}
    }

	return result;
}

e_eFSP_Res dataUnPackPopU32(s_eFSP_DataUnPackCtx* const ctx, uint32_t* dataP)
{
	/* Local variable */
	e_eFSP_Res result;
    uint32_t tempS;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == dataP ) )
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
            /* Check internal status validity */
            if( false == isUnPackStatusStillCoherent(ctx) )
            {
                result = ECU_RES_BADPARAM;
            }
			else
			{
				/* Check if we can pop that amount */
				if( ( ctx->memUPKACntr + sizeof(uint32_t) ) > ctx->memUPKASize )
				{
					result = ECU_RES_OUTOFMEM;
				}
				else
				{
					*dataP = 0u;

					if( true == ctx->isLE)
					{
                        /* Copy data Little endian */
                        tempS = (uint32_t) ctx->memUPKA[ctx->memUPKACntr];
                        *dataP |= ( tempS & 0x000000FFu );
                        ctx->memUPKACntr++;

                        tempS =  (uint32_t) ( ( (uint32_t) ctx->memUPKA[ctx->memUPKACntr] ) << 8u  );
						*dataP |= ( tempS & 0x0000FF00u );
						ctx->memUPKACntr++;

                        tempS =  (uint32_t) ( ( (uint32_t) ctx->memUPKA[ctx->memUPKACntr] ) << 16u  );
						*dataP |= ( tempS & 0x00FF0000u );
						ctx->memUPKACntr++;

                        tempS =  (uint32_t) ( ( (uint32_t) ctx->memUPKA[ctx->memUPKACntr] ) << 24u  );
						*dataP |= ( tempS & 0xFF000000u );
						ctx->memUPKACntr++;
					}
					else
					{
						/* Copy data big endian */
                        tempS =  (uint32_t) ( ( (uint32_t) ctx->memUPKA[ctx->memUPKACntr] ) << 24u  );
						*dataP |= ( tempS & 0xFF000000u );
						ctx->memUPKACntr++;

                        tempS =  (uint32_t) ( ( (uint32_t) ctx->memUPKA[ctx->memUPKACntr] ) << 16u  );
                        *dataP |= ( tempS & 0x00FF0000u );
                        ctx->memUPKACntr++;

                        tempS =  (uint32_t) ( ( (uint32_t) ctx->memUPKA[ctx->memUPKACntr] ) << 8u  );
						*dataP |= ( tempS & 0x0000FF00u );
						ctx->memUPKACntr++;

                        tempS = (uint32_t) ctx->memUPKA[ctx->memUPKACntr];
                        *dataP |= ( tempS & 0x000000FFu );
                        ctx->memUPKACntr++;
					}

					result = ECU_RES_OK;
				}
			}
		}
	}

	return result;
}

e_eFSP_Res dataUnPackPopU64(s_eFSP_DataUnPackCtx* const ctx, uint64_t* dataP)
{
	/* Local variable */
	e_eFSP_Res result;
    uint64_t tempS;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == dataP ) )
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
            /* Check internal status validity */
            if( false == isUnPackStatusStillCoherent(ctx) )
            {
                result = ECU_RES_BADPARAM;
            }
			else
			{
				/* Check if we can pop that amount */
				if( ( ctx->memUPKACntr + sizeof(uint64_t) ) > ctx->memUPKASize )
				{
					result = ECU_RES_OUTOFMEM;
				}
				else
				{
					*dataP = 0u;

					if( true == ctx->isLE)
					{
                        /* Copy data Little endian */
                        tempS = (uint64_t) ctx->memUPKA[ctx->memUPKACntr];
                        *dataP |= ( tempS & 0x00000000000000FFUL );
                        ctx->memUPKACntr++;

                        tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 8u  );
						*dataP |= ( tempS & 0x000000000000FF00UL );
						ctx->memUPKACntr++;

                        tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 16u  );
						*dataP |= ( tempS & 0x0000000000FF0000UL );
						ctx->memUPKACntr++;

                        tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 24u  );
						*dataP |= ( tempS & 0x00000000FF000000UL );
						ctx->memUPKACntr++;

                        tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 32u  );
						*dataP |= ( tempS & 0x000000FF00000000UL );
						ctx->memUPKACntr++;

                        tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 40u  );
						*dataP |= ( tempS & 0x0000FF0000000000UL );
						ctx->memUPKACntr++;

                        tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 48u  );
						*dataP |= ( tempS & 0x00FF000000000000UL );
						ctx->memUPKACntr++;

                        tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 56u  );
						*dataP |= ( tempS & 0xFF00000000000000UL );
						ctx->memUPKACntr++;
					}
					else
					{
						/* Copy data big endian */
 						tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 56u  );
                        *dataP |= ( tempS & 0xFF00000000000000UL );
						ctx->memUPKACntr++;

                        tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 48u  );
                        *dataP |= ( tempS & 0x00FF000000000000UL );
                        ctx->memUPKACntr++;

						tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 40u  );
                        *dataP |= ( tempS & 0x0000FF0000000000UL );
						ctx->memUPKACntr++;

 						tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 32u  );
                        *dataP |= ( tempS & 0x000000FF00000000UL );
						ctx->memUPKACntr++;

                        tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 24u  );
						*dataP |= ( tempS & 0x00000000FF000000UL );
						ctx->memUPKACntr++;

                        tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 16u  );
                        *dataP |= ( tempS & 0x0000000000FF0000UL );
                        ctx->memUPKACntr++;

                        tempS =  (uint64_t) ( ( (uint64_t) ctx->memUPKA[ctx->memUPKACntr] ) << 8u  );
						*dataP |= ( tempS & 0x000000000000FF00UL );
						ctx->memUPKACntr++;

                        tempS = (uint64_t) ctx->memUPKA[ctx->memUPKACntr];
                        *dataP |= ( tempS & 0x00000000000000FFUL );
                        ctx->memUPKACntr++;
					}

					result = ECU_RES_OK;
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
bool_t isUnPackStatusStillCoherent(const s_eFSP_DataUnPackCtx* ctx)
{
    bool_t result;

	/* Check context validity */
	if( ( ctx->memUPKASize <= 0u ) || ( NULL == ctx->memUPKA ) )
	{
		result = false;
	}
	else
	{
		/* Check queue data coherence */
		if( ctx->memUPKACntr > ctx->memUPKASize )
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



