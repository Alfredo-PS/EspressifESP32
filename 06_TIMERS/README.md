# Timers.
El GPTimer de la ESP32 es un controlador dedicado para el manejo de temporizadores, el cual puede seleccionar entre diferentes fuentes de reloj y preescaladores, cuenta con varias funciones adicionales, como por ejemplo el registro de tiempo transcurrido, alarmas de eventos unicos, y ciclos de tiempo muy precisos.
El primer paso para utilizar el controlador es incluir la libreria propia:
```C
#include "driver/gptimer.h"
```
## Gptimer para conteo
La primera forma de utilizar el timer y la más fácil es usarlo para la medicón de tiempos o conteo durante algún proceso, como en anteriores perifericos se crea la instancia del gptimer, luego se llena la estructura de configuración y finalmente hay que crear la nueva instancia con la función adecuada, estos pasos se presentan a continuación:
```C
gptimer_handle_t gptimer = NULL;
gptimer_config_t timer_config = {
    .clk_src = GPTIMER_CLK_SRC_DEFAULT, // Fuente de reloj
    .direction = GPTIMER_COUNT_UP,      // Dirección de conteo ascendente
    .resolution_hz = 1 * 1000 * 1000,   // Resolución en Hz
};

// Creación de la instancia
ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));
```
Una vez creada la instancia se habilita el timer, y finalmente se puede dar la instrucción de comenzar el conteo, ambas aciones por medio de las funciones correspondientes y el handle del gptimer:
```C
// Se habilita el timer
ESP_ERROR_CHECK(gptimer_enable(gptimer));
// Comienza el conteo
ESP_ERROR_CHECK(gptimer_start(gptimer));
```
Si se desea conocer la cuenta actual del timer (como valor crudo), se utiliza la función `gptimer_get_raw_count()`, para comvertir el valor de conteo a una unidad temporal se utiliza la resolución del timer, la cual tambien se puede obtener por medio de la función `gptimer_get_resolution()`, el tiempo en segundos se obtiene dividiendo el conteo sobre la resolución:
```C
uint64_t cuentas;

// Se lee el valor de conteo actual
ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &cuentas));

// (Opcional) Se comvierte el valor de las cuentas a segundos
uint32_t resolucion_hz;

// Se obtiene la resolución 
ESP_ERROR_CHECK(gptimer_get_resolution(gptimer, &resolucion_hz));

double time = (double)cuentas / resolucion_hz;

```
## Gptimer con alarma, repetitiva o de evento unico.
La utilización del timer como alarma sigue practicamente el mismo proceso anterior con la diferencia de que se añaden algunos pasos extra y se puede crear una función de llamada que se activará cuando el timer termine de contar, permitiendo ejecutar codigo cada cierto tiempo. Los pasos de la configuración de muestran a continuación:
```C
// Antes del main
gptimer_handle_t gptimer = NULL;
//Definción de la función de llamada para el timer:
static bool funcion_ejemplo_alarma_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    // Función de ejemplo que sólo desativa el conteo del timer
    gptimer_stop(timer);
    return false;
}
// Configuración y creación de la instancia del GPTimer (igual que en el caso anterior)

gptimer_config_t timer_config = {
    .clk_src = GPTIMER_CLK_SRC_DEFAULT, // Select the default clock source
    .direction = GPTIMER_COUNT_UP,      // Counting direction is up
    .resolution_hz = 1 * 1000 * 1000,   // Resolution is 1 MHz, i.e., 1 tick equals 1 microsecond
};
ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));
```
En el caso anterior se declaran de manera global el timer, y se crea una función de callback para las cuentas deel timer, que será llamada cada vez que el gptimer timerne su conteo establecido. 
Posteriormente se configuran las funciones de alarma del GPTimer y se le indica al controlador cual será la función que time que llamar cuando se active el evento de la alarma, adicionalmente se le pueden pasar los datos o información relevante a la función de callback para procesarlos dentro de la misma, y finalmente se habilita el timer:
```C
gptimer_alarm_config_t alarm_config = {
    .alarm_count = 1000000, // Set the actual alarm period, since the resolution is 1us, 1000000 represents 1s
    .flags.auto_reload_on_alarm = false; // Aquí se activa o desativa la repetición de la alarma
};
// Set the timer's alarm action
ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

gptimer_event_callbacks_t cbs = {
    .on_alarm = example_timer_on_alarm_cb, // se declara la función de llamada definida por el usuario
};
// Se registra la función de Callback del temporizador que permite trasladar los datos deseados a la función
ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
// Se habilita el timer
ESP_ERROR_CHECK(gptimer_enable(gptimer));
// Comienza el timer
ESP_ERROR_CHECK(gptimer_start(gptimer));
```
