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
