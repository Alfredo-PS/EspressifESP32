/*
--- Lectura de pulsos por vuelta de un encoder de cuadratura ---

Este codigo está diseñado para realizar 20 lecturas de un ecoder magnetico por medio del PCNT de la ESP32, 
la lectura se guarda en un arreglo cada vez que se activa una interrupción asociada al pin del sensor de limite 
que detecta cuando el motor completa una vuelta, esto permite verificar la integridad del encoder y conocer
el número de pulsos por vuelta, el código está optimizado para realizar las lecturas a una alta velocidad

Laboratorio de Róbotica. Facultad de Ingenieria, UNAM.
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/pulse_cnt.h"
#include "esp_log.h"
#include "driver/gpio.h"


// Definición de Pines 
#define ENCODER_GPIO_A 18
#define ENCODER_GPIO_B 19
#define BOTON_GPIO 4

//Definción de variables globales para que sean accesibles a la interrupción
pcnt_unit_handle_t pcnt_unit = NULL;
int historial_conteo[20];
volatile int indice = 0; // 'volatile' porque cambia dentro de una ISR

// Manejador de la interrupción (ISR)
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    // Verificamos que no nos hayamos pasado del tamaño del arreglo
    if (indice < 20) {
        pcnt_unit_get_count(pcnt_unit, &historial_conteo[indice]);
        ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit)); 
        indice++;
    }
}

void app_main(void)
{
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

    // CONFIGURACIÓN DEL PIN DE INTERRUPCIÓN (GPIO) ---
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_POSEDGE,    // Flanco de subida
        .mode = GPIO_MODE_INPUT,           // Modo entrada
        .pin_bit_mask = (1ULL << BOTON_GPIO),
        .pull_down_en = 1,                 // Pull-down 
        .pull_up_en = 0,
    };
    gpio_config(&io_conf);



    // Instalar el servicio de ISR y añadir el manejador
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BOTON_GPIO, gpio_isr_handler, NULL);
    
    // Habilitar e Iniciar la unidad
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));

    int pulse_count = 0;
    uint32_t io_num;

    while (indice<19)
    {
        /* code */ //Ciclo que espera a que terminen las 20 lecturas
    }
       
    for(int i =0; i<20; i++){ // imprime las 20 lecturas
            printf("Conteo: %d\n", historial_conteo[i]);
    }
    
}
