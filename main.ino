#include <Adafruit_NeoPixel.h>

#define BTN_HIGH 20
#define BTN_1 0
#define BTN_2 1
#define BTN_3 2
#define BTN_4 3
#define N_LEDS 30

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

void setup()
{
  Serial.begin(115200);
  pinMode(sendPin, OUTPUT);
  strip.begin();

  // Configura todos os receivePins como entrada inicialmente
  for (byte i = 0; i < 4; i++)
  {
    pinMode(receivePins[i], INPUT);
  }
  Serial.println("Iniciando a luminária...");
}

void loop()
{
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
