| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

# MPU6050 IMU de 6 Grados de libertad

Este proyeccto muestra un ejemplo de uso del sensor MPU6050 con ESP-IDF para la ESP32. EL proyecto incluye todos los componentes necesarios extríados de la biblioteca https://github.com/UncleRus/esp-idf-lib/tree/master, además incluye los archivos de configuración para el simulador Wokwi, en el cual puedes observar al sensor en funcionamiento en VScode.
La conexión de los pines es la misma que en el repositorio orginal:

| PIN Sensor  | GPIO ESP32 |
| ------------- |:-------------:|
| SCL      | GPIO 22     |
| SDA      | GPIO 21     |

A diferencia del repositorio orginal se coloco la dirección del sensor en 0x68 (revisar documentación de tu sensor puede cambiar enntre 0x68 y 0x69), y se cambio la referencia de los pines por su nombre. Es necesario saber que en el ejemplo la frecuencia de reloj para el I2C está configurada en 10p [kHz].

![ Ejemplo visual del funcionamiento del MPU6050. ](https://github.com/Alfredo-PS/EspressifESP32/blob/b8177fe6ae337a77b8a3b062bc62a4aeead2ec37/.addons/OTROS/Captura%20desde%202026-03-05%2016-44-00.png)

## Aspectos clave.
Para agregar la biblioteca de componentes se deben seguir los siguientes pasos:

1. **Descargar carpeta de componentes en la PC.**
Lo primero es abrir una consola o bash y colocarse en el espacio de trabajo en donde se quiera guardar todos los componentes, se recomienda crear una nueva carpeta reconocible. En el entorno de ESP-IDF en la barra inferior de herramientas encontrarás la consola bash ubicada en la carpeta del proyecto. Ve a la carpeta que acabas de crear y clona los componentes a tu computadora mediante el comando:
```
      git clone https://github.com/UncleRus/esp-idf-lib.git
```
El proceso de descarga comenzará, sólo hay que esperar.

2. **Declarar los compoenentes en CMakeList.**
En el archivo CMakeList.txt general (que está en la carpeta principal del proyecto, no la del main), se agregará una línea adicional en la que se le indica al compilador donde encontrará el codigo de las librerías y componentes empleados.
```C
cmake_minimum_required(VERSION 3.16)
set(EXTRA_COMPONENT_DIRS /home/alfredo/Documentos/librerias/esp-idf-lib/components)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(clase2)
```

Si el proceso fue llevado a cabo con éxito entonces el sistema reconocerá las bibliotecas, y se compilará sin ningun error. En caso de que el compilador marque un error con respecto a alguna parte del codigo propios de las bibliotecas del componente puedes probar comentarla y volver a compilar.
