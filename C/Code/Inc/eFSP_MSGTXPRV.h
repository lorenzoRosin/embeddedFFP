/**
 * @file       eFSP_MSGTXPRV.h
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
#include "eFSP_TYPE.h"
#include "eFSP_MSGE.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    MSGTX_PRV_CHECKINIT = 0,
    MSGTX_PRV_CHECKINITTIMEOUT,
    MSGTX_PRV_CHECKIFBUFFERTX,
    MSGTX_PRV_RETRIVECHUNK,
    MSGTX_PRV_SENDBUFF,
    MSGTX_PRV_CHECKTIMEOUTAFTERTX,
    MSGTX_PRV_ELABDONE
}e_eFSP_MSGTX_Priv_state;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGTRANSMITTER_PRIV_H */