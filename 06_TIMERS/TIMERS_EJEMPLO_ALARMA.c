#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"
#include "esp_log.h"

volatile int dato;

// Función de llamada
static bool IRAM_ATTR funcion_ejemplo_alarma_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx){  
    dato = 1;
    return pdTRUE; 
}

void app_main(void)
{
    gptimer_handle_t gptimer = NULL;
    // Configuración del timer
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1MHz = 1us por tick
    };

    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    // Configuración de la alarma
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = 1000000, // 1 segundo
        .flags.auto_reload_on_alarm = true,
    };

    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

    // Llamada de la función del timer
    gptimer_event_callbacks_t cbs = {
        .on_alarm = funcion_ejemplo_alarma_cb,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
    
    // Activación del timer
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));


    while(1) {
        // La tarea se queda "dormida" sin gastar CPU hasta que el timer envía algo
        if (dato == 1) {
            printf("Hola mundo - Mensaje desde el Timer\n");
            dato = 0;
        }
        vTaskDelay(pdTICKS_TO_MS(1));
    }
}
