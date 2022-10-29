/**
 * @file       eFSPMsgTransmitter_Priv.h
 *
 * @brief      Message transmitter utils private header
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSPMSGTRANSMITTER_PRIV_H
#define EFSPMSGTRANSMITTER_PRIV_H



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
typedef enum
{
    MSGD_PRV_INSERTCHUNK = 0,
    MSGD_PRV_MSG_END_CHECK,
    MSGD_PRV_MSG_OK_CHECK,
    MSGD_PRV_ELABDONE
}e_eFSP_Trnsmt_Priv_state;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGTRANSMITTER_PRIV_H */