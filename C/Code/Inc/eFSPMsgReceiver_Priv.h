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
    MSGRX_PRV_CHECKIFBUFFERRX = 0,
    MSGRX_PRV_INSERTCHUNK,
    MSGRX_PRV_CHECKTIMEOUTBEFORRX,
    MSGRX_PRV_RECEIVE_BUFF,
    MSGRX_PRV_ELABDONE
}e_eFSP_MSGRX_Priv_state;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGRECEIVER_PRIV_H */