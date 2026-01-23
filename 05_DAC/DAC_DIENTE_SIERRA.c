#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/dac_continuous.h"

void app_main(void)
{
    dac_continuous_handle_t dac_handle;


    dac_continuous_config_t cont_cfg = {
        .chan_mask = DAC_CHANNEL_MASK_CH0, 
        .desc_num = 4,
        .buf_size = 512,                  
        .freq_hz = 25000,
        .offset = 50,
        .clk_src = DAC_DIGI_CLK_SRC_APLL,   
        .chan_mode = DAC_CHANNEL_MODE_SIMUL,
    };

    // Intentar inicializar
    esp_err_t err = dac_continuous_new_channels(&cont_cfg, &dac_handle);

    uint8_t data[512];
    for (int i = 0; i < 512; i++) data[i] = i;

    ESP_ERROR_CHECK(dac_continuous_enable(dac_handle));


    ESP_ERROR_CHECK(dac_continuous_write_cyclically(dac_handle, data, sizeof(data), NULL));

    printf("DAC DMA iniciado correctamente.\n");
    while(1){
        //Hola mundo
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
