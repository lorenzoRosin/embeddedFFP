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
                               const uint32_t maxDataRxSize, const uint32_t timeToReceive, uint32_t* const timeElapsed );

typedef enum
{
    MSGTRX_RES_OK = 0,
    MSGTRX_RES_BADPARAM,
    MSGTRX_RES_BADPOINTER,
	MSGTRX_RES_CORRUPTCTX,
    MSGTRX_RES_OUTOFMEM,
    MSGTRX_RES_MESSAGERECEIVED,
    MSGTRX_RES_MESSAGETIMEOUT,
    MSGTRX_RES_NOINITLIB,
	MSGTRX_RES_CRCCLBKERROR,
    MSGTRX_RES_RXCLBKERROR,
}e_eFSP_MsgRx_Res;

typedef struct
{
    s_eFSP_MsgDCtx  msgDecoderCtnx;
	uint8_t*        rxBuff;
	uint32_t        rxBuffSize;
	uint32_t        rxBuffCntr;
    uint32_t        rxBuffFill;
    cb_rx_msge      cbRxP;
    void*           cbRxCtx;
    uint32_t        timeCounterMs;
    uint32_t        frameTimeoutMs;
    uint32_t        timePerRecMs;
}s_eFSP_MsgRxCtx;

typedef struct
{
    uint8_t*        i_memArea;
    uint32_t        i_memAreaSize;
    uint8_t*        i_sendBuffArea;
    uint32_t        i_sendBuffAreaSize;
    cb_crc32_msgd   i_cbCrcP;
    void*           i_cbCrcCrx;
    cb_rx_msge      i_cbRxP;
    void*           i_cbRxCtx;
    uint32_t        i_frameTimeoutMs;
    uint32_t        i_timePerRecMs;
}s_eFSP_MsgRxInitData;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the message receiver context
 *
 * @param[in]   ctx         - Msg receiver context
 * @param[in]   initData    - Init data
 *
 * @return      MSGTRX_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGTRX_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              MSGTRX_RES_OK             - Operation ended correctly
 */
e_eFSP_MsgRx_Res msgReceiverInitCtx(s_eFSP_MsgRxCtx* const ctx, const s_eFSP_MsgRxInitData* initData);

/**
 * @brief       Start receiving a new message, loosing the previous stored decoded msg frame, but not the data in rx
 *              buffer, retrived toghether with the current discharged frame
 *
 * @param[in]   ctx         - Msg receiver context
 *
 * @return      MSGTRX_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGTRX_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGTRX_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGTRX_RES_OK           	- Operation ended correctly
 */
e_eFSP_MsgRx_Res msgReceiverStartNewMsg(s_eFSP_MsgRxCtx* const ctx);

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
 * @return      MSGTRX_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGTRX_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGTRX_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGTRX_RES_OK           	- Operation ended correctly
 */
e_eFSP_MsgRx_Res msgReceiverGetDecodedData(s_eFSP_MsgRxCtx* const ctx, uint8_t** dataP, uint32_t* const retrivedLen);

/**
 * @brief       Receive encoded chunk that the alg will decode byte per byte
 *
 * @param[in]   ctx         	 - Msg receiver context
 * @param[in]   encArea     	 - Pointer to the encoded Data that we will decode
 * @param[in]   encLen      	 - Size of the encArea
 * @param[out]  consumedEncData  - Pointer to an uint32_t were we will store how many encoded data byte has been
 *                                 analized. keep in mind that unalized data were not decoded and will need to be
 *                                 reparsed
 * @param[out]  errSofRec        - Pointer to an uint32_t were we will store how many protocol error were detected.
 *                                 Even with some error detected, the protocol will continue parsing data discharging
 *                                 error.
 *
 * @return  MSGTRX_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		    MSGTRX_RES_NOINITLIB    	- Need to init context before taking some action
 *		    MSGTRX_RES_BADPARAM     	- In case of an invalid parameter passed to the function
 *		    MSGTRX_RES_CORRUPTCTX   	- In case of an corrupted context
 *          MSGTRX_RES_OUTOFMEM     	- Can not decode data, initial mem pointer was too small. The only way to
 *                                        resolve the issue is increasing the size of the memory area passed to init.
 *                                        Keep in mind that if there are multiple and frequent error in data tx or rx
 *                                        we could end in this situation when data size in frame is corrupted and
 *                                        interpreted higher that it should be and EOF + SOF of the next frame are lost.
 *                                        So this status could be due to a transmissione error, but it's not possible
 *                                        to know the reason of the error without storing all the data and checking CRC.
 *		    MSGTRX_RES_MESSAGEENDED  	- Frame ended, restart context in order to parse a new frame. Every other call
 *                                        to this function will not have effect until we call msgDecoderStartNewMsg.
 *                                        In this situation bear in mind that some data could be left out the parsing.
 *          MSGTRX_RES_MESSAGETIMEOUT   - The message is not received before "frameTimeoutMs". Restart to continue.
 *		    MSGTRX_RES_RXCLBKERROR      - Some error reported by the user receive function. Restart to continue.
 *		    MSGTRX_RES_CRCCLBKERROR     - The crc callback returned an error when the decoder where verifing CRC
 *          MSGTRX_RES_OK           	- Operation ended correctly. The chunk is parsed correclty but the frame is not
 *                                        finished yet
 */
e_eFSP_MsgRx_Res msgReceiverInsEncChunk(s_eFSP_MsgRxCtx* const ctx, const uint8_t* encArea, const uint32_t encLen,
                                      uint32_t* const consumedEncData, uint32_t* errSofRec);
#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGRECEIVER_H */