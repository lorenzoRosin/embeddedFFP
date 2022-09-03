/**
 * @file eCUDataPackUnPackTest.c
 *
 */
#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCUDataPackUnPackTest.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void dataPackUnPackCommon(void);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void dataPackUnPackTest(void)
{
	(void)printf("\n\nDATA PACK UNPACK TEST START \n\n");

    dataPackUnPackCommon();

    (void)printf("\n\nDATA PACK UNPACK END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void dataPackUnPackCommon(void)
{
    /* Local variable */
    s_eCU_DataPackCtx ctxPack;
    s_eCU_DataUnPackCtx ctxUnPack;
    uint8_t  dataPackPool[50u];
    uint32_t supportbuffget;
    uint8_t  var8;
    uint16_t var16;
    uint32_t var32;
    uint64_t var64;

    /* Function */
    if( ECU_RES_OK == dataPackinitCtx(&ctxPack, dataPackPool, sizeof(dataPackPool), true) )
    {
        (void)printf("dataPackUnPackCommon 1  -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 1  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU8(&ctxPack, 0xF1u) )
    {
        (void)printf("dataPackUnPackCommon 2  -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 2  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU16(&ctxPack, 0xF1F2u) )
    {
        (void)printf("dataPackUnPackCommon 3  -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 3  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU32(&ctxPack, 0xF1F2F3F4u) )
    {
        (void)printf("dataPackUnPackCommon 4  -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 4  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackPushU64(&ctxPack, 0xF1F2F3F4F5F6F7F8UL) )
    {
        (void)printf("dataPackUnPackCommon 5  -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 5  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackGetNPushed(&ctxPack, &supportbuffget) )
    {
        (void)printf("dataPackUnPackCommon 6  -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 6  -- FAIL \n");
    }

    if( ECU_RES_OK == dataPackReset( &ctxPack ) )
    {
        (void)printf("dataPackUnPackCommon 7  -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 7  -- FAIL \n");
    }

    /* Function */
    if( ECU_RES_OK == dataUnPackinitCtx(&ctxUnPack, dataPackPool, supportbuffget, true) )
    {
        (void)printf("dataPackUnPackCommon 8  -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 8  -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU8(&ctxUnPack, &var8) )
    {
        (void)printf("dataPackUnPackCommon 10 -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 10 -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU16(&ctxUnPack, &var16) )
    {
        (void)printf("dataPackUnPackCommon 11 -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 11 -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU32(&ctxUnPack, &var32) )
    {
        (void)printf("dataPackUnPackCommon 12 -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 12 -- FAIL \n");
    }

    if( ECU_RES_OK == dataUnPackPopU64(&ctxUnPack, &var64) )
    {
        (void)printf("dataPackUnPackCommon 13 -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 13 -- FAIL \n");
    }

    /* Function */
    if( 0xF1u == var8 )
    {
        (void)printf("dataPackUnPackCommon 14 -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 14 -- FAIL \n");
    }

    if( 0xF1F2u == var16 )
    {
        (void)printf("dataPackUnPackCommon 15 -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 15 -- FAIL \n");
    }

    if( 0xF1F2F3F4u == var32 )
    {
        (void)printf("dataPackUnPackCommon 16 -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 16 -- FAIL \n");
    }

    if( 0xF1F2F3F4F5F6F7F8UL == var64 )
    {
        (void)printf("dataPackUnPackCommon 17 -- OK \n");
    }
    else
    {
        (void)printf("dataPackUnPackCommon 17 -- FAIL \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
#endif