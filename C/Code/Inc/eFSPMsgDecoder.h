/**
 * @file eFSPMsgDecoder.h
 *
 */

#ifndef EFSPMSGDECODER_H
#define EFSPMSGDECODER_H



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
 
/* Call back to a function that will calculate the CRC for this modules
 * the cntx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the MSG DECODER module */
typedef bool_t (*cb_crc32_msgd) ( void* cntx, const uint32_t seed, const uint8_t dataS[], const uint32_t dataSLen,
                                  uint32_t* const crc32SVal );
typedef enum
{
    MSGD_RES_OK = 0,
    MSGD_RES_BADPARAM,
    MSGD_RES_BADPOINTER,
	MSGD_RES_CORRUPTCTX,
    MSGD_RES_OUTOFMEM,
	MSGD_RES_FRAMEENDED,
    MSGD_RES_NOINITLIB,
}e_eFSP_MsgD_Res;

typedef struct
{
    bool_t   isInit;
	uint8_t* memArea;
	uint32_t memAreaSize;
	uint32_t memAreaCntr;
    bool_t   precedentWasEsc;
    bool_t   needSof;
    bool_t   needEof;
}s_eFSP_MsgDCtx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * Initialize the data decoder context
 * @param  ctx data decoder context
 * @param  memArea Pointer to a memory area that we will use to save udecoded data payload
 * @param  memAreaSize Dimension in byte of the memory area
 * @param cbCrcP Pointer to a CRC 32 seed callback function
 * @param clbCtx Custom context passed to the callback function
 * @return MSGD_RES_BADPOINTER in case of bad pointer
 *		   MSGD_RES_BADPARAM in case of an invalid parameter
 *         MSGD_RES_OK operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderInitCtx(s_eFSP_MsgDCtx* const ctx, uint8_t* const memArea, const uint32_t memAreaSize, 
								 cb_crc32_msgd cbCrcP, void* const clbCtx);

/**
 * Reset data Decoder and restart from memory start
 * @param  ctx data decoder context
 * @return MSGD_RES_BADPOINTER in case of bad pointer
 *		   MSGD_RES_NOINITLIB need to init context before taking some action
 *         MSGD_RES_OK operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderReset(s_eFSP_MsgDCtx* const ctx);

/**
 * Retrive how many raw byte of pyaload we have decoded
 * @param  ctx data decoder context
 * @param  retrivedLen Pointer to a memory area were we will store size of the encoded raw data of payload
 * @return MSGD_RES_BADPOINTER in case of bad pointer
 *		   MSGD_RES_NOINITLIB need to init context before taking some action
 *		   MSGD_RES_CORRUPTCTX in case of an corrupted context
 *         MSGD_RES_OK operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderGetNDecoded(s_eFSP_MsgDCtx* const ctx, uint32_t* const retrivedLen);


/**
 * Retrive how many raw byte of pyaload we have decoded
 * @param  ctx data decoder context
 * @param  retrivedLen Pointer to a memory area were we will store size of the encoded raw data of payload
 * @return MSGD_RES_BADPOINTER in case of bad pointer
 *		   MSGD_RES_NOINITLIB need to init context before taking some action
 *		   MSGD_RES_CORRUPTCTX in case of an corrupted context
 *         MSGD_RES_OK operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderGetPayloadDataLocation(s_eFSP_MsgDCtx* const ctx, uint8_t** const payloadLocation);


/**
 * Retrive the minimum amount of Encoded data we can insert witouth starting a new frame parsing
 * @param  ctx data decoder context
 * @param  retrivedLen Pointer to a memory area were we will store size the minimum amount of encoded data we can
 *         insert before the frame end
 * @return MSGD_RES_BADPOINTER in case of bad pointer
 *		   MSGD_RES_NOINITLIB need to init context before taking some action
 *		   MSGD_RES_CORRUPTCTX in case of an corrupted context
 *         MSGD_RES_OK operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderGetNMinBeforeFrameRest(s_eFSP_MsgDCtx* const ctx, uint32_t* const retrivedLen);

/**
 * Insert encoded chunk chunk of data 
 * @param  ctx data decoder context
 * @param  encArea Pointer to the encoded Data that we will decode
 * @param  encLen data to decode size
 * @param  consumedStuffData Pointer to an uint32_t were we will store how many stuffed data byte were analyzed
 *         and that dosent need to be inserted in this function anymore
 * @param  errSofRec Pointer to an uint32_t were we will store how many protocol error were detected. Even with
 *         some error detected, the protocol will continue parsing data discharging error
 * @return DBUSTF_RES_BADPOINTER in case of bad pointer
 *		   DBUSTF_RES_NOINITLIB need to init context before taking some action
 *		   DBUSTF_RES_BADPARAM in case of an invalid parameter or state
 *		   DBUSTF_RES_CORRUPTCTX in case of an corrupted context
 *         DBUSTF_RES_OUTOFMEM Can not unstuff data, initial mem pointer was too small
 *		   DBUSTF_RES_FRAMEENDED Frame ended, restart context in order to parse a new frame
 *         DBUSTF_RES_OK operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderInsertDChunk(s_eFSP_MsgDCtx* const ctx, const uint8_t* encArea, const uint32_t encLen,
									   uint32_t* const consumedStuffData, uint32_t* errSofRec);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGDECODER_H */