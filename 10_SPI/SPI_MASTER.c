// Master as transmitter for SPI communication
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_system.h"

// Pines a usar
#define GPIO_MOSI 23
#define GPIO_MISO 19
#define GPIO_SCLK 18
#define GPIO_CS 5


void app_main(void)
{
    spi_device_handle_t handle;
    esp_err_t ret;

    // Configuración del SPI
    spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1};

    // Configuration for the SPI device on the other side of the bus
    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .clock_speed_hz = 5000000,
        .duty_cycle_pos = 128, // 50% duty cycle
        .mode = 0,
        .spics_io_num = GPIO_CS,
        .cs_ena_posttrans = 3, // Keep the CS low 3 cycles after transaction
        .queue_size = 3};

    char sendbuf[128] = {0}; // Buffer de envio
    char recvbuf[128] = {0}; // Buffer de recepcion

    spi_transaction_t t; //Estrcutura que guarda toda la información de la comunicación

    memset(&t, 0, sizeof(t));

    // ----- APERTURA DE COMUNICACIÓN SPI -----
    printf("Inicializando bus SPI...\n");
    ret = spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO); // Se incia la comunicación
    if (ret != ESP_OK) {
        printf("Error inicializando bus SPI: %d\n", ret);
        return;
    }

    printf("Añadiendo dispositivo SPI...\n");
    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &handle); // Se añade el dispositivo (HSPI_HOST)
    if (ret != ESP_OK) {
        printf("Error añadiendo dispositivo SPI: %d\n", ret);
        spi_bus_free(HSPI_HOST); // Liberar bus si falla
        return;
    }

    printf("Comunicación SPI abierta correctamente\n");
    printf("Master output:\n");

    // Ciclo para comunicarse por un tiempo limitado, ejemplo de 10 transmisiones
    for (int i = 0; i < 10; i++) {

        snprintf(sendbuf, sizeof(sendbuf), "Iteración en curso: %d de 9, %d", i, (i*10)); // Se construye el mensaje a enviar
        
        // Se llena la estructura "t" con la información
        t.length = (strlen(sendbuf) * 8) + 16; // Longitud en bits solo del mensaje real, se agregan 2 bytes
        t.tx_buffer = sendbuf; // Se copia el mesaje en el buffer de envio
        t.rx_buffer = recvbuf; // Buffer de recepción
        t.rxlength = t.length; // Para evitar errores debe ser igual a t.length
        
        // ESCRITURA/TRANSMISIÓN
        ret = spi_device_transmit(handle, &t); // Función que establece comunicación (envío y recepción)

        if (ret != ESP_OK) {
            printf("Error en transmisión: %d\n", ret);
            break;
        }
        
        int bytes_received = t.rxlength / 8; 

        if (bytes_received > 0) {
            recvbuf[bytes_received] = '\0';  // Terminar string
            printf("Master envió: '%s'\n", sendbuf); 
            printf("Master recibió: '%s' (%d bytes)\n", recvbuf, bytes_received);
        } else {
            printf("Master envió: '%s' (sin respuesta)\n", sendbuf);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // ----- CIERRE DE COMUNICACIÓN SPI -----
    printf("Cerrando comunicación SPI...\n");
    
    // 1. Remover dispositivo del bus
    ret = spi_bus_remove_device(handle);
    if (ret != ESP_OK) {
        printf("Error removiendo dispositivo: %d\n", ret);
    } else {
        printf("Dispositivo removido correctamente\n");
    }

    // 2. Liberar el bus SPI
    ret = spi_bus_free(HSPI_HOST);
    if (ret != ESP_OK) {
        printf("Error liberando bus SPI: %d\n", ret);
    } else {
        printf("Bus SPI liberado correctamente\n");
    }

    printf("Comunicación SPI cerrada completamente\n");

    // Mantiene la aplicación corriendo 
    while (1) {
        printf("SPI cerrado - Aplicación en espera\n");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
