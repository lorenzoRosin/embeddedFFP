/**
 * @file eFSPMsgDecoder.h
 *
 */

#ifndef EFSPMSGENCODER_H
#define EFSPMSGENCODER_H



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
 * by the MSG ENCODER module */
typedef bool_t (*cb_crc32_msge) ( void* cntx, const uint32_t seed, const uint8_t dataS[], const uint32_t dataSLen,
                                  uint32_t* const crc32SVal );

typedef enum
{
    MSGE_RES_OK = 0,
    MSGE_RES_BADPARAM,
    MSGE_RES_BADPOINTER,
	MSGE_RES_CORRUPTCTX,
    MSGE_RES_FRAMEENDED,
    MSGE_RES_NOINITLIB,
}e_eFSP_MsgE_Res;

typedef struct
{
    bool_t isInit;
    uint32_t usedBaseSeed;
	uint32_t digestedTimes;
    uint32_t lastDigest;
    cb_crc32_msge cbCrcPointer;
    void* cbCrcCtx;
}s_eFSP_MsgECtx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * Initialize the message encoder context
 * @param ctx Message Encoder context
 * @param memArea Pointer to a memory area that we will use to retrive the raw payload of the message we want to encode
 * @param memAreaSize Dimension in byte of the memory area
 * @param cbCrcP Pointer to a CRC 32 seed callback function
 * @param clbCtx Custom context passed to the callback function
 * @return MSGE_RES_BADPOINTER in case of bad pointer
 *		   MSGE_RES_BADPARAM in case of an invalid parameter
 *         MSGE_RES_OK operation ended correctly
 */
e_eFSP_MsgE_Res msgEncoderInitCtx(s_eFSP_MsgECtx* const ctx, const uint8_t* memArea, const uint32_t memAreaSize,
								  cb_crc32_msge cbCrcP, void* const clbCtx);

/**
 * Reset data encoder and restart from memory start
 * @param  ctx Message Encoder context
 * @return MSGE_RES_BADPOINTER in case of bad pointer
 *		   MSGE_RES_NOINITLIB need to init the message encoder context before taking some action
 *         MSGE_RES_OK operation ended correctly
 */
e_eFSP_MsgE_Res msgEncoderReset(s_eFSP_MsgECtx* const ctx);

/**
 * Retrive how many byte we can still retrive
 * @param  ctx Message Encoder context
 * @param  retrivedLen Pointer to a memory area were we will store size of retrivable data
 * @return MSGE_RES_BADPOINTER in case of bad pointer
 *		   MSGE_RES_NOINITLIB need to init the message encoder context before taking some action
 *		   MSGE_RES_CORRUPTCTX in case of an corrupted context
 *         MSGE_RES_OK operation ended correctly
 */
e_eFSP_MsgE_Res msgEncoderGetRemToEncode(s_eFSP_MsgECtx* const ctx, uint32_t* const retrivedLen);

/**
 * Retrive how many raw byte of pyaload we have decoded
 * @param  ctx data decoder context
 * @param  retrivedLen Pointer to a memory area were we will store size of the encoded raw data of payload
 * @return MSGD_RES_BADPOINTER in case of bad pointer
 *		   MSGD_RES_NOINITLIB need to init context before taking some action
 *		   MSGD_RES_CORRUPTCTX in case of an corrupted context
 *         MSGD_RES_OK operation ended correctly
 */
e_eFSP_MsgD_Res msgEncoderGetPayloadDataLocation(s_eFSP_MsgDCtx* const ctx, uint8_t** const payloadLocation);

/**
 * Retrive encoded data chunk
 * @param  ctx Message Encoder context
 * @param  encodeDest Pointer to the destination area of encoded data
 * @param  maxDestLen max fillable size of the destination area
 * @param  filledLen Pointer to an uint32_t were we will store the amount of encoded data
 * @return MSGE_RES_BADPOINTER in case of bad pointer
 *		   MSGE_RES_NOINITLIB need to init the message encoder context before taking some action
 *		   MSGE_RES_BADPARAM in case of an invalid parameter
 *		   MSGE_RES_CORRUPTCTX in case of an corrupted context
 *         MSGE_RES_FRAMEENDED No more data that we can elaborate
 *         MSGE_RES_OK operation ended correctly
 */
e_eFSP_MsgE_Res msgEncoderRetriveEChunk(s_eFSP_MsgECtx* const ctx, uint8_t* const encodeDest,
									   const uint32_t maxDestLen, uint32_t* const filledLen);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGENCODER_H */