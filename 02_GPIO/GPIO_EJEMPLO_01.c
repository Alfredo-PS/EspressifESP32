// ------------- CODIGO DE EJEMPLO ---------------------
/*  Configuración básica de un GPIO  */

#include <stdio.h>       // Librería estandar de C para inputs/outputs
#include "driver/gpio.h" // Lirería para gestionar las funciones de los gpios

void app_main(void) // Función principal de un programa de ESP32, el firmware busca esta función para empezar operaciones.
{
    printf("# # # Configuración básica de un pin # # #\n");

    gpio_reset_pin(GPIO_NUM_4); // Se reinicia la configuración del pin 4
    gpio_reset_pin(GPIO_NUM_16);
    gpio_reset_pin(GPIO_NUM_17);

    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT); // Establece el pin 4 como Salida
    gpio_set_direction(GPIO_NUM_16, GPIO_MODE_OUTPUT); // Establece el pin 16 como Salida
    gpio_set_direction(GPIO_NUM_17, GPIO_MODE_INPUT); // Establece el pin 17 como Entrada

    gpio_set_pull_mode(GPIO_NUM_17, GPIO_PULLDOWN_ONLY) // Habilita la resistencia de pull-down en el GPIO 17

    gpio_set_level(GPIO_NUM_4, 0); // Establece en bajo el nivel lógico del GPIO 4
    gpio_set_level(GPIO_NUM_16, 1); // Establece en alto el nivel lógico del GPIO 16

    int valor = gpio_get_level(GPIO_NUM_17); // Guarda el valor del pin 17 en "Valor"

    printf(valor); // Imprime valor
}
