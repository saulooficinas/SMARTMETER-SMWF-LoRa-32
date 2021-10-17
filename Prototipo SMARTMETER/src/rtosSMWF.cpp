#include "rtosSMWF.h"
#include "secret_info.h"


//Aqui irei gerenciar as tasks e etc. Abaixo temos as tasks e timers utilizados e descrição
/************************************************************************************************|         
|     NOME       |     CORE       |    PRIORIDADE     |               DESCRIÇÃO                  |
|----------------|----------------|-------------------|------------------------------------------|
|-vTaskSensor    |      00        |       01          |     Faz a leitura do sensor              |
|-vTaskMYSQL     |      01        |       02          |     Publica dados no banco de dados      |
|-vTaskPrint     |      00        |       01          |     Imprime valores no display           | 
|-vTaskBluetooth |      00        |       03          |     Publica/ Recebe dados com Bluetooth  |
|-vTaskMedMovel  |      01        |       02          |     Coleta dados do sensor para  filtro  |
*************************************************************************************************/
/*
    NÚCLEOS:
        => APP_CPU_NUM = 00
        => PRO_CPU_NUM = 01       
*/

//Handle da fila de comunicação entre o display e a task do sensor.
QueueHandle_t xFila; 
QueueHandle_t xFilaAcumula;
QueueHandle_t xFilaBancoDados;

//Gera handles das taks
TaskHandle_t vTaskSensor_Handle;
TaskHandle_t vTaskMYSQL_Handle;
TaskHandle_t vTaskPrint_Handle;
TaskHandle_t vTaskBluetooth_Handle;
TaskHandle_t vTaskMedMov_Handle;

/*Gerando o protótipo da task*/
void vTaskSensor(void* pvParamaters);
void vTaskMYSQL(void* pvParamaters);
void vTaskPrint(void* pvParamaters);
void vTaskBluetooth(void* pvParamaters);
void vTaskMedMov(void* pvParamaters);


//Função de inicialização das ferramentos do freeRTOS
void init_freeRTOS()
{
    Serial.println("init_freeRTOS: INICIANDO...")
    //Criando variáveis para gerenciar se as tasks foram criadas
    BaseType_t return1,return2,return3,return4;

    //Gerando fila de comunicação entre o display e o sensor
    xFila=xQueueCreate(
                        1 //Tamanho da fila
                        ,sizeof(float)); //Tamanho dos itens da fila

    //Fila para pegar dados para imprimir.
    xFilaAcumula=xQueueCreate(1,sizeof(float));

    if(xFila == NULL)
    {
        Serial.println("Não foi possível criar a fila");
        while(1);
    }

    xFilaBancoDados=xQueueCreate(1,sizeof(float));

    //Gerando tasks
    //Task do sensor
    if(xFila!=NULL && xFilaAcumula!=NULL &&xFilaBancoDados!=NULL)
    {
        //Somente irá gerenciar as tasks se a fila for criada. Ela é importante
        //Para cada task, temos um teste de paralisação, para ter crtz que foi criada
        return1=xTaskCreatePinnedToCore(
                                vTaskSensor //Nome da task
                                ,"TaskSENSOR" //Nome para debug
                                , configMINIMAL_STACK_SIZE+1024 // Tamanho de memória que irá utilizar
                                , NULL //Parâmetro que irá receber
                                , 1 //Prioridade da tarefa do sensor
                                , &vTaskSensor_Handle //Endereço objeto que será manipulado pela tarefa
                                , APP_CPU_NUM);//Utilizará o primeiro núcleo de processamento

        if(return1 != pdTRUE)
        {
            Serial.println("Não foi possível criar a task do sensor");
            while(1);
        }
        //Task da comunicação MYSQL
        return2=xTaskCreatePinnedToCore(    
                                vTaskMYSQL //Nome da task
                                ,"TaskMYSQL" //Nome para debug
                                , configMINIMAL_STACK_SIZE+1024 //Tamanho da memória que irá utilizar
                                , NULL //Parâmetro inicial que irá receber
                                , 2 //Prioridade da tarefa
                                , &vTaskMYSQL_Handle // Handle da task
                                , PRO_CPU_NUM);  // Irá utilizar o segundo núcleo de processamento

        if(return2!= pdTRUE)
        {
            Serial.println("Não foi possível criar a task de comunicação MYSQL   ");
            while(1);           
        }

        //Task de imprimir no Display
        return3=xTaskCreatePinnedToCore(
                                 vTaskPrint //Nome do procedimento que irá utilizar
                                , "TaskPrint" //Nome da task para debug
                                , configMINIMAL_STACK_SIZE+2048 //Tamanho da memória utilizada
                                , NULL//Parâmetro inicial que irá receber
                                , 1 //Prioridade da tarefa
                                , &vTaskPrint_Handle //Handle da task do display
                                , APP_CPU_NUM);//Irá utilizar o primeiro núcleo de processamento.
        if(return3!= pdTRUE)
        {
            Serial.println("Não foi possível criar a task do Display");
            while(1);
        }
        return4=xTaskCreatePinnedToCore(
                                        vTaskMedMov //Nome do procedimento
                                        ,"TaskMedMov"//Nome para debug
                                        ,configMINIMAL_STACK_SIZE+1024//Tamanho de memória utilizado
                                        ,NULL//Parâmetro inicial
                                        ,2 //prioridade
                                        ,&vTaskMedMov_Handle //Handle da task de média móvel
                                        ,PRO_CPU_NUM); //Qual CPU irá utilizar
        

        if(return4!=pdTRUE)
        {
            Serial.println("Não foi possível criar a task de Média móvel");
            while(1);
        }

        Serial.println("Função finalizada....");
    }
}


//Implementação das tasks

//Task do Sensor
void vTaskSensor(void* pvParamaters)
{
    (void) pvParamaters;

    while(1)
    {
        //Variáveis importantes.
        struct sensorRef sensorValue;
        float media;

        //Espera chegar dados da xFila acumula
        if(xQueueReceive(xFilaAcumula,&media,portMAX_DELAY)==pdTRUE)
        {
            //Enviado dados do sensor pino
            sensorValue.pino[0] = PIN_REF;
            sensorValue.valor[0] = 0;
            
            //Enviado dados do protótipo.
            sensorValue.pino[1] = PIN_PROTOTIPO;
            sensorValue.valor[1] = trafLitros(media);
            //Enviar valores para a fila que irá para a task de imprimir
            xQueueSend(xFila, &sensorValue, portMAX_DELAY);
            vTaskDelay(1);
        }
        //Delay para evitar erros.
 
    }
}

//Task da comunicação MYSQL
void vTaskMYSQL(void* pvParamaters)
{
    int a=50;
    while(1)
    {
        int id_sensor=1;
        a+=2;
        struct  sensorRef sensorValue;
        if(xQueueReceive(xFilaBancoDados,&sensorValue,portMAX_DELAY)==pdTRUE)
        {

            //Conectando ao webcliente.

            Serial.print("Conectando com: ");
            Serial.println(host);

            //Criando objeto WiFiCliente, para guardar dados.
            WiFiClient client;

            const int httpPort=80;

            if(!client.connect(host,httpPort))
            {
                Serial.println("Falha na conexão");
                return;
            }

            //Pedindo o url para salvar dados
            String url="http://localhost/nodemcu/salvar.php?";
                    url+="vazao=";
                    url+=a;
                    url+="&id_medidor="
                    url+=id_sensor;

            
            Serial.print("Requisitando URL:");
            Serial.println(url);

            //Faz a solicitação de enviar dados
            client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");

            //Verifica tempo de conexão
            unsigned long timeout = millis();
            while (client.available() == 0) {
                if (millis() - timeout > 5000) {
                Serial.println(">>> Client Timeout !");
                client.stop();
                return;
                }
            }

            while (client.available()) {
                String line = client.readStringUntil('\r');
                //Serial.print(line);

                if (line.indexOf("salvo_com_sucesso") != -1)
                {
                    Serial.println();
                    Serial.println("Arquivo foi salvo com sucesso.");
                }
                else if (line.indexOf("erro_ao_salvar") != -1)
                {
                    Serial.println();
                    Serial.println("Houve um erro.");
                    //Poderia ligar um led para me avisar que não está funcionando.
                }
            }

            Serial.println();
            Serial.println("Fechando conexão");
            vTaskDelay(pdMS_TO_TICKS(10000));//Espera 1 min até cadastrar um novo dado de vazão
        }    
    }
}

//Task do display
void vTaskPrint(void* pvParamaters)
{
    (void) pvParamaters;
    while(1)
    {
        struct  sensorRef sensorValue;

        if(xQueueReceive(xFila,&sensorValue,portMAX_DELAY)==pdTRUE)
        {
            //Envia dado para a Web
            xQueueSend(xFilaBancoDados,&sensorValue,portMAX_DELAY);

            //Imprime dados no display.
            imprimeDisplay(sensorValue.valor[0],sensorValue.valor[1]);
        }
        vTaskDelay(3);
    }

}

//Task do uso do bluetooth 
void vTaskBluetooth(void* pvParamaters)
{
    while(1)
    {
        vTaskDelete(NULL);
    }
}

void vTaskMedMov(void* pvParamaters)
{
    while(1)
    {
        //Array para captar os dados
        static float dados[max_int];

        //Variável para acumular os valores
        float acc=0,media;

        //Coleta os dados para a média móvel.
        for(int i=0; i<max_int;i++)
        {
            dados[i]=8.88;
            acc+=dados[i];
        }

        //Média dos valores
        media=acc/max_int;

        //Envia o valor para a xFilaAcumula
        xQueueSend(xFilaAcumula,&media,portMAX_DELAY);

        vTaskDelay(1);
    }
}

void init_WiFi()
{
    /*INICIALMENTE, FAZ A CONEXÃO COM A INTERNET*/
    //Irá enviar valores do sensor para o banco de dados
    Serial.println("Init_WiFi: INICIANDO.")
    Serial.print("Conectando a ");
    Serial.println(ssid);

    //Inicia o A comunicação com o WiFi, passando nome e senha da rede.
    WiFi.begin(ssid, password);

    //Testa se o Wi-fi foi conectado.
    while(WiFi.status()!= WL_CONNECTED)
    {   
        //Carregando
        Serial.print(".");
        delay(500);
    }

    //Wi-fi conectado, mostra que conectou.
    Serial.println("");
    Serial.println("WiFi conectada.");
    Serial.println("Endereço de IP: ");
    Serial.println(WiFi.localIP());
}