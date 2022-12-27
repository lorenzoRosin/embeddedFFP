/**
 * @file       eFSP_MSGRX.h
 *
 * @brief      Message receiver utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSP_MSGRX_H
#define EFSP_MSGRX_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_TYPE.h"
#include "eFSP_MSGD.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
/* Define a generic receiver func callback context that must be implemented by the user */
typedef struct t_eFSP_MSGRX_RxCtxUser t_eFSP_MSGRX_RxCtx;

/* Define a generic timer func callback context that must be implemented by the user */
typedef struct t_eFSP_MSGRX_TimCtxUser t_eFSP_MSGRX_TimCtx;

/* Call back to a function that will receive data.
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this TX callback, and will not be used
 * by the MSG RECEIVER module */
typedef bool_t (*f_eFSP_MSGRX_RxCb) ( t_eFSP_MSGRX_RxCtx* p_ptCtx, uint8_t* p_puDataRx, uint32_t* const p_puDataRxL,
                                      const uint32_t p_uCaxDataRxL, const uint32_t p_uTimeRx);

/* Call backs to functions that act as a timer */
typedef bool_t (*f_eFSP_MSGRX_TimStart) ( t_eFSP_MSGRX_TimCtx* p_ptCtx, const uint32_t p_uTimeout );
typedef bool_t (*f_eFSP_MSGRX_TimGetRemaing) ( t_eFSP_MSGRX_TimCtx* p_ptCtx, uint32_t* const p_puRemain );

typedef struct
{
    t_eFSP_MSGRX_TimCtx*        ptTimCtx;
    f_eFSP_MSGRX_TimStart       fTimStart;
    f_eFSP_MSGRX_TimGetRemaing  fTimGetRemain;
}t_eFSP_MSGRX_Timer;

typedef enum
{
    e_eFSP_MSGRX_RES_OK = 0,
    e_eFSP_MSGRX_RES_BADPARAM,
    e_eFSP_MSGRX_RES_BADPOINTER,
	e_eFSP_MSGRX_RES_CORRUPTCTX,
    e_eFSP_MSGRX_RES_OUTOFMEM,
    e_eFSP_MSGRX_RES_BADFRAME,
    e_eFSP_MSGRX_RES_FRAMERESTART,
    e_eFSP_MSGRX_RES_MESSAGERECEIVED,
    e_eFSP_MSGRX_RES_MESSAGETIMEOUT,
    e_eFSP_MSGRX_RES_NOINITLIB,
	e_eFSP_MSGRX_RES_CRCCLBKERROR,
    e_eFSP_MSGRX_RES_RXCLBKERROR,
    e_eFSP_MSGRX_RES_TIMCLBKERROR
}e_eFSP_MSGRX_RES;

typedef struct
{
    t_eFSP_MSGD_Ctx msgd_Ctx;
	uint8_t*        p_rxBuff;
	uint32_t        rxBuffSize;
	uint32_t        rxBuffCntr;
    uint32_t        rxBuffFill;
    f_eFSP_MSGRX_RxCb      f_Rx;
    t_eFSP_MSGRX_RxCtx*           p_RxCtx;
    t_eFSP_MSGRX_Timer  rxTim;
    uint32_t        timeoutMs;
    uint32_t        timePerRecMs;
    bool_t          needWaitFrameStart;
}t_eFSP_MSGRX_Ctx;

