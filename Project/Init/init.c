/*
 *********************************************************************************************************
 *                                 COPYRIGHT(c) 2016 ECLIPSE SOLAR CAR
 *********************************************************************************************************
 *
 * Filename    : init.c
 * Author      : Olivier C. Larocque
 * Description : Board initialisation
 *
 *********************************************************************************************************
 * COPYRIGHT(c) 2016, Eclipse Solar Car
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Eclipse nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************************************
 */

/*
 *********************************************************************************************************
 *                                               INCLUDES
 *********************************************************************************************************
 */
#include "can_sig.h"
#include "can_msg.h"
#include "can_bus.h"
#include "can_err.h"
#include "app_cfg.h"

#include "memstruct.h"
#include "can_tx_task.h"
#include "can_rx_task.h"

/*
 *********************************************************************************************************
 *                                            GLOBAL DATA
 *********************************************************************************************************
 */
extern const CANBUS_PARA CanCfg;
extern const CANMSG_PARA CanMsg[];
#if (CANSIG_STATIC_CONFIG == 0u)
extern const CANSIG_PARA CanSig[];
#endif

/*
 *********************************************************************************************************
 *                                         FUNCTION PROTOTYPES
 *********************************************************************************************************
 */
static void init_services(void);
static void init_tasks(void);
static void init_can_stack(void);

/*
 *********************************************************************************************************
 * Init_Board
 *
 * Description : Board initialisation
 *
 * Argument(s) : none
 *
 * Return(s)   : none
 *********************************************************************************************************
 */
void Init_Board(void)
{
    init_services();
    init_tasks();
    init_can_stack();
}

/*
 *********************************************************************************************************
 *                                          LOCAL FUNCTIONS
 *********************************************************************************************************
 */

/*
 *********************************************************************************************************
 * init_services
 *
 * Description : Services initialisation
 *
 * Argument(s) : none
 *
 * Return(s)   : none
 *********************************************************************************************************
 */
static void init_services(void)
{
}

/*
 *********************************************************************************************************
 * init_tasks
 *
 * Description : Tasks initialisation
 *
 * Argument(s) : none
 *
 * Return(s)   : none
 *********************************************************************************************************
 */
static void init_tasks(void)
{
    can_tx_task_create();
    can_rx_task_create();
}

/*
 *********************************************************************************************************
 * init_can_stack
 *
 * Description : Can stack initialisation
 *
 * Argument(s) : none
 *
 * Return(s)   : none
 *********************************************************************************************************
 */
static void init_can_stack(void)
{
    CPU_INT16S can_err;
    CANMSG_PARA *m;
#if (CANSIG_STATIC_CONFIG == 0u)
    CANSIG_PARA *s;
#endif

    CanSigInit(0L); /* Initialize CAN Signals.*/
#if (CANSIG_STATIC_CONFIG == 0u)
    s = CanSig;
    while (s < &CanSig[S_MAX]) { /* Create CAN Signals*/
        can_err = CanSigCreate(s);
        if (can_err < 0) {
            while (1); /* Failure Handling Here.*/
        }
        s++;
    }
#endif

    CanMsgInit(0L); /* Initialize CAN Messages.*/
    m = (CANMSG_PARA *)CanMsg;
    while (m < &CanMsg[CANMSG_N]) { /* Create CAN Messages.*/
        can_err = CanMsgCreate(m);
        if (can_err < 0) {
            while (1)
                ; /* Failure Handling Here.*/
        }
        m++;
    }

    CanBusInit(0L); /* Initialize CAN Objects & Bus Layer.*/
    can_err = CanBusEnable((CANBUS_PARA *)&CanCfg); /* Enable CAN Device according to Configuration.*/
    if (can_err != CAN_ERR_NONE) {
        while (1)
            ; /* Failure Handling Here.                               */
    }
}
