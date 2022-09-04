/**
 * @file eFSPType.h
 *
 */

#ifndef EFSP_TYPE_H
#define EFSP_TYPE_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPStandardType.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    EFSP_RES_OK = 0,
    EFSP_RES_BADPARAM,
    EFSP_RES_BADPOINTER,
    EFSP_RES_OUTOFMEM,
    EFSP_RES_NOINITLIB,
}e_eFSP_Res;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSP_TYPE_H */