typedef struct
{
    uint8_t*            puIMemArea;
    uint32_t            uIMemAreaL;
    uint8_t*            puIRxBuffArea;
    uint32_t            uIRxBuffAreaL;
    f_eFSP_MSGD_CrcCb   fICrc;
    t_eFSP_MSGD_CrcCtx*               ptICbCrcCtx;
    f_eFSP_MSGRX_RxCb   fIRx;
    t_eFSP_MSGRX_RxCtx*               ptICbRxCtx;
    t_eFSP_MSGRX_Timer  tIRxTim;
    uint32_t            uITimeoutMs;
    uint32_t            uITimePerRecMs;
    bool_t              bINeedWaitFrameStart;
}t_eFSP_MSGRX_InitData;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the message receiver context
 *
 * @param[in]   p_ptCtx         - Msg receiver context
 * @param[in]   p_ptInitData    - Init data
 *
 * @return      e_eFSP_MSGRX_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eFSP_MSGRX_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              e_eFSP_MSGRX_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGRX_RES eFSP_MSGRX_InitCtx(t_eFSP_MSGRX_Ctx* const p_ptCtx, const t_eFSP_MSGRX_InitData* p_ptInitData);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Msg receiver context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSP_MSGRX_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSP_MSGRX_RES_OK            - Operation ended correctly
 */
