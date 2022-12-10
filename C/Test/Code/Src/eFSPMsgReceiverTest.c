/**
 * @file       eFSPMsgReceiverTest.h
 *
 * @brief      Message receiver test
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSPMsgReceiverTest.h"
#include "eCUCrc.h"

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
    /* Suppressed for code clarity in test execution*/
#endif

#include <stdio.h>

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-20.9", "MISRAC2012-Rule-21.6"
#endif



/***********************************************************************************************************************
 *   PRIVATE TEST FUNCTION DECLARATION
 **********************************************************************************************************************/
typedef struct
{
    e_eCU_CRC_Res lastError;
}s_eCU_crcAdapterCtx;

typedef struct
{
    bool sendIsError;
}s_eCU_msgSendAdapterCtx;

typedef struct
{
    bool sendIsError;
}s_eCU_timerAdapterCtx;

static bool_t c32SAdapt(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);
static bool_t c32SAdaptEr(void* cntx, const uint32_t s, const uint8_t d[], const uint32_t dLen, uint32_t* const c32Val);
static bool_t receiveMsg( void* cntx, const uint8_t dataToSend[], const uint32_t dataToSendLen, uint32_t* const dataSended,
                       const uint32_t timeToSend );
static bool_t receiveMsgErr( void* cntx, const uint8_t dataToSend[], const uint32_t dataToSendLen, uint32_t* const dataSended,
                          const uint32_t timeToSend );
static bool_t timStart ( void* cntx, const uint32_t timeoutVal );
static bool_t timGetRemaining ( void* cntx, uint32_t* const remainings );
static bool_t timStartErr ( void* cntx, const uint32_t timeoutVal );
static bool_t timGetRemainingErr ( void* cntx, uint32_t* const remainings );



/***********************************************************************************************************************
 *   PRIVATE FUNCTION DECLARATION
 **********************************************************************************************************************/
static void msgReceiverTestBadPointer(void);
static void msgReceiverTestBadInit(void);
static void msgReceiverTestBadIniMsg(void);
static void msgReceiverTestBadParamEntr(void);
static void msgReceiverTestCorruptContext(void);
static void msgReceiverTestBadClBckCrc(void);
static void msgReceiverTestBadClBckReceive(void);
static void msgReceiverTestBadClBckTim(void);




/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
void msgReceiverTest(void)
{
	(void)printf("\n\nMESSAGE RECEIVER TEST START \n\n");

    msgReceiverTestBadPointer();
    msgReceiverTestBadInit();
    msgReceiverTestBadIniMsg();
    msgReceiverTestBadParamEntr();
    msgReceiverTestCorruptContext();
    msgReceiverTestBadClBckCrc();
    msgReceiverTestBadClBckReceive();
    msgReceiverTestBadClBckTim();

    (void)printf("\n\nMESSAGE RECEIVER TEST END \n\n");
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTION
 **********************************************************************************************************************/
void msgReceiverTestBadPointer(void)
{

}


void msgReceiverTestBadInit(void)
{

}

void msgReceiverTestBadIniMsg(void)
{

}

void msgReceiverTestBadParamEntr(void)
{

}

void msgReceiverTestCorruptContext(void)
{

}

void msgReceiverTestBadClBckCrc(void)
{

}

void msgReceiverTestBadClBckReceive(void)
{

}

void msgReceiverTestBadClBckTim(void)
{

}

