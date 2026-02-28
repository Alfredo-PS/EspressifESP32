// Slave as receiver for SPI communication
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/spi_slave.h"
#include "driver/gpio.h"
#include "esp_system.h"

// Pines usados - deben coincidir con el Master
#define GPIO_MOSI 23  // Master OUT -> Slave IN
#define GPIO_MISO 19  // Master IN  <- Slave OUT
#define GPIO_SCLK 18  // Clock
#define GPIO_CS   5   // Chip Select

// Buffer size (Recomendado, estandar)
#define RECEIVE_BUF_SIZE 128


void app_main(void)
{
    // Variable para registro de errores;
    esp_err_t ret;
    
    // Configuración del SPI, como SLAVE
    spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = RECEIVE_BUF_SIZE
    };

    // Configuración para el SPI, como SLAVE
    spi_slave_interface_config_t slvcfg = {
        .mode = 0,                 // Modo SPI 0 (debe coincidir con Master)
        .spics_io_num = GPIO_CS,   // Pin de Chip Select
        .queue_size = 3,           // Tamaño de cola de transacciones
        .flags = 0,                // Sin flags especiales
        .post_setup_cb = NULL,     // Callback después de setup (opcional)
        .post_trans_cb = NULL      // Callback después de transacción (opcional)
    };
    
    gpio_set_pull_mode(GPIO_MOSI, GPIO_PULLUP_ONLY); //Activar resistencia pull up para los pines de entrada
    gpio_set_pull_mode(GPIO_SCLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_CS, GPIO_PULLUP_ONLY);

    char recvbuf[RECEIVE_BUF_SIZE] = {0}; //recvbuf -> buffer en donde se guardará el mensaje
    char sendbuf[RECEIVE_BUF_SIZE] = {0};
    spi_slave_transaction_t t; // Vector que contendrá toda la información del mensaje

    memset(&t, 0, sizeof(t)); // Limpia el buffer

    //  ----- APERTURA DE COMUNICACIÓN SPI COMO SLAVE -----
    printf("Inicializando SPI Slave...\n");
    
    // Inicializa la comunicación como SPI Slave
    ret = spi_slave_initialize(HSPI_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        printf("Error inicializando SPI Slave: %d\n", ret);
        return; // En caso de error termina el programa
    }

    printf("SPI Slave inicializado correctamente\n");
    printf("Esperando datos del Master...\n");

    

    int numDatosRecibidos = 0;

    // Bucle de recepción de datos, ejemplo de aplicación

    while (numDatosRecibidos < 10) {  // Bucle para recibir 10 mensajes como ejemplo
        
        // Limpiar buffer antes de cada recepción
        memset(recvbuf, 0, sizeof(recvbuf));
        memset(sendbuf, 0, sizeof(sendbuf));

        
        snprintf(sendbuf, sizeof(sendbuf), "Recibido: %d de 9, %d", numDatosRecibidos, (numDatosRecibidos*10));

        // Configurar la transacción de recepción
        t.length = RECEIVE_BUF_SIZE * 8;   // Longitud en bits
        t.rx_buffer = recvbuf;          // Buffer de recepción
        t.tx_buffer = sendbuf;          // Buffer de transmisión
        t.trans_len = 0;                // 
        
        // ESPERA Y RECEPCIÓN DE DATOS
        printf("Esperando transmisión...\n");
        //  ----- RECEPCIÓN DE DATOS -----
        ret = spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY); // RECEPCIÓN DE DATOS, espera indefinidamente
        
        // En caso de error en la recepción salta a la siguiente interación
        if (ret != ESP_OK) {
            printf("Error en recepción: %d\n", ret);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        // Datos recibidos correctamente
        numDatosRecibidos++;
        
        
        // Procesar datos recibidos (ejemplo simple)
        if (strlen(recvbuf) > 0) {
            printf("Mensaje %d: %s (length: %d bytes)\n", 
               numDatosRecibidos, recvbuf, t.trans_len / 8);
        }
        
        vTaskDelay(1000 / portTICK_PERIOD_MS); 
    }

    // ----- CIERRE DE COMUNICACIÓN SPI SLAVE -----
    printf("Cerrando comunicación SPI Slave...\n");
    
    // Liberar el bus SPI slave
    ret = spi_slave_free(HSPI_HOST);
    if (ret != ESP_OK) {
        printf("Error liberando SPI Slave: %d\n", ret);
    } else {
        printf("SPI Slave liberado correctamente\n");
    }

    printf("Comunicación SPI Slave cerrada completamente\n");

    // Mantener la aplicación corriendo
    while (1) {
        printf("SPI Slave cerrado - Aplicación en espera\n");
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}
