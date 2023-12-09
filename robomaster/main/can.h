#ifndef CAN_H
#define CAN_H

#include <stdio.h>
#include "driver/twai.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <inttypes.h>

void send_command(int command_number);
void send_can(twai_message_t message);

void twai_init(void* parameters);
void twai_read(void * parameters);
void can_print_alerts(void);

void command_slicer(int command_number);
void command_slicer_b(int command_number);

#endif // !CAN_H
