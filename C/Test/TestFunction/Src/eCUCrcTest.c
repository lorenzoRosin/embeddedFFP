/**
 * @file eCUCrcTest.c
 *
 */
#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
    /* Suppressed for code clarity in test execution*/
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCUCrcTest.h"
#include <stdio.h>



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void crc32TestBadPointer(void);
static void crc32TestValue(void);
static void crc32TestCombined(void);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void crc32Test(void)
{
	(void)printf("\n\nCRC TEST START \n\n");

    crc32TestBadPointer();
    crc32TestValue();
    crc32TestCombined();

    (void)printf("\n\nCRC TEST END \n\n");
}





/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void crc32TestBadPointer(void)
{
    /* Local variable */
    uint32_t crc32SValTest;
    uint8_t  crc8SValTest[1u];

    /* Function */
    if( ECU_RES_BADPOINTER == crc32Seed(0u, NULL, 1u, &crc32SValTest) )
    {
        (void)printf("crc32TestBadPointer 1  -- OK \n");
    }
    else
    {
        (void)printf("crc32TestBadPointer 1  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == crc32Seed(0u, crc8SValTest, 1u, NULL) )
    {
        (void)printf("crc32TestBadPointer 2  -- OK \n");
    }
    else
    {
        (void)printf("crc32TestBadPointer 2  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == crc32(NULL, 1u, &crc32SValTest) )
    {
        (void)printf("crc32TestBadPointer 3  -- OK \n");
    }
    else
    {
        (void)printf("crc32TestBadPointer 3  -- FAIL \n");
    }

    if( ECU_RES_BADPOINTER == crc32(crc8SValTest, 1u, NULL) )
    {
        (void)printf("crc32TestBadPointer 4  -- OK \n");
    }
    else
    {
        (void)printf("crc32TestBadPointer 4  -- FAIL \n");
    }
}

void crc32TestValue(void)
{
    /* Local variable */
    uint32_t crcTestValSeed;
    uint8_t crcTestData[] = {0x00u, 0x01u, 0x02u};
    uint32_t crcTestValRet;

    /* Function */
    crcTestValSeed = 0xFFFFFFFFu;
    if( ECU_RES_OK == crc32Seed(crcTestValSeed, crcTestData, sizeof(crcTestData), &crcTestValRet) )
    {
        if( 0x6CFF87B2u == crcTestValRet)
        {
            (void)printf("crc32TestValue 1  -- OK \n");
        }
        else
        {
            (void)printf("crc32TestValue 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("crc32TestValue 1  -- FAIL \n");
    }

    crcTestValSeed = 0x0u;
    if( ECU_RES_OK == crc32Seed(crcTestValSeed, crcTestData, sizeof(crcTestData), &crcTestValRet) )
    {
        if( 0xDB9BFAB2u == crcTestValRet)
        {
            (void)printf("crc32TestValue 1  -- OK \n");
        }
        else
        {
            (void)printf("crc32TestValue 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("crc32TestValue 1  -- FAIL \n");
    }

    if( ECU_RES_OK == crc32(crcTestData, sizeof(crcTestData), &crcTestValRet) )
    {
        if( 0x6CFF87B2u == crcTestValRet)
        {
            (void)printf("crc32TestValue 1  -- OK \n");
        }
        else
        {
            (void)printf("crc32TestValue 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("crc32TestValue 1  -- FAIL \n");
    }
}

void crc32TestCombined(void)
{
    /* Local variable */
    uint32_t crcTestValSeedC;
    uint8_t crcTestDataC[] = {0x00u, 0x01u, 0x02u, 0x03u, 0x04u, 0x05u};
    uint8_t crcTestDataC2[] = {0x03u, 0x04u, 0x05u};
    uint32_t crcTestValRetC;

    /* Function */
    crcTestValSeedC = 0xFFFFFFFFu;
    if( ECU_RES_OK == crc32Seed(crcTestValSeedC, crcTestDataC, sizeof(crcTestDataC), &crcTestValRetC) )
    {
        if( 0x9FE54C6Du == crcTestValRetC)
        {
            (void)printf("crc32TestCombined 1  -- OK \n");
        }
        else
        {
            (void)printf("crc32TestCombined 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("crc32TestCombined 1  -- FAIL \n");
    }

    crcTestValSeedC = 0xFFFFFFFFu;
    if( ECU_RES_OK == crc32Seed(crcTestValSeedC, crcTestDataC, 0x03u, &crcTestValRetC) )
    {
        if( 0x6CFF87B2u == crcTestValRetC)
        {
            (void)printf("crc32TestCombined 1  -- OK \n");
        }
        else
        {
            (void)printf("crc32TestCombined 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("crc32TestCombined 1  -- FAIL \n");
    }

    crcTestValSeedC = crcTestValRetC;
    if( ECU_RES_OK == crc32Seed(crcTestValSeedC, crcTestDataC2, 0x03u, &crcTestValRetC) )
    {
        if( 0x9FE54C6Du == crcTestValRetC)
        {
            (void)printf("crc32TestCombined 1  -- OK \n");
        }
        else
        {
            (void)printf("crc32TestCombined 1  -- FAIL \n");
        }
    }
    else
    {
        (void)printf("crc32TestCombined 1  -- FAIL \n");
    }
}

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
#endif