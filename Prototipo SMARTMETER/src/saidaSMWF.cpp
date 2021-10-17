#include "saidasSMWF.h"

void init_saidas()
{
    Serial.println("Init_saidas(): INICIANDO ");

    //Configurando os pinos
    pinMode(PIN_PROTOTIPO, INPUT);
    pinMode(PIN_REF, INPUT);

    Serial.println("Init_saidas(): INICIANDO ");
    Serial.print("PIN_PROTOTIPO:");
    Serial.println(PIN_PROTOTIPO);
    Serial.println("PIN_REF:");
    Serial.print(PIN_REF);
    Serial.println(" Função finalizada.");

    //OBS: Os Serial.print são apenas para DEBUGG.
}
