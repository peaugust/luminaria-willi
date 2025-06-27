// Carrega a biblioteca de controle do NeoPixel
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Número do pino onde o LED foi conectado
#define LED_PIN 13
// Número do pino onde o sensor de presença foi conectado
#define SENSOR_PIN 2

#define BUTTON_1 12 // Cima
#define BUTTON_2 8  // Esquerda
#define BUTTON_3 7  // Baixo
#define BUTTON_4 4  // Direita

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

struct RGB
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

bool operator!=(const RGB &a, const RGB &b)
{
  return a.r != b.r || a.g != b.g || a.b != b.b;
}

const RGB estadoCores[] = {
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
    {255, 255, 255}  // ACORDADO
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
// Variável que armazena cor atual
RGB cor_atual = estadoCores[estado_atual];

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

void mudaCorDoLed(RGB cor);
void mudaEstado(ESTADO novoEstado);
/****************************************
 * Loop
 */
void loop()
{
  Serial.println(estado_atual);
  // Fita de led liga
  pixels.clear(); // Set all pixel colors to 'off'
  // Lê todos os botões
  leEstadoDosBotoes();
  mudaCorDoLed(estadoCores[estado_atual]);
  switch (estado_atual)
  {
  case AGUARDANDO:
    if (bt1 && bt3)
    {
      mudaEstado(DORMINDO);
    }
    else if (bt1)
    {
      mudaEstado(BRANCO_BAIXO);
    }
    break;
  case BRANCO_BAIXO:
    if (bt1)
    {
      mudaEstado(VERMELHO_BAIXO);
    }

    if (bt4)
    {
      mudaEstado(BRANCO_MEDIO);
    }

    if (bt3)
    {
      mudaEstado(AGUARDANDO);
    }
    break;
  case BRANCO_MEDIO:
    if (bt1)
    {
      mudaEstado(VERMELHO_MEDIO);
    }

    if (bt2)
    {
      mudaEstado(BRANCO_BAIXO);
    }

    if (bt4)
    {
      mudaEstado(BRANCO_ALTO);
    }
    break;
  case BRANCO_ALTO:
    if (bt1)
    {
      mudaEstado(VERMELHO_ALTO);
    }

    if (bt2)
    {
      mudaEstado(BRANCO_MEDIO);
    }
    break;
  case VERMELHO_BAIXO:
    if (bt1)
    {
      mudaEstado(VERDE_BAIXO);
    }
    if (bt3)
    {
      mudaEstado(BRANCO_BAIXO);
    }
    if (bt4)
    {
      mudaEstado(VERMELHO_MEDIO);
    }
    break;
  case VERMELHO_MEDIO:
    if (bt1)
    {
      mudaEstado(VERDE_MEDIO);
    }
    if (bt2)
    {
      mudaEstado(VERMELHO_BAIXO);
    }
    if (bt3)
    {
      mudaEstado(BRANCO_MEDIO);
    }
    if (bt4)
    {
      mudaEstado(VERMELHO_ALTO);
    }
    break;
  case VERMELHO_ALTO:
    if (bt1)
    {
      mudaEstado(VERDE_ALTO);
    }
    if (bt2)
    {
      mudaEstado(VERMELHO_MEDIO);
    }
    if (bt3)
    {
      mudaEstado(BRANCO_ALTO);
    }
    break;
  case VERDE_BAIXO:
    if (bt1)
    {
      mudaEstado(AZUL_BAIXO);
    }
    if (bt3)
    {
      mudaEstado(VERMELHO_BAIXO);
    }
    if (bt4)
    {
      mudaEstado(VERDE_MEDIO);
    }
    break;
  case VERDE_MEDIO:
    if (bt1)
    {
      mudaEstado(AZUL_MEDIO);
    }
    if (bt2)
    {
      mudaEstado(VERDE_BAIXO);
    }
    if (bt3)
    {
      mudaEstado(VERMELHO_MEDIO);
    }
    if (bt4)
    {
      mudaEstado(VERDE_ALTO);
    }
    break;
  case VERDE_ALTO:
    if (bt1)
    {
      mudaEstado(AZUL_ALTO);
    }
    if (bt2)
    {
      mudaEstado(VERDE_MEDIO);
    }
    if (bt3)
    {
      mudaEstado(VERMELHO_ALTO);
    }
    break;
  case AZUL_BAIXO:
    if (bt1)
    {
      mudaEstado(AMARELO_BAIXO);
    }
    if (bt3)
    {
      mudaEstado(VERDE_BAIXO);
    }
    if (bt4)
    {
      mudaEstado(AZUL_MEDIO);
    }
    break;
  case AZUL_MEDIO:
    if (bt1)
    {
      mudaEstado(AMARELO_MEDIO);
    }
    if (bt2)
    {
      mudaEstado(AZUL_BAIXO);
    }
    if (bt3)
    {
      mudaEstado(VERDE_MEDIO);
    }
    if (bt4)
    {
      mudaEstado(AZUL_ALTO);
    }
    break;
  case AZUL_ALTO:
    if (bt1)
    {
      mudaEstado(AMARELO_ALTO);
    }
    if (bt2)
    {
      mudaEstado(AMARELO_MEDIO);
    }
    if (bt3)
    {
      mudaEstado(VERDE_ALTO);
    }
    break;
  case AMARELO_BAIXO:
    if (bt1)
    {
      mudaEstado(AGUARDANDO);
    }
    if (bt3)
    {
      mudaEstado(AZUL_BAIXO);
    }
    if (bt4)
    {
      mudaEstado(AMARELO_MEDIO);
    }
    break;
  case AMARELO_MEDIO:
    if (bt2)
    {
      mudaEstado(AMARELO_BAIXO);
    }
    if (bt3)
    {
      mudaEstado(AZUL_MEDIO);
    }
    if (bt4)
    {
      mudaEstado(AMARELO_ALTO);
    }
    break;
  case AMARELO_ALTO:
    if (bt2)
    {
      mudaEstado(AMARELO_MEDIO);
    }
    if (bt3)
    {
      mudaEstado(AZUL_ALTO);
    }
    break;
  case DORMINDO:
    Serial.println("DORMINDO");
    if (sp == HIGH)
    {
      mudaEstado(ACORDADO);
    }
    break;
  case ACORDADO:
    Serial.println("ACORDADO");
    delay(3000);
    break;
  }
  delay(200);
}

void mudaEstado(ESTADO novoEstado)
{
  if (estado_atual != novoEstado)
  {
    estado_atual = novoEstado;
  }
}

void leEstadoDosBotoes()
{
  bt1 = digitalRead(BUTTON_1);
  bt2 = digitalRead(BUTTON_2);
  bt3 = digitalRead(BUTTON_3);
  bt4 = digitalRead(BUTTON_4);
}

void mudaCorDoLed(RGB cor)
{
  if (cor_atual != cor)
  {
    cor_atual = cor;
    for (int i = 0; i < NUMPIXELS; i++)
    { // Para cada led na fita
      // pixels.Color() recebe valores RGB de 0,0,0 até 255,255,255
      pixels.setPixelColor(i, pixels.Color(cor.r, cor.g, cor.b));
    }
    pixels.show(); // Send the updated pixel colors to the hardware.
  }
}
