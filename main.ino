#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define BTN_HIGH 20
#define BTN_1 0
#define BTN_2 1
#define BTN_3 2
#define BTN_4 3
#define N_LEDS 30
#define ID_MQTT "mqtt_luminaria_willi"
int LED_BUILTIN = 2;

enum ESTADO
{
  AGUARDANDO,
  BRANCO_BAIXO,
  BRANCO_MEDIO,
  BRANCO_ALTO,
  VERMELHO_BAIXO,
  VERMELHO_MEDIO,
  VERMELHO_ALTO,
  VERDE_BAIXO,
  VERDE_MEDIO,
  VERDE_ALTO,
  AZUL_BAIXO,
  AZUL_MEDIO,
  AZUL_ALTO,
  AMARELO_BAIXO,
  AMARELO_MEDIO,
  AMARELO_ALTO,
  DORMINDO,
  ACORDADO,
};

// Pino de envio comum
const byte sendPin = 12;

// Pinos de recepção para os 4 botões
const byte receivePins[4] = {14, 23, 22, 21};

const byte sensorPin = 2;

const byte stripPin = 13;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, stripPin, NEO_GRB + NEO_KHZ800);

struct RGB
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

static const RGB corPorEstado[] = {
    {0, 0, 0},       // AGUARDANDO
    {32, 32, 32},    // BRANCO_BAIXO
    {128, 128, 128}, // BRANCO_MEDIO
    {255, 255, 255}, // BRANCO_ALTO
    {32, 0, 0},      // VERMELHO_BAIXO
    {128, 0, 0},     // VERMELHO_MEDIO
    {255, 0, 0},     // VERMELHO_ALTO
    {0, 32, 0},      // VERDE_BAIXO
    {0, 128, 0},     // VERDE_MEDIO
    {0, 255, 0},     // VERDE_ALTO
    {0, 0, 32},      // AZUL_BAIXO
    {0, 0, 128},     // AZUL_MEDIO
    {0, 0, 255},     // AZUL_ALTO
    {32, 32, 0},     // AMARELO_BAIXO
    {128, 128, 0},   // AMARELO_MEDIO
    {255, 255, 0},   // AMARELO_ALTO
    {0, 0, 0},       // DORMINDO
    {255, 255, 255}, // ACORDADO
};

// Armazena os valores lidos de cada botão
unsigned int valores[4];

// Controle do serial monitor
unsigned long anterior = 0;

// Variável que armazena o valor lido do sensor de presença
int sp = 0;

// Variável que armazena o valor do estado atual da luminária
ESTADO estado_atual = AGUARDANDO;

const char *SSID = "";                          // Nome do wifi
const char *PASSWORD = "";                      // Senha do wifi
const char *BROKER_MQTT = "mqtt-dashboard.com"; // URL do broker
int BROKER_PORT = 1883;                         // Porta do broker

WiFiClient espClient;         // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient

void initWiFi(void)
{
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");
  reconnectWiFi();
}

