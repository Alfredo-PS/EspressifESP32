#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"
#include "driver/pulse_cnt.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

int cuentas;
#define ENCODER_GPIO_A 18
#define ENCODER_GPIO_B 19
#define PWM1 16
#define PWM2 17
volatile int SET_POINT = 1024;
pcnt_unit_handle_t pcnt_unit = NULL;
volatile int bandera = 0; 

ledc_channel_config_t canalPWM_1 = {0};
ledc_channel_config_t canalPWM_2 = {0};

// ------------------- PID ESTRUCTURA ----------------------------
typedef struct
{
    float kp;
    float ki;
    float kd;

    float limMin;
    float limMax;

    float integral;
    float errPrev;
    float T;

    float salida;
} PIDcontrol;

void PID_Config(PIDcontrol *pid, float p, float i, float d, float t, float min, float max);
float PID_Calculo(PIDcontrol *pid, float referencia, float medicion);
PIDcontrol motorPid;

// ---------------- FUNCION TIMER ---------------------------
// Función de llamada
static bool IRAM_ATTR funcion_ejemplo_alarma_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx){  
    bandera = 1;
    return pdTRUE; 
}
// ----------------- FUNCION TAREA -------------------------
// Función tarea de control
void vTareaControl(void *pvParameters) {
    int valorControl = 0;
    cuentas = 0;

    while(1) {
        // Lógica principal
        if (bandera == 1){       
            pcnt_unit_get_count(pcnt_unit, &cuentas); 
            
            valorControl = PID_Calculo(&motorPid, SET_POINT, cuentas);

            if(valorControl > 0){
                ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0, 0);
                ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, valorControl, 0);
            }else if(valorControl < 0){
                ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0, 0);
                ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, (-1 * valorControl), 0);
            }else{
                ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, 0, 0);
                ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0, 0);
            }
            bandera = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main(void)
{
// ------------------------------ PID -----------------------------------------

    PID_Config(&motorPid, 5.0f, 1.0f, 0.05f, 750, -950, 950);

//------------------- PNCT ---------------------------------------------------------------    
    // Se habilitan resistencias para asegurar el correcto funcionamiento
    gpio_set_pull_mode(18, GPIO_PULLDOWN_ONLY); // Canal A
    gpio_set_pull_mode(19, GPIO_PULLDOWN_ONLY); // Canal B
    // Configuración de la Unidad PCNT
    pcnt_unit_config_t unit_config = {
        .high_limit = 20480,   // Límite máximo
        .low_limit = -20480,  // Límite mínimo
    };
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    // Configuración del Canal A
    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num = ENCODER_GPIO_A,
        .level_gpio_num = ENCODER_GPIO_B,
    };
    pcnt_channel_handle_t pcnt_chan_a = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_a_config, &pcnt_chan_a));

    // Configuración del Canal B
    pcnt_chan_config_t chan_b_config = {
        .edge_gpio_num = ENCODER_GPIO_B,
        .level_gpio_num = ENCODER_GPIO_A,
    };
    pcnt_channel_handle_t pcnt_chan_b = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_b_config, &pcnt_chan_b));

    // Definir acciones para cuadratura
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    // Filtro de Glitch (opcional, elimina ruido)
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 100, // bajo valor para que la lectura no pierda pasos
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));
    
    // Habilitar e Iniciar la unidad
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
//------------------- TASK --------------------------------------------------------------- 
TaskHandle_t tarea_handle = NULL;
    xTaskCreate(
            vTareaControl,
            "Tarea Control",
            2048,
            NULL,
            1,
            &tarea_handle
    ); 
bandera = 0;   

