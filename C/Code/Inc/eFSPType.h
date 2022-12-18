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
 *      DEFINES
 **********************************************************************************************************************/
#define EFSP_MSGEN_HEADERSIZE                               									  ( ( uint32_t ) 0x08u )
#define EFSP_MIN_MSGEN_PAYLOAD                               									  ( ( uint32_t ) 0x01u )
#define EFSP_MIN_MSGEN_BUFFLEN                       ( ( uint32_t ) ( EFSP_MSGEN_HEADERSIZE + EFSP_MIN_MSGEN_PAYLOAD ) )

#define EFSP_MSGDE_HEADERSIZE                               									  ( ( uint32_t ) 0x08u )
#define EFSP_MIN_MSGDE_PAYLOAD                               									  ( ( uint32_t ) 0x01u )
#define EFSP_MIN_MSGDE_BUFFLEN                       ( ( uint32_t ) ( EFSP_MSGDE_HEADERSIZE + EFSP_MIN_MSGDE_PAYLOAD ) )

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSP_TYPE_H */