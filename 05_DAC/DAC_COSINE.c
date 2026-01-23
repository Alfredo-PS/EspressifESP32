#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/dac_cosine.h" 

void app_main(void)
{
    // Declarar el handle del canal
    dac_cosine_handle_t chan_handle;

    // Configuración de la onda
    dac_cosine_config_t cos_cfg = {
        .chan_id = DAC_CHAN_0,         // Canal 0 = GPIO 25
        .freq_hz = 10000,               // Frecuencia: 1000 Hz (1 kHz)
        .clk_src = DAC_COSINE_CLK_SRC_DEFAULT, 
        .atten = DAC_COSINE_ATTEN_DEFAULT, // Amplitud máxima (~3.3V pico a pico)
        .phase = DAC_COSINE_PHASE_0,       // Fase inicial 0°
        .offset = 0,                       // Desplazamiento de voltaje (DC Offset)
    };

    // Inicializar el canal
    ESP_ERROR_CHECK(dac_cosine_new_channel(&cos_cfg, &chan_handle));

    // Iniciar la generación de la onda
    ESP_ERROR_CHECK(dac_cosine_start(chan_handle));

    printf("Generando onda senoidal de 1kHz en GPIO 25...\n");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
