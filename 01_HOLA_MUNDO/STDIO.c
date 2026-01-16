#include <stdio.h>
#include "driver/uart.h"
#include "esp_vfs_dev.h"
 
void app_main(void)
{  
    // 1. Configuración básica de UART
    ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0));
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    char nombre[50]; 

    printf("Escribe tu nombre: \n");
    scanf(" %[^\n]", nombre);   
    printf("\nHola %s, bienvenido a IDF de Espressif\n\n", nombre);
    
}
