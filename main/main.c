#include <stdio.h>
#include "batt_mon.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"

static const char *TAG = "BATT";
QueueHandle_t uartEventQueue;

void app_main(void)
{
    batt_cfg cfg={
        .adc_channel=ADC_CHANNEL_7,
        .adc_unit=ADC_UNIT_1,
        .pg_pin=42,
        .s2_pin=41,
        .s1_pin=40
    };
    batt_mon_init(cfg);
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_param_config(UART_NUM_0, &uart_config);

    uart_set_pin(UART_NUM_0, 43, 44, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_driver_install(UART_NUM_0, 4096 * 2, 0, 16, &uartEventQueue, 0);
    while(1)
    {
        float batt_level = get_batt_level();
        esp_rom_printf("Batt volt:%d",(int)batt_level);
        int batt_status = get_batt_status(); 
        esp_rom_printf("Batt status:%d",batt_status);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}