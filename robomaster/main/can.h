#ifndef CAN_H
#define CAN_H

#include <stdio.h>
#include "driver/twai.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <inttypes.h>

#define TABLE_ROWS 20
#define TABLE_COLUMNS 80



// volatile uint counter_blaster;

typedef struct{
    int msg_count;
    int length;
    char data[TABLE_COLUMNS];
}message_struct;

void send_command(int command_number);


void twai_send(void * parameters);

void twai_init(void* parameters);
void twai_read(void * parameters);
void can_print_alerts(void);

void appendCRC16CheckSum(uint8_t *pchMessage, uint32_t dwLength);
void appendCRC8CheckSum(uint8_t *pchMessage, uint32_t dwLength);

void fire_blaster();

void command_slicer(int command_number);
void command_slicer_b(int command_number);

// void assign_message(short (*message_table)[TABLE_ROWS][TABLE_COLUMNS], short (*message)[TABLE_COLUMNS]);
void assign_message(message_struct (*message_table)[TABLE_ROWS], short (*data)[TABLE_COLUMNS]);
// void print_message_table(short (*message_table)[TABLE_ROWS][TABLE_COLUMNS]);
void print_message_table(message_struct (*message_table)[TABLE_ROWS]);

#endif // !CAN_H
