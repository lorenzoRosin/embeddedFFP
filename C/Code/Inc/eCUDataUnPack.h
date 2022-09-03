/**
 * @file eCUDataUnPack.h
 *
 */

#ifndef ECUDATAUNPACK_H
#define ECUDATAUNPACK_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCUType.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef struct
{
    bool_t isInit;
	bool_t isLE;
	uint8_t* memUPKA;
	uint32_t memUPKASize;
	uint32_t memUPKACntr;
}s_eCU_DataUnPackCtx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * Initialize the data unpacker context
 * @param ctx data unpacker context
 * @param memUPKA Pointer to a memory area that we will use get data to pop
 * @param memUPKASize Dimension in byte of the data unpacker area
 * @param isLEnd Select if data unpacker must work in Little Endian or Big Endian
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OK data unpacker is initialized correctly
 */
e_eCU_Res dataUnPackinitCtx(s_eCU_DataUnPackCtx* const ctx, uint8_t* const memUPKA, const uint32_t memUPKASize,
                         const bool_t isLEnd);

/**
 * Reset data unpacker and restart from memory start
 * @param ctx data unpacker context
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data unpacker before taking some action
 *         ECU_RES_OK operation ended correctly
 */
 e_eCU_Res dataUnPackReset(s_eCU_DataUnPackCtx* const ctx);

/**
 * Retrive how many byte we can still pop
 * @param ctx data unpacker context
 * @param retrivedLen Pointer to a memory area were we will store size of serialized data
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data unpacker before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OK operation ended correctly
 */
e_eCU_Res dataUnPackGetRemToPop(s_eCU_DataUnPackCtx* const ctx, uint32_t* const retrivedLen);

/**
 * Pop some raw data
 * @param ctx Data packer context
 * @param dataDest Pointer to a memory area where popped data will be copied
 * @param retrivedLen The amount of data we want to pop
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data packer before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OUTOFMEM Not enought memory to copy all the data
 *         ECU_RES_OK circular queue is initialized correctly
 */
e_eCU_Res dataUnPackPopArray(s_eCU_DataUnPackCtx* const ctx, uint8_t* const dataDest, uint32_t const retrivedLen);

/**
 * Pop one byte in data packer
 * @param ctx Data Unpacker context
 * @param dataP pointer to a variable that will contain the popped data
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data packer before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OUTOFMEM Not enought memory to copy all the data
 *         ECU_RES_OK circular queue is initialized correctly
 */
e_eCU_Res dataUnPackPopU8(s_eCU_DataUnPackCtx* const ctx, uint8_t* dataP);

/**
 * Pop 2 byte in data packer
 * @param ctx Data Unpacker context
 * @param dataP pointer to a variable that will contain the popped data
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data packer before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OUTOFMEM Not enought memory to copy all the data
 *         ECU_RES_OK circular queue is initialized correctly
 */
e_eCU_Res dataUnPackPopU16(s_eCU_DataUnPackCtx* const ctx, uint16_t* dataP);

/**
 * Pop 4 byte in data packer
 * @param ctx Data Unpacker context
 * @param dataP pointer to a variable that will contain the popped data
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data packer before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OUTOFMEM Not enought memory to copy all the data
 *         ECU_RES_OK circular queue is initialized correctly
 */
e_eCU_Res dataUnPackPopU32(s_eCU_DataUnPackCtx* const ctx, uint32_t* dataP);

/**
 * Pop 8 byte in data packer
 * @param ctx Data Unpacker context
 * @param dataP pointer to a variable that will contain the popped data
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data packer before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OUTOFMEM Not enought memory to copy all the data
 *         ECU_RES_OK circular queue is initialized correctly
 */
e_eCU_Res dataUnPackPopU64(s_eCU_DataUnPackCtx* const ctx, uint64_t* dataP);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECUDATAUNPACK_H */