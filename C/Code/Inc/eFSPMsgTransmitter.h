/**
 * @file eFSPMsgTransmitter.h
 *
 */

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
    cb_tx_msge      cbTxP;
    void*           cbTxCtx;
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
e_eFSP_MsgTx_Res msgTransmitterInitCtx(s_eFSP_MsgTxCtx* const ctx, const s_eFSP_MsgTxInitData* initData);













#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGTRANSMITTER_H */