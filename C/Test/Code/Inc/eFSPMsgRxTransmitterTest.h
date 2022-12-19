/**
 * @file       eFSPMsgRxTransmitterTest.h
 *
 * @brief      Message Tx Rx test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef EFSPMSGRXTRANSMITTERTEST_H
#define EFSPMSGRXTRANSMITTERTEST_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPType.h"
#include "eFSPMsgTransmitter.h"
#include "eFSPMsgReceiver.h"



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Test the message Rx Rx module
 *
 * @param[in]   none
 *
 * @return      none
 */
void eFSP_TEST_msgRxTransmitter(void);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSPMSGRXTRANSMITTERTEST_H */