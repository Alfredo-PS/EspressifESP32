# Puertos GPIO.
## Funciones básicas de entrada y salida.

El chip de la ESP32 cuenta con 40 pads GPIO físicos, aunque algunos no se pueden usar o no tienen un pin correspondiente en el encapsulado, normalmente los modelos más comerciales de la ESP32 son de 38 o 36 pines con salidas a determinados puertos GPIO e incluyen salidas a VCC y GND.

![Pines y función de cada uno de la ESP32.](https://github.com/Alfredo-PS/EspressifESP32/blob/fe776c0069030b63c296f1ba1b49156af699bb2b/.addons/img01/ESP32-PINES.png) 

Algunas consideraciones importantes son que Los GPIO del 6 al 11 se utilizan para flashear (Cargar el programa en la memoria de la ESP32), los GPIO  del 34 al 39 solo se pueden configurar como modo de entrada y no poseen funciones de pullup o pulldown por software.
La libreria empleada para el control de los GPIO y que incluye la gran mayoria de funciones a utilizar es la siguiente:
```C++
#include "driver/gpio.h" // Librería para gestionar las funciones de los gpios
```
A lo largo del siguiente documento se realizará un código de ejemplo para utilizar los puertos GPIO, se recomienda realizar el código junto al grupo.

``` C++
// ------------- CODIGO DE EJEMPLO ---------------------
/*  Configuración básica de un GPIO  */

#include <stdio.h>       // Librería estandar de C para inputs/outputs
#include "driver/gpio.h" // Lirería para gestionar las funciones de los gpios

void app_main(void) // Función principal de un programa de ESP32, el firmware busca esta función para empezar operaciones.
{
    printf("# # # Configuración básica de un pin # # #\n");
}
```

## Función Reset pin
La primera función a tomar en cuenta es:

```C++
    esp_err_t gpio_reset_pin( gpio_num_t gpio_num )
```
Esta función restablece al estado predeterminado a un pin, esto implica que se enciende la funcion GPIO, se coloca una resistencia pullup y se deshabilitada la entrada y salida. Siempre devuelve ESP_OK como parametro, sólo necesita el número de GPIO a resetear.

La biblioteca incluye las definiciones de los pines GPIO como: `GPIO_NUM_0`, `GPIO_NUM_1`, `GPIO_NUM_2`, y así sucesivamente; las numeraciones existen hasta el pin 39, excluyendo a los pines 20, 24, y del 28 al 31. Recordar que los pines del 34 al 39 son sólo de entrada.

``` C++
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
}

```

## Función Set Direction
La función Set Direction configura la dirección del GPIO, ya sea como entrada, salida o entrada y salida.
```C++
    esp_err_t gpio_set_direction( gpio_num_t gpio_num , gpio_mode_t modo )
```
En el primer parametro se coloca el numero de pin, para el segundo existen varias opciones, y se debe elegir en función de lo que se necesite. A continuación se presentan los diferentes modos que existen:

| Modo          | Descripción |
| ------------- |:-------------:|
| GPIO_MODE_DISABLE      | Deshabilita entrada y salida    |
| GPIO_MODE_INPUT      | Solo entrada     |
| GPIO_MODE_OUTPUT      | Solo salida     |
| GPIO_MODE_INPUT_OUTPUT_OD | Salida y entrada con modo open-drain |
| GPIO_MODE_INPUT_OUTPUT | modo de entrada y salida |

Lo más comunes son `GPIO_MODE_INPUT` y `GPIO_MODE_OUTPUT`.

``` C++
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
}

```
## Función Set Pull Mode
Esta función permite configurar la resistencias internas de los puertos GPIO, ya sea de pull-up o pull down.
```C++
    esp_err_t gpio_set_pull_mode( gpio_num_t gpio_num , gpio_pull_mode_t pull )
```
Como primer parámetro se coloca el número de GPIO, y por segundo parametro se coloca el modo de subida o bajada.
| Modo  | Descripción |
| ------------- |:-------------:|
| GPIO_PULLUP_ONLY      | Resistencia pull-up     |
| GPIO_PULLDOWN_ONLY     | Resistencia pull-down     |
| GPIO_PULLUP_PULLDOWN     | Resistencia pull-up + pull-down    |
| GPIO_FLOATING | Alta impedancia, el pin no se conecta ni a 3.3 ni a GND |

![Comparativa entre Pull-up y pull-down.](https://github.com/Alfredo-PS/EspressifESP32/blob/66483b89631f111d1da697beac82e846a62a543f/.addons/img01/Pull-up-pull-down.jpg) 

``` C++
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
}

```

## Función Set Level
Establece el nivel de voltaje a la salida del GPIO, según el nivel lógico que se desee.
```C++
    esp_err_t gpio_set_level( gpio_num_t gpio_num , uint32_t nivel )
```
Como primer parametro se coloca el número de pin, mientras que el segundo se puede colocar 0 para nivel bajo, y 1 para nivel alto. 

``` C++
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
}

```

## Función Get Level.
Esta función permite conocer el estado de entrada de un pin, se debe tener en cuenta que si el pad no está configurado como entrada (o entrada y salida), el valor devuelto siempre será 0.

```
    int gpio_get_level( gpio_num )
```
Como unico parametro recibe el número de pin del cual se quiere conocer el valor, y devuelve 0 o 1, según el nivel lógico al que se encuentre el pin.

``` C++
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

```

