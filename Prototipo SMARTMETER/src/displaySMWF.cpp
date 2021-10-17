#include "displaySMWF.h"


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 16);

void inicializaDisplay()
{
    Serial.println("inicializaDisplay(): INICIADA...")
    if(!display.begin(SSD1306_SWITCHCAPVCC,OLED_ADDR))
    {
        Serial.println("Display OLED: falha ao inicializar");
    }
    else
    {
        Serial.println("Display OLED: inicialização ok");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
    }
    Serial.println("Finalizando inicializaDisplay()");

}

void imprimeDisplay(float data_sensor, float data_ref)
{

    display.clearDisplay(); //Limpa a tela do Oled
    
    //Configurando escrita.
    display.setCursor(0,OLED_LINE1);
    display.println("Teste do Protótipo");

    //Dados do sensor
    display.setCursor(0,OLED_LINE2);
    display.print("Sensor: ");
    display.print(data_sensor);
    display.println(" L");

    //Dados do sensor de referência
    display.setCursor(0,OLED_LINE3);
    display.print("Referência:");
    display.print(data_ref);
    display.println(" L");

}