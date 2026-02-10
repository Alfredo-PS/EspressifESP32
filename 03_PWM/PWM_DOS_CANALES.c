#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/ledc.h"


#define PWM1 16
#define PWM2 17



void app_main(void)
{
    // Configuración PWM
    // Se declara la estrutura de configuración
    ledc_timer_config_t timer_conf_1 = {0}; 

    // Llenado de parámetros
    timer_conf_1.speed_mode = LEDC_HIGH_SPEED_MODE;
    timer_conf_1.duty_resolution = LEDC_TIMER_12_BIT;
    timer_conf_1.timer_num = LEDC_TIMER_0; 
    timer_conf_1.freq_hz = 10000;             // 20kHz



    // Configuración del timer con las caracteristicas deseadas
    ledc_timer_config(&timer_conf_1); // La estrucutura se envía como puntero (dirección)

    // canal 1
    ledc_channel_config_t canalPWM_1 = {0};

    // Se rellena la estructura
    canalPWM_1.gpio_num = PWM1;
    canalPWM_1.speed_mode = LEDC_HIGH_SPEED_MODE;
    canalPWM_1.channel = LEDC_CHANNEL_0;
    canalPWM_1.intr_type = LEDC_INTR_DISABLE;
    canalPWM_1.timer_sel = LEDC_TIMER_0;
    canalPWM_1.duty = 1000;  // Ciclo de trabajo en cero

    // Se configura el canal, y el PWM comienza, la señal se envia al GPIO 16

    ledc_channel_config(&canalPWM_1);

    // canal 1
    ledc_channel_config_t canalPWM_2 = {0};

    // Se rellena la estructura
    canalPWM_2.gpio_num = PWM2;
    canalPWM_2.speed_mode = LEDC_HIGH_SPEED_MODE;
    canalPWM_2.channel = LEDC_CHANNEL_1;
    canalPWM_2.intr_type = LEDC_INTR_DISABLE;
    canalPWM_2.timer_sel = LEDC_TIMER_0;
    canalPWM_2.duty = 2000;  // Ciclo de trabajo en cero

    // Se configura el canal, y el PWM comienza, la señal se envia al GPIO 17
    ledc_channel_config(&canalPWM_2);


    while(1) {
        vTaskDelay(pdTICKS_TO_MS(1));
    }
}
