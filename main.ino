// Carrega a biblioteca de controle do NeoPixel
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Número do pino onde o LED foi conectado
#define LED_PIN 13
// Número do pino onde o sensor de presença foi conectado
#define SENSOR_PIN 2

#define BUTTON_1 4  // Direita
#define BUTTON_2 7  // Baixo
#define BUTTON_3 8  // Esquerda
#define BUTTON_4 12 // Cima

// Comprimento da fita de LED
#define NUMPIXELS 15

// Enumeração dos estados possíveis
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

// Inicializa a fita de LED
// Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Variável que armazena o valor do estado atual da luminária
ESTADO estado_atual = AGUARDANDO;
// Variável que armazena o valor lido do sensor de presença
int sp = 0;
// Variável que armazena o valor do botão 1
int bt1 = 0;
// Variável que armazena o valor do botão 2
int bt2 = 0;
// Variável que armazena o valor do botão 3
int bt3 = 0;
// Variável que armazena o valor do botão 4
int bt4 = 0;

/****************************************
 * SetUp
 */
void setup()
{
  // Inicia a porta serial
  Serial.begin(9600);

  // Inicia a fita de LED
  pixels.begin();
  // Inicia o sensor de presença
  pinMode(SENSOR_PIN, INPUT);
  // Inicia o botão 1
  pinMode(BUTTON_1, INPUT);
  // Inicia o botão 2
  pinMode(BUTTON_2, INPUT);
  // Inicia o botão 3
  pinMode(BUTTON_3, INPUT);
  // Inicia o botão 3
  pinMode(BUTTON_4, INPUT);
}

/****************************************
 * Loop
 */
void loop()
{
  readAllButtons();

  switch (estado_atual)
  {
  case AGUARDANDO:
    Serial.println("AGUARDANDO");

    if (bt1 && bt3)
    {
      estado_atual = DORMINDO;
    }
    else if (bt1)
    {
      estado_atual = BRANCO_BAIXO;
    }
    break;
  case BRANCO_BAIXO:
    Serial.println("BRANCO_BAIXO");

    if (bt1)
    {
      estado_atual = VERMELHO_BAIXO;
    }
    break;
  case BRANCO_MEDIO:
    Serial.println("BRANCO_MEDIO");
    break;
  case BRANCO_ALTO:
    Serial.println("BRANCO_ALTO");
    break;
  case VERMELHO_BAIXO:
    Serial.println("VERMELHO_BAIXO");
    if (bt1)
    {
      estado_atual = VERDE_BAIXO;
    }
    break;
  case VERMELHO_MEDIO:
    Serial.println("VERMELHO_MEDIO");
    break;
  case VERMELHO_ALTO:
    Serial.println("VERMELHO_ALTO");
    break;
  case VERDE_BAIXO:
    Serial.println("VERDE_BAIXO");
    if (bt1)
    {
      estado_atual = AZUL_BAIXO;
    }
    break;
  case VERDE_MEDIO:
    Serial.println("VERDE_MEDIO");
    break;
  case VERDE_ALTO:
    Serial.println("VERDE_ALTO");
    break;
  case AZUL_BAIXO:
    Serial.println("AZUL_BAIXO");
    if (bt1)
    {
      estado_atual = AMARELO_BAIXO;
    }
    break;
  case AZUL_MEDIO:
    Serial.println("AZUL_MEDIO");
    break;
  case AZUL_ALTO:
    Serial.println("AZUL_ALTO");
    break;
  case AMARELO_BAIXO:
    Serial.println("AMARELO_BAIXO");
    if (bt1)
    {
      estado_atual = BRANCO_BAIXO;
    }
    break;
  case AMARELO_MEDIO:
    Serial.println("AMARELO_MEDIO");
    break;
  case AMARELO_ALTO:
    Serial.println("AMARELO_ALTO");
    break;
  case DORMINDO:
    Serial.println("DORMINDO");
    break;
  case ACORDADO:
    Serial.println("ACORDADO");
    break;
  }
  delay(100);
}

void readAllButtons()
{
  bt1 = digitalRead(BUTTON_1);
  bt2 = digitalRead(BUTTON_2);
  bt3 = digitalRead(BUTTON_3);
  bt4 = digitalRead(BUTTON_4);
}
