# Comunicación SPI.
# ESP32 SPI Slave Bidireccional

## SPI Slave
El presente código sirve para configurar la ESP32 como esclavo para la comunicación por SPI, permitiendo el envío y recepción de datos, programado por medio de ESP-IDF SDK Espressif.

## Conexiones.

| ESP32 Master  | ESP32 Slave   |
| ------------- |:-------------:|
| MOSI   23     | MOSI   23     |
| MISO   19     | MISO   19     |
| SCLK   18     | SCLK   18     |
| CS     5      | CS     5      | 
| GND           | GND           |

En caso de utilizar otro dispositivo distinto de la ESP32, mantener el orden y cablear con los correspondientes pines.

### Descripción del programa.
El programa de ejemplo envía y recibe 16 bytes en cada mensaje, para ello primero se configura el sistema SPI, definiendo los pines de entradas y salidas, el modo de trabajo, el máximo de bytes por transacción, y los buffers de envío y recepción, se inicializa la comunicación. Dentro del bucle principal que se repite 10 veces, se realiza una transacción, la cual comienza por limpiar los buffers del contenido basura, luego se prepara el mensaje que será enviado del Slave al Master, se configura la transacción, para que se manden y reciban los mensajes al mismo tiempo, finalmente, se realiza la verificación de errores, por ser un ejemplo se implementa un delay de 1 segundo.

### Consideraciones de uso.
Este manejo de comunicación SPI es adecuado bajo ciertas circunstancias, si se elimina el delay es funcional si el Master envía datos continuamente y no es relevante que el Slave alcance a recibir la totalidad de los mensajes. Si se requiere que el proceso de comunicación sea totalmente simultaneo y el Slave reciba todos los datos, entonces se debe implementar el encolamiento, y un pin adicional que sirva como Handshake que le indique al master cuando el Slave esté listo para realizar otra transacción, sin embargo, este manejo además de ser más complejo demanda muchos más recursos del microcontrolador. Si se utiliza la configuración mostrada se recomienda realizar un correcto manejo y comprobación de los datos, pues si la distancia es muy grande o existen muchas perturbaciones  la información se puede ver fácilmente comprometida o alterada.
