/*
 *********************************************************************************************************
 *                                        ECLIPSE SOLAR CAR 2016
 *********************************************************************************************************
 *
 * Filename    : can_callbacks.h
 * Author      : Jean-Francois
 * Description : CAN callbacks definitions
 *
 *********************************************************************************************************
 */
#ifndef PROJECT_SERVICES_SERVICE_CAN_CALLBACK_H_
#define PROJECT_SERVICES_SERVICE_CAN_CALLBACK_H_

/*
 *********************************************************************************************************
 *                                               INCLUDES
 *********************************************************************************************************
 */
#include "os.h"
#include "can_sig.h"

/*
 *********************************************************************************************************
 *                                             DEFINES
 *********************************************************************************************************
 */
#define S_TEMPLATE_LED_callback template_led_callback

/*
 *********************************************************************************************************
 *                                         FUNCTION PROTOTYPES
 *********************************************************************************************************
 */
void template_led_callback(void*  arg, CANSIG_VAL_T*  value, CPU_INT32U  CallbackId);

#endif

