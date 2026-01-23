#include "driver/mcpwm_prelude.h"
#include "esp_log.h"

#define PWM_FREQ_HZ 50 
#define PWM_PERIOD_TICKS 40000
static const char *TAG = "MCPWM_PHASE";

void app_main(void) {
    mcpwm_timer_handle_t timers[3];
    mcpwm_oper_handle_t operators[3];
    mcpwm_gen_handle_t generators[3];
    int gpios[3] = {17, 15, 19};
    
    // Cálculos de fase: 60° (1/6 de T) y 120° (2/6 de T)
    uint32_t phases[3] = {
        0, 
        (PWM_PERIOD_TICKS * 1)/ 3,       
        (PWM_PERIOD_TICKS * 2) / 3  
    };

    for (int i = 0; i < 3; i++) {
        mcpwm_timer_config_t timer_conf = {
            .group_id = 0,
            .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
            .resolution_hz = PWM_FREQ_HZ * PWM_PERIOD_TICKS,
            .period_ticks = PWM_PERIOD_TICKS,
            .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
        };
        ESP_ERROR_CHECK(mcpwm_new_timer(&timer_conf, &timers[i]));

        mcpwm_operator_config_t oper_conf = {.group_id = 0};
        ESP_ERROR_CHECK(mcpwm_new_operator(&oper_conf, &operators[i]));
        ESP_ERROR_CHECK(mcpwm_operator_connect_timer(operators[i], timers[i]));

        mcpwm_generator_config_t gen_conf = {.gen_gpio_num = gpios[i]};
        ESP_ERROR_CHECK(mcpwm_new_generator(operators[i], &gen_conf, &generators[i]));

        mcpwm_cmpr_handle_t comparator;
        mcpwm_comparator_config_t comp_conf = {.flags.update_cmp_on_tez = true};
        ESP_ERROR_CHECK(mcpwm_new_comparator(operators[i], &comp_conf, &comparator));
        mcpwm_comparator_set_compare_value(comparator, PWM_PERIOD_TICKS / 2);

        mcpwm_generator_set_action_on_timer_event(generators[i], 
            MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));
        mcpwm_generator_set_action_on_compare_event(generators[i], 
            MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW));
    }

    mcpwm_sync_handle_t timer0_sync_src = NULL;
    mcpwm_timer_sync_src_config_t sync_src_conf = {
        .timer_event = MCPWM_TIMER_EVENT_EMPTY, 
    };
    ESP_ERROR_CHECK(mcpwm_new_timer_sync_src(timers[0], &sync_src_conf, &timer0_sync_src));

    for (int i = 0; i < 3; i++) {
        mcpwm_timer_sync_phase_config_t phase_conf = {
            .sync_src = timer0_sync_src,   
            .count_value = phases[i],     
            .direction = MCPWM_TIMER_DIRECTION_UP,
        };
        ESP_ERROR_CHECK(mcpwm_timer_set_phase_on_sync(timers[i], &phase_conf));
    }


    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_timer_enable(timers[i]));
        ESP_ERROR_CHECK(mcpwm_timer_start_stop(timers[i], MCPWM_TIMER_START_NO_STOP));
    }

    ESP_LOGI(TAG, "PWM de 3 fases (0, 60, 120) iniciado correctamente.");
}
