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
 * @param[in]   p_ctx         - Message Encoder context
 * @param[in]   a_memArea     - Pointer to a memory area that we will use to encode message
 * @param[in]   memAreaSize   - Dimension in byte of the a_memArea
 * @param[in]   f_Crc         - Pointer to a CRC 32 seed callback function
 * @param[in]   p_clbCtx      - Custom context passed to the callback function
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_Res eFSP_MSGE_InitCtx(s_eFSP_MSGE_Ctx* const p_ctx, uint8_t a_memArea[], const uint32_t memAreaSize,
								  cb_crc32_msge f_Crc, void* const p_clbCtx);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ctx         - Message Encoder context
 * @param[out]  p_isInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      MSGE_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              MSGE_RES_OK            - Operation ended correctly
 */
e_eFSP_MSGE_Res eFSP_MSGE_IsInit(s_eFSP_MSGE_Ctx* const p_ctx, bool_t* p_isInit);

/**
 * @brief       Retrive the pointer of the buffer that the user can use to insert data payload that need to be encoded
 *
 * @param[in]   p_ctx         - Message Encoder context
 * @param[out]  pp_data       - Pointer to a Pointer where the raw data needs to be copied before starting a message
 * @param[out]  p_maxDataL    - Pointer to a uint32_t variable where the max number of data that can be copied in pp_data
 *                              will be placed
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_Res eFSP_MSGE_GetWherePutData(s_eFSP_MSGE_Ctx* const p_ctx, uint8_t** pp_data, uint32_t* const p_maxDataL);

/**
 * @brief       Start to encode a new msg given the dimension of raw payload it self. This function suppouse that
 *              data payload that need to be encoded were already copied in memory.( see eFSP_MSGE_GetWherePutData
 *              in order to know how get the data pointer, and copy the data )
 *
 * @param[in]   p_ctx         - Message Encoder context
 * @param[in]   messageLen    - lenght of the raw payload present in the frame that we need to encode ( no header, only
 *                              raw data )
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *				MSGE_RES_CRCCLBKERROR   - The crc callback function returned an error
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_Res eFSP_MSGE_NewMessage(s_eFSP_MSGE_Ctx* const p_ctx, const uint32_t messageLen);

/**
 * @brief       Restart to encode the already passed payload/the current frame
 *
 * @param[in]   p_ctx         - Message Encoder context
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_NOINITMESSAGE  - Need to start a message before restarting it
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_Res eFSP_MSGE_RestartMessage(s_eFSP_MSGE_Ctx* const p_ctx);

/**
 * @brief       Retrive the numbers of stuffed bytes + header that can be retrived using eFSP_MSGE_GetEncChunk
 *              (e.g. if the value of the returned value is zero it's means that the message encoding is ended ).
 *
 * @param[in]   p_ctx         - Message Encoder context
 * @param[out]  p_retrivedLen - Pointer to a uint32_t variable where the numbers of retrivable encoded data will be placed
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_NOINITMESSAGE  - Need to start a message before taking some action
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_Res eFSP_MSGE_GetRemByteToGet(s_eFSP_MSGE_Ctx* const p_ctx, uint32_t* const p_retrivedLen);

/**
 * @brief       Retrive encoded data chunk. The raw data copied in the buffer using the function
 *              MSGE_GetPayloadLocation will be encoded (header and byte stuffing) and retrived by this function.
 *
 * @param[in]   p_ctx         - Message Encoder context
 * @param[in]   p_encodeDest  - Pointer to the destination area of encoded data that will be placed by this function
 * @param[in]   maxDestLen  - Max fillable size of the destination area
 * @param[out]  p_filledLen   - Pointer to an uint32_t were we will store the number encoded data inserted in p_encodeDest.
 *                            Note that if the function return MSGE_RES_OK the value of p_filledLen will be equals to
 *                            maxDestLen. The value p_filledLen infact could be lower than max dest size only if
 *                            some error is returned or if the frame is ended ( MSGE_RES_MESSAGEENDED )
 *
 * @return      MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *		        MSGE_RES_NOINITMESSAGE  - Need to start a message before taking some action
 *		        MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGE_RES_MESSAGEENDED   - No more data that we can elaborate, restart or start a new frame to proceed.
 *                                        This means that we have finished encoding the current frame. Keep in mind
 *                                        in this case that the value of p_filledLen could be lower than maxDestLen.
 *              MSGE_RES_OK             - Operation ended correctly. This dosent mean that the encoding process is
 *                                        completed, but we can be sure that p_filledLen will have the same value of
 *                                        maxDestLen
 */
e_eFSP_MSGE_Res eFSP_MSGE_GetEncChunk(s_eFSP_MSGE_Ctx* const p_ctx, uint8_t p_encodeDest[], const uint32_t maxDestLen,
                                      uint32_t* const p_filledLen);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGENCODER_H */