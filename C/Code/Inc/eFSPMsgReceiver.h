/**
 * @file       eFSPMsgReceiver.h
 *
 * @brief      Message receiver utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSPMSGRECEIVER_H
#define EFSPMSGRECEIVER_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPType.h"
#include "eFSPMsgDecoder.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/

/* Call back to a function that will receive data.
 * the cntx parameter is a custom pointer that can be used by the creator of this TX callback, and will not be used
 * by the MSG RECEIVER module */
typedef bool_t (*cb_rx_msge) ( void* cntx, const uint8_t dataReceived[], uint32_t* const receivedLen,
                               const uint32_t maxDataRxSize, const uint32_t timeToReceive, uint32_t* const timeElaps );

typedef enum
{
    MSGRX_RES_OK = 0,
    MSGRX_RES_BADPARAM,
    MSGRX_RES_BADPOINTER,
	MSGRX_RES_CORRUPTCTX,
    MSGRX_RES_OUTOFMEM,
    MSGRX_RES_MESSAGERECEIVED,
    MSGRX_RES_MESSAGETIMEOUT,
    MSGRX_RES_NOINITLIB,
	MSGRX_RES_CRCCLBKERROR,
    MSGRX_RES_RXCLBKERROR,
}e_eFSP_MSGRX_Res;

typedef struct
{
    s_eFSP_MSGD_Ctx msgDecoderCtnx;
	uint8_t*        rxBuff;
	uint32_t        rxBuffSize;
	uint32_t        rxBuffCntr;
    uint32_t        rxBuffFill;
    cb_rx_msge      cbRxP;
    void*           cbRxCtx;
    uint32_t        timeCounterMs;
    uint32_t        frameTimeoutMs;
    uint32_t        timePerRecMs;
    bool_t          needWaitFrameStart;
    bool_t          waitingFrameStart;
}s_eFSP_MSGRX_Ctx;

typedef struct
{
    uint8_t*        i_memArea;
    uint32_t        i_memAreaSize;
    uint8_t*        i_receiveBuffArea;
    uint32_t        i_receiveBuffAreaSize;
    cb_crc32_msgd   i_cbCrcP;
    void*           i_cbCrcCrx;
    cb_rx_msge      i_cbRxP;
    void*           i_cbRxCtx;
    uint32_t        i_frameTimeoutMs;
    uint32_t        i_timePerRecMs;
    bool_t          i_needWaitFrameStart;
}s_eFSP_MSGRX_InitData;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the message receiver context
 *
 * @param[in]   ctx         - Msg receiver context
 * @param[in]   initData    - Init data
 *
 * @return      MSGRX_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGRX_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              MSGRX_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGRX_Res MSGRX_InitCtx(s_eFSP_MSGRX_Ctx* const ctx, const s_eFSP_MSGRX_InitData* initData);

/**
 * @brief       Start receiving a new message, loosing the previous stored decoded msg frame, but not the data in rx
 *              buffer, retrived toghether with the current discharged frame. If i_needWaitFrameStart is true the
 *              timeout will start counting after the first byte of the frame is received, otherwise it will start
 *              couting just after this function is called.
 *
 * @param[in]   ctx         - Msg receiver context
 *
 * @return      MSGRX_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGRX_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGRX_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGRX_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGRX_Res MSGRX_StartNewMsg(s_eFSP_MSGRX_Ctx* const ctx);

/**
 * @brief       Start receiving a new message, loosing the previous stored decoded msg frame, and cleaning rx buffer.
 *              If i_needWaitFrameStart is true the timeout will start counting after the first byte of the frame
 *              is received, otherwise it will start couting just after this function is called.
 *
 * @param[in]   ctx         - Msg receiver context
 *
 * @return      MSGRX_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGRX_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGRX_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGRX_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGRX_Res MSGRX_StartNewMsgNClean(s_eFSP_MSGRX_Ctx* const ctx);

/**
 * @brief       Retrive the pointer to the stored decoded data payload ( NO HEADER ), and the data size of the frame.
 *              Keep in mind that the message parsing could be ongoing, and if an error in the frame occour the
 *              retrivedLen could be setted to 0 again. We will retrive only payload size and no CRC + LEN header
 *
 * @param[in]   ctx         - Msg receiver context
 * @param[out]  dataP       - Pointer to a Pointer pointing to the decoded data payload ( NO CRC NO DATA SIZE )
 * @param[out]  retrivedLen - Pointer to a uint32_t variable where the size of the decoded data will be placed (raw
 *                            paylod data len )
 *
 * @return      MSGRX_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGRX_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGRX_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGRX_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGRX_Res MSGRX_GetDecodedData(s_eFSP_MSGRX_Ctx* const ctx, uint8_t** dataP, uint32_t* const retrivedLen);

/**
 * @brief       Receive encoded chunk that the alg will decode byte per byte
 *
 * @param[in]   ctx         	 - Msg receiver context
 * @param[in]   errRec         	 - Pointer to a variable where the number of detected protocol error where stored after
 *                                 every call.
 *
 * @return  MSGRX_RES_BADPOINTER   	    - In case of bad pointer passed to the function
 *		    MSGRX_RES_NOINITLIB    	    - Need to init context before taking some action
 *		    MSGRX_RES_BADPARAM     	    - In case of an invalid parameter passed to the function
 *		    MSGRX_RES_CORRUPTCTX   	    - In case of an corrupted context
 *          MSGRX_RES_OUTOFMEM     	    - Can not decode data, initial mem pointer was too small. The only way to
 *                                        resolve the issue is increasing the size of the memory area passed to init.
 *                                        Keep in mind that if there are multiple and frequent error in data tx or rx
 *                                        we could end in this situation when data size in frame is corrupted and
 *                                        interpreted higher that it should be and EOF + SOF of the next frame are lost.
 *                                        So this status could be due to a transmissione error, but it's not possible
 *                                        to know the reason of the error without storing all the data and checking CRC.
 *		    MSGRX_RES_MESSAGEENDED      - Frame ended, restart context in order to parse a new frame. Every other call
 *                                        to this function will not have effect until we call MSGRX_StartNewMsg or
 *                                        MSGRX_StartNewMsgNClean. In this situation bear in mind that some data could
 *                                        be left out the parsing and can remain saved inside a RX buffer. The only
 *                                        way to completly clean RX buffer is calling MSGRX_StartNewMsgNClean.
 *          MSGRX_RES_MESSAGETIMEOUT    - The message is not received before "i_frameTimeoutMs". Restart to continue.
 *		    MSGRX_RES_RXCLBKERROR       - Some error reported by the user receive function. Restart to continue.
 *		    MSGRX_RES_CRCCLBKERROR      - The crc callback returned an error when the decoder where verifing CRC
 *          MSGRX_RES_OK           	    - Operation ended correctly. The chunk is parsed correctly but the frame is not
 *                                        finished yet. This function return OK when the i_timePerRecMs timeout is
 *                                        reached, but i_frameTimeoutMs is not elapsed.
 */
e_eFSP_MSGRX_Res MSGRX_ReceiveChunk(s_eFSP_MSGRX_Ctx* const ctx, uint32_t* errRec);
#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGRECEIVER_H */