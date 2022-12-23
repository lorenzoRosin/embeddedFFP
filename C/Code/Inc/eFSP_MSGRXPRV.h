/**
 * @file       eFSP_MSGRX.h
 *
 * @brief      Message receiver utils private header
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSP_MSGRXPRV_H
#define EFSP_MSGRXPRV_H



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
    e_eFSP_MSGRXPRV_SM_CHECKINIT = 0,
    e_eFSP_MSGRXPRV_SM_CHECKINITTIMEOUT,
    e_eFSP_MSGRXPRV_SM_CHECKHOWMANYDATA,
    e_eFSP_MSGRXPRV_SM_CHECKIFBUFFERRX,
    e_eFSP_MSGRXPRV_SM_INSERTCHUNK,
    e_eFSP_MSGRXPRV_SM_CHECKTIMEOUTAFTERRX,
    e_eFSP_MSGRXPRV_SM_RECEIVEBUFF,
    e_eFSP_MSGRXPRV_SM_ELABDONE
}e_eFSP_MSGRXPRV_SM;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSP_MSGRXPRV_H */