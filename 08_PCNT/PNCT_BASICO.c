#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/pulse_cnt.h"
#include "esp_log.h"
#include "driver/gpio.h"

// Definición de Pines 
#define ENCODER_GPIO_A 18
#define ENCODER_GPIO_B 19

pcnt_unit_handle_t pcnt_unit = NULL;

volatile int indice = 0; // 'volatile' 

void app_main(void)
{
    int conteo;
    // Se habilitan resistencias para asegurar el correcto funcionamiento
    gpio_set_pull_mode(18, GPIO_PULLDOWN_ONLY);
    gpio_set_pull_mode(19, GPIO_PULLDOWN_ONLY);
    // Configuración de la Unidad PCNT
    pcnt_unit_config_t unit_config = {
        .high_limit = 20480,   // Límite máximo
        .low_limit = -20480,  // Límite mínimo
    };
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    // Configuración del Canal A
    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num = ENCODER_GPIO_A,
        .level_gpio_num = ENCODER_GPIO_B,
    };
    pcnt_channel_handle_t pcnt_chan_a = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_a_config, &pcnt_chan_a));

    // Configuración del Canal B
    pcnt_chan_config_t chan_b_config = {
        .edge_gpio_num = ENCODER_GPIO_B,
        .level_gpio_num = ENCODER_GPIO_A,
    };
    pcnt_channel_handle_t pcnt_chan_b = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_b_config, &pcnt_chan_b));

    // Definir acciones para cuadratura
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    // Filtro de Glitch (opcional, elimina ruido)
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 100, // bajo valor para que la lectura no pierda pasos
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));
    
    // Habilitar e Iniciar la unidad
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));

    while (1)
    {
        /* code */ //Ciclo que espera a que terminen las 20 lecturas
        pcnt_unit_get_count(pcnt_unit, &conteo);
        printf("Conteo: %d\n", conteo);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
