/**
 * @file       eFSP_MSGDPRV.h
 *
 * @brief      Message decoder utils private header
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSP_MSGDPRV_H
#define EFSP_MSGDPRV_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_TYPE.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eFSP_MSGDPRV_SM_PRECHECK = 0,
    e_eFSP_MSGDPRV_SM_INSERTCHUNK,
    e_eFSP_MSGDPRV_SM_ENDCHECK,
    e_eFSP_MSGDPRV_SM_OKCHECK,
    e_eFSP_MSGDPRV_SM_ELABDONE
}e_eFSP_MSGDPRV_SM;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSP_MSGDPRV_H */