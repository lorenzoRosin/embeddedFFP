/**
 * @file       eFSP_MSGD.h
 *
 * @brief      Message decoder utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSP_MSGD_H
#define EFSP_MSGD_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_TYPE.h"
#include "eCU_BUNSTF.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
/* Define a generic crc callback context that must be implemented by the user */
typedef struct t_eFSP_MSGD_CrcCtxUser t_eFSP_MSGD_CrcCtx;

/* Call back to a function that will calculate the CRC for this modules
 * the p_ptCtx parameter is a custom pointer that can be used by the creator of this CRC callback, and will not be used
 * by the MSG DECODER module */
typedef bool_t (*f_eFSP_MSGD_CrcCb) ( t_eFSP_MSGD_CrcCtx* const p_ptCtx, const uint32_t p_uSeed, const uint8_t* p_puDat,
                                      const uint32_t p_uDataL, uint32_t* const p_puCrc32Val );

typedef enum
{
    e_eFSP_MSGD_RES_OK = 0,
    e_eFSP_MSGD_RES_BADPARAM,
    e_eFSP_MSGD_RES_BADPOINTER,
	e_eFSP_MSGD_RES_CORRUPTCTX,
    e_eFSP_MSGD_RES_OUTOFMEM,
    e_eFSP_MSGD_RES_BADFRAME,
	e_eFSP_MSGD_RES_MESSAGEENDED,
    e_eFSP_MSGD_RES_FRAMERESTART,
    e_eFSP_MSGD_RES_NOINITLIB,
	e_eFSP_MSGD_RES_CRCCLBKERROR,
}e_eFSP_MSGD_RES;

typedef struct
{
    t_eCU_BUNSTF_Ctx    tBUNSTFCtx;
    f_eFSP_MSGD_CrcCb   fCrc;
    t_eFSP_MSGD_CrcCtx* ptCrcCtx;
}t_eFSP_MSGD_Ctx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the data decoder context
 *
 * @param[in]   p_ptCtx      - Msg decoder context
 * @param[in]   p_puBuff     - Pointer to a memory area that we will use to save decoded data
 * @param[in]   p_uBuffL     - Dimension in byte of the memory area
 * @param[in]   p_fCrc       - Pointer to a CRC 32 p_uSeed callback function
 * @param[in]   p_ptFctx     - Custom context passed to the callback function
 *
 * @return      e_eFSP_MSGD_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eFSP_MSGD_RES_BADPARAM       - In case of an invalid parameter passed to the function
 *              e_eFSP_MSGD_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGD_RES eFSP_MSGD_InitCtx(t_eFSP_MSGD_Ctx* const p_ptCtx, uint8_t* p_puBuff, const uint32_t p_uBuffL,
                                  f_eFSP_MSGD_CrcCb p_fCrc, t_eFSP_MSGD_CrcCtx* const p_ptFctx);

/**
 * @brief       Check if the lib is initialized
 *
 * @param[in]   p_ptCtx         - Msg decoder context
 * @param[out]  p_pbIsInit      - Pointer to a bool_t variable that will be filled with true if the lib is initialized
 *
 * @return      e_eFSP_MSGD_RES_BADPOINTER    - In case of bad pointer passed to the function
 *              e_eFSP_MSGD_RES_OK            - Operation ended correctly
 */
e_eFSP_MSGD_RES eFSP_MSGD_IsInit(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* p_pbIsInit);

