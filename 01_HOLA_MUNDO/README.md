# Hola mundo en IDF
## Instalación y configuración.
El IDF de Espressif se puede descargar de manera independiente desde la página oficial, en tal caso se descargan las dependencia necesarias, además existe un entorno de desarrollo propio, sin embargo para este curso se prefiere el uso de Visual Studio Code como entorno de desarrollo, y el IDF se instalará cómo extensión dentro de VS.
Por lo tanto el primer paso es contar con Visual Studio descargado, esto se puede hacer totalmente gratis y de forma segura en: [ Visual Studio Code ](https://code.visualstudio.com/download) .

Una vez instalado se mostrará una interfaz como la siguiente, para descargar la extensión se da clic en el ícono de extensiones en la barra lateral izquierda, al hacerlo se abrirá el buscador de extensiones, en donde con sólo colocar *"idf"* se mostrará la extensión que necesitamos. La extensión se llama **ESP IDF**, por *Espressif Systems*, al darle clic a la extensión se desplegará una pestaña con la información de la extensión, basta con dar clic en instalar para que comience a descargar los archivos necesarios, este proceso toma tiempo, y se recomienda realizarlo antes de inciado el curso.

![ Interfaz de Visual Studio Code para descargar IDF. ](https://github.com/Alfredo-PS/EspressifESP32/blob/7202ef3a354451bd3a916934e2a386c68b894cac/.addons/img00/VS-02.png)

Una vez finalice el proceso de instalación hay que configurar la extesión, lo escencial es configurar como servidor a Github para el control de versiones, **la version de IDF preferente en este curso será la 5.2**, es importante descargar esta versión para que todos los códigos funcionen correctamente, entre versiones suelen cambiar librerias y drivers, por lo que puede que si no se descarga esta versión las plantillas y otros códigos puede que no funcionen correctamente; las carpetas en donde se guardaran las herramientas y codigos se pueden dejar por defecto, o selecionar según las necesidades.



