/*
 *********************************************************************************************************
 *                                 COPYRIGHT(c) 2016 ECLIPSE SOLAR CAR
 *********************************************************************************************************
 *
 * Filename    : service_can_callbacks.c
 * Author      : Jean-Francois Chenier
 * Description : CAN callback function declaration
 * Note        : You can declare CAN callback functions here or in task
 *               files if it is closely related
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

#include "can_sig.h"

#include "service_can_callbacks.h"

/*
 *********************************************************************************************************
 *                                          LOCAL FUNCTIONS
 *********************************************************************************************************
 */

/*
 *********************************************************************************************************
 * template_led_callback
 *
 * Description : This function is called when the led signal is updated
 *               it toggle the led.
 *
 * Argument(s) : arg         Unused
 *               value       Unused
 *               CallbackId  tell if the callback have been called on
 *                           canSigRead or CanSigWrite
 *
 * Return(s)   : none.
 *********************************************************************************************************
 */
void template_led_callback(void* arg, CANSIG_VAL_T* value, CPU_INT32U CallbackId)
{
    UNUSED(&value);
    UNUSED(&arg);

    if (CallbackId == CANSIG_CALLBACK_WRITE_ID) {
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    }
}
