#include "os.h"
#include "stm32f1xx_hal.h"
#include "can_sig.h"
#include "can_msg.h"
#include "can_bus.h"
#include "can_err.h"
#include "app_cfg.h"
#include "memstruct.h"

static OS_TCB  CANRxTaskTCB;
static CPU_STK CANRxTaskStk[APP_CFG_CAN_RX_TASK_STK_SIZE];

static void can_rx_task(void *p_arg)
{
    CANFRM     frm;
    CPU_INT16S msg;

    (void)p_arg;

    while (DEF_ON) {
        CanBusRead(0, (void *)&frm, sizeof(CANFRM));
        msg = CanMsgOpen(0, frm.Identifier, 0);
        if(msg >= 0) {
            CanMsgWrite(msg,(void *)&frm,sizeof(CANFRM));
        }
    }
}

void can_rx_task_create(void)
{
    OS_ERR err;

    OSTaskCreate((OS_TCB    *)&CANRxTaskTCB,
                 (CPU_CHAR  *)"CAN Receive Task",
                 (OS_TASK_PTR )can_rx_task,
                 (void       *)0,
                 (OS_PRIO     )APP_CFG_CAN_RX_TASK_PRIO,
                 (CPU_STK   *)&CANRxTaskStk[0],
                 (CPU_STK_SIZE)CANRxTaskStk[APP_CFG_CAN_RX_TASK_STK_SIZE / 10],
                 (CPU_STK_SIZE)APP_CFG_CAN_RX_TASK_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    *)&err);
}
