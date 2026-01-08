/*  Configuración básica de un GPIO  */

#include <stdio.h>       // Librería estandar de C para inputs/outputs
#include "driver/gpio.h" // Lirería para gestionar las funciones de los gpios

#define pin 22  // Se define el número de pin que queremos configurar

void app_main(void) // Función principal de un programa de ESP32, el firmware busca esta función para empezar operaciones.
{
    printf("# # # Configuración básica de un pin # # #\n");

    gpio_reset_pin(pin);                       // Se reinicia la configuración del pin
    gpio_set_direction(pin, GPIO_MODE_OUTPUT); // Se configura la dirección input/output del pin

    gpio_set_level(pin, 1); // Se cambia el estado del gpio de salida
}
