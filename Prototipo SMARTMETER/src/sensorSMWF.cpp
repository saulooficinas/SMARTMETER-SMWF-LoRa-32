#include "sensorSMWF.h"
#include "saidasSMWF.h"

float readPrototipo()
{
    float value ;

    //Variavel recebe o valor do sensor
    value = analogRead(PIN_PROTOTIPO)*(25.0)/(1023.0); 
    
    return value ;
}

//Função para transformar os dados de tensão litros
float trafLitros(float data)
{
    return data*(PI*DIAM_TUBE)/(4*K_SENSOR*CAMPO_BOBINA);
}

//Coleta dados do sensor de referência.
float readSensor()
{
    float dado;

    dado=analogRead(PIN_REF);

    return dado;
}

//Dá a diferença percentual entre dois valores;
float difValues(float a, float b)
{
    return 100*(b-a)/b;
}


    /*Fórmula utilizada:
     V = U.(pi.D)/(4.k.B)

     V: Vazão dentro do tubo (m^3/s);
     U: Tensão nos eletrodos (V)
     pi: 3.1415926535...;
     D: Diâmetro do tubo; (m)
     k: Constante encontrada nos testes;
     B: Módulo do campo gerado pelas bobinas; (T)

     */