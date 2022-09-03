/**
 * @file eCUStandardType.h
 *
 */

#ifndef ECU_STANDARDTYPE_H
#define ECU_STANDARDTYPE_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>



/***********************************************************************************************************************
 *      DEFINES
 **********************************************************************************************************************/
#ifndef NULL
  #define NULL          ( _NULL )
#endif

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_disable = "MISRAC2004-19.4"
    /* Suppressed for code clarity in test execution*/
#endif

    #ifndef bool_t
        #define bool_t           bool
    #endif

#ifdef __IAR_SYSTEMS_ICC__
    #pragma cstat_restore = "MISRAC2004-19.4"
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECU_STANDARDTYPE_H */
