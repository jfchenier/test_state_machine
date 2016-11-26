#include "os.h"
#include "stm32f4xx_hal.h"
#include "can_sig.h"
#include "can_msg.h"
#include "can_bus.h"
#include "can_err.h"
#include "app_cfg.h"
#include "memstruct.h"

static OS_TCB  CANTxTaskTCB;
static CPU_STK CANTxTaskStk[APP_CFG_CAN_TX_TASK_STK_SIZE];


/*
 *********************************************************************************************************
 * StatusTask
 *
 * Description : main loop for can transmission. You should replace M_MAX_TEMPLATE
 *               and ID_OFFSET_TEMPLATE by your own project defines. You need to generate
 *               it with the memstruct builder.
 *
 * Argument(s) : p_arg     Unused
 *
 * Return(s)   : none.
 *********************************************************************************************************
 */
static void can_tx_task(void *p_arg)
{
    OS_ERR err;
    CANFRM frm;
    CPU_INT16S msg;

    (void)p_arg;

    while (DEF_ON){
        for (int i = 0; i < M_MAX_TEMPLATE; i++)
                {
            msg = CanMsgOpen(0, ID_OFFSET_TEMPLATE + i, 0);
            if (msg >= 0)
                    {
                CanMsgRead(msg, &frm, sizeof(CANFRM));
                CanBusWrite(0, &frm, sizeof(CANFRM));
            }
        }
        OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}


void can_tx_task_create(void)
{
    OS_ERR err;

    OSTaskCreate((OS_TCB    *)&CANTxTaskTCB,                 /* Create the start task                                */
                 (CPU_CHAR  *)"CAN Transmit Task",
                 (OS_TASK_PTR )can_tx_task,
                 (void       *)0,
                 (OS_PRIO     )APP_CFG_CAN_TX_TASK_PRIO,
                 (CPU_STK   *)&CANTxTaskStk[0],
                 (CPU_STK_SIZE)CANTxTaskStk[APP_CFG_CAN_TX_TASK_STK_SIZE / 10],
                 (CPU_STK_SIZE)APP_CFG_CAN_TX_TASK_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);
}
