/**
 * @file eFSPByteUnStuffer.h
 *
 */

#ifndef ECUBYTEUNSTUFFER_H
#define ECUBYTEUNSTUFFER_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPType.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef struct
{
    bool_t   isInit;
	uint8_t* memArea;
	uint32_t memAreaSize;
	uint32_t memAreaCntr;
    bool_t   precedentWasEsc;
    bool_t   needSof;
    bool_t   needEof;
}e_eFSP_BUStuffCtx;

typedef e_eFSP_Res (*cb_crc32_seed) ( const uint32_t seed, const uint8_t dataS[], const uint32_t dataSLen,
                                     uint32_t* const crc32SVal );

/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * Initialize the byte unstuffer context
 * @param  ctx Byte Unstuffer context
 * @param  memArea Pointer to a memory area that we will use to save unstuffed data
 * @param  memAreaSize Dimension in byte of the memory area
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OK operation ended correctly
 */
e_eFSP_Res bUStufferInitCtx(e_eFSP_BUStuffCtx* const ctx, uint8_t* const memArea, const uint32_t memAreaSize);

/**
 * Reset data Unstuffer and restart from memory start
 * @param  ctx Byte Unstuffer context
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data unstuffer context before taking some action
 *         ECU_RES_OK operation ended correctly
 */
e_eFSP_Res bUStufferReset(e_eFSP_BUStuffCtx* const ctx);

/**
 * Retrive how many raw byte we have unstuffed
 * @param  ctx Byte Unstuffer context
 * @param  retrivedLen Pointer to a memory area were we will store size of the unstuffed raw data
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data unstuffer context before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OK operation ended correctly
 */
e_eFSP_Res bUStufferGetNUnstuf(e_eFSP_BUStuffCtx* const ctx, uint32_t* const retrivedLen);

/**
 * Insert stuffed data chunk
 * @param  ctx Byte Unstuffer context
 * @param  stuffedArea Pointer to the stuffed Data that we will unstuff
 * @param  stuffLen data to unstuff size
 * @param  consumedStuffData Pointer to an uint32_t were we will store how many stuffed data byte were analyzed
 *         and that dosent need to be inserted in this function anymore
 * @param  errSofRec Pointer to an bool_t were we will store if some protocol error was detected. Even with this flag
 *         sette to true, the protocol will continue parsing data discharging error
 * @param  eofRec Pointer to an bool_t were we will store if the end of frame is reached. After the end of frame is
 *         found no other data will be parse (consumedStuffData = 0) till a reset or a init is called
 * @return ECU_RES_BADPOINTER in case of bad pointer
 *		   ECU_RES_NOINITLIB need to init the data unstuffer context before taking some action
 *		   ECU_RES_BADPARAM in case of an invalid parameter or state
 *         ECU_RES_OUTOFMEM Can not unstuff data, initial mem pointer was too small
 *         ECU_RES_OK operation ended correctly
 */
e_eFSP_Res bUStufferInsStufChunk(e_eFSP_BUStuffCtx* const ctx, const uint8_t* stuffedArea, const uint32_t stuffLen,
                                  uint32_t* const consumedStuffData, bool_t* errSofRec, bool_t* eofRec);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECUBYTEUNSTUFFER_H */