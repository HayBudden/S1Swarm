#include <stdio.h>
#include <stdint.h>
#include "driver/twai.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/twai_types.h"
#include "portmacro.h"
#include "can.h"


void app_main(void)
{
    xTaskCreate(twai_init, "CAN Init", 8196, NULL, 2, NULL);

    // for(int k = 0; k < 100; k++){
    //     printf("command_number = %i, ",k);
    //     send_command(0);
    //     printf("\n");
    // }

    xTaskCreate(twai_read, "CAN Read", 8196, NULL, 2, NULL);

    while(1){}
}
