#include "can.h"
#include <driver/twai.h>
#include <hal/twai_types.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>


void twai_init(void * parameters){
    //TWAI (CAN) init task
    for(;;){
        //TWAI_GENERAL_CONFIG_DEFAULT(tx_io_num, rx_io_num, op_mode)
        twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_23, GPIO_NUM_22, TWAI_MODE_NO_ACK);
        twai_timing_config_t t_config = TWAI_TIMING_CONFIG_1MBITS();
        twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

        //Install TWAI driver
        if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
            printf("Driver installed\n");
        } else {
            printf("Failed to install driver\n");
        }
        //Start TWAI driver
        if (twai_start() == ESP_OK) {
            printf("Driver started\n");
        } else {
            printf("Failed to start driver\n");
        }

    vTaskDelete(NULL);
    }
}

void twai_read(void * parameters){
    twai_message_t message;
    twai_message_t temp_message;
    short data_buffer[64];
    int pointer = 0;
    int length = 0;
    uint32_t id = 0;

    for(;;){
        if(twai_receive(&temp_message, 5000) == ESP_OK){
            if(temp_message.identifier != 513){
            }
            else{
                message = temp_message;

                for (int i = 0; i < message.data_length_code; i++){
                    if(message.data[i] == 0x55){

                        //print out data_buffer
                        printf("\n\nRecieved message, ID = %" PRIu32 ", length = %i, pointer = %i", id, length, pointer);
                        for(int i = 0; i < pointer; i++){
                            if(i % 8 == 0){
                                printf("\n");
                            }
                            printf("%#02x ", data_buffer[i]);
                        }
                        if(pointer == length){
                            printf("\nMSG OK\n");
                        }
                        else if(pointer < length){
                            printf("\nMSG TOO SHORT\n");
                        }
                        else{
                            printf("\nMSG TOO LONG\n");
                        }
                
                        //initiate new data_buffer
                        id = message.identifier;
                        length = message.data[i+1];
                        pointer = 0;
                    }

                    data_buffer[pointer] = message.data[i];
                    printf("%#02x ", data_buffer[pointer]);
                    pointer++;
                }
            }
        }

        else{
            printf("No message\n");
        }
    } 
}

void send_command(int command_number){
    static const uint8_t can_command_list[1][73] = {
        {0x55,0x0E,0x04,0x66,0x09,0x03,0xd4,0x71,0xA0,0x48,0x08,0x01,0xF1,0xa3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };


    int command_len = can_command_list[command_number][1];
    int data_len = 0;

    if(command_len % 8 != 0){
        data_len = command_len + 8 - command_len % 8;
    }

    printf("command_len = %i\n", command_len);

    twai_message_t message;

    message.identifier = 0x201;
    message.extd = 1;
    message.data_length_code = 8;

    for(int j = 0; j < data_len/8; j++){
        message.data_length_code = 0;
        printf("msg %i: ",j);

        for(int i = 0; i < 8; i++){
            if((8*j + i) < command_len){
                message.data_length_code++;
                printf("%#02x | ",can_command_list[command_number][i + j*8]);
                message.data[i] = can_command_list[command_number][i + j*8];
            }
        }
        send_can(message);
    }
}

void send_can(twai_message_t message){
    if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        printf("Message queued for transmission\n");
    } else {
        printf("Failed to queue message for transmission\n");
    }
}

void can_print_alerts(void){
    uint32_t alerts = 0;
    printf("CAN alerts: %i\n", (int) alerts);
    twai_read_alerts(&alerts, 0);
    printf("CAN alerts: %i\n", (int) alerts);
}
