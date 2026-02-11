#define BLYNK_TEMPLATE_ID "Preencher"
#define BLYNK_TEMPLATE_NAME "Preencher"
#define BLYNK_AUTH_TOKEN "Preencher"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>  // Adicionado para manipulação de data e hora

// Definições de configuração do WiFi e Blynk
char ssid[] = "Nome wifi";
char pass[] = "Senha wifi";
char auth[] = BLYNK_AUTH_TOKEN;

// Pinos dos sensores e atuadores
#define PINO_RELE_BOMBA 26
#define PINO_RELE_LUZ_CRESCIMENTO 25
#define PINO_LED_VERDE 14
#define PINO_LED_VERMELHO 27
#define PINO_LED_AZUL 33

// Variáveis de configuração
int umidadeMinima = 40;
int umidadeMaxima = 50; // Novo valor para desligamento da bomba
int horasLuzCrescimento = 14;
int alturaMaximaReservatorio = 50;
bool alertaReservatorioEnviado = false;

BlynkTimer timer;
int horaSimulada = 6; // Inicializar em 6h para simulação de horário
int umidadeAtual = 80; // Inicializa um valor fixo para simular a umidade do solo
int distanciaAtual = 25; // Inicializa um valor fixo para simular a distância do nível de água
bool bombaLigada = false;

// Função para verificar o nível de água do reservatório
void verificaNivelAgua() {
  if (bombaLigada) {
    distanciaAtual = max(0, distanciaAtual - 1); // Decresce a distância ao nível da água se a bomba estiver ligada
  }
  Blynk.virtualWrite(V3, distanciaAtual); // Envia o valor do nível do reservatório
  Serial.print("Distância ao nível de água (simulada): ");
  Serial.print(distanciaAtual);
  Serial.println(" cm");
  


  if (distanciaAtual < 20 && !alertaReservatorioEnviado) {
    Blynk.logEvent("nivel_baixo_reservatorio", "Atenção: O reservatório de água está vazio!");
    alertaReservatorioEnviado = true;
    digitalWrite(PINO_LED_VERMELHO, HIGH); // Acende o LED vermelho
    Serial.println("LED vermelho: Ligado");
  } else if (distanciaAtual >= 20) {
    alertaReservatorioEnviado = false;
    digitalWrite(PINO_LED_VERMELHO, LOW); // Apaga o LED vermelho
    Serial.println("LED vermelho: Desligado");
  }
}

// Função para monitorar a umidade do solo e ativar a bomba de água
void verificaUmidadeSolo() {
  if (!bombaLigada) {
    umidadeAtual = max(0, umidadeAtual - 1); // Decresce a umidade ao longo do tempo
  } else {
    umidadeAtual = min(100, umidadeAtual + 5); // Aumenta a umidade quando a bomba está ligada
  }
  Blynk.virtualWrite(V4, umidadeAtual); // Envia o valor da umidade do solo simulada
  Serial.print("Umidade do solo (simulada): ");
  Serial.println(umidadeAtual);


  if (umidadeAtual < umidadeMinima && !bombaLigada) {
    digitalWrite(PINO_RELE_BOMBA, HIGH);
    Serial.println("Bomba de água: Ligada");
    Blynk.logEvent("umidade_baixa", "A umidade do solo está baixa. Iniciando irrigação automática.");
    bombaLigada = true;
    digitalWrite(PINO_LED_AZUL, HIGH); // Acende o LED azul
    Serial.println("LED azul: Ligado");
  } else if (umidadeAtual >= umidadeMaxima && bombaLigada) {
    digitalWrite(PINO_RELE_BOMBA, LOW);
    Serial.println("Bomba de água: Desligada");
    bombaLigada = false;
    digitalWrite(PINO_LED_AZUL, LOW); // Apaga o LED azul
    Serial.println("LED azul: Desligado");
  }
}

// Função para controlar a luz de crescimento manualmente via Blynk
BLYNK_WRITE(V1) {
  int estadoLuzCrescimento = param.asInt();
  digitalWrite(PINO_RELE_LUZ_CRESCIMENTO, estadoLuzCrescimento);
  digitalWrite(PINO_LED_VERDE, estadoLuzCrescimento); // Controla o LED verde
  Serial.print("Luz de crescimento (manual): ");
  Serial.println(estadoLuzCrescimento ? "Ligada" : "Desligada");
  Serial.println(estadoLuzCrescimento ? "LED verde: Ligado" : "LED verde: Desligado");
}

// Função para controlar a bomba de água manualmente via Blynk
BLYNK_WRITE(V2) {
  int estadoBomba = param.asInt();
  digitalWrite(PINO_RELE_BOMBA, estadoBomba);
  digitalWrite(PINO_LED_AZUL, estadoBomba); // Controla o LED azul
  bombaLigada = estadoBomba;
  Serial.print("Bomba de água (manual): ");
  Serial.println(estadoBomba ? "Ligada" : "Desligada");
  Serial.println(estadoBomba ? "LED azul: Ligado" : "LED azul: Desligado");
}

// Função para controlar o horário da luz de crescimento
void controlaLuzCrescimento() {
  // Incrementar o horário simulado
  horaSimulada = (horaSimulada + 1) % 24;
  Serial.print("Hora simulada: ");
  Serial.println(horaSimulada);
  if (horaSimulada >= 6 && horaSimulada < (6 + horasLuzCrescimento)) {
    digitalWrite(PINO_RELE_LUZ_CRESCIMENTO, HIGH);
    Serial.println("Luz UV: Ligada");
    digitalWrite(PINO_LED_VERDE, HIGH); // Acende o LED verde
    Serial.println("LED verde: Ligado");
    Serial.println("Luz de crescimento (automática): Ligada");
    Blynk.virtualWrite(V1, 1); // Atualiza o estado da luz de crescimento no Blynk
  } else {
    digitalWrite(PINO_RELE_LUZ_CRESCIMENTO, LOW);
    Serial.println("Luz UV: Desligada");
    digitalWrite(PINO_LED_VERDE, LOW); // Apaga o LED verde
    Serial.println("LED verde: Desligado");
    Serial.println("Luz de crescimento (automática): Desligada");
    Blynk.virtualWrite(V1, 0); // Atualiza o estado da luz de crescimento no Blynk
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PINO_RELE_BOMBA, OUTPUT);
  pinMode(PINO_RELE_LUZ_CRESCIMENTO, OUTPUT);
  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_VERMELHO, OUTPUT);
  pinMode(PINO_LED_AZUL, OUTPUT);

  digitalWrite(PINO_RELE_BOMBA, LOW);
  digitalWrite(PINO_RELE_LUZ_CRESCIMENTO, LOW);
  digitalWrite(PINO_LED_VERDE, LOW);
  digitalWrite(PINO_LED_VERMELHO, LOW);
  digitalWrite(PINO_LED_AZUL, LOW);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(5000L, verificaUmidadeSolo); // Reduzi o intervalo para 5 segundos
  timer.setInterval(10000L, verificaNivelAgua); // Reduzi o intervalo para 10 segundos
  timer.setInterval(3600000L, controlaLuzCrescimento);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Conexão WiFi perdida, tentando reconectar...");
    WiFi.begin(ssid, pass);
    delay(1000);
  }

  // Atualizar os valores no Blynk no início do loop
  Blynk.virtualWrite(V3, map(distanciaAtual, alturaMaximaReservatorio, 0, 0, 50));
  Blynk.virtualWrite(V4, umidadeAtual);

  Blynk.run();
  timer.run();
}