void initMQTT(void)
{
  MQTT.setServer(BROKER_MQTT, BROKER_PORT); // informa qual broker e porta deve ser conectado
  MQTT.setCallback(mqtt_callback);          // atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
  String msg;
  String topico;
  /* obtem a string do payload recebido */
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    msg += c;
  }
  topico = topic;
  Serial.print("Chegou a seguinte string via MQTT: ");
  Serial.println(msg);
  Serial.print("Do topico: ");
  Serial.println(topico);
  if (topico.equals("luminaria/cor"))
  {
    if (msg.equals("branco"))
    {
      estado_atual = BRANCO_BAIXO;
    }
    else if (msg.equals("vermelho"))
    {
      estado_atual = VERMELHO_BAIXO;
    }
    else if (msg.equals("verde"))
    {
      estado_atual = VERDE_BAIXO;
    }
    else if (msg.equals("azul"))
    {
      estado_atual = AZUL_BAIXO;
    }
    else if (msg.equals("amarelo"))
    {
      estado_atual = AMARELO_BAIXO;
    }
    else
    {
      if (estado_atual != AGUARDANDO || estado_atual != ACORDADO || estado_atual != DORMINDO)
      {
        if (msg.equals("baixo"))
        {
          if (estado_atual == BRANCO_MEDIO || estado_atual == BRANCO_ALTO)
          {
            estado_atual = BRANCO_BAIXO;
          }
          else if (estado_atual == VERMELHO_MEDIO || estado_atual == VERMELHO_ALTO)
          {
            estado_atual = VERMELHO_BAIXO;
          }
          else if (estado_atual == VERDE_MEDIO || estado_atual == VERDE_ALTO)
          {
            estado_atual = VERDE_BAIXO;
          }
          else if (estado_atual == AZUL_MEDIO || estado_atual == AZUL_ALTO)
          {
            estado_atual = AZUL_BAIXO;
          }
          else if (estado_atual == AMARELO_MEDIO || estado_atual == AMARELO_ALTO)
          {
            estado_atual = AMARELO_BAIXO;
          }
          else
          {
            MQTT.publish("luminaria/cor", "Escolha uma cor para escolher sua intensidade");
          }
        }
        else if (msg.equals("medio"))
        {
          if (estado_atual == BRANCO_BAIXO || estado_atual == BRANCO_ALTO)
          {
            estado_atual = BRANCO_MEDIO;
          }
          else if (estado_atual == VERMELHO_BAIXO || estado_atual == VERMELHO_ALTO)
          {
            estado_atual = VERMELHO_MEDIO;
          }
          else if (estado_atual == VERDE_BAIXO || estado_atual == VERDE_ALTO)
          {
            estado_atual = VERDE_MEDIO;
          }
          else if (estado_atual == AZUL_BAIXO || estado_atual == AZUL_ALTO)
          {
            estado_atual = AZUL_MEDIO;
          }
          else if (estado_atual == AMARELO_BAIXO || estado_atual == AMARELO_ALTO)
          {
            estado_atual = AMARELO_MEDIO;
          }
          else
          {
            MQTT.publish("luminaria/cor", "Escolha uma cor para escolher sua intensidade");
          }
        }
        else if (msg.equals("alto"))
        {
          if (estado_atual == BRANCO_BAIXO || estado_atual == BRANCO_MEDIO)
          {
            estado_atual = BRANCO_ALTO;
          }
          else if (estado_atual == VERMELHO_BAIXO || estado_atual == VERMELHO_MEDIO)
          {
            estado_atual = VERMELHO_ALTO;
          }
          else if (estado_atual == VERDE_BAIXO || estado_atual == VERDE_MEDIO)
          {
            estado_atual = VERDE_ALTO;
          }
          else if (estado_atual == AZUL_BAIXO || estado_atual == AZUL_MEDIO)
          {
            estado_atual = AZUL_ALTO;
          }
          else if (estado_atual == AMARELO_BAIXO || estado_atual == AMARELO_MEDIO)
          {
            estado_atual = AMARELO_ALTO;
          }
          else
          {
            MQTT.publish("luminaria/cor", "Escolha uma cor para escolher sua intensidade");
          }
        }
      }
    }
  }
}

void reconnectMQTT(void)
{
  while (!MQTT.connected())
  {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT))
    {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      //************************* inscrever-se nos tópicos ***************************
      MQTT.subscribe("luminaria/#");
      MQTT.subscribe("luminaria/cor");
    }
    else
    {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Havera nova tentatica de conexao em 2s");
      delay(2000);
    }
  }
}

void VerificaConexoesWiFIEMQTT(void)
{
  if (!MQTT.connected())
  {
    reconnectMQTT(); // se não há conexão com o Broker, a conexão é refeita
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    reconnectWiFi(); // se não há conexão com o WiFI, a conexão é refeita
  }
}

void reconnectWiFi(void)
{
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);

  // se já está conectado a rede WI-FI, nada é feito.
  // Caso contrário, são efetuadas tentativas de conexão
  if (WiFi.status() == WL_CONNECTED)
    return;
  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 20000)
  {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("\n❌ Falha ao conectar ao Wi-Fi. Verifique SSID e senha.");
    return;
  }
  Serial.println();
  Serial.print("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);
  pinMode(sendPin, OUTPUT);
  strip.begin();
  initWiFi();
  initMQTT();
  // Configura todos os receivePins como entrada inicialmente
  for (byte i = 0; i < 4; i++)
  {
    pinMode(receivePins[i], INPUT);
  }
  Serial.println("Iniciando a luminária...");
}

