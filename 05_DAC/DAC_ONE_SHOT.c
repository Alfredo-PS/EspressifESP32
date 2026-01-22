#include "freertos/FreeRTOS.h"
#include "driver/dac_oneshot.h"  // Header del DAC One Shot
#include "esp_log.h"

static const char *TAG = "EJEMPLO";

void app_main(void)
{
    // Se crea el handle del canal
    dac_oneshot_handle_t dac_handle;

    // Configuración del canal (Canal 0 = GPIO 25)
    dac_oneshot_config_t dac_cfg = {
        .chan_id = DAC_CHAN_0, 
    };

    // Inicializar el canal
    ESP_ERROR_CHECK(dac_oneshot_new_channel(&dac_cfg, &dac_handle));

    ESP_LOGI(TAG, "DAC inicializado en GPIO 25 (Canal 0)");

    int valorSalida = 155;

    while (1) {
        // Escribir el valor en el DAC
        ESP_ERROR_CHECK(dac_oneshot_output_voltage(dac_handle, valorSalida));

        ESP_LOGI(TAG, "Salida configurada a: %d (~2.0V)", valorSalida);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
