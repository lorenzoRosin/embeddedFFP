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
    MSGD_RES_BADFRAME,
	MSGD_RES_MESSAGEENDED,
    MSGD_RES_FRAMERESTART,
    MSGD_RES_NOINITLIB,
	MSGD_RES_CRCCLBKERROR,
}e_eFSP_MSGD_Res;

typedef struct
{
    s_eCU_BUNSTF_Ctx byteUStufferCtnx;
    cb_crc32_msgd    cbCrcPtr;
    void*            cbCrcCtx;
}s_eFSP_MSGD_Ctx;



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
e_eFSP_MSGD_Res eFSP_MSGD_InitCtx(s_eFSP_MSGD_Ctx* const ctx, uint8_t memArea[], const uint32_t memAreaSize,
								 cb_crc32_msgd cbCrcP, void* const clbCtx);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   ctx         - Msg decoder context
 * @param[out]  isInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      MSGD_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              MSGD_RES_OK            - Operation ended correctly
 */
e_eFSP_MSGD_Res eFSP_MSGD_IsInit(s_eFSP_MSGD_Ctx* const ctx, bool_t* isInit);

/**
 * @brief       Start receiving a new message, loosing the previous stored decoded msg frame. Clear even error state
 *
 * @param[in]   ctx         - Msg decoder context
 *
 * @return      MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGD_Res eFSP_MSGD_StartNewMsg(s_eFSP_MSGD_Ctx* const ctx);

/**
 * @brief       Retrive the pointer to the stored decoded data payload ( NO HEADER ), and the data size of the frame.
 *              Keep in mind that the message parsing could be ongoing, and some value could change after parsing more
 *              data. We will retrive only payload size and no CRC + LEN header.
 *              Use this function when the whole message is decoded.
 *
 * @param[in]   ctx         - Msg decoder context
 * @param[out]  dataP       - Pointer to a Pointer pointing to the decoded data payload ( NO CRC NO DATA SIZE )
 * @param[out]  retrivedLen - Pointer to a uint32_t variable where the size of the decoded data will be placed (raw
 *                            paylod data len, no header no crc )
 *
 * @return      MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGD_Res eFSP_MSGD_GetDecodedData(s_eFSP_MSGD_Ctx* const ctx, uint8_t** dataP, uint32_t* const retrivedLen);

/**
 * @brief       Retrive the size of encoded data payload frame. Keep in mind that the message parsing could be ongoing,
 *              and some value could change after parsing more data. We will retrive only
 * 				payload size and no CRC + LEN header. Use this function when the whole message is decoded.
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
e_eFSP_MSGD_Res eFSP_MSGD_GetDecodedLen(s_eFSP_MSGD_Ctx* const ctx, uint32_t* const retrivedLen);

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
e_eFSP_MSGD_Res eFSP_MSGD_IsWaitingSof(s_eFSP_MSGD_Ctx* const ctx, bool_t* const isWaitingSof);

/**
 * @brief       Check if the current message is finished or if we need to decode some more data to have the full frame.
 *              If a bad frame is received this function will consider it as an unfinished frame.
 *
 * @param[in]   ctx         - Msg decoder context
 * @param[out]  isMsgDec 	- Pointer to a bool_t variable where we will store if the message decoded ended.
 *
 * @return      MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *				MSGD_RES_CRCCLBKERROR   - The crc callback returned an error when the decoder here verifing CRC
 *              MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGD_Res eFSP_MSGD_IsAFullMsgDecoded(s_eFSP_MSGD_Ctx* const ctx, bool_t* const isMsgDec);

/**
 * @brief       Check if the current received data compose a bad frame. If a bad frame is detected we can only
 *              call MSGD_StartNewMsg before parsing new data.
 *
 * @param[in]   ctx         - Msg decoder context
 * @param[out]  isFrameBad 	- Pointer to a bool_t variable where we will store if the frame is bad formed
 *
 * @return      MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *				MSGD_RES_CRCCLBKERROR   - The crc callback returned an error when the decoder were verifing CRC
 *              MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGD_Res eFSP_MSGD_IsCurrentFrameBad(s_eFSP_MSGD_Ctx* const ctx, bool_t* const isFrameBad);

/**
 * @brief       Return the most efficient numbers of data that needs to be passed to MSGD_InsEncChunk in the next
 *              function call in order to speedup operation. When this function return 0 it's mean that the frame
 *              is fully received or that the frame is corrupted and we need to start receiving a new frame.
 *
 * @param[in]   ctx             - Msg decoder context
 * @param[out]  mostEffPayload  - Pointer to an uint32_t were we will store the most efficient numbers of data len
 *                                that we could parse in MSGD_InsEncChunk
 *
 * @return      MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGD_Res eFSP_MSGD_GetMostEffDatLen(s_eFSP_MSGD_Ctx* const ctx, uint32_t* const mostEffPayload);

/**
 * @brief       Insert the encoded data chunk that the alg will decode byte per byte
 *
 * @param[in]   ctx         	 - Msg decoder context
 * @param[in]   encArea     	 - Pointer to the encoded Data that we will decode
 * @param[in]   encLen      	 - Size of the encArea
 * @param[out]  consumedEncData  - Pointer to an uint32_t were we will store how many encoded data has been
 *                                 analized. keep in mind that unalized data were not decoded and will need to be
 *                                 be reparsed. Un parsed data happens when the frame ended earlier
 *                                 ( MSGD_RES_MESSAGEENDED, MSGD_RES_BADFRAME or MSGD_RES_FRAMERESTART is returned ) or
 *                                 when some error is returned. When the function return MSGD_RES_OK consumedStuffData
 *                                 will always be returned has encLen.
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
 *                                        to this function will not have effect until we call MSGD_StartNewMsg.
 *                                        In this situation bear in mind that some data could be left out the parsing
 *                                        and so we need to reparse that data after calling MSGD_StartNewMsg.
 *              MSGD_RES_BADFRAME       - Found an error while parsing, the frame passed is invalid.
 *                                        Restart context in order to parse a new frame. Every other call
 *                                        to this function will not have effect until we call MSGD_StartNewMsg.
 *                                        In this situation bear in mind that some data could be left out the parsing,
 *                                        and so we need to reparse that data after calling MSGD_StartNewMsg.
 *              MSGD_RES_FRAMERESTART   - During frame receiving another start of frame is received. In this situation
 *                                        clear old data and restart the frame, witouth the need to call any other
 *                                        function. In this situation bear in mind that some data could be left out
 *                                        the parsing and so we need to reparse that data with another call of
 *                                        MSGD_InsEncChunk.
 *				MSGD_RES_CRCCLBKERROR   - The crc callback returned an error when the decoder were verifing CRC
 *              MSGD_RES_OK           	- Operation ended correctly. The chunk is parsed correclty but the frame is not
 *                                        finished yet. In this situation consumedEncData is always reported with a
 *                                        value equals to encLen.
 */
e_eFSP_MSGD_Res eFSP_MSGD_InsEncChunk(s_eFSP_MSGD_Ctx* const ctx, uint8_t encArea[], const uint32_t encLen,
                                 uint32_t* const consumedEncData);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGDECODER_H */