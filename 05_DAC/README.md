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

## DAC Continuos (por DMA).
En está configuración se utiliza un bufer o arreglo de enteros para guardar los valores que se cargarán al DAC, y se hace uso del DMA (Direct Memory Accsess), el cual permite transferir datos desde la RAM al periferico sin intervención constante de la CPU, lo que permite generar formas de onda complejas a alta frecuencia.

Así cómo en el ADC y el DAC one shot se debe crear un handle, al igual que una estructura de configuración para despues inicializar el canal, de la manera siguiente:
```C
    // Creación del handle
    dac_continuous_handle_t dac_handle;

    //Configuración del DAC
    dac_continuous_config_t cont_cfg = {
        .chan_mask = DAC_CHANNEL_MASK_CH0, 
        .desc_num = 4,
        .buf_size = 512,                  
        .freq_hz = 25000,
        .offset = 50,
        .clk_src = DAC_DIGI_CLK_SRC_APLL,   
        .chan_mode = DAC_CHANNEL_MODE_SIMUL,
    };

    // Inicialización
    esp_err_t err = dac_continuous_new_channels(&cont_cfg, &dac_handle);
```
Los parametros de configuración se describen a continuación:

| Parametro  | Descripción |
| ------------- |-------------|
| `chan_mask`      | Máscara de los canales a utilizar: **DAC_CHANNEL_MASK_CH0** para GPIO 25, **DAC_CHANNEL_MASK_CH1** para GPIO 26 Y **DAC_CHANNEL_MASK_ALL** para ambos|
| `desc_num`      |  Número de descriptores (entero), minimo 2, máximo 255, recomendado entre 4 y 8    |
| `buf_size`      | Tamaño del búfer, debe estar entre 32 y 4092 bytes, cada descriptor tiene un búfer de este tamaño     |
| `freq_hz`      | Frecuencia de conversión del DAC en Hz, los limites de la frecuencia dependen del reloj asociado, no se recomiendad frecuencias superiores a 2MHz     |
| `offset`      | Offset de los datos digitales del DAC, Rango de -128 hasta 127     |
| `clk_src`      | Fuente de reloj: **DAC_DIGI_CLK_SRC_DEFAULT** tiene un rango de 19.6 kHz a varios MHz, y **DAC_DIGI_CLK_SRC_APLL** para un rago de 648 Hz a varios MHz.     |
| `chan_mode`      | Establece como se envia el voltaje a los GPIO, **DAC_CHANNEL_MODE_SIMUL** envia el mismo dato a ambos GPIO si estan activos, **DAC_CHANNEL_MODE_ALTER** los datos se reparten alternadamente entre los dos canales, **DAC_CHANNEL_MODE_SINGLE** solo envía datos a un canal, el activo.   |

Para que el DAC comience la conversión y se tenga salida en el GPIO hay dos maneras de hacerlo, la primera que escribe el valor de manera continua, y otro que lo hace de forma ciclica, cada manera depende de la intención que se busque, la función dac_continuous_write() toma los datos del búfer y los escribe por DMA, una vez el bufér está vacio el pin tomará el valor final hasta que se escriban nuevos valores, al llegar al final se enciende una bandera si ya no hay más datos. Es ideal por ejemplo para reproducir señales de audio.
```C
esp_err_t ret = dac_continuous_write(dac_handle, bufer, sizeof(bufer), &num_Dat, pdMS_TO_TICKS(100));

```
El primer parametro es el handle creado anteriormente, en el segundo se tendrá el arreglo con los datos que se enviarán, despues se coloca el tamaño del bufer, se puede conocer el numero de datos realemnte escritos en el DAC para ello colocar un apuntador a una variable entera, finalmente se debe configurar un delay entre cada conversión o envio de datos.

Otra forma de utilizar el dalay de manera continua es por medio de una escritura ciclica, la cual funciona de una manera muy parecida a `dac_continuous_write()`, pero al llegar al final regresa al incio del bufer de datos, lo que permite crear señales periodicas de cualquier tipo con relavita precisión, este método emplea: `dac_continuous_write_cyclically()`, la forma de usarla es la siguiente:

```C
ESP_ERROR_CHECK(dac_continuous_write_cyclically(dac_handle, data, sizeof(data), NULL));
```
Como primer parámetro recibe el handle del DAC, luego el búfer de datos, luego el tamaño del búfer y finalmente se puede colocar un apuntador para registrar los bytes que se han cargado en el DMA, normalmente se utiliza en NULL.

A continuación se presenta un ejemplo, en el que se configura el DAC para crear una señal diente de sierra:

```C
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/dac_continuous.h"

void app_main(void)
{
    dac_continuous_handle_t dac_handle;


    dac_continuous_config_t cont_cfg = {
        .chan_mask = DAC_CHANNEL_MASK_CH0, 
        .desc_num = 4,
        .buf_size = 512,                  
        .freq_hz = 25000,
        .offset = 50,
        .clk_src = DAC_DIGI_CLK_SRC_APLL,   
        .chan_mode = DAC_CHANNEL_MODE_SIMUL,
    };

    // Intentar inicializar
    esp_err_t err = dac_continuous_new_channels(&cont_cfg, &dac_handle);

    uint8_t data[512];
    for (int i = 0; i < 512; i++) data[i] = i;

    ESP_ERROR_CHECK(dac_continuous_enable(dac_handle));


    ESP_ERROR_CHECK(dac_continuous_write_cyclically(dac_handle, data, sizeof(data), NULL));

    printf("DAC DMA iniciado correctamente.\n");
    while(1){
        //Hola mundo
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
```
## DAC Cosine.
Con este modo se puede generar una señal cosenoidal, la librería del DAC Cosine a incluir es:
```C
    #include "driver/dac_cosine.h"
```
De manera similar que en los anteriores temas, se genera un handle, se rellena la estrucutra de configuración y se inicializa un canal, la ESP32 sólo puede general una señal senoidal en un canal, esta señal se genera por hardware, lo que hace que sea un proceso eficiente energeticamente y que no requiere un alto procesamiento por parte de la CPU.

Primero se genera el handle:
```C
    dac_cosine_handle_t cos_handle;
```
Luego se genera la estrucutra de configuración:
```C
    dac_cosine_config_t cos_cfg = {
        .chan_id = DAC_CHAN_0,         // Canal 0 = GPIO 25
        .freq_hz = 10000,               // Frecuencia: 1000 Hz (1 kHz)
        .clk_src = DAC_COSINE_CLK_SRC_DEFAULT, 
        .atten = DAC_COSINE_ATTEN_DEFAULT, // Amplitud máxima (~3.3V pico a pico)
        .phase = DAC_COSINE_PHASE_0,       // Fase inicial 0°
        .offset = 0,                       // Desplazamiento de voltaje (DC Offset)
    };
```
La estructura contiene los parametos siguientes: canal o GPIO, frecuencia de la señal cosenoidal creada 
