## 🌱 Sistema de Automação para Irrigação IoT
**Autores:** 🧑‍💻 Matheus Finamor & Tiago Ferreira

Este projeto consiste em um sistema de automação para irrigação inteligente desenvolvido com ESP32 e Blynk IoT. Ele monitora e controla automaticamente a umidade do solo, nível de água do reservatório e ciclo de iluminação para cultivos.

---

## 📋 Funcionalidades do Sistema

O sistema oferece as seguintes funcionalidades:

**Controle Automático de Irrigação** – Liga/desliga a bomba de água baseado na umidade do solo.

**Monitoramento do Reservatório** – Verifica o nível de água e emite alertas quando está baixo.

**Ciclo de Iluminação Automático** – Controla luzes de crescimento por 14 horas diárias.

**Controle Manual Remoto** – Permite controle via aplicativo Blynk (iOS/Android).

**Sistema de Alertas** – Envia notificações para o smartphone em eventos críticos.

**Indicação Visual por LEDs** – LEDs mostram status do sistema em tempo real.

---

## ⚙️ Estrutura do Código
O código é organizado em funções separadas, facilitando manutenção e expansão:

**setup()** – Configura pinos, WiFi e conexão Blynk.

**loop()** – Gerencia conexões e executa timers.

**verificaUmidadeSolo()** – Controla irrigação baseado na umidade.

**verificaNivelAgua()** – Monitora nível do reservatório.

**controlaLuzCrescimento()** – Gerencia ciclo diário de iluminação.

**BLYNK_WRITE(V1)** – Callback para controle manual da luz via app.

**BLYNK_WRITE(V2)** – Callback para controle manual da bomba via app.

---

## 🛠️ Como Configurar e Executar
Hardware Necessário:

Placa ESP32

Módulo relé de 2 canais

LEDs (verde, vermelho, azul)

Fonte de alimentação 5V/12V

---

Configuração do Software:

Instale Arduino IDE e bibliotecas (Blynk, TimeLib)

Configure credenciais no código:
#define BLYNK_TEMPLATE_ID "SEU_TEMPLATE"
#define BLYNK_AUTH_TOKEN "SEU_TOKEN"
char ssid[] = "SEU_WIFI";
char pass[] = "SENHA_WIFI";

---

Conexões dos Pinos (ESP32 → Componentes):

GPIO26 → Relé da Bomba
GPIO25 → Relé da Luz
GPIO14 → LED Verde
GPIO27 → LED Vermelho
GPIO33 → LED Azul

---

Execução:

Conecte o ESP32 via USB

Faça upload do código na Arduino IDE

Abra o Serial Monitor (115200 baud)

Configure o app Blynk com os widgets necessários

---

## 📱 Interface Blynk
Widget	Pino Virtual	Função
Botão	V1	Controle manual da luz
Botão	V2	Controle manual da bomba
Medidor	V3	Nível do reservatório (0-50cm)
Medidor	V4	Umidade do solo (0-100%)

---

## ⚡ Funcionamento Automático

**Irrigação:** Verifica umidade a cada 5s, ativa bomba se <40%, desliga se ≥50%

**Iluminação:** Ciclo automático das 6h às 20h (14 horas)

**Reservatório:** Verifica nível a cada 10s, alerta se <20cm

**Alertas:** Notificações no app para "umidade_baixa" e "nivel_baixo_reservatorio"

---

## 📄 Licença
Este projeto está sob licença MIT. Veja o arquivo LICENSE para detalhes.

