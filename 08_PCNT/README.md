# Contador de pulsos PCNT.
El contador de pulsos PCNT está diseñado para contar el número de flancos ascendentes y descendentes de ñas señales de entrada, cada unidad del PCNT es un contador independiente con multiples canales, donde cada nal incrementa o decrementa un contador en un clanco ascendente o descendente, cada canal se configura por separado; ademas está equipado con un filtro para eliminar el reuido de la señal y es útil para decodificar señales de cuadratura en velocidad y dirección de los Encoders.

Como es habitual la configuración de los PCNT comienza con la asignación de recuros meidnate `pcnt_unit_handle_t`, posteriormente se completa la estructura de configuración para la unidad del PCNT con `pcnt_unit_config_t`, y finalmente se crea la nueva unidad con la función `pcnt_new_unit()`, los pasos anteriores se relizan como sigue:
```C
  pcnt_unit_handle_t pcnt_unit = NULL;

  pcnt_unit_config_t unit_config = {
      .high_limit = 100,
      .low_limit = -100,
  };

  ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));
```
Con la unidad lista se procede a crear un nuevo canal por medio de la estructura de configuración correspondiente `pcnt_chan_config_t`, en está estructura se define un GPIO de flanco y otro de nivel para cada canal deseado, en general si se utilizan A y B, entonces el canal A tendra el GPIO de A en flanco y el gpio de nivel en B, y lo contrario para el canal B, lo que permitira la lectura de la cuadratura; finalmente se inicia el nuevo canal configurado con pcnt_new_channel(), la configuración para un canal se muestra a continuación:
```C
    
    pcnt_channel_handle_t pcnt_chan_a = NULL;

    // Configuración del Canal A
    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num = ENCODER_GPIO_A,   // ENCODER_GPIO_A = 18
        .level_gpio_num = ENCODER_GPIO_B,  // ENCODER_GPIO_B = 19
    };
    
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_a_config, &pcnt_chan_a));
```
Se debe repetir el codigo anterior para cada canal deseado, normalmente los encoder son de dos canales A y B, por lo que se tendrá la configuración para A (mostrada arriba) y la configuración para B.

Con los canales listo se proocese a definir las acciones para cada canal por medio de pcnt_channel_set_edge_action() para el flanco de subida y pcnt_channel_set_level_action() para el nivel, ambas funciones reciben el handle de cada canal y las acciones que deben ejecutar, por ejemplo para un encoder de cuadratura con dos canales A y B: 
```C
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

```

Adiconalmente se puede configurar un filtro opcional que elimina el ruido, para ello nuevamente se crea la estructura de configuración con `pcnt_glitch_filter_config_t` en donde se coloca el valor en nano segundos, y se utiliza la función `pcnt_unit_set_glitch_filter()` para activarlo, la cual solo requiere el handle de la unidad del PCNT y un apuntador a la estructura de configuracción, la implementación de este filtro para el encoder de un motor a media velocidad sería:
```C
    // Filtro de Glitch (opcional, elimina ruido)
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 100, // bajo valor para que la lectura no pierda pasos
    };

    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

```

Para incial el periferico y que comience el conteo de los pulsos se utilizan `pcnt_unit_enable()` que habilita la unidad, y `pcnt_unit_start()` que comeinza a contar. ambas sólo reciben en handle de la unidad:
```C
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
```
Finalmente para conocer le numero de cuentas, se hace uso de la función `pcnt_unit_get_count()`, que recibe el handle del PCNT, y un apuntador a la variable entera donde se guardará el valor del conteo:

```C
          int dato;
          pcnt_unit_get_count(pcnt_unit, &dato);
```
Cuando se obtiene el numero de cuentas hasta el momento en realidad el contador sigue incrementando o decrementando segun las señales de entrada, y solo se reincia o vuelve a cero en caso de que llegue a cualquiera de los dos limites, superior o inferior, si sedesea eliminar el numero de cuentas en cualquier momento se puede emeplear la función `pcnt_unit_clear_count()`, que recibe como unico parametro en handle de la unidad:
```C
        ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit)); 
```
