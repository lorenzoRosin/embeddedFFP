/**
 * @file       eFSPMsgReceiver.h
 *
 * @brief      Message receiver utils private header
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSPMSGRECEIVER_PRIV_H
#define EFSPMSGRECEIVER_PRIV_H



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
    MSGRX_PRV_CHECKIFBUFFERTX = 0,
    MSGRX_PRV_SEND_BUFF,
    MSGRX_PRV_CHECK_RETRIVECHUNK,
    MSGRX_PRV_ELABDONE
}e_eFSP_MSGRX_Priv_state;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGRECEIVER_PRIV_H */