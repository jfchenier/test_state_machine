#include "can_sig.h"
#include "can_msg.h"
#include "can_bus.h"
#include "can_err.h"
#include "app_cfg.h"
#include "memstruct.h"
#include "can_tx_task.h"
#include "can_rx_task.h"

extern const CANBUS_PARA CanCfg;
extern const CANMSG_PARA CanMsg[];
#if (CANSIG_STATIC_CONFIG == 0u)
extern const CANSIG_PARA CanSig[];
#endif

static void init_services    (void);
static void init_tasks       (void);
static void init_can_stack   (void);

void Init_Board(void)
{
    init_services();
    init_tasks();
    init_can_stack();
}

static void init_services(void)
{
}

static void init_tasks(void)
{
    //can_tx_task_create();
    can_rx_task_create();
}

static void init_can_stack(void)
{
    CPU_INT16S    can_err;
    CANMSG_PARA  *m;
#if (CANSIG_STATIC_CONFIG == 0u)
    CANSIG_PARA  *s;
#endif

    CanSigInit(0L);                                           /* Initialize CAN Signals.                              */
#if (CANSIG_STATIC_CONFIG == 0u)
    s = CanSig;
    while (s < &CanSig[S_MAX]) {                              /* Create CAN Signals                                   */
        can_err = CanSigCreate(s);
        if (can_err < 0) {
           while (1);                                         /* Failure Handling Here.                               */
        }
        s++;
    }
#endif

    CanMsgInit(0L);                                           /* Initialize CAN Messages.                             */
    m = (CANMSG_PARA *)CanMsg;
    while (m < &CanMsg[CANMSG_N]) {                           /* Create CAN Messages.                                 */
        can_err = CanMsgCreate(m);
        if (can_err < 0) {
           while (1);                                         /* Failure Handling Here.                               */
        }
        m++;
    }

    CanBusInit(0L);                                           /* Initialize CAN Objects & Bus Layer.                  */
    can_err = CanBusEnable((CANBUS_PARA *)&CanCfg);           /* Enable CAN Device according to Configuration.        */
    if (can_err != CAN_ERR_NONE) {
        while (1);                                            /* Failure Handling Here.                               */
    }
}
