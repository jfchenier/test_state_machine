/*
 *********************************************************************************************************
 *                                 COPYRIGHT(c) 2016 ECLIPSE SOLAR CAR
 *********************************************************************************************************
 *
 * Filename    : can_tx_task.c
 * Author      : Olivier C. Larocque
 * Description : This task initialise the board and send the OS tick on CAN
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
#include "app_cfg.h"

#include "init.h"
#include "memstruct.h"

/*
 *********************************************************************************************************
 *                                            LOCAL DATA
 *********************************************************************************************************
 */
static OS_TCB StartTaskTCB;
static CPU_STK StartTaskStk[APP_CFG_START_TASK_STK_SIZE];

/*
 *********************************************************************************************************
 *                                          LOCAL FUNCTIONS
 *********************************************************************************************************
 */

/*
 *********************************************************************************************************
 * can_tx_task
 *
 * Description : Board initialisation
 *
 * Note        : You shall replace S_TEMPLATE_OSTICK by your own project defines.
 *               You need to generate it with the memstruct builder.
 *
 * Argument(s) : p_arg     Unused
 *
 * Return(s)   : none.
 *********************************************************************************************************
 */
static void StartTask(void *p_arg)
{
    CPU_INT32U cpu_clk_freq;
    CPU_INT32U cnts;
    OS_ERR err;

    cpu_clk_freq = HAL_RCC_GetHCLKFreq(); /* Determine SysTick reference freq.                    */
    cnts = cpu_clk_freq /* Determine nbr SysTick increments                     */
    / (CPU_INT32U)OSCfg_TickRate_Hz;

    OS_CPU_SysTickInit(cnts); /* Init uC/OS periodic time src (SysTick).              */

    Init_Board();

    while (1) {
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

        uint32_t osTime = OSTimeGet(&err);
        CanSigWrite(S_TEMPLATE_OSTICK, &osTime, sizeof(osTime));

        OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}

/*
 *********************************************************************************************************
 *                                         PUBLIC FUNCTIONS
 *********************************************************************************************************
 */

/*
 *********************************************************************************************************
 * StartTask_Create
 *
 * Description : create start task
 *
 * Argument(s) : none
 *
 * Return(s)   : none.
 *********************************************************************************************************
 */
void StartTask_Create(void)
{
    OS_ERR err;

    OSTaskCreate((OS_TCB *)&StartTaskTCB, /* Create the start task                                */
    (CPU_CHAR *)"Start Task",
            (OS_TASK_PTR)StartTask,
            (void *)0,
            (OS_PRIO)APP_CFG_START_TASK_PRIO,
            (CPU_STK *)&StartTaskStk[0],
            (CPU_STK_SIZE)StartTaskStk[APP_CFG_START_TASK_STK_SIZE / 10],
            (CPU_STK_SIZE)APP_CFG_START_TASK_STK_SIZE,
            (OS_MSG_QTY)0,
            (OS_TICK)0,
            (void *)0,
            (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ),
            (OS_ERR *)&err);
}
