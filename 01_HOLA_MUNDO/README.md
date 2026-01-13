# Hola mundo en IDF
## Instalación y configuración.
El IDF de Espressif se puede descargar de manera independiente desde la página oficial, en tal caso se descargan las dependencia necesarias, además existe un entorno de desarrollo propio, sin embargo para este curso se prefiere el uso de Visual Studio Code como entorno de desarrollo, y el IDF se instalará cómo extensión dentro de VS.
Por lo tanto el primer paso es contar con Visual Studio descargado, esto se puede hacer totalmente gratis y de forma segura en: [ Visual Studio Code ](https://code.visualstudio.com/download) .

Una vez instalado se mostrará una interfaz como la siguiente, para descargar la extensión se da clic en el ícono de extensiones en la barra lateral izquierda, al hacerlo se abrirá el buscador de extensiones, en donde con sólo colocar *"idf"* se mostrará la extensión que necesitamos. La extensión se llama **ESP IDF**, por *Espressif Systems*, al darle clic a la extensión se desplegará una pestaña con la información de la extensión, basta con dar clic en instalar para que comience a descargar los archivos necesarios, este proceso toma tiempo, y se recomienda realizarlo antes de inciado el curso.

![ Interfaz de Visual Studio Code para descargar IDF. ](https://github.com/Alfredo-PS/EspressifESP32/blob/7202ef3a354451bd3a916934e2a386c68b894cac/.addons/img00/VS-02.png)

Una vez finalice el proceso de instalación hay que configurar la extesión, lo escencial es configurar como servidor a Github para el control de versiones, **la version de IDF preferente en este curso será la 5.2**, es importante descargar esta versión para que todos los códigos funcionen correctamente, entre versiones suelen cambiar librerias y drivers, por lo que puede que si no se descarga esta versión las plantillas y otros códigos puede que no funcionen correctamente; las carpetas en donde se guardaran las herramientas y codigos se pueden dejar por defecto, o selecionar según las necesidades.

![ Configuración de la versión de IDF. ](https://github.com/Alfredo-PS/EspressifESP32/blob/92984d6a61cb13c7d31a2c4791d50c9f1e0d6cdf/.addons/img00/VS-04.png)

Una vez hecho esto la extensión está lista para usarse, el IDF ya incluye algunos ejemplos, como primer paso abriremos un nuevo proyecto, al hacerlo en la barra superior del programa se muestra la cominfugración del framework a utilizar, por defeccto seleciona la version descargada de IDF, verificar que sea la **v5.2.5.**

![Menú principal de Espressif IDF extension. ](https://github.com/Alfredo-PS/EspressifESP32/blob/de539458b6c3f0c02fc0a38ddb0c7062d3a0de3e/.addons/img00/VS-05.png)

Una vez cargue el nuevo proyecto se habrirá la configuración de éste, en donde se puede colocar el nombre, el directorio donde se guardará, ademas se debe selecionar la tarjeta utilizada, en general selecionar **esp32**, a menos de que utilices un modelo especifico puedes seleccionarlo, en *Board* escoger el **KIT 3.3V**, en serial port se puede seleccionar o dejar que lo detecte, y finalmente hay un espacio para añadir compnentes al directorio, por ahora la configuración debe quedar algo parecido a la siguiente imagen, finalmente dar clic en *Choose template.*

![ Creación de un nuevo proyecto. ](https://github.com/Alfredo-PS/EspressifESP32/blob/de539458b6c3f0c02fc0a38ddb0c7062d3a0de3e/.addons/img00/VS-06.png)

En este menú se puede escoger entre una serie de ejemplos y plantillas, para probar las funciones de IDF seleccionar la plantilla de **hello_World**. Para crear nuevos proyectos puedes empezar desde la plantilla *sample_project*.
![ Plantilla Hola mundo. ](https://github.com/Alfredo-PS/EspressifESP32/blob/de539458b6c3f0c02fc0a38ddb0c7062d3a0de3e/.addons/img00/VS-07.png)

Finalmente, se abrirá una nueva pestaña con la vista del proyecto, en el explorador de archivos a la derecha, se observa el árbol de archivos con las diferentes secciones que integran al proyecto, en la sección main es donde se encontrarán los archivos a trabajar, tanto el punto C, como el CMakeLists. La estructura de los proyectos está basada en CMake que es un sistema que organiza la compilación de los archivos fuente para generar el ejecutable. Todo componente, como es el main debe tener un archivo CMakeLists.txt, en donde se declaran los archivos que se deben compilar y las carpetas que contienen los encabezados.

El main.c es el punto de entrada del programa, y se ejecuta siempre al inicio de la tarjeta como una tarea en el procesador. Los archivos *sdkconfig* contienen todoa la configuracipon de hardware y software del proyecto, no se debe editar a mano, se puede configurar por medio de la misma extensión en *menuconfig*
![Vista principal del archivo hello_world_main.c ](https://github.com/Alfredo-PS/EspressifESP32/blob/de539458b6c3f0c02fc0a38ddb0c7062d3a0de3e/.addons/img00/VS-08.png).

En la parte inferior de VScode se encuentra una barra con diferentes opciones, primero observarás la versión de IDF, despues en el simbolo de estrella es posible configurar el método de carga o Flaseo de la tarjeta, se recomienda selecionar siempre **UART**, despues encontrará el simbolo de un conector en donde hay que escoger el puerto USB en que está conectada la ESP32, seguidamente se encuentra la tarjeta con la que se está trabajando, por defecto se seleciona la **esp32**, al darle clic a esta opción de desplegara nuevamente el Kit en donde se selecciona el de 3.3V, en el símbolo de engranaje se despliega el menú de configuración del SDK, en donde se pueden cambiar las propiedades internas de la tarjeta como por ejemplo la frecuencia del reloj principal, configurar los protocolos de comunicación, étc.

![ Barra de herramientas inferior de IDF. ](https://github.com/Alfredo-PS/EspressifESP32/blob/b91711a1a0122722230877021f5a5ebc66dac3b9/.addons/img00/VS-12.png)

En el símbolo de basura se puede eliminar la configuración general, es recomendable no tocarlo, seguidamente el simbolo de llave permite compilar el proyecto, al presionarlo se crea el ejecutable que se cargará a la tarjeta, la primera vez tardará algo de tiempo, despues se realizará más rapidamente mientras no se cambie la configuración. Posteriormente, el símbolo de un rayo es donde se cargará el programa compilado a la tarjeta, el proceso que se debe seguir es primero compilar y verificar que no existen errores, y despues cargar el programa.

La extensión de IDF cuenta con un monitor serial que nos permite visualizar la ejecución del programa, está se puede abrir en el simbolo de monitor, si se vuelve a presionar el programa se reiniciará.

Con las herrameintas anteriormente mencionadas es posible compilar y cargar el programa a la ESP32, verificar que en la consola se tenga una salida parecida a la siguiente imagen. 
![ Salida en consola del programa de ejemplo Hola mundo. ](https://github.com/Alfredo-PS/EspressifESP32/blob/b91711a1a0122722230877021f5a5ebc66dac3b9/.addons/img00/VS-11.png)

## Herrameintas útiles para el curso.
IDF permite la programación tanto en C como en C++, a continuación se presentan algunas caracteristicas del lenguaje C, sólo para repasar conceptos básicos. 

### Tipos de datos básicos

| Tipo de dato  | Descripción |
| :-------------: |-------------|
| char      | 8 bits, almacena un sólo carácter     |
| int      | 32 bits, entero estándar     |
| float      | 32 bits, para decimales     |
| double    | 64 bits, decimales largos    |
| void       | Representa la ausencia de tipo |

### Tipos de tamaño fijo de stdint.h

| Tamaño  | Significado |  Rango  |
| :-------------: |:-------------:|:-------------:|
| uint8_t      | Entero sin signo de 8 bits     |0 a 255|
| int8_t      | Entero con signo de 8 bits     |-128 a 127  |
| uint16_t     | Entero sin signo de 16 bits     | 0 a 65,535|
| uint32_t    | Entero sin signo de 32 bits  | 0 a 4,294,967,295 |
| int32_t     | Entero con signo de 32 bits  | -2,147,483,648 a 2,147,483,647 |

### Calificadores de Variables

Encargados de modificar el comportamientto de las variables en memoria.

| Calificador  | Right columns |
| :-------------: |:-------------:|
| const      | Variable de sólo lectura, se guarda en la memoria Flash     |
| static      | Dentro de funciones mantiene su valor despues de la termine la función, y fuera de ellas hace que la variable solo sea visible dentro de ese archivo     |
| volatile      | variable pensada para cambiar en cualquier momento     |

### Estructuras
Permiten agrupar las variables de distintos tipos bajo un solo nombre, toda la configuración de perifericos y sistemas en IDF se realiza por medio de estructuras.

```C
struct SensorData {
    uint32_t id;
    float temperatura;
    bool activo;
}; // Esta estructura ocupa aprox 9-12 bytes dependiendo del alineamiento.
```
### Operadores.

| Operadores  | Simbolos |
| ------------- |:-------------:|
| Aritméticos      | +,-,*,/,%; pow(), sqr()     |
| Lógicos      | $$ (AND), ll (OR), ! (NOT)    |
| De comparación      | ==, !=, >, <, >=, <=     |
| Bit a bit | &, l, ^, <<, >> |
### Standard Input Output, stdio.h
Es una biblioteca de C que actúa como la interfaz entre el código y el mundo exterior, es decir entre la ESP32 y la consola de IDF en este caso. La primera función a revisar en `printf()`, la cual sirve para enviar texto y valores por consola, por ejemplo:
```C
#include <stdio.h>

void app_main(void) {
    printf("¡Hola mundo!");
    int lectura = 4095;
    // %d es un especificador de formato para enteros
    printf("La lectura del sensor es: %d\n", lectura);
}
```
Cuenta con varios especifiadores de formato que defines los datos que se mostraran en consola, los más comunes son:

| Especificador de formato  | Tipo de dato |
| :-------------: |:-------------:|
| %d      | int: entero con signo     |
| %u      | uint32_t: entero sin signo     |
| %f      | float: decimal simple     |
| %x| hexadecimal|
| %s | string: cadena de caracteres|
| %p | Dirección a un puntero|
Nota: En C un string se forma al crear un arreglo de caracteres, en donde por defento se reserva un espacio adicional para el carácter "\0" que indica la terminación de la cadena, por ejemplo: `char texto[] = "Hola mundo";`.

Para leer datos desde la consola se puede utilizar la función `scanf()`, con ciertas reservas, pues este método detiene la ejecución del programa y espera a que el usuario escriba algo en el monitor serial y presione enter, lo que detiene cualquier otro proceso dentro del main, más adelante se revisará un metodo no bloqueante.
```C

```


### Control de flujo.
Condicionales:
```C
if (voltaje > 3.3) {
    // Código si es verdadero
} else {
    // Código si es falso
}

switch (estado) {
    case 1: /* hacer algo */ break;
    default: /* opcional */ break;
}
```
De bucle:
```C
// For: ideal cuando sabes cuántas veces repetir
for (int i = 0; i < 10; i++) { ... }

// While: ideal para bucles infinitos en ESP32
while (true) {
    // Tu código principal aquí
}
```
### Punteros o apuntadores.
Los punteros son herramientas vitales en la programación en C, en IDF son ampliamente usados, tanto para configurar perifericos, como en ciertas funciones de librerias, y en general como un método eficiente de manejo de datos, a continuación se realiza un resumen simple de su forma de trabajo.
Lo primero que hay que recorda es que una variable es en realidad una dirección de memoria, en que se puede almacenar información. Los simbolos utilizados en el manejo de apuntadores son:


| Símbolo  | Significado |
| ------------- |:-------------:|
| &varible      | Dame la direción de *"variable"*     |
| *puntero      | ve a la dirección almacenada en *"puntero"* y dame el contenido     |

Ejemplo básico:
```C
#include <stdio.h>
void app_main(void)
{
    printf("Hello world!\n");

    int edad = 25;       // Una variable normal
    int *p_edad = &edad; // Un puntero que apunta a la dirección de 'edad'

    printf("Valor de edad: %d \n", edad);     // Imprime 25
    printf("Direccion de edad: %p \n", &edad); // Imprime algo como 0x3FFB...
    printf("Valor del puntero: %p \n", p_edad); // Imprime la misma dirección
    printf("Contenido apuntado: %d \n", *p_edad); // El asterisco entra a la dirección
}
```
![ Ejemplo del uso de punteros. ](https://github.com/Alfredo-PS/EspressifESP32/blob/a5a3b9ba2a9f69262d1bd0ecb22ca07615c2f507/.addons/img00/VS-13.png)



