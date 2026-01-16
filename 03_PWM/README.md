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

* __gpio_num__: GPIO de salida, 0, 1, 2... 33 (Revisar disponibilidad GPIO), con gpio.h GPIO_NUM_0
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



