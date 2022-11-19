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
}e_eFSP_MSGE_Res;

typedef struct
{
    s_eCU_BSTF_Ctx  byteStufferCtnx;
    cb_crc32_msge   cbCrcPtr;
    void*           cbCrcCtx;
}s_eFSP_MSGE_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the message encoder context
 *
 * @param[in]   ctx         - Message Encoder context
 * @param[in]   memArea     - Pointer to a memory area that we will use to encode message
 * @param[in]   memAreaSize - Dimension in byte of the memArea
 * @param[in]   cbCrcP      - Pointer to a CRC 32 seed callback function
 * @param[in]   clbCtx      - Custom context passed to the callback function
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_Res MSGE_InitCtx(s_eFSP_MSGE_Ctx* const ctx, uint8_t memArea[], const uint32_t memAreaSize,
								  cb_crc32_msge cbCrcP, void* const clbCtx);

/**
 * @brief       Start to encode a new msg given the dimension of raw payload it self. This function suppouse that
 *              data payload that need to be encoded were already copied in memory.( see MSGE_GetPayloadLocation
 *              in order to know how get the data pointer, and copy the data )
 *
 * @param[in]   ctx         - Message Encoder context
 * @param[in]   messageLen  - lenght of the raw payload present in the frame that we need to encode ( no header, only
 *                            raw data )
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *				MSGE_RES_CRCCLBKERROR   - The crc callback function returned an error
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_Res MSGE_StartNewMessage(s_eFSP_MSGE_Ctx* const ctx, const uint32_t messageLen);

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
e_eFSP_MSGE_Res MSGE_GetPayloadLocation(s_eFSP_MSGE_Ctx* const ctx, uint8_t** dataP, uint32_t* const maxDataSize);

/**
 * @brief       Restart to encode the already passed payload/the current frame
 *
 * @param[in]   ctx         - Message Encoder context
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_NOINITMESSAGE  - Need to start a message before restarting it
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_Res MSGE_RestartCurrentMessage(s_eFSP_MSGE_Ctx* const ctx);

/**
 * @brief       Retrive the numbers of stuffed bytes + header that can be retrived using MSGE_RetriveEChunk
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
e_eFSP_MSGE_Res MSGE_GetRemToRetrive(s_eFSP_MSGE_Ctx* const ctx, uint32_t* const retrivedLen);

/**
 * @brief       Retrive encoded data chunk. The raw data copied in the buffer by using the function
 *              MSGE_GetPayloadLocation will be encoded (header and byte stuffing) and retrived by this function.
 *
 * @param[in]   ctx         - Message Encoder context
 * @param[in]   encodeDest  - Pointer to the destination area of encoded data that will be placed by this function
 * @param[in]   maxDestLen  - Max fillable size of the destination area
 * @param[out]  filledLen   - Pointer to an uint32_t were we will store the number encoded data inserted in encodeDest.
 *                            Note that if the function return MSGE_RES_OK the value of filledLen will be equals to
 *                            maxDestLen. The value filledLen infact could be lower than max dest size only if
 *                            some error is returned or if the frame is ended ( MSGE_RES_MESSAGEENDED )
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *		        MSGE_RES_NOINITMESSAGE  - Need to start a message before taking some action
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGE_RES_MESSAGEENDED   - No more data that we can elaborate, restart or start a new frame to proceed.
 *                                        This means that we have finished encoding the current frame. Keep in mind
 *                                        in this case that the value of filledLen could be lower than maxDestLen.
 *              MSGE_RES_OK             - Operation ended correctly. This dosent mean that the encoding process is
 *                                        completed, but we can be sure that filledLen will have the same value of
 *                                        maxDestLen
 */
e_eFSP_MSGE_Res MSGE_RetriveEChunk(s_eFSP_MSGE_Ctx* const ctx, uint8_t encodeDest[], const uint32_t maxDestLen,
                                   uint32_t* const filledLen);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGENCODER_H */