/**
 * @file eFSPByteStuffer.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPByteStuffer.h"



/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t isBSStatusStillCoherent(const e_eFSP_BStuffCtx* ctx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_Res bStufferInitCtx(e_eFSP_BStuffCtx* const ctx, uint8_t* const memArea, const uint32_t memAreaSize)
{
	/* Local variable */
	e_eFSP_Res result;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == memArea ) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
        /* Check data validity */
        if( memAreaSize <= 0u )
        {
            result = ECU_RES_BADPARAM;
        }
        else
        {
            /* Initialize internal status */
            ctx->isInit = true;
            ctx->memArea = memArea;
            ctx->memAreaSize = memAreaSize;
            ctx->memAreaCntr = 0u;
            ctx->precedentToCheck = false;
            ctx->needSof = true;
            ctx->needEof = true;
            result = ECU_RES_OK;
        }
	}

	return result;
}

e_eFSP_Res bStufferReset(e_eFSP_BStuffCtx* const ctx)
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
			ctx->memAreaCntr = 0u;
            ctx->precedentToCheck = false;
            ctx->needSof = true;
            ctx->needEof = true;
			result = ECU_RES_OK;
		}
	}

	return result;
}

e_eFSP_Res bStufferGetRemToStuf(e_eFSP_BStuffCtx* const ctx, uint32_t* const retrivedLen)
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
            if( false == isBSStatusStillCoherent(ctx) )
            {
                result = ECU_RES_BADPARAM;
            }
            else
            {
                if( false == ctx->precedentToCheck )
                {
                    *retrivedLen = ctx->memAreaSize - ctx->memAreaCntr;
                }
                else
                {
                    *retrivedLen = ctx->memAreaSize - ctx->memAreaCntr + 1u;
                }

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

e_eFSP_Res bStufferRetriStufChunk(e_eFSP_BStuffCtx* const ctx, uint8_t* const stuffedDest, const uint32_t maxDestLen,
                                  uint32_t* const filledLen)
{
	/* Local variable */
	e_eFSP_Res result;
    uint32_t nExamByte;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == stuffedDest ) || ( NULL == filledLen ) )
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
            if(maxDestLen <= 0u)
            {
                result = ECU_RES_BADPARAM;
            }
            else
            {
                /* Check internal status validity */
                if( false == isBSStatusStillCoherent(ctx) )
                {
                    result = ECU_RES_BADPARAM;
                }
                else
                {
                    /* Init counter */
                    nExamByte = 0u;

                    /* Detect start of frame here to maximize efficency */
                    if( true == ctx->needSof )
                    {
                        /* Start of frame */
                        stuffedDest[nExamByte] = ECU_SOF;
                        nExamByte++;
                        ctx->needSof = false;
                    }

                    /* Execute parsing cycle */
                    while( ( nExamByte < maxDestLen ) && ( true == ctx->needEof ) )
                    {
                        if( true == ctx->precedentToCheck )
                        {
                            /* Something from an old iteration  */
                            stuffedDest[nExamByte] = ( (uint8_t) ~( ctx->memArea[ctx->memAreaCntr - 1u] ) );
                            ctx->precedentToCheck = false;
                            nExamByte++;
                        }
                        else if( ctx->memAreaCntr == ctx->memAreaSize )
                        {
                            /* End of frame */
                            stuffedDest[nExamByte] = ECU_EOF;
                            ctx->needEof = false;
                            nExamByte++;
                        }
                        else
                        {
                            /* Current iteration */
                            if( ECU_SOF == ctx->memArea[ctx->memAreaCntr] )
                            {
                                /* Stuff with escape */
                                stuffedDest[nExamByte] = ECU_ESC;
                                ctx->precedentToCheck = true;
                                nExamByte++;
                                ctx->memAreaCntr++;
                            }
                            else if( ECU_EOF == ctx->memArea[ctx->memAreaCntr] )
                            {
                                /* Stuff with escape */
                                stuffedDest[nExamByte] = ECU_ESC;
                                ctx->precedentToCheck = true;
                                nExamByte++;
                                ctx->memAreaCntr++;
                            }
                            else if( ECU_ESC == ctx->memArea[ctx->memAreaCntr] )
                            {
                                /* Stuff with escape */
                                stuffedDest[nExamByte] = ECU_ESC;
                                ctx->precedentToCheck = true;
                                nExamByte++;
                                ctx->memAreaCntr++;
                            }
                            else
                            {
                                /* Can insert data */
                                stuffedDest[nExamByte] = ctx->memArea[ctx->memAreaCntr];
                                nExamByte++;
                                ctx->memAreaCntr++;
                            }
                        }
                    }

                    /* Save counter */
                    *filledLen = nExamByte;

                    /* result? */
                    if( 0u == nExamByte )
                    {
                        /* Nothing more */
                        result = ECU_RES_OUTOFMEM;
                    }
                    else
                    {
                        result = ECU_RES_OK;
                    }
                }
            }
		}
	}

	return result;
}

/***********************************************************************************************************************
 *  PRIVATE FUNCTION
 **********************************************************************************************************************/
bool_t isBSStatusStillCoherent(const e_eFSP_BStuffCtx* ctx)
{
    bool_t result;
    uint8_t precedentByte;

	/* Check context validity */
	if( ( ctx->memAreaSize <= 0u ) || ( NULL == ctx->memArea ) || ( ctx->memAreaCntr > ctx->memAreaSize ) )
	{
		result = false;
	}
	else
	{
		/* Check data coherence */
		if( ( ( true == ctx->needSof ) && ( false == ctx->needEof ) ) ||
            ( ( false == ctx->needSof ) && ( false == ctx->needEof ) && ( ctx->memAreaCntr != ctx->memAreaSize ) ) ||
            ( ( true == ctx->needSof ) && ( true == ctx->precedentToCheck ) ) ||
            ( ( false == ctx->needEof ) && ( true == ctx->precedentToCheck ) ) ||
            ( ( true == ctx->needSof ) && ( 0u != ctx->memAreaCntr ) ) )
		{
            result = false;
		}
		else
		{
            if( ( 0u == ctx->memAreaCntr ) && ( true == ctx->precedentToCheck ) )
            {
                result = false;
            }
            else
            {
                if( true == ctx->precedentToCheck )
                {
                    precedentByte = ctx->memArea[ctx->memAreaCntr - 1u];
                    if( (ECU_ESC != precedentByte) && (ECU_EOF != precedentByte) && (ECU_SOF != precedentByte) )
                    {
                        result = false;
                    }
                    else
                    {
                        result = true;
                    }
                }
                else
                {
                    result = true;
                }
            }
		}
	}

    return result;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-17.4_b"
#endif

