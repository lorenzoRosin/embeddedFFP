/**
 * @file eCUDataStuffUnStuffTest.c
 *
 */
#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCUDataStuffUnStuffTest.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void dataStuffUnStuffCommon(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void dataStuffUnStuffTest(void)
{
	(void)printf("\n\nDATA STUFF UNSTUFF TEST START \n\n");

    dataStuffUnStuffCommon();

    (void)printf("\n\nDATA STUFF UNSTUFF END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void dataStuffUnStuffCommon(void)
{
    /* Local variable */
    e_eCU_BUStuffCtx ctxUnStuff;
    e_eCU_BStuffCtx ctxStuff;
    uint8_t  dataStuffPool[5u];
    uint8_t  dataUnStuffPool[50u];
    uint8_t  tempPool[50u];
    uint32_t  temp32;
    uint32_t  temp32sec;
    bool_t errSofRec;
    bool_t eofRec;

    /* Function Init */
    if( ECU_RES_OK == bStufferInitCtx(&ctxStuff, dataStuffPool, sizeof(dataStuffPool)) )
    {
        (void)printf("dataStuffUnStuffCommon 1  -- OK \n");
    }
    else
    {
        (void)printf("dataStuffUnStuffCommon 1  -- FAIL \n");
    }

    if( ECU_RES_OK == bUStufferInitCtx(&ctxUnStuff, dataUnStuffPool, sizeof(dataUnStuffPool)) )
    {
        (void)printf("dataStuffUnStuffCommon 2  -- OK \n");
    }
    else
    {
        (void)printf("dataStuffUnStuffCommon 2  -- FAIL \n");
    }

    /* Stuff */
    dataStuffPool[0x00] = 0x01u;
    dataStuffPool[0x01] = ECU_SOF;
    dataStuffPool[0x02] = ECU_EOF;
    dataStuffPool[0x03] = ECU_ESC;
    dataStuffPool[0x04] = 0x21u;
    if( ECU_RES_OK == bStufferRetriStufChunk(&ctxStuff, tempPool, sizeof(tempPool), &temp32) )
    {
        (void)printf("dataStuffUnStuffCommon 3  -- OK \n");
    }
    else
    {
        (void)printf("dataStuffUnStuffCommon 3  -- FAIL \n");
    }

    /* unstuff */
    if( ECU_RES_OK == bUStufferInsStufChunk( &ctxUnStuff, tempPool, temp32, &temp32sec, &errSofRec, &eofRec ) )
    {
        if( (true == errSofRec) || (false == eofRec) )
        {
            (void)printf("dataStuffUnStuffCommon 4  -- FAIL \n");
        }
        else
        {
            if( ECU_RES_OK == bUStufferGetNUnstuf(&ctxUnStuff, &temp32sec) )
            {
                if( sizeof(dataStuffPool) == temp32sec )
                {
                    (void)printf("dataStuffUnStuffCommon 4  -- OK \n");
                }
                else
                {
                    (void)printf("dataStuffUnStuffCommon 4  -- FAIL \n");
                }
            }
            else
            {
                (void)printf("dataStuffUnStuffCommon 4  -- FAIL \n");
            }
        }
    }
    else
    {
        (void)printf("dataStuffUnStuffCommon 4  -- FAIL \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
#endif