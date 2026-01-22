# ADC
El convertidor analógico a digitial ADC, está integrado en el chip y es capaz de medir señales analógicas desde pines especificos, puede generar conversiones de un disparo o de manera continua.
La primera configuración que se revisará y la más sencilla es **"ADC One Shot"**.

## ADC One Shot
Para utilizar éste método se debe incluir la librería:
```C
    #include "esp_adc/adc_oneshot.h"
```
Los pasos básicos para usar el ADC son:
*  Asignación de recursos
*  Configuración de la unidad
*  Lectura

### Asignación de recursos.
El primer paso es declarar la instancia del ADC, por medio del identificador `adc_oneshot_unit_handle_t`, despues se debe configurar la estructura que asigna los recursos al ADC, ésta estructura se llama: `adc_oneshot_unit_init_cfg_t` seleccionando el ADC y la fuente de reloj para éste; los dos pasos se resumen en lo siguiente:
```C
    adc_oneshot_unit_handle_t handle_adc2;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_2,
        .ulp_mode = ADC_ULP_MODE_DISABLE
    };
```
Con el handle creado y la estructura de configuración lista sólo hace falta crear la unidad, por medio de la función `adc_oneshot_new_unit ()`, a continuación se presenta su implementación:
```C
  esp_err_t err = adc_oneshot_new_unit(&init_config, &handle_adc2);
```

### Configuración de la unidad.
Con el handle creado ahora se vinculará el ADC a un pin especifico por medio del canal, y se definirá la atenuación para determinar el rango de voltaje. Para lo anterior se rellena la estructura de configuración `adc_oneshot_chan_cfg_t`, que incluye los parametros de resolución y atenuación, por ejemplo:
```C
    adc_oneshot_chan_cfg_t canal_config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, // Usa el ancho por defecto Maximo (normalmente 12 bits)
        .atten = ADC_ATTEN_DB_12 // Rango hasta 3.1V
    };
```
El parametro `bitwidth` tiene los siguientes valores posibles:
* __ADC_BITWIDTH_DEFAULT:__ valor por defecto, normalmente el máximo ancho admitido.
* __ADC_BITWIDTH_9.__
* __ADC_BITWIDTH_10.__
* __ADC_BITWIDTH_11.__
* __ADC_BITWIDTH_12.__
* __ADC_BITWIDTH_13.__: consulte el máximo posible en la datasheet del modelo correspondiente

Por otra parte para el parametro de atenuación `atten` se admiten los valores:
* __ADC_ATTEN_DB_0__:       0v a 950mv
* __ADC_ATTEN_DB_2_5__:     0v a 1250 mv
* __ADC_ATTEN_DB_6__:       0v a 1750 mv
* __ADC_ATTEN_DB_12__:      0v a31000 mv

Para terminar la configuración del canal, y determinar el pin de entrada de la señal se hace uso de la función `adc_oneshot_config_channel()`, que tiene tres parametros: **1.** Handle del ADC, **2.** Canal del ADC, **3.** Estructura de configuración para el canal.
```C
  // Se configura el canal 8 del ADC2
  adc_oneshot_config_channel(handle_adc2, ADC_CHANNEL_8, &canal_config);
```
Dependiendo del modulo que se estpe utilizando de ADC, ya sea el 1 o el 2, hay diferentes opciones para el canal, las cuales se muestran a continuación para la ESP32 estándar:

| Canal ADC 1  | GPIO asociado |
| :-------------: |:-------------:|
| ADC_CHANNEL_0      |   36   |
| ADC_CHANNEL_1      |   37   |
| ADC_CHANNEL_2      |   38   |
| ADC_CHANNEL_3      |   39   |
| ADC_CHANNEL_4      |   32   |
| ADC_CHANNEL_5      |   33   |
| ADC_CHANNEL_6      |   34   |
| ADC_CHANNEL_7      |   35   |

| Canal ADC 2  | GPIO asociado |
| :-------------: |:-------------:|
| ADC_CHANNEL_0      |   4   |
| ADC_CHANNEL_1      |   0   |
| ADC_CHANNEL_2      |   2   |
| ADC_CHANNEL_3      |   15   |
| ADC_CHANNEL_4      |   13   |
| ADC_CHANNEL_5      |   12   |
| ADC_CHANNEL_6      |   14   |
| ADC_CHANNEL_7      |   27   |
| ADC_CHANNEL_8      |   25   |
| ADC_CHANNEL_9      |   26   |

