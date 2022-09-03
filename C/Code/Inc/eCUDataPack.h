/**
 * @file eCUDataPack.h
 *
 */

#ifndef ECUDATAPACK_H
#define ECUDATAPACK_H



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
	uint8_t* memPKA;
	uint32_t memPKASize;
	uint32_t memPKACntr;
}s_eCU_DataPackCtx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * Initialize the data packer context
 * @param ctx Data packer context
 * @param memPKA Pointer to a memory area that we will fill with packet data
 * @param memPKASize Dimension in byte of the data packer area
 * @param isLEnd Select if data packer must work in Little Endian or Big Endian
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OK data packer is initialized correctly
 */
e_eCU_Res dataPackinitCtx(s_eCU_DataPackCtx* const ctx, uint8_t* const memPKA, const uint32_t memPKASize,
					   const bool_t isLEnd);

/**
 * Reset data packer and restart packing data from memory start
 * @param ctx Data packer context
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data packer before taking some action
 *         ECU_RES_OK operation ended correctly
 */
e_eCU_Res dataPackReset(s_eCU_DataPackCtx* const ctx);

/**
 * Retrive how many byte we have pushed
 * @param ctx Data packer context
 * @param retrivedLen Pointer to a memory area were we will store size of serialized data
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data packer before taking some action
 *         ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OK operation ended correctly
 */
e_eCU_Res dataPackGetNPushed(s_eCU_DataPackCtx* const ctx, uint32_t* const retrivedLen);

/**
 * Push an array in data packer
 * @param ctx Data packer context
 * @param data Pointer to a memory area where data are stored
 * @param dataLen  data size
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data packer before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OUTOFMEM Not enought memory to copy all the data
 *         ECU_RES_OK circular queue is initialized correctly
 */
e_eCU_Res dataPackPushArray(s_eCU_DataPackCtx* const ctx, const uint8_t* data, const uint32_t dataLen);

/**
 * Push one byte in data packer
 * @param ctx Data packer context
 * @param dataToPush Byte to push in data packer
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data packer before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OUTOFMEM Not enought memory to copy all the data
 *         ECU_RES_OK circular queue is initialized correctly
 */
e_eCU_Res dataPackPushU8(s_eCU_DataPackCtx* const ctx, const uint8_t dataToPush);

/**
 * Push 2 byte in data packer
 * @param ctx Data packer context
 * @param dataToPush Byte to push in data packer
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data packer before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OUTOFMEM Not enought memory to copy all the data
 *         ECU_RES_OK circular queue is initialized correctly
 */
e_eCU_Res dataPackPushU16(s_eCU_DataPackCtx* const ctx, const uint16_t dataToPush);

/**
 * Push 4 byte in data packer
 * @param ctx Data packer context
 * @param dataToPush Byte to push in data packer
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data packer before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OUTOFMEM Not enought memory to copy all the data
 *         ECU_RES_OK circular queue is initialized correctly
 */
e_eCU_Res dataPackPushU32(s_eCU_DataPackCtx* const ctx, const uint32_t dataToPush);

/**
 * Push 8 byte in data packer
 * @param ctx Data packer context
 * @param dataToPush Byte to push in data packer
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data packer before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OUTOFMEM Not enought memory to copy all the data
 *         ECU_RES_OK circular queue is initialized correctly
 */
 e_eCU_Res dataPackPushU64(s_eCU_DataPackCtx* const ctx, const uint64_t dataToPush);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECUDATAPACK_H */