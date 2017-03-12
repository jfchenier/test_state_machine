/*
 *********************************************************************************************************
 *                                 COPYRIGHT(c) 2016 ECLIPSE SOLAR CAR
 *********************************************************************************************************
 *
 * Filename    : state_machine_rx_task.c
 * Author      : Jean-François Chenier
 * Description : main state machine loop
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

enum {
	STATE_1 = 1,
	STATE_2,
	STATE_3,
	STATE_4,
	STATE_5
};

/*
 *********************************************************************************************************
 *                                            LOCAL DATA
 *********************************************************************************************************
 */
static OS_TCB stateMachineTaskTCB;
static CPU_STK stateMachineTaskStk[APP_CFG_STATE_MACHINE_TASK_STK_SIZE];

static uint16_t next_state = 1;

void state_1(void);
void state_2(void);
void state_3(void);
void state_4(void);
void state_5(void);

/*
 *********************************************************************************************************
 *                                          LOCAL FUNCTIONS
 *********************************************************************************************************
 */

/*
 *********************************************************************************************************
 * state_machine_task
 *
 * Description : Main loop for state machine.
 *
 * Argument(s) : p_arg     Unused
 *
 * Return(s)   : none.
 *********************************************************************************************************
 */
static void state_machine_task(void *p_arg) {
	while (1) {
		switch (next_state) {

		case STATE_1:
			state_1();
			break;

		case STATE_2:
			state_2();
			break;

		case STATE_3:
			state_3();
			break;

		case STATE_4:
			state_4();
			break;

		case STATE_5:
			state_5();
			break;

		default:
			break;
			state_2();
		}
	}
}

void state_1(void) {
	OS_ERR err;

	printf("this is the state 1\n\r");
	OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &err);

	if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
		next_state = STATE_5;
	else
		next_state = STATE_2;
}

void state_2(void) {
	OS_ERR err;

	printf("this is the state 2\n\r");
	OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &err);

	if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
		next_state = STATE_1;
	else
		next_state = STATE_3;
}

void state_3(void) {
	OS_ERR err;

	printf("this is the state 3\n\r");
	OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &err);

	if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
		next_state = STATE_2;
	else
		next_state = STATE_4;
}

void state_4(void) {
	OS_ERR err;

	printf("this is the state 4\n\r");
	OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &err);

	if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
		next_state = STATE_3;
	else
		next_state = STATE_5;
}

void state_5(void) {
	OS_ERR err;

	printf("this is the state 5\n\r");
	OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &err);

	if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
		next_state = STATE_4;
	else
		next_state = STATE_1;
}

/*
 *********************************************************************************************************
 *                                         PUBLIC FUNCTIONS
 *********************************************************************************************************
 */

/*
 *********************************************************************************************************
 * state_machine_task_create
 *
 * Description : create state machine task
 *
 * Argument(s) : none
 *
 * Return(s)   : none.
 *********************************************************************************************************
 */
void state_machine_task_create(void) {
	OS_ERR err;

	OSTaskCreate((OS_TCB *) &stateMachineTaskTCB,
			(CPU_CHAR *) "state machine task", (OS_TASK_PTR) state_machine_task,
			(void *) 0, (OS_PRIO) APP_CFG_STATE_MACHINE_TASK_PRIO,
			(CPU_STK *) &stateMachineTaskStk[0],
			(CPU_STK_SIZE) stateMachineTaskStk[APP_CFG_STATE_MACHINE_TASK_STK_SIZE
					/ 10], (CPU_STK_SIZE) APP_CFG_STATE_MACHINE_TASK_STK_SIZE,
			(OS_MSG_QTY) 0, (OS_TICK) 0, (void *) 0,
			(OS_OPT) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ),
			(OS_ERR *) &err);
}
