/**
 * @file       eFSPMsgDecoder.h
 *
 * @brief      Message decoder utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSPMSGDECODER_H
#define EFSPMSGDECODER_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPType.h"
#include "eCUByteUnStuffer.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/

/* Call back to a function that will calculate the CRC for this modules
 * the cntx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the MSG DECODER module */
typedef bool_t (*cb_crc32_msgd) ( void* cntx, const uint32_t seed, const uint8_t dataS[], const uint32_t dataSLen,
                                  uint32_t* const crc32SVal );

typedef enum
{
    MSGD_RES_OK = 0,
    MSGD_RES_BADPARAM,
    MSGD_RES_BADPOINTER,
	MSGD_RES_CORRUPTCTX,
    MSGD_RES_OUTOFMEM,
	MSGD_RES_MESSAGEENDED,
    MSGD_RES_NOINITLIB,
	MSGD_RES_CRCCLBKERROR,
}e_eFSP_MsgD_Res;

typedef struct
{
    s_eCU_BUStuffCtx byteUStufferCtnx;
    cb_crc32_msgd    cbCrcPtr;
    void*            cbCrcCtx;
}s_eFSP_MsgDCtx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the data decoder context
 *
 * @param[in]   ctx         - Msg decoder context
 * @param[in]   memArea     - Pointer to a memory area that we will use to save decoded data
 * @param[in]   memAreaSize - Dimension in byte of the memory area
 * @param[in]   cbCrcP      - Pointer to a CRC 32 seed callback function
 * @param[in]   clbCtx      - Custom context passed to the callback function
 *
 * @return      MSGD_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        MSGD_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              MSGD_RES_OK             - Operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderInitCtx(s_eFSP_MsgDCtx* const ctx, uint8_t memArea[], const uint32_t memAreaSize,
								 cb_crc32_msgd cbCrcP, void* const clbCtx);

/**
 * @brief       Start receiving a new message, loosing the previous stored decoded msg frame
 *
 * @param[in]   ctx         - Msg decoder context
 *
 * @return      MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderStartNewMsg(s_eFSP_MsgDCtx* const ctx);

/**
 * @brief       Retrive the pointer to the stored decoded data payload ( NO HEADER ), and the data size of the frame.
 *              Keep in mind that the message parsing could be ongoing, and if an error in the frame occour the
 *              retrivedLen could be setted to 0 again. We will retrive only payload size and no CRC + LEN header
 *
 * @param[in]   ctx         - Msg decoder context
 * @param[out]  dataP       - Pointer to a Pointer pointing to the decoded data payload ( NO CRC NO DATA SIZE )
 * @param[out]  retrivedLen - Pointer to a uint32_t variable where the size of the decoded data will be placed (raw
 *                            paylod data len )
 *
 * @return      MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderGetDecodedData(s_eFSP_MsgDCtx* const ctx, uint8_t** dataP, uint32_t* const retrivedLen);

/**
 * @brief       Retrive the size of encoded data payload frame. Keep in mind that the message parsing could be ongoing,
 *              and if an error in the frame occour the retrivedLen could be setted to 0 again. We will retrive only
 * 				payload size and no CRC + LEN header
 *
 * @param[in]   ctx         - Msg decoder context
 * @param[out]  retrivedLen - Pointer to a uint32_t variable where the size of the decoded data will be placed (raw
 *                            paylod data len, no header size )
 *
 * @return      MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderGetDecodedLen(s_eFSP_MsgDCtx* const ctx, uint32_t* const retrivedLen);

/**
 * @brief       Retrive if the MsgDecoder is currently waiting for the Start of frame.
 *
 * @param[in]   ctx         - Msg decoder context
 * @param[out]  isMsgDec 	- Pointer to a bool_t variable that will be filled with true if we are waiting SOF
 *
 * @return      MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderIsWaitingSof(s_eFSP_MsgDCtx* const ctx, bool_t* const isWaitingSof);

/**
 * @brief       Check if the current message is finished or if we need to decode some more data to have the full frame
 *
 * @param[in]   ctx         - Msg decoder context
 * @param[out]  isMsgDec 	- Pointer to a bool_t variable where we will store if the message parsing is ongoing
 *
 * @return      MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderIsAFullMsgUnstuff(s_eFSP_MsgDCtx* const ctx, bool_t* const isMsgDec);

/**
 * @brief       Return the most efficient numbers of data that needs to be passed to msgDecoderInsEncChunk in the next
 *              function call in order to speedup operation
 *
 * @param[in]   ctx             - Msg decoder context
 * @param[out]  mostEffPayload  - Pointer to an uint32_t were we will store the most efficient numbers of data len
 *                                that we could parse in msgDecoderInsEncChunk
 *
 * @return      MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MsgD_Res msgDecoderGetMostEffDatLen(s_eFSP_MsgDCtx* const ctx, uint32_t* const mostEffPayload);

/**
 * @brief       Insert the encoded data chunk that the alg will decode byte per byte
 *
 * @param[in]   ctx         	 - Msg decoder context
 * @param[in]   encArea     	 - Pointer to the encoded Data that we will decode
 * @param[in]   encLen      	 - Size of the encArea
 * @param[out]  consumedEncData  - Pointer to an uint32_t were we will store how many encoded data byte has been
 *                                 analized. keep in mind that unalized data were not decoded and will need to be
 *                                 reparsed
 * @param[out]  errSofRec        - Pointer to an uint32_t were we will store how many protocol error were detected.
 *                                 Even with some error detected, the protocol will continue parsing data discharging
 *                                 error.
 *
 * @return      MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGD_RES_BADPARAM     	- In case of an invalid parameter passed to the function
 *		        MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGD_RES_OUTOFMEM     	- Can not decode data, initial mem pointer was too small. The only way to
 *                                        resolve the issue is increasing the size of the memory area passed to init.
 *                                        Keep in mind that if there are multiple and frequent error in data tx or rx
 *                                        we could end in this situation when data size in frame is corrupted and
 *                                        interpreted higher that it should be and EOF + SOF of the next frame are lost.
 *                                        So this status could be due to a transmissione error, but it's not possible
 *                                        to know the reason of the error without storing all the data and checking CRC.
 *		        MSGD_RES_MESSAGEENDED  	- Frame ended, restart context in order to parse a new frame. Every other call
 *                                        to this function will not have effect until we call msgDecoderStartNewMsg.
 *                                        In this situation bear in mind that some data could be left out the parsing.
 *				MSGE_RES_CRCCLBKERROR   - The crc callback returned an error when the decoder where verifing CRC
 *              MSGD_RES_OK           	- Operation ended correctly. The chunk is parsed correclty but the frame is not
 *                                        finished yet
 */
e_eFSP_MsgD_Res msgDecoderInsEncChunk(s_eFSP_MsgDCtx* const ctx, uint8_t encArea[], const uint32_t encLen,
                                      uint32_t* const consumedEncData, uint32_t* errSofRec);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGDECODER_H */