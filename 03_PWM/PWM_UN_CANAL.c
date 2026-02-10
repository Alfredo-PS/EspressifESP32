#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/ledc.h"

#define PWM 16

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
    canalPWM_1.gpio_num = PWM;
    canalPWM_1.speed_mode = LEDC_HIGH_SPEED_MODE;
    canalPWM_1.channel = LEDC_CHANNEL_0;
    canalPWM_1.intr_type = LEDC_INTR_DISABLE;
    canalPWM_1.timer_sel = LEDC_TIMER_0;
    canalPWM_1.duty = 1000;  // Ciclo de trabajo 

    // Se configura el canal, y el PWM comienza, la señal se envia al GPIO 16

    ledc_channel_config(&canalPWM_1);

    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 3500); // Actualiza el PARÁMETRO de duty del PWM, con 10 bits se encuentra al 50% del ciclo de trabajo

    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0); // Actualiza la SALIDA FISICA del periférico

    while(1) {
        vTaskDelay(pdTICKS_TO_MS(1));
    }
}