void loop()
{
  VerificaConexoesWiFIEMQTT();
  // Lê os 4 sensores
  for (byte i = 0; i < 4; i++)
  {
    valores[i] = lerSensorCapacitivo(receivePins[i], 5);
  }
  Serial.print("Estado atual: E");
  Serial.println(estado_atual);
  mudaCorDoLed(estado_atual);

  switch (estado_atual)
  {
  case AGUARDANDO:
    MQTT.publish("luminaria/estado", "AGUARDANDO");
    if (valores[BTN_1] >= BTN_HIGH && valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(DORMINDO);
    }
    else if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(BRANCO_BAIXO);
    }
    break;
  case BRANCO_BAIXO:
    MQTT.publish("luminaria/estado", "BRANCO_BAIXO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(VERMELHO_BAIXO);
    }

    if (valores[BTN_4] >= BTN_HIGH)
    {
      mudaEstado(BRANCO_MEDIO);
    }

    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(AGUARDANDO);
    }
    break;
  case BRANCO_MEDIO:
    MQTT.publish("Luminaria/estado", "BRANCO_MEDIO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(VERMELHO_MEDIO);
    }

    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(BRANCO_BAIXO);
    }

    if (valores[BTN_4] >= BTN_HIGH)
    {
      mudaEstado(BRANCO_ALTO);
    }
    break;
  case BRANCO_ALTO:
    MQTT.publish("luminaria/estado", "BRANCO_ALTO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(VERMELHO_ALTO);
    }

    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(BRANCO_MEDIO);
    }
    break;
  case VERMELHO_BAIXO:
    MQTT.publish("luminaria/estado", "VERMELHO_BAIXO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(VERDE_BAIXO);
    }
    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(AGUARDANDO);
    }
    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(BRANCO_BAIXO);
    }
    if (valores[BTN_4] >= BTN_HIGH)
    {
      mudaEstado(VERMELHO_MEDIO);
    }
    break;
  case VERMELHO_MEDIO:
    MQTT.publish("luminaria/estado", "VERMELHO_MEDIO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(VERDE_MEDIO);
    }
    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(VERMELHO_BAIXO);
    }
    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(BRANCO_MEDIO);
    }
    if (valores[BTN_4] >= BTN_HIGH)
    {
      mudaEstado(VERMELHO_ALTO);
    }
    break;
  case VERMELHO_ALTO:
    MQTT.publish("luminaria/estado", "VERMELHO_ALTO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(VERDE_ALTO);
    }
    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(VERMELHO_MEDIO);
    }
    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(BRANCO_ALTO);
    }
    break;
  case VERDE_BAIXO:
    MQTT.publish("luminaria/estado", "VERDE_BAIXO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(AZUL_BAIXO);
    }
    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(AGUARDANDO);
    }
    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(VERMELHO_BAIXO);
    }
    if (valores[BTN_4] >= BTN_HIGH)
    {
      mudaEstado(VERDE_MEDIO);
    }
    break;
  case VERDE_MEDIO:
    MQTT.publish("luminaria/estado", "VERDE_MEDIO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(AZUL_MEDIO);
    }
    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(VERDE_BAIXO);
    }
    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(VERMELHO_MEDIO);
    }
    if (valores[BTN_4] >= BTN_HIGH)
    {
      mudaEstado(VERDE_ALTO);
    }
    break;
  case VERDE_ALTO:
    MQTT.publish("luminaria/estado", "VERDE_ALTO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(AZUL_ALTO);
    }
    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(VERDE_MEDIO);
    }
    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(VERMELHO_ALTO);
    }
    break;
  case AZUL_BAIXO:
    MQTT.publish("luminaria/estado", "AZUL_BAIXO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(AMARELO_BAIXO);
    }
    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(AGUARDANDO);
    }
    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(VERDE_BAIXO);
    }
    if (valores[BTN_4] >= BTN_HIGH)
    {
      mudaEstado(AZUL_MEDIO);
    }
    break;
  case AZUL_MEDIO:
    MQTT.publish("luminaria/estado", "AZUL_MEDIO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(AMARELO_MEDIO);
    }
    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(AZUL_BAIXO);
    }
    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(VERDE_MEDIO);
    }
    if (valores[BTN_4] >= BTN_HIGH)
    {
      mudaEstado(AZUL_ALTO);
    }
    break;
  case AZUL_ALTO:
    MQTT.publish("luminaria/estado", "AZUL_ALTO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(AMARELO_ALTO);
    }
    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(AMARELO_MEDIO);
    }
    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(VERDE_ALTO);
    }
    break;
  case AMARELO_BAIXO:
    MQTT.publish("luminaria/estado", "AMARELO_BAIXO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(AGUARDANDO);
    }
    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(AGUARDANDO);
    }
    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(AZUL_BAIXO);
    }
    if (valores[BTN_4] >= BTN_HIGH)
    {
      mudaEstado(AMARELO_MEDIO);
    }
    break;
  case AMARELO_MEDIO:
    MQTT.publish("luminaria/estado", "AMARELO_MEDIO");
    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(AMARELO_BAIXO);
    }
    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(AZUL_MEDIO);
    }
    if (valores[BTN_4] >= BTN_HIGH)
    {
      mudaEstado(AMARELO_ALTO);
    }
    break;
  case AMARELO_ALTO:
    MQTT.publish("luminaria/estado", "AMARELO_ALTO");
    if (valores[BTN_1] >= BTN_HIGH)
    {
      mudaEstado(AGUARDANDO);
    }
    if (valores[BTN_2] >= BTN_HIGH)
    {
      mudaEstado(AMARELO_MEDIO);
    }
    if (valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(AZUL_ALTO);
    }
    break;
  case DORMINDO:
    MQTT.publish("luminaria/estado", "DORMINDO");
    sp = digitalRead(sensorPin);
    if (sp == HIGH)
    {
      mudaEstado(ACORDADO);
    }

    if (valores[BTN_1] >= BTN_HIGH && valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(AGUARDANDO);
    }

    break;
  case ACORDADO:
    MQTT.publish("luminaria/estado", "ACORDADO");
    Serial.println("ACORDADO");
    delay(2000);
    sp = digitalRead(sensorPin);
    if (sp == LOW)
    {
      mudaEstado(DORMINDO);
    }

    if (valores[BTN_1] >= BTN_HIGH && valores[BTN_3] >= BTN_HIGH)
    {
      mudaEstado(AGUARDANDO);
    }
    break;
  }
  MQTT.loop();
  delay(1000);
}

