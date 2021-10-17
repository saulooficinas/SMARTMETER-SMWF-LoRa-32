#ifndef _DISPLAY_H_
    #define _DISPLAY_H_

    /*Incluir as bibliotecas necessárias*/
    #include <Arduino.h>
    #include "Adafruit_GFX.h"
    #include "Adafruit_SSD1306.h"
    #include "Wire.h"

    //Usar display O-LED da heltec
    #define OLED_ADDR 0x3c

    //Define das linhas
    #define OLED_LINE1 0
    #define OLED_LINE2 10
    #define OLED_LINE3 208
    #define OLED_LINE4 30
    #define OLED_LINE5 40

    //define tamanhos
    #define SCREEN_WIDTH 128
    #define SCREEN_HEIGHT 64    

    /*Gerando função para iniciar display*/
    void inicializaDisplay();
    void imprimeDisplay(float data_sensor, float data_ref);

#endif