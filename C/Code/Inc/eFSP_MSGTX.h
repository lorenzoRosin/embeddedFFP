/**
 * @file       eFSP_MSGTX.h
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
#include "eFSP_TYPE.h"
#include "eFSP_MSGE.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/

/* Call back to a function that will trasmit the passed data.
 * the p_ctx parameter is a custom pointer that can be used by the creator of this TX callback, and will not be used
 * by the MSG TRANSMITTER module */
typedef bool_t (*cb_tx_msge) ( void* p_ctx, const uint8_t* p_dataToSend, const uint32_t dataToSendLen,
                               uint32_t* const p_dataSended, const uint32_t timeToSend );

/* Call backs to functions that act as a timer */
typedef bool_t (*cb_tx_tim_start) ( void* p_ctx, const uint32_t timeoutVal );
typedef bool_t (*cb_tx_tim_getRemaining) ( void* p_ctx, uint32_t* const p_remainings );

typedef struct
{
    void*                   p_timCtx;
    cb_tx_tim_start         f_timStart;
    cb_tx_tim_getRemaining  f_timGetRemaining;
}s_eFSP_TXTIMER;

typedef enum
{
    MSGTX_RES_OK = 0,
    MSGTX_RES_BADPARAM,
    MSGTX_RES_BADPOINTER,
	MSGTX_RES_CORRUPTCTX,
    MSGTX_RES_MESSAGESENDED,
    MSGTX_RES_MESSAGETIMEOUT,
    MSGTX_RES_NOINITLIB,
    MSGTX_RES_NOINITMESSAGE,
	MSGTX_RES_CRCCLBKERROR,
    MSGTX_RES_TXCLBKERROR,
    MSGTX_RES_TIMCLBKERROR
}e_eFSP_MSGTX_Res;

typedef struct
{
    s_eFSP_MSGE_Ctx msge_Ctx;
	uint8_t*        p_rxBuff;
	uint32_t        txBuffSize;
	uint32_t        txBuffCntr;
    uint32_t        txBuffFill;
    cb_tx_msge      f_Tx;
    void*           p_TxCtx;
    s_eFSP_TXTIMER  txTim;
    uint32_t        timeoutMs;
    uint32_t        timePerSendMs;
}s_eFSP_MSGTX_Ctx;

typedef struct
{
    uint8_t*        p_i_memArea;
    uint32_t        i_memAreaSize;
    uint8_t*        p_i_txBuffArea;
    uint32_t        i_txBuffAreaSize;
    cb_crc32_msge   f_i_Crc;
    void*           p_i_cbCrcCtx;
    cb_tx_msge      f_i_Tx;
    void*           p_i_cbTxCtx;
    s_eFSP_TXTIMER  i_txTim;
    uint32_t        i_timeoutMs;
    uint32_t        i_timePerSendMs;
}s_eFSP_MSGTX_InitData;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the message transmitter context
 *
 * @param[in]   p_ctx         - Message transmitter context
 * @param[in]   p_initData    - Init data
 *
 * @return      MSGTX_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        MSGTX_RES_BADPARAM     - In case of an invalid parameter passed to the function
 *              MSGTX_RES_OK           - Operation ended correctly
 */
e_eFSP_MSGTX_Res eFSP_MSGTX_InitCtx(s_eFSP_MSGTX_Ctx* const p_ctx, const s_eFSP_MSGTX_InitData* p_initData);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ctx         - Message transmitter context
 * @param[out]  p_isInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      MSGTX_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              MSGTX_RES_OK            - Operation ended correctly
 */
e_eFSP_MSGTX_Res eFSP_MSGTX_IsInit(s_eFSP_MSGTX_Ctx* const p_ctx, bool_t* p_isInit);