e_eFSP_MSGRX_RES eFSP_MSGRX_IsInit(t_eFSP_MSGRX_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Start receiving a new message, loosing the previous stored decoded msg frame, but not the data in rx
 *              buffer, retrived toghether with the current discharged frame. If bINeedWaitFrameStart is true the
 *              timeout will start counting after the first byte of the frame is received, otherwise it will start
 *              couting just after this function is called.
 *
 * @param[in]   p_ptCtx         - Msg receiver context
 *
 * @return      e_eFSP_MSGRX_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        e_eFSP_MSGRX_RES_NOINITLIB    	- Need to init context before taking some action
 *		        e_eFSP_MSGRX_RES_CORRUPTCTX   	- In case of an corrupted context
 *              e_eFSP_MSGRX_RES_TIMCLBKERROR  - The timer function returned an error
 *              e_eFSP_MSGRX_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGRX_RES eFSP_MSGRX_NewMsg(t_eFSP_MSGRX_Ctx* const p_ptCtx);

/**
 * @brief       Start receiving a new message, loosing the previous stored decoded msg frame, and cleaning rx buffer.
 *              If bINeedWaitFrameStart is true the timeout will start counting after the first byte of the frame
 *              is received, otherwise it will start couting just after this function is called.
 *
 * @param[in]   p_ptCtx         - Msg receiver context
 *
 * @return      e_eFSP_MSGRX_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        e_eFSP_MSGRX_RES_NOINITLIB    	- Need to init context before taking some action
 *		        e_eFSP_MSGRX_RES_CORRUPTCTX   	- In case of an corrupted context
 *              e_eFSP_MSGRX_RES_TIMCLBKERROR  - The timer function returned an error
 *              e_eFSP_MSGRX_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGRX_RES eFSP_MSGRX_NewMsgNClean(t_eFSP_MSGRX_Ctx* const p_ptCtx);

/**
 * @brief       Retrive the pointer to the stored decoded data payload ( NO HEADER ), and the data size of the frame.
 *              Keep in mind that the message parsing could be ongoing, and if an error in the frame occour the
 *              p_puGetL could be setted to 0 again. We will retrive only payload size and no CRC + LEN header
 *
 * @param[in]   p_ptCtx         - Msg receiver context
 * @param[out]  p_ppuData       - Pointer to a Pointer pointing to the decoded data payload ( NO CRC NO DATA SIZE )
 * @param[out]  p_puGetL      - Pointer to a uint32_t variable where the size of the decoded data will be placed ( raw
 *                              paylod data len )
 *
 * @return      e_eFSP_MSGRX_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        e_eFSP_MSGRX_RES_NOINITLIB    	- Need to init context before taking some action
 *		        e_eFSP_MSGRX_RES_CORRUPTCTX   	- In case of an corrupted context
 *              e_eFSP_MSGRX_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGRX_RES eFSP_MSGRX_GetDecodedData(t_eFSP_MSGRX_Ctx* const p_ptCtx, uint8_t** p_ppuData, uint32_t* const p_puGetL);

/**
 * @brief       Receive encoded chunk that the alg will decode byte per byte.
 *              The whole message can be received calling multiple times this function. Eache time this function will
 *              try to read all the data that can be readed in "uITimePerRecMs". The whole frame instead can be received
 *              in "uITimeoutMs" milliseconds. This function can return different status, but if we keep call
 *              this function even after uITimeoutMs it will start returning only e_eFSP_MSGRX_RES_MESSAGETIMEOUT.
 *              If the flag bINeedWaitFrameStart is true the timeout is started in the moment we receive the
 *              start of frame (if another SOF is received the timeout is reloaded). If the flag bINeedWaitFrameStart
 *              is false the timeout is started in the moment we call the function eFSP_MSGRX_NewMsg or
 *              MSGRX_StartNewMsgNClean is called.
 *
 * @param[in]   p_ptCtx         	 - Msg receiver context
 *
 * @return  e_eFSP_MSGRX_RES_BADPOINTER   	    - In case of bad pointer passed to the function
 *		    e_eFSP_MSGRX_RES_NOINITLIB    	    - Need to init context before taking some action
 *		    e_eFSP_MSGRX_RES_BADPARAM     	    - In case of an invalid parameter passed to the function
 *		    e_eFSP_MSGRX_RES_CORRUPTCTX   	    - In case of an corrupted context
 *          e_eFSP_MSGRX_RES_OUTOFMEM     	    - Can not decode data, initial mem pointer was too small. The only way to
 *                                        resolve the issue is increasing the size of the memory area passed to init.
 *                                        Keep in mind that if there are multiple and frequent error in data tx or rx
 *                                        we could end in this situation when data size in frame is corrupted and
 *                                        interpreted higher that it should be and EOF + SOF of the next frame are lost.
 *                                        So this status could be due to a transmissione error, but it's not possible
 *                                        to know the reason of the error without storing all the data and checking CRC.
 *		    e_eFSP_MSGRX_RES_MESSAGERECEIVED   - Frame ended, restart context in order to parse a new frame. Every other call
 *                                        to this function will not have effect until we call eFSP_MSGRX_NewMsg,
 *                                        MSGRX_StartNewMsgNClean or timeout elapse. In this situation bear in mind
 *                                        that some data could be left out the parsing and can remain saved inside a
 *                                        RX buffer. The onlyway to completly clean RX buffer is calling
 *                                        MSGRX_StartNewMsgNClean.
 *          e_eFSP_MSGRX_RES_MESSAGETIMEOUT    - The message is not received before "uITimeoutMs". Restart to continue.
 *          e_eFSP_MSGRX_RES_BADFRAME          - Found an error while parsing, the frame passed is invalid.
 *                                        Restart context in order to parse a new frame. Every other call
 *                                        to this function will not have effect until we call eFSP_MSGRX_NewMsg or
 *                                        timeout elapse. In this situation bear in mind that some data could be left
 *                                        out the parsing and can remain saved inside a RX buffer.
 *          e_eFSP_MSGRX_RES_FRAMERESTART      - During frame receiving another start of frame is received. In this situation
 *                                        clear old data and restart the frame, witouth the need to call any other
 *                                        function. In this situation bear in mind that some data could be left
 *                                        out the parsing and can remain saved inside a RX buffer.
 *		    e_eFSP_MSGRX_RES_RXCLBKERROR       - Some error reported by the user receive function. Restart to continue.
 *		    e_eFSP_MSGRX_RES_CRCCLBKERROR      - The crc callback returned an error when the decoder where verifing CRC
 *          e_eFSP_MSGRX_RES_TIMCLBKERROR      - The timer function returned an error
 *          e_eFSP_MSGRX_RES_OK           	    - Operation ended correctly. The chunk is parsed correctly but the frame is not
 *                                        finished yet. This function return OK when the uITimePerRecMs timeout is
 *                                        reached, but uITimeoutMs is not elapsed.
 */
e_eFSP_MSGRX_RES eFSP_MSGRX_ReceiveChunk(t_eFSP_MSGRX_Ctx* const p_ptCtx);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSP_MSGRX_H */