/**
 * @brief       Start receiving a new message, loosing the previous stored decoded msg frame. Clear even error state
 *
 * @param[in]   p_ptCtx         - Msg decoder context
 *
 * @return      e_eFSP_MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        e_eFSP_MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        e_eFSP_MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              e_eFSP_MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGD_RES eFSP_MSGD_NewMsg(t_eFSP_MSGD_Ctx* const p_ptCtx);

/**
 * @brief       Retrive the pointer to the stored decoded data payload ( NO HEADER ), and the data size of the frame.
 *              Keep in mind that the message parsing could be ongoing, and some value could change after parsing more
 *              data. We will retrive only payload size and no CRC + LEN header.
 *              Use this function when the whole message is decoded.
 *
 * @param[in]   p_ptCtx       - Msg decoder context
 * @param[out]  p_ppuDat      - Pointer to a Pointer pointing to the decoded data payload ( NO CRC NO DATA SIZE )
 * @param[out]  p_puGetL      - Pointer to a uint32_t variable where the size of the decoded data will be placed (raw
 *                              paylod data len, no header no crc )
 *
 * @return      e_eFSP_MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        e_eFSP_MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        e_eFSP_MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              e_eFSP_MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGD_RES eFSP_MSGD_GetDecodedData(t_eFSP_MSGD_Ctx* const p_ptCtx, uint8_t** p_ppuDat, uint32_t* const p_puGetL);

/**
 * @brief       Retrive the size of encoded data payload frame. Keep in mind that the message parsing could be ongoing,
 *              and some value could change after parsing more data. We will retrive only
 * 				payload size and no CRC + LEN header. Use this function when the whole message is decoded.
 *
 * @param[in]   p_ptCtx       - Msg decoder context
 * @param[out]  p_puRetrivedL - Pointer to a uint32_t variable where the size of the decoded data will be placed (raw
 *                              paylod data len, no header size )
 *
 * @return      e_eFSP_MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        e_eFSP_MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        e_eFSP_MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              e_eFSP_MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGD_RES eFSP_MSGD_GetDecodedLen(t_eFSP_MSGD_Ctx* const p_ptCtx, uint32_t* const p_puRetrivedL);

/**
 * @brief       Retrive if the MsgDecoder is currently waiting for the Start of frame.
 *
 * @param[in]   p_ptCtx         - Msg decoder context
 * @param[out]  p_pbIsMsgDec 	- Pointer to a bool_t variable that will be filled with true if we are waiting SOF
 *
 * @return      e_eFSP_MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        e_eFSP_MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        e_eFSP_MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              e_eFSP_MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGD_RES eFSP_MSGD_IsWaitingSof(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_pbIsWaitingSof);

/**
 * @brief       Check if the current message is finished or if we need to decode some more data to have the full frame.
 *              If a bad frame is received this function will consider it as an unfinished frame.
 *
 * @param[in]   p_ptCtx         - Msg decoder context
 * @param[out]  p_pbIsMsgDec 	- Pointer to a bool_t variable where we will store if the message decoded ended.
 *
 * @return      e_eFSP_MSGD_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eFSP_MSGD_RES_NOINITLIB      - Need to init context before taking some action
 *		        e_eFSP_MSGD_RES_CORRUPTCTX     - In case of an corrupted context
 *				e_eFSP_MSGD_RES_CRCCLBKERROR   - The crc callback returned an error when the decoder here verifing CRC
 *              e_eFSP_MSGD_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGD_RES eFSP_MSGD_IsAFullMsgDecoded(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_pbIsMsgDec);

/**
 * @brief       Check if the current received data compose a bad frame. If a bad frame is detected we can only
 *              call MSGD_StartNewMsg before parsing new data.
 *
 * @param[in]   p_ptCtx         - Msg decoder context
 * @param[out]  p_pbIsFrameBad 	- Pointer to a bool_t variable where we will store if the frame is bad formed
 *
 * @return      e_eFSP_MSGD_RES_BADPOINTER     - In case of bad pointer passed to the function
 *		        e_eFSP_MSGD_RES_NOINITLIB      - Need to init context before taking some action
 *		        e_eFSP_MSGD_RES_CORRUPTCTX     - In case of an corrupted context
 *				e_eFSP_MSGD_RES_CRCCLBKERROR   - The crc callback returned an error when the decoder were verifing CRC
 *              e_eFSP_MSGD_RES_OK             - Operation ended correctly
 */
e_eFSP_MSGD_RES eFSP_MSGD_IsFrameBad(t_eFSP_MSGD_Ctx* const p_ptCtx, bool_t* const p_pbIsFrameBad);

/**
 * @brief       Return the most efficient numbers of data that needs to be passed to eFSP_MSGD_InsEncChunk in the next
 *              function call in order to speedup operation. When this function return 0 it's mean that the frame
 *              is fully received or that the frame is corrupted and we need to start receiving a new frame.
 *
 * @param[in]   p_ptCtx          - Msg decoder context
 * @param[out]  p_puMostEffPayL  - Pointer to an uint32_t were we will store the most efficient numbers of data len
 *                                 that we could parse in eFSP_MSGD_InsEncChunk
 *
 * @return      e_eFSP_MSGD_RES_BADPOINTER   	- In case of bad pointer passed to the function
 *		        e_eFSP_MSGD_RES_NOINITLIB    	- Need to init context before taking some action
 *		        e_eFSP_MSGD_RES_CORRUPTCTX   	- In case of an corrupted context
 *              e_eFSP_MSGD_RES_OK           	- Operation ended correctly
 */
