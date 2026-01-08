# Puertos GPIO.
## Funciones básicas de entrada y salida.

El chip de la ESP32 cuenta con 40 pads GPIO físicos, aunque algunos no se pueden usar o no tienen un pin correspondiente en el encapsulado, normalmente los modelos más comerciales de la ESP32 son de 38 o 36 pines con salidas a determinados puertos GPIO e incluyen salidas a VCC y GND.

![Pines y función de cada uno de la ESP32.](https://github.com/Alfredo-PS/EspressifESP32/blob/46660ab65ca48f275c184f119a226219ae52b4f0/GPIO/imagenesGPIO/ESP32-PINES.png) 

Algunas consideraciones importantes son que Los GPIO del 6 al 11 se utilizan para flashear (Cargar el programa en la memoria de la ESP32), los GPIO  del 34 al 39 solo se pueden configurar como modo de entrada y no poseen funciones de pullup o pulldown por software.
La libreria empleada para el control de los GPIO y que incluye la gran mayoria de funciones a utilizar es la siguiente:
```
#include "driver/gpio.h" // Lirería para gestionar las funciones de los gpios
```
### Función Reset pin
La primera función a tomar en cuenta es:

```
    esp_err_t  gpio_reset_pin(gpio_num)
```
Esta función restablece al estado predeterminado a un pin, el estado predeterminado implica que se enciende la funcion GPIO, se coloca una resistencia pullup y se deshabilitada la entrada y salida. Siempre devuelve ESP_OK, como parametro sólo necesita el número de GPIO a resetear.

La biblioteca incluye las definiciones de los pines GPIO como: `GPIO_NUM_0`, `GPIO_NUM_1`, `GPIO_NUM_2`, y así sucesivamente; las numeraciones existen hasta el pin 39, excluyendo a los pines 20, 24, y del 28 al 31. Recordar que los pines del 34 al 39 son sólo de entrada.

### Función Set Direction
La función Set Direction configura la dirección del GPIO, ya sea como entrada, salida o entrada y salida.
```
    esp_err_t   gpio_set_direction( gpio_num , modo )
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

### Función Set Level
Establece el nivel de voltaje a la salida del GPIO, según el nivel lógico que se desee.
```
    esp_err_t   gpio_set_level( gpio_num , uint32_t nivel )
```
Como primer parametro se coloca el número de pin, mientras que el segundo se puede colocar 0 para nivel bajo, y 1 para nivel alto. 

### Función Get Level.
Esta función permite conocer el estado de entrada de un pin, se debe tener en cuenta que si el pad no está configurado como entrada (o entrada y salida), el valor devuelto siempre será 0.

```
    int gpio_get_level( gpio_num )
```
Como unico parametro recibe el número de pin del cual se quiere conocer el valor, y devuelve 0 o 1, según el nivel lógico al que se encuentre el pin.

### Función Set Pull Mode
Esta función permite configurar la resistencias internas de los puertos GPIO, ya sea de pull-up o pull down.
```C++
    //Definición en libreria:
    esp_err_t gpio_set_pull_mode( gpio_num_t gpio_num , gpio_pull_mode_t pull )
    //Uso común
    gpio_set_pull_mode(numero, pull);
```
Como primer parámetro se coloca el número de GPIO, y por segundo parametro se coloca el modo de subida o bajada.
| Modo  | Descripción |
| ------------- |:-------------:|
| GPIO_PULLUP_ONLY      | Resistencia pull-up     |
| GPIO_PULLDOWN_ONLY     | Resistencia pull-down     |
| GPIO_PULLUP_PULLDOWN     | Resistencia pull-up + pull-down    |
| GPIO_FLOATING | Alta impedancia, el pin no se conecta ni a 3.3 ni a GND |

