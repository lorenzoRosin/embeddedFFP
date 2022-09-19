/**
 * @file       eFSPMsgDecoder.h
 *
 * @brief      Message encoder utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSPMSGENCODER_H
#define EFSPMSGENCODER_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPType.h"
#include "eCUByteStuffer.h"



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
    MSGE_RES_MESSAGEENDED,
    MSGE_RES_NOINITLIB,
    MSGE_RES_NOINITMESSAGE,
	MSGE_RES_CRCCLBKERROR,
}e_eFSP_MsgE_Res;

typedef struct
{
    e_eCU_BStuffCtx byteStufferCtnx;
    cb_crc32_msge   cbCrcPointer;
    void*           cbCrcCtx;
}s_eFSP_MsgECtx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the message encoder context
 *
 * @param[in]   ctx         - Message Encoder context
 * @param[in]   memArea     - Pointer to a memory area that we will use encode message
 * @param[in]   memAreaSize - Dimension in byte of the memory area
 * @param[in]   cbCrcP      - Pointer to a CRC 32 seed callback function
 * @param[in]   clbCtx      - Custom context passed to the callback function
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MsgE_Res msgEncoderInitCtx(s_eFSP_MsgECtx* const ctx, const uint8_t* memArea, const uint32_t memAreaSize,
								  cb_crc32_msge cbCrcP, void* const clbCtx);

/**
 * @brief       Start to encode a new msg given the dimension of raw payload it self. This function suppouse that
 *              data payload that need to be encoded were already copied in memory.( see msgEncoderGetPayloadLocation
 *              in order to know how get the data pointer )
 *
 * @param[in]   ctx         - Message Encoder context
 * @param[in]   messageLen  - lenght of the raw data present in the frame that we need to encode
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *				MSGE_RES_CRCCLBKERROR   - The crc callback function returned an error
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MsgE_Res msgEncoderStartNewMessage(s_eFSP_MsgECtx* const ctx, const uint32_t  messageLen);

/**
 * @brief       Retrive the pointer of the buffer that the user can use to insert data payload that need to be encoded
 *
 * @param[in]   ctx         - Message Encoder context
 * @param[out]  dataP       - Pointer to a Pointer where the raw data needs to be copied before starting a message
 * @param[out]  maxDataSize - Pointer to a uint32_t variable where the max number of data that can be copied in dataP
 *                            will be placed
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MsgE_Res msgEncoderGetPayloadLocation(s_eFSP_MsgECtx* const ctx, uint8_t** dataP, uint32_t* const maxDataSize);

/**
 * @brief       Restart to encode the already passed/the current frame
 *
 * @param[in]   ctx         - Message Encoder context
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_NOINITMESSAGE  - Need to start a message before restarting it
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MsgE_Res msgEncoderRestartCurrentMessage(s_eFSP_MsgECtx* const ctx);

/**
 * @brief       Retrive the numbers of stuffed bytes + header that can be retrived using msgEncoderRetriveEChunk
 *              (e.g. if the value of the returned value is zero it's means that the message encoding is ended ).
 *
 * @param[in]   ctx         - Message Encoder context
 * @param[out]  retrivedLen - Pointer to a uint32_t variable where the numbers of retrivable encoded data will be placed
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_NOINITMESSAGE  - Need to start a message before taking some action
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MsgE_Res msgEncoderGetRemToRetrive(s_eFSP_MsgECtx* const ctx, uint32_t* const retrivedLen);

/**
 * @brief       Retrive encoded data chunk. The raw data copied in the buffer by using the function
 *              msgEncoderGetPayloadLocation will be encoded and retrived by this function.
 *
 * @param[in]   ctx         - Message Encoder context
 * @param[in]   encodeDest  - Pointer to the destination area of encoded data
 * @param[in]   maxDestLen  - Max fillable size of the destination area
 * @param[out]  filledLen   - Pointer to an uint32_t were we will store the amount of encoded data
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *		        MSGE_RES_NOINITMESSAGE  - Need to start a message before taking some action
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGE_RES_MESSAGEENDED   - No more data that we can elaborate, restart or start a new msg to proceed.
 *                                        This means that we have finished encoding the current message.
 *              MSGE_RES_OK             - Operation ended correctly, message is not still fully encoded 
 */
e_eFSP_MsgE_Res msgEncoderRetriveEChunk(s_eFSP_MsgECtx* const ctx, uint8_t* const encodeDest,
									   const uint32_t maxDestLen, uint32_t* const filledLen);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGENCODER_H */