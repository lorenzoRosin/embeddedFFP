/**
 * @file eCUCircularQueue.h
 *
 */

#ifndef ECUCIRCULARQUEUE_H
#define ECUCIRCULARQUEUE_H



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
    bool_t   isInit;
    uint8_t* memPool;
    uint32_t memPoolSize;
    uint32_t memPoolUsedSize;
    uint32_t memPoolFrstFreeIdx;
    uint32_t memPoolFrstOccIdx;
}s_eCU_circQCtx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * Initialize the circular queue context
 * @param ctx Circular queue context
 * @param memPool Pointer to a memory pool that we will use to manage the circular queue
 * @param memPoolSize Dimension in byte of the circular queue
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OK circular queue is initialized correctly
 */
e_eCU_Res circQInitCtx(s_eCU_circQCtx* const ctx, uint8_t* memPool, const uint32_t memPoolSize);

/**
 * Reset the state of the circular queue and discharge all saved data
 * @param ctx Circular queue context
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the queue before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OK circular queue is reset correctly
 */
e_eCU_Res circQReset(s_eCU_circQCtx* const ctx);

/**
 * Get free space present in the queue
 * @param ctx Circular queue context
 * @param freeSpace Pointer to variable where free space present in queue will be stored
 * @return ECU_RES_BADPOINTER in case of bad pointer
 * 		   ECU_RES_NOINITLIB need to init the queue before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OK operation returned correctly
 */
e_eCU_Res circQGetFreeSapce(s_eCU_circQCtx* const ctx, uint32_t* const freeSpace);

/**
 * Get the used space in the queue
 * @param ctx Circular queue context
 * @param usedSpace Pointer to variable where used space present in queue will be stored
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the queue before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OK operation returned correctly
 */
e_eCU_Res circQGetOccupiedSapce(s_eCU_circQCtx* const ctx, uint32_t* const usedSpace);

/**
 * Insert data in the queue if free space is avaiable
 * @param ctx Circular queue context
 * @param data Pointer to the data that we want to insert in the queue
 * @param datalen Lenght of the data present in the pointer passed
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the queue before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *		   ECU_RES_OUTOFMEM no memory avaiable for this operation
 *         ECU_RES_OK operation returned correctly
 */
e_eCU_Res circQInsertData(s_eCU_circQCtx* const ctx, const uint8_t* data, const uint32_t datalen);

/**
 * Retrive data from the queue if avaiable
 * @param ctx Circular queue context
 * @param data Pointer to the data that we want to retrive from queue
 * @param datalen Lenght of the data that we want to retrive
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the queue before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *		   ECU_RES_OUTOFMEM no memory avaiable for this operation
 *         ECU_RES_OK operation returned correctly
 */
e_eCU_Res circQRetriveData(s_eCU_circQCtx* const ctx, uint8_t* const data, const uint32_t datalen);

/**
 * Peek data from the queue if avaiable
 * @param ctx Circular queue context
 * @param data Pointer to the data that we want to peek from queue
 * @param datalen Lenght of the data that we want to peek
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the queue before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *		   ECU_RES_OUTOFMEM no memory avaiable for this operation
 *         ECU_RES_OK operation returned correctly
 */
e_eCU_Res circQPeekData(s_eCU_circQCtx* const ctx, uint8_t* const data, const uint32_t datalen);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECUCIRCULARQUEUE_H */