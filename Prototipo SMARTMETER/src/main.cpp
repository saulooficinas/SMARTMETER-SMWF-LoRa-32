/*****************************************************************************************
 * PROGRAMA PRINCIPAL DO SMWF ( Smart Meter Water Flux) - OFICINAS 4.0 - Versão 1.0 
 * Instituição: Instituto Federal de Alagoas - Campus Palmeira dos índios
 * 
 * Descrição: Ele irá fazer a leitura de eletrodos no fluído
 * e irá enviar a vazão para um servidor WEB. Além disso, também receberá dados
 * via Bluetooth e terá opções de auto-teste para avaliar seu funcionamento.
 * 
 * Ultima atualização: 17/10/2021 (SAULO)
 ******************************************************************************************/

//Inclui bibioteca principal "main.h"
#include "main.h"

void setup() {
  //Funções de iniciar dados
  Serial.begin(115200); //Iniciando porta Serial. 
  Wire.begin(4,15); //Iniciando a comunicação Wire.

  //Funções de inicialização
  init_WiFi(); //Conectando à internet.
  init_saidas();//Inicia pinos de saída e comunicação Serial
  inicializaDisplay();//Inicia display
  init_freeRTOS();//inicia tasks timers e etc do freeRTOS

}

void loop() {
  vTaskDelete(NULL); //Deletando tarefa de loop.
}