e_eFSP_MSGD_RES eFSP_MSGD_GetMostEffDatLen(t_eFSP_MSGD_Ctx* const p_ptCtx, uint32_t* const p_puMostEffPayL);

/**
 * @brief       Insert the encoded data chunk that the alg will decode byte per byte
 *
 * @param[in]   p_ptCtx          - Msg decoder context
 * @param[in]   p_puEncArea      - Pointer to the encoded Data that we will decode
 * @param[in]   p_uEncL      	 - Size of the p_puEncArea
 * @param[out]  p_puUsedEncB     - Pointer to an uint32_t were we will store how many encoded data has been
 *                                 analized. keep in mind that unalized data were not decoded and will need to be
 *                                 be reparsed. Un parsed data happens when the frame ended earlier
 *                                 ( e_eFSP_MSGD_RES_MESSAGEENDED, e_eFSP_MSGD_RES_BADFRAME or
 *                                 e_eFSP_MSGD_RES_FRAMERESTART is returned ) or when some error is returned. When the
 *                                 function return e_eFSP_MSGD_RES_OK consumedStuffData will always be returned
 *                                 has p_uEncL.
 *
 * @return      e_eFSP_MSGD_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        e_eFSP_MSGD_RES_NOINITLIB    - Need to init context before taking some action
 *		        e_eFSP_MSGD_RES_BADPARAM     - In case of an invalid parameter passed to the function
 *		        e_eFSP_MSGD_RES_CORRUPTCTX   - In case of an corrupted context
 *              e_eFSP_MSGD_RES_OUTOFMEM     - Can not decode data, initial mem pointer was too small. The only way to
 *                                             resolve the issue is increasing the size of the memory area passed to
 *                                             init. Keep in mind that if there are multiple and frequent error in data
 *                                             tx or rx we could end in this situation when data size in frame is
 *                                             corrupted and interpreted higher that it should be and EOF + SOF of the
 *                                             next frame are lost. So this status could be due to a transmissione
 *                                             error, but it's not possible to know the reason of the error without
 *                                             storing all the data and checking CRC.
 *		        e_eFSP_MSGD_RES_MESSAGEENDED - Frame ended, restart context in order to parse a new frame. Every other
 *                                             call to this function will not have effect until we call
 *                                             MSGD_StartNewMsg. In this situation bear in mind that some data could be
 *                                             left out the parsing and so we need to reparse that data after calling
 *                                             MSGD_StartNewMsg.
 *              e_eFSP_MSGD_RES_BADFRAME     - Found an error while parsing, the frame passed is invalid.
 *                                             Restart context in order to parse a new frame. Every other call
 *                                             to this function will not have effect until we call MSGD_StartNewMsg.
 *                                             In this situation bear in mind that some data could be left out the
 *                                             parsing, and so we need to reparse that data after calling
 *                                             MSGD_StartNewMsg.
 *              e_eFSP_MSGD_RES_FRAMERESTART - During frame receiving another start of frame is received. In this
 *                                             situation clear old data and restart the frame, witouth the need to call
 *                                             any other function. In this situation bear in mind that some data could
 *                                             be left out the parsing and so we need to reparse that data with another
 *                                             call of eFSP_MSGD_InsEncChunk.
 *				e_eFSP_MSGD_RES_CRCCLBKERROR - The crc callback returned an error when the decoder were verifing CRC
 *              e_eFSP_MSGD_RES_OK           - Operation ended correctly. The chunk is parsed correclty but the frame is
 *                                             not finished yet. In this situation p_puUsedEncB is always reported with
 *                                             a value equals to p_uEncL.
 */
e_eFSP_MSGD_RES eFSP_MSGD_InsEncChunk(t_eFSP_MSGD_Ctx* const p_ptCtx, uint8_t* p_puEncArea, const uint32_t p_uEncL,
                                      uint32_t* const p_puUsedEncB);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSP_MSGD_H */