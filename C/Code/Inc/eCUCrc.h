/**
 * @file eCUCrc.h
 *
 */

#ifndef ECUCRC_H
#define ECUCRC_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCUType.h"



/***********************************************************************************************************************
 *      DEFINES
 **********************************************************************************************************************/
#define ECU_CRC_BASE_SEED                               ( 0xffffffffu )



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * Calculate the CRC 32 (CRC-32/MPEG-2) of a passed buffer using as seed the default value of 0xffffffffu
 * @param data Pointer to the data buffer where we will calculate the CRC 32
 * @param dataLen how many byte will be used to calculate the CRC 32
 * @param crc32Val Pointer to an uint32_t were we will store the calculated CRC 32
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *         ECU_RES_OK crc 32 calculated successfully
 */
e_eCU_Res crc32(const uint8_t data[], const uint32_t dataLen, uint32_t* const crc32Val);

/**
 * Calculate the CRC 32 (CRC-32/MPEG-2) of a passed buffer using a custom seed
 * @param seed Seed that will be used to calculate the CRC 32
 * @param data Pointer to the data buffer where we will calculate the CRC 32
 * @param dataLen how many byte will be used to calculate the CRC 32
 * @param crc32ValSeed Pointer to an uint32_t were we will store the calculated CRC 32
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *         ECU_RES_OK crc 32 calculated successfully
 */
e_eCU_Res crc32Seed(const uint32_t seed, const uint8_t dataS[], const uint32_t dataSLen, uint32_t* const crc32SVal);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECUCRC_H */