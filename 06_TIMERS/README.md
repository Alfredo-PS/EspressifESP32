# Timers.
El GPT Timer de la ESP32 es un controlador dedicado para el manejo de temporizadores, el cual puede seleccionar diferentes fuentes de reloj y preescaladores, cuenta con varias funciones adicionales, como por ejemplo registro de tiempo transcurrido, alarmas de eventos unicos, y ciclos de tiempo muy precisos.
El primer paso para utilizar el controlador es incluir la libreria propia:
```C
#include "driver/gptimer.h"
```
## Gptimer para conteo
La primera forma de utilizar el timer y la más fácil es usarlo para la medicón de tiempos o conteo durante algún proceso, como en anteriores perfericos se crea la instancia del gptimer, luego se llena la estructura de configuración y finalmente hay que crear la nueva instancia con la función adecuada, estos pasos se presentan a continuación:
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
## Gptimer como alarma