// Função que simula leitura capacitiva para um determinado pino de leitura
unsigned int lerSensorCapacitivo(byte rxPin, byte amostras)
{
  unsigned long sum = 0;

  for (int i = 0; i < amostras; i++)
  {
    unsigned long tempo = 0;

    digitalWrite(sendPin, LOW);
    pinMode(rxPin, OUTPUT);
    digitalWrite(rxPin, LOW);
    delayMicroseconds(100);

    pinMode(rxPin, INPUT);
    digitalWrite(sendPin, HIGH);

    while (digitalRead(rxPin) == LOW && tempo < 10000)
    {
      tempo++;
    }

    sum += tempo;
  }

  return (unsigned int)(sum / amostras);
}

// Imprime os valores dos botões: Usar para debugar a aplicação
void imprimeBotoes()
{
  unsigned long atual = millis();
  if (atual - anterior > 250)
  {
    Serial.print("B1: ");
    Serial.print(valores[0]);
    Serial.print(" | B2: ");
    Serial.print(valores[1]);
    Serial.print(" | B3: ");
    Serial.print(valores[2]);
    Serial.print(" | B4: ");
    Serial.println(valores[3]);

    anterior = atual;
  }
}

void mudaEstado(ESTADO novoEstado)
{
  if (estado_atual != novoEstado)
  {
    estado_atual = novoEstado;
  }
}

void mudaCorDoLed(ESTADO estado)
{
  for (int i = 0; i < N_LEDS; i++)
  { // Para cada led na fita recebe valores RGB de 0,0,0 até 255,255,255
    RGB const cor = corPorEstado[estado];
    strip.setPixelColor(i, strip.Color(cor.r, cor.g, cor.b));
  }
  strip.show();
}
