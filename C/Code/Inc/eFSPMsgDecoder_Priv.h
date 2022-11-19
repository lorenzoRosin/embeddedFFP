/**
 * @file       eFSPMsgDecoder_Priv.h
 *
 * @brief      Message decoder utils private header
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSPMSGDECODER_PRIV_H
#define EFSPMSGDECODER_PRIV_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPType.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    MSGD_PRV_PRECHECK = 0,
    MSGD_PRV_INSERTCHUNK,
    MSGD_PRV_MSG_END_CHECK,
    MSGD_PRV_MSG_OK_CHECK,
    MSGD_PRV_ELABDONE
}e_eFSP_MSGD_Priv_state;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGDECODER_PRIV_H */