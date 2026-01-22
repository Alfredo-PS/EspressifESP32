# Convertidor digital-analogico. DAC.

El DAC de la ESP32 toma un valor de 8 bits (de 0 a 255) y lo convierte a un voltaje en los pines con salida analogica, la ESP32 sólo cuenta con dos canales y es ampliamente utilizado para generar ondas de sonido, señales de control o voltajes de referencia.
El canal 1 corresponde al GPIO 25, mientras que el canal 2 al GPIO 26; en este sentido hay tres formas de utilizar éste periferico: 
* Modo de voltaje directo: permite colocar un valor de salida, el voltaje se mantendrá hasta que se cambie.
* Modo continuo DMA: este modo utiliza acceso directo a memoria DMA para enviar una secuencia de datos a altas velocidades, y permite generar señales complejas sin cargar la CPU.
* Generador de Ondas Cosenoidales: la ESP32 cuenta con un generador de señal cosenoidal configurable en frecuencia y fase.

## DAC One Shot.
Se utiliza la libreria correspondiente:

```C
    #include "driver/dac_oneshot.h" // Para modo directo (one-shot)
```
Para la creación y configuración del DAC se sigue la misma lógica que el ADC, creando un handle, luego se rellena la estructura de configuración y se inicializa, los pasos se desglozan a continuación:
```C
    dac_oneshot_handle_t handle_dac1; //Creación del handle

    dac_oneshot_config_t dac_cfg = {
        .chan_id = DAC_CHAN_0, // DAC_CHAN_0 es GPIO 25, DAC_CHAN_1 es GPIO 26
    };

    ESP_ERROR_CHECK(dac_oneshot_new_channel(&dac_cfg, &handle_dac1)); // Se crea un nuevo canal
```
La configuración en la estructura `dac_oneshot_config_t` solo requiere el id del canal, así mismo la función dac_oneshot_new_channel() recibe las direcciones de la configuración y del handle respectivamente.

Finalmente se coloca el valor que el DAC mandentrá hasta que se actualice de nuevo, recordar que ambos canales tanto en el GPIO 25 como en el 26 son de 8 bits, por lo que los valores posibles van de 0 a 255, para colocar el valor se emplea `dac_oneshot_output_voltage()`, la cual recibe el handle y el valor de salida como entero:
```C
#include "freertos/FreeRTOS.h"
#include "driver/dac_oneshot.h"  // Header del DAC One Shot
#include "esp_log.h"

static const char *TAG = "EJEMPLO";

void app_main(void)
{
    // Se crea el handle del canal
    dac_oneshot_handle_t dac_handle;

    // Configuración del canal (Canal 0 = GPIO 25)
    dac_oneshot_config_t dac_cfg = {
        .chan_id = DAC_CHAN_0, 
    };

    // Inicializar el canal
    ESP_ERROR_CHECK(dac_oneshot_new_channel(&dac_cfg, &dac_handle));

    ESP_LOGI(TAG, "DAC inicializado en GPIO 25 (Canal 0)");

    int valorSalida = 155;

    while (1) {
        // Escribir el valor en el DAC
        ESP_ERROR_CHECK(dac_oneshot_output_voltage(dac_handle, valorSalida));

        ESP_LOGI(TAG, "Salida configurada a: %d (~2.0V)", valorSalida);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
```
El calculo del valor de salida se puede realizar como:
```C
    //Cálculo para 2V:
     //valor = (Voltaje_deseado / VDD) * 255
     valor = (2.0 / 3.3) * 255 // valor = 154.54 -> Usamos 154 o 155   
```
Se recomienda medir el voltaje VDD para mejorar la funcionalidad.
