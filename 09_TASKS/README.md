# Tareas con FreeRTOS.
Una tarea es una función independiente y autónoma que ejecuta una acción especifica dentro de un sistema. En la ESP32 el planificador o scheduler gestiona estas tareas asignandole prioridades y cambiando entre ellas para dar la ilusión de ejecución simultanea en cada nucleo.
La librería correspondiente para crear una tarea es: 

```C
#include "freertos/task.h"
```

La creación de una tarea se debe inciar antes (o despues) del main, programando la función que implementará las acciones deseadas por la tarea, la principal caracteristica y más importante que no se debe omitir nunca en la creación de tareas es el uso de un bucle principal que generalmente se repite siempre, y dentro del mismo colocar un `if(bandera)` o en algunos casos un delay, esto es obligatorio pues permite ceder tiempo para evitar que el Watchdog se dispare  debido a que la tarea acapara el CPU. Las tareas deben implementarse para que nunca retornen es decir deben ser de bucle continio. La estructura general de una tarea se puede ver como:

```C
void vTareaEjemplo(void *pvParameters) {
    // Configuración inicial (se ejecuta una vez)
    printf("Iniciando tarea...\n");

    while(1) {
        // Lógica principal
        if (flag){
            printf("Haciendo algo divertido :)\n");
        }

        // El "Respiro" (Bloqueo)
        // Es OBLIGATORIO ceder tiempo. Si no hay delay o bloqueo, 
        // el Watchdog se disparará porque la tarea "acapara" el CPU.
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
```
Luego dentro del main se crea la tarea, en general existen dos metodos: `xTaskCreate()` donde el sistema es el que decide en que núcleo colocar la tarea, usualmente donde haya menos carga pero ademas los bloques de memoria para las estructuras de datos y las pilas se asignan dinamicamente. El otro método de cración de tareas es `xTaskCreateStatic()` este metodo permite crear una tarea el programador debe asignar la memoria necesaria.

La configuración más básica y general de una tarea es la siguiente:
```C
TaskHandle_t *const pxCreatedTask)
TaskHandle_t tarea_handle = NULL;
// static inline BaseType_t xTaskCreate(TaskFunction_t pxTaskCode, const char *const pcName, const configSTACK_DEPTH_TYPE usStackDepth, void *const pvParameters, UBaseType_t uxPriority, TaskHandle_t *const pxCreatedTask)
xTaskCreate(
            vTareaEjemplo,
            "NombreTarea1",
            2048,
            NULL,
            1,
            &tarea_handle
          )     
```
En donde los parametros de la función en orden son:
| Parametro  | Valor |
| :-------------: |-------------|
| **pxTaskCode**      | puntero a la función de entrada de la tarea  |
| **pcName**      | nombre descriptivo para la tarea, con fines de depuración, longitud maxima de 16     |
| **usStackDepth**      | tamaño de la pila, especificado en Bytes     |
| **uxPriority**      |  Prioridad con la que se debe ejecutar la tarea    |
| **pxCreatedTask**      | identificador de la tarea (handle)     |

Un ejemplo básico de una tarea es:

```C
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
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

Normalmente las tareas se "despiertan" o activan por medio de una bandera como se explicó anteriormente, y dicha bandera se alza de diferentes maneras, por ejemplo a travez de una ISR por GPIO, o con un GPTimer, una implementación basica de estos casos es la siguiente:

Finalmente, existen diferentes formas para comunicarse entre tareas o interrupciones, la primera de estás formas es con colas `Queues`, este metodo consiste en crear una cola a la cual pueden acceder globalmente las tareas o las ISR, el uso de colas es la forma más robusta de hacerlo, y permite enviar información especifica.

El metodo de configuración es la siguiente:
Se comienza por incluir la libreria correspondiente:
```C
#include "freertos/queue.h"
```

