/**
 * @file       eFSPMsgTransmitter.h
 *
 * @brief      Message transmitter utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSPMSGTRANSMITTER_H
#define EFSPMSGTRANSMITTER_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPType.h"
#include "eFSPMsgEncoder.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/

/* Call back to a function that will trasmit the passed data.
 * the cntx parameter is a custom pointer that can be used by the creator of this TX callback, and will not be used
 * by the MSG TRANSMITTER module */
typedef bool_t (*cb_tx_msge) ( void* cntx, const uint8_t dataToSend[], const uint32_t dataToSendLen,
                               uint32_t* const dataSended, const uint32_t timeToSend, uint32_t* const timeElapsed );

typedef enum
{
    MSGTTX_RES_OK = 0,
    MSGTTX_RES_BADPARAM,
    MSGTTX_RES_BADPOINTER,
	MSGTTX_RES_CORRUPTCTX,
    MSGTTX_RES_MESSAGESENDED,
    MSGTTX_RES_MESSAGETIMEOUT,
    MSGTTX_RES_NOINITLIB,
    MSGTTX_RES_NOINITMESSAGE,
	MSGTTX_RES_CRCCLBKERROR,
    MSGTTX_RES_TXCLBKERROR,
}e_eFSP_MsgTx_Res;

typedef struct
{
    s_eFSP_MsgECtx  msgEncoderCtnx;
	uint8_t*        sendBuff;
	uint32_t        sendBuffSize;
	uint32_t        sendBuffCntr;
    uint32_t        sendBuffFill;
    cb_tx_msge      cbTxP;
    void*           cbTxCtx;
    uint32_t        timeCounterMs;
    uint32_t        frameTimeoutMs;
    uint32_t        timePerSendMs;
}s_eFSP_MsgTxCtx;

typedef struct
{
    uint8_t*        i_memArea;
    uint32_t        i_memAreaSize;
    uint8_t*        i_sendBuffArea;
    uint32_t        i_sendBuffAreaSize;
    cb_crc32_msge   i_cbCrcP;
    void*           i_cbCrcCtx;
    cb_tx_msge      i_cbTxP;
    void*           i_cbTxCtx;
    uint32_t        i_frameTimeoutMs;
    uint32_t        i_timePerSendMs;
}s_eFSP_MsgTxInitData;


/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the message transmitter context
 *
 * @param[in]   ctx         - Message transmitter context
 * @param[in]   initData    - Init data
 *
 * @return      MSGTTX_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        MSGTTX_RES_BADPARAM     - In case of an invalid parameter passed to the function
 *              MSGTTX_RES_OK           - Operation ended correctly
 */
e_eFSP_MsgTx_Res msgTransmInitCtx(s_eFSP_MsgTxCtx* const ctx, const s_eFSP_MsgTxInitData* initData);

/**
 * @brief       Start to encode a new msg given the dimension of raw payload it self. This function suppouse that
 *              data payload that need to be encoded were already copied in memory.( see msgTransmGetPayloadLocation
 *              in order to know how get the data pointer, and copy the data )
 *
 * @param[in]   ctx         - Message Transmitter context
 * @param[in]   messageLen  - lenght of the raw payload present in the frame that we need to encode ( no header )
 *
 * @return      MSGTTX_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGTTX_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *		        MSGTTX_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGTTX_RES_CORRUPTCTX     - In case of an corrupted context
 *				MSGTTX_RES_CRCCLBKERROR   - The crc callback function returned an error
 *              MSGTTX_RES_OK             - Operation ended correctly
 */
e_eFSP_MsgTx_Res msgTransmStartNewMessage(s_eFSP_MsgTxCtx* const ctx, const uint32_t messageLen);

/**
 * @brief       Retrive the pointer of the buffer that the user can use to insert data payload that need to be encoded
 *
 * @param[in]   ctx         - Message Transmitter context
 * @param[out]  dataP       - Pointer to a Pointer where the raw data needs to be copied before starting a message
 * @param[out]  maxDataSize - Pointer to a uint32_t variable where the max number of data that can be copied in dataP
 *                            will be placed
 *
 * @return      MSGTTX_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGTTX_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGTTX_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGTTX_RES_OK             - Operation ended correctly
 */
e_eFSP_MsgTx_Res msgTransmGetPayloadLocation(s_eFSP_MsgTxCtx* const ctx, uint8_t** dataP, uint32_t* const maxDataSize);

/**
 * @brief       Restart to encode and send the already passed payload/the current frame
 *
 * @param[in]   ctx         - Message Transmitter context
 *
 * @return      MSGTTX_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGTTX_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGTTX_RES_NOINITMESSAGE  - Need to start a message before restarting it
 *		        MSGTTX_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGTTX_RES_OK             - Operation ended correctly
 */
e_eFSP_MsgTx_Res msgTransmRestartCurrentMessage(s_eFSP_MsgTxCtx* const ctx);

/**
 * @brief       Send encoded data chunk. The raw data copied in the buffer by using the function
 *              msgTransmGetPayloadLocation will be encoded (header and byte stuffing) and sended by this function.
 *              The whole message can be sended calling multiple times this function. Eache time this function will
 *              try to send all the data that can be send in "timePerSendMs" variable.
 *
 * @param[in]   ctx         - Message Transmitter context
 *
 * @return      MSGTTX_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGTTX_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGTTX_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *		        MSGTTX_RES_NOINITMESSAGE  - Need to start a message before taking some action
 *		        MSGTTX_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGTTX_RES_MESSAGESENDED  - No more data that we can send, restart or start a new msg to proceed.
 *                                          This means that we have finished sending the current message.
 *              MSGTTX_RES_MESSAGETIMEOUT - The message is not sended before "frameTimeoutMs". Restart to continue.
 *              MSGTTX_RES_TXCLBKERROR    - Some error reported by the user send function. Restart to continue.
 *              MSGTTX_RES_OK             - Operation ended correctly, message is not still fully encoded
 */
e_eFSP_MsgTx_Res msgTransmSendChunk(s_eFSP_MsgTxCtx* const ctx);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGTRANSMITTER_H */