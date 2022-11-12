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
    MSGTX_PRV_CHECKIFBUFFERTX = 0,
    MSGTX_PRV_SENDBUFF,
    MSGTX_PRV_RETRIVECHUNK,
    MSGTX_PRV_ELABDONE
}e_eFSP_MSGTX_Priv_state;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGTRANSMITTER_PRIV_H */