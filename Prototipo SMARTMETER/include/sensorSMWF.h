#ifndef _SENSOR_H_
    #define _SENSOR_H_
    /*Biblioteca do Arduino*/
    #include <Arduino.h>

    //Definições importantes
    #define CAMPO_BOBINA 10 //Pode mudar (Teslas)
    #define DIAM_TUBE 0.06 // 60 mm
    #define MAT_PI 3.1415926535 //Valor de pi
    #define K_SENSOR 0.45 //Adimensional (Encontrar)

    //Valor de interações do filtro de média móvel
    #define max_int 100

    //Criando uma variável para ler o valor do sensor.
    float readSensor();
    float readPrototipo();
    float trafLitros(float data);
    float difValues(float a, float b);
#endif