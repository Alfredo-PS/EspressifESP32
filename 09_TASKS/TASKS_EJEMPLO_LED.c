/*

El presente código es un ejemplo simple en el que el encendido y apagado de un led se ejecuta mediante una tarea, que cambia el valor
del GPIO cada segundo, en el main simplemente se tiene un retraso de casi 1 segundo y se cambia el valor de la bandera que despierta a la tarea.

La salida se configuro en el GPIO 4

*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
int flag;

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
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void app_main(void)
{
    TaskHandle_t tarea_handle = NULL;
    xTaskCreate(
            vTareaEjemplo,
            "NombreTarea1",
            2048,
            NULL,
            1,
            &tarea_handle
    );
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_5, 0); 
    flag = 0;   

    while (1) {

        flag = 1;
        vTaskDelay(pdMS_TO_TICKS(950));
    }
}
