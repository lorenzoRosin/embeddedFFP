/**
 * @file       eFSP_MSGDMSGETST.h
 *
 * @brief      Message encoder dencoder test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSPMSGDEENCODERTEST_H
#define EFSPMSGDEENCODERTEST_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSP_TYPE.h"
#include "eFSP_MSGE.h"
#include "eFSP_MSGD.h"



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Test the message encoder dencoder module
 *
 * @param[in]   none
 *
 * @return      none
 */
void eFSP_TEST_msgDeEncoder(void);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGDEENCODERTEST_H */