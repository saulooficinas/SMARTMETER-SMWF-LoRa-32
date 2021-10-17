#ifndef _RTOS1_H_
    #define _RTOS1_H_

    /*Incluindo os outros HEADERS*/
    #include "saidasSMWF.h"
    #include "sensorSMWF.h"
    #include "displaySMWF.h"
    
    /*Incluindo biblitoeca do arduino*/
    #include <Arduino.h>
    #include "WiFi.h"

    /*Incluindo bibliotecas do FreeRTOS*/
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "freertos/queue.h"

    //Construindo struct para carregar dados para o display;
    struct sensorRef
    {
        //Endereço dos pinos
        int pino[2];

        //Valor medido no pino.
        float valor[2];
    };

    void init_freeRTOS();
    void init_WiFi();

#endif