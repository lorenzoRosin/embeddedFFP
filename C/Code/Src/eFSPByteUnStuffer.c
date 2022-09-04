/**
 * @file eFSPByteUnStuffer.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPByteUnStuffer.h"
#include "eFSPByteStuffer.h"


/***********************************************************************************************************************
 *  PRIVATE STATIC FUNCTION DECLARATION
 **********************************************************************************************************************/
static bool_t isBUSStatusStillCoherent(const e_eFSP_BUStuffCtx* ctx);
static void errFoundRestart(e_eFSP_BUStuffCtx* const ctx);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSP_Res bUStufferInitCtx(e_eFSP_BUStuffCtx* const ctx, uint8_t* const memArea, const uint32_t memAreaSize)
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
            ctx->precedentWasEsc = false;
            ctx->needSof = true;
            ctx->needEof = true;
            result = ECU_RES_OK;
        }
	}

	return result;
}

e_eFSP_Res bUStufferReset(e_eFSP_BUStuffCtx* const ctx)
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
            ctx->precedentWasEsc = false;
            ctx->needSof = true;
            ctx->needEof = true;
			result = ECU_RES_OK;
		}
	}

	return result;
}

e_eFSP_Res bUStufferGetNUnstuf(e_eFSP_BUStuffCtx* const ctx, uint32_t* const retrivedLen)
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
            if( false == isBUSStatusStillCoherent(ctx) )
            {
                result = ECU_RES_BADPARAM;
            }
            else
            {
                *retrivedLen =  ctx->memAreaCntr;
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

e_eFSP_Res bUStufferInsStufChunk(e_eFSP_BUStuffCtx* const ctx, const uint8_t* stuffedArea, const uint32_t stuffLen,
                                  uint32_t* const consumedStuffData, bool_t* errSofRec, bool_t* eofRec)
{
	/* Local variable */
	e_eFSP_Res result;
    uint32_t nExamByte;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == stuffedArea ) || ( NULL == consumedStuffData )|| ( NULL == errSofRec ) ||
        ( NULL == eofRec ) )
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
            if(stuffLen <= 0u)
            {
                result = ECU_RES_BADPARAM;
            }
            else
            {
                /* Check internal status validity */
                if( false == isBUSStatusStillCoherent(ctx) )
                {
                    result = ECU_RES_BADPARAM;
                }
                else
                {
                    /* Init counter */
                    nExamByte = 0u;

                    /* Init var */
                    *errSofRec = false;
                    *eofRec = false;

                    /* Init result */
                    result = ECU_RES_OK;

                    /* Elab all data */
                    while( ( nExamByte < stuffLen ) && ( true == ctx->needEof ) &&
                           ( ctx->memAreaCntr <= ctx->memAreaSize ) && ( ECU_RES_OK == result ) )
                    {
                        if( true == ctx->needSof )
                        {
                            /* Wait SOF, discharge others */
                            if( ECU_SOF == stuffedArea[nExamByte] )
                            {
                                /* Found start */
                                errFoundRestart(ctx);
                                ctx->needSof = false;
                            }
                            else
                            {
                                /* Waiting for start, no other bytes */
                                errFoundRestart(ctx);
                                *errSofRec = true;
                            }
                            nExamByte++;
                        }
                        else
                        {
                            if( ECU_SOF == stuffedArea[nExamByte] )
                            {
                                /* Found start, but wasn't expected */
                                errFoundRestart(ctx);
                                ctx->needSof = false;
                                *errSofRec = true;

                                nExamByte++;
                            }
                            else if( ECU_EOF == stuffedArea[nExamByte] )
                            {
                                if( 0u == ctx->memAreaCntr )
                                {
                                    /* Found end, but no data received..  */
                                    errFoundRestart(ctx);
                                    *errSofRec = true;
                                }
                                else if( true == ctx->precedentWasEsc )
                                {
                                    /* Received eof but was expecting data..*/
                                    errFoundRestart(ctx);
                                    *errSofRec = true;
                                }
                                else
                                {
                                    /* Can close the frame, yey */
                                    ctx->needEof = false;
                                }

                                nExamByte++;
                            }
                            else if( ECU_ESC == stuffedArea[nExamByte] )
                            {
                                if( true == ctx->precedentWasEsc )
                                {
                                    /* Impossible receive two esc */
                                    errFoundRestart(ctx);
                                    *errSofRec = true;
                                    nExamByte++;
                                }
                                else if( ctx->memAreaCntr == ctx->memAreaSize )
                                {
                                    /* No more data avaiable to save that thing */
                                    result = ECU_RES_OUTOFMEM;
                                }
                                else
                                {
                                    /* Next data will be data */
                                    ctx->precedentWasEsc = true;
                                    nExamByte++;
                                }
                            }
                            else
                            {
                                /* Received good data */
                                if( ctx->memAreaCntr == ctx->memAreaSize )
                                {
                                    /* No more data avaiable to save that thing */
                                    result = ECU_RES_OUTOFMEM;
                                }
                                else if( true == ctx->precedentWasEsc )
                                {
                                    /* Is it true? */
                                    if( ( ECU_SOF == ( ( uint8_t ) ~stuffedArea[nExamByte] ) ) ||
                                        ( ECU_EOF == ( ( uint8_t ) ~stuffedArea[nExamByte] ) ) ||
                                        ( ECU_ESC == ( ( uint8_t ) ~stuffedArea[nExamByte] ) ) )
                                    {
                                        /* current data is neg */
                                        ctx->memArea[ctx->memAreaCntr] = ( uint8_t ) ( ~stuffedArea[nExamByte] );
                                        ctx->precedentWasEsc = false;
                                        ctx->memAreaCntr++;
                                        nExamByte++;
                                    }
                                    else
                                    {
                                        /* Impossible receive a data after esc that is not SOF EOF or ESC neg */
                                        errFoundRestart(ctx);
                                        *errSofRec = true;
                                        nExamByte++;
                                    }

                                }
                                else
                                {
                                    /* Only raw data */
                                    ctx->memArea[ctx->memAreaCntr] = stuffedArea[nExamByte];
                                    ctx->memAreaCntr++;
                                    nExamByte++;
                                }
                            }
                        }
                    }

                    /* Save the result */
                    *consumedStuffData = nExamByte;

                    if( false == ctx->needEof )
                    {
                        *eofRec = true;
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
bool_t isBUSStatusStillCoherent(const e_eFSP_BUStuffCtx* ctx)
{
    bool_t result;

	/* Check context validity */
	if( ( ctx->memAreaSize <= 0u ) || ( NULL == ctx->memArea ) || ( ctx->memAreaCntr > ctx->memAreaSize ) )
	{
		result = false;
	}
	else
	{
		/* Check data coherence */
		if( ( ( true == ctx->needSof ) && ( false == ctx->needEof ) ) ||
            ( ( true == ctx->needSof ) && ( true == ctx->precedentWasEsc ) ) ||
            ( ( false == ctx->needEof ) && ( true == ctx->precedentWasEsc ) ) )
		{
            result = false;
		}
		else
		{
            if( ( true == ctx->needSof ) && ( 0u != ctx->memAreaCntr ) )
            {
                result = false;
            }
            else
            {
                result = true;
            }
		}
	}

    return result;
}

void errFoundRestart(e_eFSP_BUStuffCtx* const ctx)
{
    /* Found start, but wasn't expected */
    ctx->memAreaCntr = 0u;
    ctx->precedentWasEsc = false;
    ctx->needSof = true;
    ctx->needEof = true;
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-17.4_b"
#endif

