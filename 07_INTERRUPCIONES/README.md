# Interrupciones GPIO.
A diferencia de otros microcontroladores la ESP32 tiene una matriz de interrupciones que permite que casi cualquier periferico active una interrupción en cualquiera de los dos nucleos, para utilizarlas con los puertos GPIO lo primero es configurar el puerto a utilizar comó se ha revisado anteriormente en Puertos GPIO:
```C
#include "driver/gpio.h"
```
Con la biblioteca adecuado se procede a confgirar el pin:
```C
// Llenado de la estractura de configuración
gpio_config_t io_conf = {
    .intr_type = GPIO_INTR_POSEDGE,    // Interrupción por flanco de subida
    .mode = GPIO_MODE_INPUT,          // Modo entrada
    .pin_bit_mask = GPIO_NUM_4,      // GPIO 4
    .pull_up_en = 1,                  // Habilitar pull-up interno
};

gpio_config(&io_conf);
```
Posteriormente se debe instalar el servisio de ISR de la ESP32 para los puertos GPIO, esto por medio de la función `gpio_install_isr_service()`, como único argumento recibe una bandera de interrupción que normalmente se coloca en `0`:
```C
  // Activación de servicios ISR
  gpio_install_isr_service(0)
```
Con el servicio instalado ahora se debe agregar el controlador ISR al pin GPIO correspondiente. La función recibe tres parametos: el numero de pin, el handle de ISR es decir el "nombre" de la función a la que queremos llamar, y finalmente un puntero a cualquier tipo de dato o estructura que se puede usar para pasar datos a la ISR:

```C
  gpio_isr_handler_add(GPIO_NUM_4, fun_interrupcion, (void*) dato);
```
La función ISR se debe programar teniendo en cuenta que su ejecución debe ser lo más rápido posible para evitar bloqueos en los nucleos o codigos de error. Se puede aregrar un atributo a la función de ISR `IRAM_ATTR` el cual permite ubicar el código de la interrupción en la RAM, lo que evita retrasos en caso de que la memoria Flash se encuentre ocupada; un ejemplo muy simple de una función sería el siguiente:
```C
void IRAM_ATTR fun_interrupcion(void* dato) {

    uint32_t valor = (uint32_t) dato;
}
```
Es importante recordar que se debe evitar el uso de funciones o metodos bloqueantes en las ISR, como lo pueden ser el uso de `printf`, `ESP_LOG`, o retardos como `vTaskDelay`, si se desea realizar un proceso complejo despues de que se active una interrupción se recomienda hacer uso de tareas con FreeRTOS.

En conjunto un programa que permite el encendido y apagado de un led en el GPIO 25 mediante interrupción con un boton en el GPIO 14:
