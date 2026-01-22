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

### Lectura.
Con todo configurado se puede realizar la lectura del ADC por medio de la función: `adc_oneshot_read()`, la cual resive como parametros **1.** Handle del ADC, **2.** Canal del ADC, y **3.** Apuntador a varible entera donde se guardará el valor leído, por ejemplo de 0 a 4095. Lo anterior se utiliza como sigue:

```C
    int dato;
    adc_oneshot_read(handle_adc2, ADC_CHANNEL_8, &dato);
```
Si se requiere conocer el voltaje en lugar de un valor entero basta con hacer una regla de tres, esto para apliaciones que no requieran gran exactitud.
```C
        Vout = Dout * Vmax / Dmax
        //Dout: resultado de adc_oneshot_read()
        //Vmax: Voltaje máximo 
```
Por lo tanto el código completo de lectura para un adc one shot queda como:
```C
#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    printf("\n### ADC2 - Analog Digital Converter ###\n");

    // Inicializar ADC2
    adc_oneshot_unit_handle_t handle_adc2;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_2,
        .ulp_mode = ADC_ULP_MODE_DISABLE
    };

    esp_err_t err = adc_oneshot_new_unit(&init_config, &handle_adc2);
    if (err != ESP_OK) {
        printf("Error en la inicialización del ADC: %d\n", err);
        return;
    }

    // Configurar canal 8 del ADC2
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, // Usa el ancho por defecto (normalmente 12 bits)
        .atten = ADC_ATTEN_DB_11 // Rango hasta 3.3V
    };

    err = adc_oneshot_config_channel(handle_adc2, ADC_CHANNEL_8, &config);
    if (err != ESP_OK) {
        printf("Error en la configuración del canal: %d\n", err);
        return;
    }

    int data;
    while (1) {
        err = adc_oneshot_read(handle_adc2, ADC_CHANNEL_8, &data);
        if (err == ESP_OK) {
            printf("Valor leído: %d\n", data);
        } else {
            printf("Error al leer ADC2: %d\n", err);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

```
### Curva de calibración.
Para aplicaciones donde se requierá mayor exactitud se puede realizar la lectura del dato como voltaje usando `adc_cali_raw_to_voltage()`, está función devuelve el valor de voltaje, usarla es mucho más exacto que utilizar una regla de tres pero requiere mayor configuración y procesamiento, esto debido a que la función considera la no linealidad de la curva de voltaje, pues a los extremos suele diferir por variaciones de fabricación por lo que utiliza una curva de calibración almacenada en memoria desde su fabricación, no usar está función puede llevar a un error de hasta 200 mV según el contexto.

La función requiere incluir dos librerías adicionales, condifurar y activar la curva de ajuste con los parametros del chip y el uso de la función solicita tres parametros:**1.** Handle del ADC, **2.** El valor entero obtenido **3.** Puntero a un entero donde se guardará la lectura en **milivolts**. A continuación se presenta el uso de `adc_cali_raw_to_voltage()`.
```C
#include "esp_adc/adc_cali.h"           // Interfaz genérica de calibración
#include "esp_adc/adc_cali_scheme.h"    // Esquemas de calibración (Line Fitting /

// Declarar el handle de calibración
adc_cali_handle_t cali_handle = NULL;

// Configurar el esquema (para ESP32 es Line Fitting)
adc_cali_line_fitting_config_t cali_config = {
    .unit_id = ADC_UNIT_1,
    .atten = ADC_ATTEN_DB_12,           // Debe coincidir con la del canal
    .bitwidth = ADC_BITWIDTH_DEFAULT,
};

// Crear el esquema
esp_err_t ret = adc_cali_create_scheme_line_fitting(&cali_config, &cali_handle);

if (ret == ESP_OK) {
    // A partir de aquí, ya se puede usar adc_cali_raw_to_voltage()
    int raw_result;
    int voltage_mv;

    // 1. Obtener el valor crudo (ejemplo: 2450)
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &raw_result));

    // 2. Convertir a voltaje usando el handle de calibración
    // Si raw_result es 2450 y la atenuación es 12dB, voltage_mv podría ser ~1850 mV
    esp_err_t ret = adc_cali_raw_to_voltage(cali_handle, raw_result, &voltage_mv);

    if (ret == ESP_OK) {
        printf("Lectura Cruda: %d | Voltaje Calibrado: %d mV\n", raw_result, voltage_mv);
    }
    
}
```
### Terminar de usar el ADC
Una vez que se haya terminado de utilizar el ADC los recurosos empleados se pueden "reciclar" mediante la función adc_oneshot_del_unit(), como único parametro recibe el handle del ADC usado, por ejemplo:
```C
    ESP_ERROR_CHECK(adc_oneshot_del_unit(handle_adc2));
```
### Referencias.
Pra más imporación puedes consultar la documentación oficial [Analog to Digital Converter (ADC) Oneshot Mode Driver](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/adc_oneshot.html).
