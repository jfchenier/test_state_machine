/*
 *********************************************************************************************************
 *                                 COPYRIGHT(c) 2016 ECLIPSE SOLAR CAR
 *********************************************************************************************************
 *
 * Filename    : can_rx_task.c
 * Author      : Olivier C. Larocque
 * Description : Manage can reception
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
#include "stm32f4xx_hal.h"

#include "os.h"
#include "can_sig.h"
#include "can_msg.h"
#include "can_bus.h"
#include "can_err.h"
#include "app_cfg.h"

#include "memstruct.h"
#include "can_rx_task.h"

/*
 *********************************************************************************************************
 *                                            LOCAL DATA
 *********************************************************************************************************
 */
static OS_TCB CANRxTaskTCB;
static CPU_STK CANRxTaskStk[APP_CFG_CAN_RX_TASK_STK_SIZE];

/*
 *********************************************************************************************************
 *                                          LOCAL FUNCTIONS
 *********************************************************************************************************
 */

/*
 *********************************************************************************************************
 * can_rx_task
 *
 * Description : Main loop for can reception.
 *
 * Argument(s) : p_arg     Unused
 *
 * Return(s)   : none.
 *********************************************************************************************************
 */
static void can_rx_task(void *p_arg)
{
    CANFRM frm;
    CPU_INT16S msg;

    (void)p_arg;

    while (DEF_ON) {
        CanBusRead(0, (void *)&frm, sizeof(CANFRM));
        msg = CanMsgOpen(0, frm.Identifier, 0);
        if (msg >= 0) {
            CanMsgWrite(msg, (void *)&frm, sizeof(CANFRM));
        }
    }
}

/*
 *********************************************************************************************************
 *                                         PUBLIC FUNCTIONS
 *********************************************************************************************************
 */

/*
 *********************************************************************************************************
 * can_rx_task_create
 *
 * Description : create can rx task
 *
 * Argument(s) : none
 *
 * Return(s)   : none.
 *********************************************************************************************************
 */
void can_rx_task_create(void)
{
    OS_ERR err;

    OSTaskCreate((OS_TCB *)&CANRxTaskTCB,
            (CPU_CHAR *)"CAN Receive Task",
            (OS_TASK_PTR)can_rx_task,
            (void *)0,
            (OS_PRIO)APP_CFG_CAN_RX_TASK_PRIO,
            (CPU_STK *)&CANRxTaskStk[0],
            (CPU_STK_SIZE)CANRxTaskStk[APP_CFG_CAN_RX_TASK_STK_SIZE / 10],
            (CPU_STK_SIZE)APP_CFG_CAN_RX_TASK_STK_SIZE,
            (OS_MSG_QTY)0,
            (OS_TICK)0,
            (void *)0,
            (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ),
            (OS_ERR *)&err);
}
