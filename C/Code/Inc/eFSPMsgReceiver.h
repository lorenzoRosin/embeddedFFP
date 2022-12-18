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
 * the p_ctx parameter is a custom pointer that can be used by the creator of this TX callback, and will not be used
 * by the MSG RECEIVER module */
typedef bool_t (*cb_rx_msge) ( void* p_ctx, uint8_t a_dataReceived[], uint32_t* const p_receivedLen,
                               const uint32_t maxDataRxSize, const uint32_t timeToReceive);

/* Call backs to functions that act as a timer */
typedef bool_t (*cb_rx_tim_start) ( void* p_ctx, const uint32_t timeoutVal );
typedef bool_t (*cb_rx_tim_getRemaining) ( void* p_ctx, uint32_t* const p_remainings );

typedef struct
{
    void*                   timerCtx;
    cb_rx_tim_start         tim_start;
    cb_rx_tim_getRemaining  tim_getRemaining;
}s_eFSP_RXTIMER;

typedef enum
{
    MSGRX_RES_OK = 0,
    MSGRX_RES_BADPARAM,
    MSGRX_RES_BADPOINTER,
	MSGRX_RES_CORRUPTCTX,
    MSGRX_RES_OUTOFMEM,
    MSGRX_RES_BADFRAME,
    MSGRX_RES_FRAMERESTART,
    MSGRX_RES_MESSAGERECEIVED,
    MSGRX_RES_MESSAGETIMEOUT,
    MSGRX_RES_NOINITLIB,
	MSGRX_RES_CRCCLBKERROR,
    MSGRX_RES_RXCLBKERROR,
    MSGRX_RES_TIMCLBKERROR
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
    s_eFSP_RXTIMER  rxTim;
    uint32_t        frameTimeoutMs;
    uint32_t        timePerRecMs;
    bool_t          needWaitFrameStart;
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
    s_eFSP_RXTIMER  i_rxTim;
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
 * @param[in]   p_ctx         - Msg receiver context
 * @param[in]   p_initData    - Init data
 *
 * @return      MSGRX_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGRX_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              MSGRX_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGRX_Res eFSP_MSGRX_InitCtx(s_eFSP_MSGRX_Ctx* const p_ctx, const s_eFSP_MSGRX_InitData* p_initData);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ctx         - Msg receiver context
 * @param[out]  p_isInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      MSGRX_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              MSGRX_RES_OK            - Operation ended correctly
 */
e_eFSP_MSGRX_Res eFSP_MSGRX_IsInit(s_eFSP_MSGRX_Ctx* const p_ctx, bool_t* p_isInit);

/**
 * @brief       Start receiving a new message, loosing the previous stored decoded msg frame, but not the data in rx
 *              buffer, retrived toghether with the current discharged frame. If i_needWaitFrameStart is true the
 *              timeout will start counting after the first byte of the frame is received, otherwise it will start
 *              couting just after this function is called.
 *
 * @param[in]   p_ctx         - Msg receiver context
 *
 * @return      MSGRX_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGRX_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGRX_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGRX_RES_TIMCLBKERROR  - The timer function returned an error
 *              MSGRX_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGRX_Res eFSP_MSGRX_NewMsg(s_eFSP_MSGRX_Ctx* const p_ctx);

/**
 * @brief       Start receiving a new message, loosing the previous stored decoded msg frame, and cleaning rx buffer.
 *              If i_needWaitFrameStart is true the timeout will start counting after the first byte of the frame
 *              is received, otherwise it will start couting just after this function is called.
 *
 * @param[in]   p_ctx         - Msg receiver context
 *
 * @return      MSGRX_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGRX_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGRX_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGRX_RES_TIMCLBKERROR  - The timer function returned an error
 *              MSGRX_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGRX_Res eFSP_MSGRX_NewMsgNClean(s_eFSP_MSGRX_Ctx* const p_ctx);

/**
 * @brief       Retrive the pointer to the stored decoded data payload ( NO HEADER ), and the data size of the frame.
 *              Keep in mind that the message parsing could be ongoing, and if an error in the frame occour the
 *              p_GetLen could be setted to 0 again. We will retrive only payload size and no CRC + LEN header
 *
 * @param[in]   p_ctx         - Msg receiver context
 * @param[out]  pp_data       - Pointer to a Pointer pointing to the decoded data payload ( NO CRC NO DATA SIZE )
 * @param[out]  p_GetLen      - Pointer to a uint32_t variable where the size of the decoded data will be placed ( raw
 *                              paylod data len )
 *
 * @return      MSGRX_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGRX_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGRX_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGRX_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGRX_Res eFSP_MSGRX_GetDecodedData(s_eFSP_MSGRX_Ctx* const p_ctx, uint8_t** pp_data, uint32_t* const p_GetLen);

/**
 * @brief       Receive encoded chunk that the alg will decode byte per byte.
 *              The whole message can be received calling multiple times this function. Eache time this function will
 *              try to read all the data that can be readed in "i_timePerRecMs". The whole frame instead can be received
 *              in "i_frameTimeoutMs" milliseconds. This function can return different status, but if we keep call
 *              this function even after i_frameTimeoutMs it will start returning only MSGRX_RES_MESSAGETIMEOUT.
 *              If the flag i_needWaitFrameStart is true the timeout is started in the moment we receive the
 *              start of frame (if another SOF is received the timeout is reloaded). If the flag i_needWaitFrameStart
 *              is false the timeout is started in the moment we call the function eFSP_MSGRX_NewMsg or
 *              MSGRX_StartNewMsgNClean is called.
 *
 * @param[in]   p_ctx         	 - Msg receiver context
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
 *		    MSGRX_RES_MESSAGERECEIVED   - Frame ended, restart context in order to parse a new frame. Every other call
 *                                        to this function will not have effect until we call eFSP_MSGRX_NewMsg,
 *                                        MSGRX_StartNewMsgNClean or timeout elapse. In this situation bear in mind
 *                                        that some data could be left out the parsing and can remain saved inside a
 *                                        RX buffer. The onlyway to completly clean RX buffer is calling
 *                                        MSGRX_StartNewMsgNClean.
 *          MSGRX_RES_MESSAGETIMEOUT    - The message is not received before "i_frameTimeoutMs". Restart to continue.
 *          MSGRX_RES_BADFRAME          - Found an error while parsing, the frame passed is invalid.
 *                                        Restart context in order to parse a new frame. Every other call
 *                                        to this function will not have effect until we call eFSP_MSGRX_NewMsg or
 *                                        timeout elapse. In this situation bear in mind that some data could be left
 *                                        out the parsing and can remain saved inside a RX buffer.
 *          MSGRX_RES_FRAMERESTART      - During frame receiving another start of frame is received. In this situation
 *                                        clear old data and restart the frame, witouth the need to call any other
 *                                        function. In this situation bear in mind that some data could be left
 *                                        out the parsing and can remain saved inside a RX buffer.
 *		    MSGRX_RES_RXCLBKERROR       - Some error reported by the user receive function. Restart to continue.
 *		    MSGRX_RES_CRCCLBKERROR      - The crc callback returned an error when the decoder where verifing CRC
 *          MSGRX_RES_TIMCLBKERROR      - The timer function returned an error
 *          MSGRX_RES_OK           	    - Operation ended correctly. The chunk is parsed correctly but the frame is not
 *                                        finished yet. This function return OK when the i_timePerRecMs timeout is
 *                                        reached, but i_frameTimeoutMs is not elapsed.
 */
e_eFSP_MSGRX_Res eFSP_MSGRX_ReceiveChunk(s_eFSP_MSGRX_Ctx* const p_ctx);
#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGRECEIVER_H */