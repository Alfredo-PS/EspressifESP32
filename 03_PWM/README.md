# PWM
El PWM (Modulación por Ancho de Pulso) es una técnica digital que controla la potencia entregada a un dispositivo variando el tiempo que una señal cuadrada está encendida, la señal digital se ajusta en dos parametros principales, la frecuencia y el ciclo de trabajo.

La ESP32 cuenta con dos sistemas diferentes capaces de generar señales de PWM, el más sencillo es el periferico de control **LEDC**, diseñado principalmente para controlar la intesidad de brillo de los LED, aunque tambien puede utilizarse para otros fines; el segundo periferico es el PWM para control de motores **MCPWM**, el cual es un generador más versátil y a la vez complejo, pero es ideal para aplicaciones de alto nivel en manejo de motores, conversión de potencia basada en conmutación, control de DAC's, y aplicaciones varias.

## Led Control. LEDC.
La configuración de éste periferico se realiza en tres pasos:
* Configuración de un temporizador, especificando la frecuencia del PWM, y la resolución del ciclo de trabajo.
* Configuración del canal para asociarlo al temporizador y al puerto GPIO.
* Ajuste de la señal de PWM para modificar el ciclo de trabajo, ya sea por software o con funciones de atenuacción por hardware.

### LEDC TIMER CONFIG
El primer paso es utilizar la siguiente función para configurar un temporizador, el cual contendrá cuatro parametros: modo, temporizador fuente, frecuencia, y resolución del ciclo de trabajo; la funcion recibe como único parametro la dirección de la estrucutura `timer_conf`.

```C
esp_err_t ledc_timer_config ( const ledc_timer_config_t * timer_conf )
```
La estrucutura `timer_conf` contiene los siguientes parámetros:

* __speed_mode__: modo de velocidad, `LEDC_LOW_SPEED_MODE` y `LEDC_HIGH_SPEED_MODE`.
* __duty_resolution__: Los valores posibles van desde `LEDC_TIMER_1_BIT`, `LEDC_TIMER_2_BIT`, ... `LEDC_TIMER_20_BIT`.
* __timer_num__: sólo existen 4 temporizadores `LEDC_TIMER_0`, `LEDC_TIMER_1`, `LEDC_TIMER_2`, y `LEDC_TIMER_3`.
* __freq_hz__: frecuencia en Hz, tipo de dato `uint32_t`.

Ejemplo de configuración: 
```C
    // Se declara la estrutura de configuración
    ledc_timer_config_t timer_conf = {0}; 

    // Llenado de parámetros
    timer_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
    timer_conf.duty_resolution = LEDC_TIMER_10_BIT;
    timer_conf.timer_num = LEDC_TIMER_0; 
    timer_conf.freq_hz = 20000;             // 20kHz

    // Configuración del timer con las caracteristicas deseadas
    ledc_timer_config(&timerConfig); // La estrucutura se envía como puntero (dirección)
```
### LEDC CHANNEL CONFIG
La función para configurar el canal especifico del PWM es:
```C
    esp_err_t ledc_channel_config(const ledc_channel_config_t *ledc_conf)
```
En está función se define el GPIO de salida, si tiene o no interrupciones, el temporizador fuente,  y ciclo de trabajo deseado, nuevamente esto se realiza a travez de una estructura `ledc_conf`; la cual tiene como parámetros: 

* __gpio_num__: GPIO de salida, 0, 1, 2... 33 (Revisar disponibilidad GPIO), con gpio.h `GPIO_NUM_0`
* __speed_mode__: `LEDC_LOW_SPEED_MODE` o `LEDC_HIGH_SPEED_MODE`
* __channel__: el canal es una etiqueta identificadora que va de `LEDC_CHANNEL_0` hasta `LEDC_CHANNEL_7`
* __intr_type__: habilitar o deshabilitar interrupción de desvanecimiento, comunmente `LEDC_INTR_DISABLE`.
* __timer_sel__: designa el timer a utilizar, se debe colocar el timer configurado anteriormente `LEDC_TIMER_x`
* __.duty__: coloca el ciclo de trabajo como un entero no signado que puede ir de 0 hasta (2^n)-1, donde n es la resolución configurada en el timer usado.

Ejemplo:
```C
    // Se crea la estrutura que guarda la configuración del canal
    ledc_channel_config_t channelConfig = {0};

    // Se rellena la estructura
    channelConfig.gpio_num = 22;
    channelConfig.speed_mode = LEDC_HIGH_SPEED_MODE;
    channelConfig.channel = LEDC_CHANNEL_0;
    channelConfig.intr_type = LEDC_INTR_DISABLE;
    channelConfig.timer_sel = LEDC_TIMER_0;
    channelConfig.duty = 0;  // Ciclo de trabajo en cero

    // Se configura el canal, y el PWM comienza, la señal se envia al GPIO 22
    ledc_channel_config(&channelConfig);
```
### Ciclo de trabajo.
Hay varias manera de cambiar el ciclo de trabajo, la primera es con las funciones `ledc_set_duty` y `ledc_update_duty`.
```C
    // Cambia el valor en el registro
    esp_err_t ledc_set_duty(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty)

    // Actualiza la salida con el nuevo ciclo de trabajo
    esp_err_t ledc_update_duty(ledc_mode_t speed_mode, ledc_channel_t channel)

```
`ledc_set_duty` Requiere:
* Modo
* canal, es decir salida que cambiará
* Nuevo ciclo de trabajo

Por otra parte `ledc_update_duty` necesita de:
* Modo
* Canal

Juntas se utilizarían:
```C
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 510); // Actualiza el PARÁMETRO de duty del PWM, con 10 bits se encuentra al 50% del ciclo de trabajo

    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0); // Actualiza la SALIDA FISICA del periférico

```
Una nueva forma implementada recientemente es por medio de `ledc_set_duty_and_update`, está función realiza ambas tareas, recibe como parametros:
* Modo
* Canal
* Ciclo de trabajo nuevo
* Hpoint, punto de incio de la fase, casi siempre en 0.
```C
esp_err_t ledc_set_duty_and_update ( ledc_mode_t modo_velocidad , ledc_channel_t canal , uint32_t duty , uint32_t hpoint ) 
```
Ejemplo:
```C
    // Actualiza la salida al 50% del ciclo de trabajo.
    ledc_set_duty_and_update(LEDC_HIGHT_SPEED_MODE, LEDC_CHANNEL_0, 510, 0);
```
### Otras herramientas.

```C
    esp_err_t ledc_stop(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t idle_level)
```
Esta función es útil para desactvar la salida PWM de un canal, permite dejar en Pin en estado alto o bajo segun se necesite, esto se configura en idl (0 para 0V, y 1 para 3.3V):
```C
    // Se corta la generación de la señal PWM y el pin de deja en 0 V
    ledc_stop(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
```
Tambien es posible cambiar la frecuencia de un Timmer mientras este opera sin necesidad de reiniciarlo. 

```C
    esp_err_t ledc_set_freq(ledc_mode_t speed_mode, ledc_timer_t timer_num, uint32_t freq_hz)
```
Recordar que un mismo Timer puede tener asignados varios canales, al cambiar la freucuencia esto afectará a todos los que esten conectados a ese Timer.
```C
    // Cambia la frecuencia del Timer 0 a 10kHz
    ledc_set_freq(LEDC_HIGHT_SPEED_MODE, LEDC_TIMER_0, 10000);
```

Para más información puede consultar: 

[LEDC Documentacion Espressif](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/ledc.html)

[MCPWM Documentacion Espressif](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/mcpwm.html) 