//------------------- TIMER ---------------------------------------------------------------
    gptimer_handle_t gptimer = NULL;
    // Configuración del timer
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1MHz = 1us por tick
    };

    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    // Configuración de la alarma
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = 750, // 750 us
        .flags.auto_reload_on_alarm = true,
    };

    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

    // Llamada de la función del timer
    gptimer_event_callbacks_t cbs = {
        .on_alarm = funcion_ejemplo_alarma_cb,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
    
    // Activación del timer
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
//------------ PWM configuración --------------------------------------
// Configuración PWM
    // Se declara la estrutura de configuración
    ledc_timer_config_t timer_conf_1 = {0}; 

    // Llenado de parámetros
    timer_conf_1.speed_mode = LEDC_HIGH_SPEED_MODE;
    timer_conf_1.duty_resolution = LEDC_TIMER_12_BIT;
    timer_conf_1.timer_num = LEDC_TIMER_0; 
    timer_conf_1.freq_hz = 10000;             // 10kHz

    // Configuración del timer con las caracteristicas deseadas
    ledc_timer_config(&timer_conf_1); // La estrucutura se envía como puntero (dirección)

    // canal 1
    // Se rellena la estructura
    canalPWM_1.gpio_num = PWM1;
    canalPWM_1.speed_mode = LEDC_HIGH_SPEED_MODE;
    canalPWM_1.channel = LEDC_CHANNEL_0;
    canalPWM_1.intr_type = LEDC_INTR_DISABLE;
    canalPWM_1.timer_sel = LEDC_TIMER_0;
    canalPWM_1.duty = 0;  // Ciclo de trabajo en cero

    // Se configura el canal, y el PWM comienza, la señal se envia al GPIO 16

    ledc_channel_config(&canalPWM_1);

    // canal 1


    // Se rellena la estructura
    canalPWM_2.gpio_num = PWM2;
    canalPWM_2.speed_mode = LEDC_HIGH_SPEED_MODE;
    canalPWM_2.channel = LEDC_CHANNEL_1;
    canalPWM_2.intr_type = LEDC_INTR_DISABLE;
    canalPWM_2.timer_sel = LEDC_TIMER_0;
    canalPWM_2.duty = 0;  // Ciclo de trabajo en cero

    // Se configura el canal, y el PWM comienza, la señal se envia al GPIO 17
    ledc_channel_config(&canalPWM_2);

    ledc_fade_func_install(0);

    while(1) {
        // La tarea se queda "dormida" sin gastar CPU hasta que el timer envía algo
        vTaskDelay(pdTICKS_TO_MS(50));
        SET_POINT = 2048;
        vTaskDelay(pdTICKS_TO_MS(50));
        SET_POINT = 3072;
        vTaskDelay(pdTICKS_TO_MS(50));
        SET_POINT = 4095;
        vTaskDelay(pdTICKS_TO_MS(50));
        SET_POINT = 3072;
        vTaskDelay(pdTICKS_TO_MS(50));
        SET_POINT = 2048;
        vTaskDelay(pdTICKS_TO_MS(50));
        SET_POINT = 1024;
        vTaskDelay(pdTICKS_TO_MS(50));
        SET_POINT = 0;
        vTaskDelay(pdTICKS_TO_MS(50));
        SET_POINT = 1024;
    }
}
// ------------------- PID FUNCIONES ------------------------------------------------------------
void PID_Config(PIDcontrol *pid, float p, float i, float d, float t, float min, float max){
    pid->kp = p;
    pid->ki = i;
    pid->kd = d;
    pid->T  = t;
    pid->limMin = min;
    pid->limMax = max;
    
    pid->integral = 0.0f;
    pid->errPrev = 0.0f;
}

float PID_Calculo(PIDcontrol *pid, float referencia, float medida){
    float error = referencia - medida;

    if(error<10 && error>-10){
        error = 0;
        pid->errPrev = error;
        pid->salida = 0;

        return pid->salida;
    }

    float proporcional = pid->kp * error;  // proporcional
    pid->integral += pid->ki * error * pid->T; // integral = integralAnterior * error * Tiempo
    float derivativo = pid->kd * (error - pid->errPrev)/ pid->T; // derivativo = (kd*[error-errorAnterior])/Tiempo
    
    pid->salida = proporcional + pid->integral + derivativo; // Salida de control del PID

    // Se limita la salida a un rango Max - minimo, y se actualiza el termino integral para el correcto calculo
    if(pid->salida > pid->limMax){
        pid->salida = pid->limMax;
    } else if( pid->salida < pid->limMin){
        pid->salida = pid->limMin;
    }

    if(pid->integral > pid->limMax){
        pid->integral = pid->limMax;
    } else if( pid->integral < pid->limMin){
        pid->integral = pid->limMin;
    }

    pid->errPrev = error;

    return pid->salida;
    
}
