### PWM
El PWM (Modulación por Ancho de Pulso) es una técnica digital que controla la potencia entregada a un dispositivo variando el tiempo que una señal cuadrada está encendida, la señal digital se ajusta en dos parametros principales, la frecuencia y el ciclo de trabajo.

La ESP32 cuenta con dos sistemas diferentes capaces de generar señales de PWM, el más sencillo es el periferico de control **LEDC**, diseñado principalmente para controlar la intesidad de brillo de los LED, aunque tambien puede utilizarse para otros fines; el segundo periferico es el PWM para control de motores (**MCPWM**), el cual es un generador más versátil y a la vez complejo, pero es ideal para aplicaciones de alto nivel en control de motores, conversión de potencia basada en conmutación, control de DAC's, y aplicaciones varias de control.

### Led Control. LEDC.
La configuración de éste periferico se realiza en tres pasos:
* Configuración de un temporizador, especificando la frecuencia del PWM, y la resolución del ciclo de trabajo.
* Configuración del canal para asociarlo al temporizador y al puerto GPIO.
* Ajuste de la señal de PWM para modificar el ciclo de trabajo, ya sea por software o con funciones de atenuacción por hardware.

## LEDC TIMER CONFIG
El primer paso es utilizar la sigueinte función para configurar un temporizador, el cual contentra tres parametros: temporizador fuente, frecuencia, y resolución del ciclo de trabajo; la funcion recibe como único parametro la estrucutura `timer_conf`.
```C
esp_err_t ledc_timer_config ( const ledc_timer_config_t * timer_conf )
```
La estrucutura `timer_conf` contiene los siguientes parámetros:
```C
    ledc_timer_config_t timer_conf = {0}; // Estrutura de configuración
    timer_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
    timer_conf.duty_resolution = resolution;
    timer_conf.timer_num = LEDC_TIMER_0; // Aguas con la aplicación de timers, ya utiliza el timer 0 con esta aplicación.
    timer_conf.freq_hz = hz;             // 20kHz
```
