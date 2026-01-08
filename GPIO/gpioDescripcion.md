# Puertos GPIO.
## Funciones básicas de entrada y salida.

El chip de la ESP32 cuenta con 40 pads GPIO físicos, aunque algunos no se pueden usar o no tienen un pin correspondiente en el encapsulado, normalmente los modelos más comerciales de la ESP32 son de 38 o 36 pines con salidas a determinados puertos GPIO e incluyen salidas a VCC y GND.

Algunas consideraciones importantes son que Los GPIO del 6 al 11 se utilizan para flashear (Cargar el programa en la memoria de la ESP32), los GPIO  del 34 al 39 solo se pueden configurar como modo de entrada y no poseen funciones de pullup o pulldown por software.
La libreria empleada para el control de los GPIO y que incluye la gran mayoria de funciones a utilizar es la siguiente:
```
#include "driver/gpio.h" // Lirería para gestionar las funciones de los gpios
```



