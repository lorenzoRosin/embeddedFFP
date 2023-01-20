/**
 * @file       eFSP_MSGE.h
 *
 * @brief      Message encoder utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSP_MSGE_H
#define EFSP_MSGE_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_TYPE.h"
#include "eCU_BSTF.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
/* Define a generic crc callback context that must be implemented by the user */
typedef struct t_eFSP_MSGE_CrcCtxUser t_eFSP_MSGE_CrcCtx;

/* Call back to a function that will calculate the CRC for this modules
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the MSG ENCODER module */
typedef bool_t (*f_eFSP_MSGE_CrcCb) ( t_eFSP_MSGE_CrcCtx* const p_ptCtx, const uint32_t p_uSeed, const uint8_t* p_puDat,
                                      const uint32_t p_uDataL, uint32_t* const p_puCrc32Val );

typedef enum
{
    e_eFSP_MSGE_RES_OK = 0,
    e_eFSP_MSGE_RES_BADPARAM,
    e_eFSP_MSGE_RES_BADPOINTER,
	e_eFSP_MSGE_RES_CORRUPTCTX,
    e_eFSP_MSGE_RES_MESSAGEENDED,
    e_eFSP_MSGE_RES_NOINITLIB,
    e_eFSP_MSGE_RES_NOINITMESSAGE,
	e_eFSP_MSGE_RES_CRCCLBKERROR,
}e_eFSP_MSGE_RES;

typedef struct
{
    t_eCU_BSTF_Ctx      tBSTFCtx;
    f_eFSP_MSGE_CrcCb   fCrc;
    t_eFSP_MSGE_CrcCtx* ptCrcCtx;
}t_eFSP_MSGE_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the message encoder context
 *
 * @param[in]   p_ptCtx    - Message Encoder context
 * @param[in]   p_puBuff   - Pointer to a memory area that we will use to encode message
 * @param[in]   p_uBuffL   - Dimension in byte of the p_puBuff
 * @param[in]   p_fCrc     - Pointer to a CRC 32 p_uSeed callback function
 * @param[in]   p_ptFctx   - Custom context passed to the callback function
 *
 * @return      e_eFSP_MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eFSP_MSGE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              e_eFSP_MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_RES eFSP_MSGE_InitCtx(t_eFSP_MSGE_Ctx* const p_ptCtx, uint8_t* p_puBuff, const uint32_t p_uBuffL,
								  f_eFSP_MSGE_CrcCb p_fCrc, t_eFSP_MSGE_CrcCtx* const p_ptFctx);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Message Encoder context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSP_MSGE_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSP_MSGE_RES_OK            - Operation ended correctly
 */
e_eFSP_MSGE_RES eFSP_MSGE_IsInit(t_eFSP_MSGE_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Retrive the pointer of the buffer that the user can use to insert data payload that need to be encoded
 *
 * @param[in]   p_ptCtx     - Message Encoder context
 * @param[out]  p_ppuDat    - Pointer to a Pointer where the raw data needs to be copied before starting a message
 * @param[out]  p_puMaxL    - Pointer to a uint32_t variable where the max number of data that can be copied in
 *                            p_ppuDat will be placed
 *
 * @return      e_eFSP_MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eFSP_MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        e_eFSP_MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              e_eFSP_MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_RES eFSP_MSGE_GetWherePutData(t_eFSP_MSGE_Ctx* const p_ptCtx, uint8_t** p_ppuDat, uint32_t* const p_puMaxL);

/**
 * @brief       Start to encode a new msg given the dimension of raw payload it self. This function suppouse that
 *              data payload that need to be encoded were already copied in memory.( see eFSP_MSGE_GetWherePutData
 *              in order to know how get the data pointer, and copy the data )
 *
 * @param[in]   p_ptCtx      - Message Encoder context
 * @param[in]   p_uMsgLen    - lenght of the raw payload present in the frame that we need to encode ( no header, only
 *                             raw data )
 *
 * @return      e_eFSP_MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eFSP_MSGE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *		        e_eFSP_MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        e_eFSP_MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *				e_eFSP_MSGE_RES_CRCCLBKERROR   - The crc callback function returned an error
 *              e_eFSP_MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_RES eFSP_MSGE_NewMessage(t_eFSP_MSGE_Ctx* const p_ptCtx, const uint32_t p_uMsgLen);

/**
 * @brief       Restart to encode the already passed payload/the current frame
 *
 * @param[in]   p_ptCtx         - Message Encoder context
 *
 * @return      e_eFSP_MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eFSP_MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        e_eFSP_MSGE_RES_NOINITMESSAGE  - Need to start a message before restarting it
 *		        e_eFSP_MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              e_eFSP_MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_RES eFSP_MSGE_RestartMessage(t_eFSP_MSGE_Ctx* const p_ptCtx);

/**
 * @brief       Retrive the numbers of stuffed bytes + header that can be retrived using eFSP_MSGE_GetEncChunk
 *              (e.g. if the value of the returned value is zero it's means that the message encoding is ended ).
 *
 * @param[in]   p_ptCtx         - Message Encoder context
 * @param[out]  p_puRetrivedLen - Pointer to a uint32_t variable where the numbers of retrivable encoded data will be
 *                                placed
 *
 * @return      e_eFSP_MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eFSP_MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        e_eFSP_MSGE_RES_NOINITMESSAGE  - Need to start a message before taking some action
 *		        e_eFSP_MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              e_eFSP_MSGE_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGE_RES eFSP_MSGE_GetRemByteToGet(t_eFSP_MSGE_Ctx* const p_ptCtx, uint32_t* const p_puRetrivedLen);

/**
 * @brief       Retrive encoded data chunk. The raw data copied in the buffer using the function
 *              MSGE_GetPayloadLocation will be encoded (header and byte stuffing) and retrived by this function.
 *
 * @param[in]   p_ptCtx     - Message Encoder context
 * @param[in]   p_puEncBuff - Pointer to the destination area of encoded data that will be placed by this function
 * @param[in]   p_uMaxBufL  - Max fillable size of the destination area
 * @param[out]  p_puGettedL - Pointer to an uint32_t were we will store the number encoded data inserted in
 *                            p_puEncBuff. Note that if the function return e_eFSP_MSGE_RES_OK the value of p_puGettedL
 *                            will be equals to p_uMaxBufL. The value p_puGettedL infact could be lower than max dest
 *                            size only if some error is returned or if the frame is ended (
 *                            e_eFSP_MSGE_RES_MESSAGEENDED )
 *
 * @return      e_eFSP_MSGE_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eFSP_MSGE_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        e_eFSP_MSGE_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *		        e_eFSP_MSGE_RES_NOINITMESSAGE  - Need to start a message before taking some action
 *		        e_eFSP_MSGE_RES_CORRUPTCTX     - In case of an corrupted context
 *              e_eFSP_MSGE_RES_MESSAGEENDED   - No more data that we can elaborate, restart or start a new frame to
 *                                               proceed. This means that we have finished encoding the current frame.
 *                                               Keep in mind in this case that the value of p_puGettedL could be lower
 *                                               than p_uMaxBufL.
 *              e_eFSP_MSGE_RES_OK             - Operation ended correctly. This dosent mean that the encoding process
 *                                               is completed, but we can be sure that p_puGettedL will have the same
 *                                               value of p_uMaxBufL.
 */
e_eFSP_MSGE_RES eFSP_MSGE_GetEncChunk(t_eFSP_MSGE_Ctx* const p_ptCtx, uint8_t* p_puEncBuff, const uint32_t p_uMaxBufL,
                                      uint32_t* const p_puGettedL);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSP_MSGE_H */