#include "os.h"
#include "stm32f4xx_hal.h"
#include "app_cfg.h"
//#include "init.h"

static  OS_TCB  StartTaskTCB;
static  CPU_STK StartTaskStk[APP_CFG_START_TASK_STK_SIZE];

static void StartTask(void *p_arg)
{
  CPU_INT32U cpu_clk_freq;
  CPU_INT32U cnts;
  OS_ERR     err;

  cpu_clk_freq = HAL_RCC_GetHCLKFreq();                       /* Determine SysTick reference freq.                    */
  cnts         = cpu_clk_freq                                 /* Determine nbr SysTick increments                     */
               / (CPU_INT32U)OSCfg_TickRate_Hz;

  OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */

  //Init_Board();

  while (1) {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &err);
  }
}

void StartTask_Create(void)
{
    OS_ERR err;

    OSTaskCreate((OS_TCB    *)&StartTaskTCB,                 /* Create the start task                                */
                 (CPU_CHAR  *)"Start Task",
                 (OS_TASK_PTR )StartTask,
                 (void       *)0,
                 (OS_PRIO     )APP_CFG_START_TASK_PRIO,
                 (CPU_STK   *)&StartTaskStk[0],
                 (CPU_STK_SIZE)StartTaskStk[APP_CFG_START_TASK_STK_SIZE / 10],
                 (CPU_STK_SIZE)APP_CFG_START_TASK_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);
}
