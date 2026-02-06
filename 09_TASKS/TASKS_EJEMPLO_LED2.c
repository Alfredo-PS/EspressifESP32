/*

Código que permite encender un led mediante una interrupción, ejemplo muuuuy básico de como usar una interrupción por GPIO para "despertar" una tarea, el boton está 
configurado como entrada en el GPIO 5, y la salida en el GPIO 4

*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
volatile int flag;

void vTareaEjemplo(void *pvParameters) {
    printf("Iniciando tarea...\n");
    int valor = 0;

    while(1) {
        // Lógica principal
        if (flag== 1){
            printf("Haciendo algo divertido :)\n");
            if(valor == 0){
                valor = 1;
            }else{
                valor = 0;
            }

            gpio_set_level(GPIO_NUM_4, valor);
            
            flag = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void IRAM_ATTR fun_interrupcion(void* datos) {
    flag = 1;
}

void app_main(void)
{
    gpio_config_t pin_salida = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << GPIO_NUM_4) 
    };
    gpio_config(&pin_salida);

    gpio_config_t pin_entrada = {
        .intr_type = GPIO_INTR_POSEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << GPIO_NUM_5), 
        .pull_up_en = 1,
        .pull_down_en = 0
    };
    gpio_config(&pin_entrada);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_NUM_5, fun_interrupcion, NULL);


    flag = 0;   
    TaskHandle_t tarea_handle = NULL;
    xTaskCreate(
            vTareaEjemplo,
            "NombreTarea1",
            2048,
            NULL,
            1,
            &tarea_handle
    );

    while (1) {
        printf("Haciendo nada en el main \n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