/**
 * @brief       Retrive the pointer of the buffer that the user can use to insert data payload that need to be encoded
 *
 * @param[in]   p_ctx         - Message Transmitter context
 * @param[out]  pp_data       - Pointer to a Pointer where the raw data needs to be copied before starting a message
 * @param[out]  p_maxDL       - Pointer to a uint32_t variable where the max number of data that can be copied in
 *                              pp_data will be placed
 *
 * @return      MSGTX_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGTX_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGTX_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGTX_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGTX_Res eFSP_MSGTX_GetWherePutData(s_eFSP_MSGTX_Ctx* const p_ctx, uint8_t** pp_data, uint32_t* const p_maxDL);

/**
 * @brief       Start to encode a new msg given the dimension of raw payload it self. This function suppouse that
 *              data payload that need to be encoded were already copied in memory.( see MSGTX_GetPayloadLocation
 *              in order to know how get the data pointer, and copy the data )
 *
 * @param[in]   p_ctx         - Message Transmitter context
 * @param[in]   messageLen    - lenght of the raw payload present in the frame that we need to encode ( no header )
 *
 * @return      MSGTX_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGTX_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *		        MSGTX_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGTX_RES_CORRUPTCTX     - In case of an corrupted context
 *				MSGTX_RES_CRCCLBKERROR   - The crc callback function returned an error
 *              MSGTX_RES_TIMCLBKERROR   - The timer function returned an error
 *              MSGTX_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGTX_Res eFSP_MSGTX_NewMessage(s_eFSP_MSGTX_Ctx* const p_ctx, const uint32_t messageLen);

/**
 * @brief       Restart to encode and send the already passed payload/the current frame
 *
 * @param[in]   p_ctx         - Message Transmitter context
 *
 * @return      MSGTX_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGTX_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGTX_RES_NOINITMESSAGE  - Need to start a message before restarting it
 *		        MSGTX_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGTX_RES_TIMCLBKERROR   - The timer function returned an error
 *              MSGTX_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGTX_Res eFSP_MSGTX_RestartMessage(s_eFSP_MSGTX_Ctx* const p_ctx);

/**
 * @brief       Send encoded data chunk. The raw data copied in the buffer by using the function
 *              MSGTX_GetPayloadLocation will be encoded (header and byte stuffing) and sended by this function.
 *              The whole message can be sended calling multiple times this function. Eache time this function will
 *              try to send all the data that can be send in "i_timePerSendMs". The whole frame instead can be sended
 *              in "i_timeoutMs" milliseconds. This function can return different status, but if we keep call
 *              this function even after i_timeoutMs it will start returning only MSGTX_RES_MESSAGETIMEOUT.
 *
 * @param[in]   p_ctx         - Message Transmitter context
 *
 * @return      MSGTX_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGTX_RES_NOINITLIB      - Need to init the data encoder context before taking some action
 *		        MSGTX_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *		        MSGTX_RES_NOINITMESSAGE  - Need to start a message before taking some action
 *		        MSGTX_RES_CORRUPTCTX     - In case of an corrupted context
 *              MSGTX_RES_MESSAGESENDED  - No more data that we can send, restart or start a new msg to proceed.
 *                                         This means that we have finished sending the current message, without
 *                                         any particular error or timeout.
 *              MSGTX_RES_MESSAGETIMEOUT - The message is not sended before "i_timeoutMs". Restart to continue.
 *              MSGTX_RES_TXCLBKERROR    - Some error reported by the user send function. Restart to continue.
 *              MSGTX_RES_TIMCLBKERROR   - The timer function returned an error
 *              MSGTX_RES_OK             - Operation ended correctly, message is not fully sended. This happnes
 *                                         when the whole message wasn't sended in "i_timePerSendMs" millisecond, but
 *                                         the "i_timeoutMs" timeout is still not reached. Call this function again
 *                                         to send another chunk of data.
 */
e_eFSP_MSGTX_Res eFSP_MSGTX_SendChunk(s_eFSP_MSGTX_Ctx* const p_ctx);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGTRANSMITTER_H */