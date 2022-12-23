/**
 * @file       eFSP_MSGTXPRV.h
 *
 * @brief      Message transmitter utils private header
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSP_MSGTXPRV_H
#define EFSP_MSGTXPRV_H



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
    e_eFSP_MSGTXPRV_SM_CHECKINIT = 0,
    e_eFSP_MSGTXPRV_SM_CHECKINITTIMEOUT,
    e_eFSP_MSGTXPRV_SM_CHECKIFBUFFERTX,
    e_eFSP_MSGTXPRV_SM_RETRIVECHUNK,
    e_eFSP_MSGTXPRV_SM_SENDBUFF,
    e_eFSP_MSGTXPRV_SM_CHECKTIMEOUTAFTERTX,
    e_eFSP_MSGTXPRV_SM_ELABDONE
}e_eFSP_MSGTXPRV_SM;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSP_MSGTXPRV_H */