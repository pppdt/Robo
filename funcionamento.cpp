#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;  // Instância do Bluetooth

enum Direcao {NORTE, LESTE, SUL, OESTE};

int x = 0, y = 0;
Direcao direcao = NORTE;

const int GRID_WIDTH = 5;
const int GRID_HEIGHT = 5;

void setup() {
  Serial.begin(115200);      // Debug no cabo USB
  SerialBT.begin("RoboESP32");  // Nome do dispositivo Bluetooth

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  Serial.println("Bluetooth iniciado como 'RoboESP32'");
}

void loop() {
  if (SerialBT.available()) {
    char comando = SerialBT.read();

    switch (comando) {
      case 'f':
        moverFrente();
        break;
      case 't':
        moverTras();
        break;
      case 'd':
        girarDireita();
        break;
      case 'e':
        girarEsquerda();
        break;
      case 'p':
        stopMotors();
        break;
      default:
        Serial.println("Comando inválido recebido via Bluetooth.");
        return;
    }

    Serial.print("Posição: (");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(") | Direção: ");
    imprimirDirecao();
    Serial.println();
  }
}

// ======= Movimento virtual + físico =======
void moverFrente() {
  if (podeMover(1)) {
    moverMotoresFrente();
    delay(1000);
    atualizarPosicao(1);
    stopMotors();
  } else {
    Serial.println("Movimento bloqueado (borda do tabuleiro).");
    stopMotors();
  }
}

void moverTras() {
  if (podeMover(-1)) {
    moverMotoresTras();
    delay(1000);
    atualizarPosicao(-1);
    stopMotors();
  } else {
    Serial.println("Movimento bloqueado (borda do tabuleiro).");
    stopMotors();
  }
}

void girarDireita() {
  moverMotoresDireita();
  delay(600);
  stopMotors();
  direcao = static_cast<Direcao>((direcao + 1) % 4);
}

void girarEsquerda() {
  moverMotoresEsquerda();
  delay(600);
  stopMotors();
  direcao = static_cast<Direcao>((direcao + 3) % 4);
}

// ======= Lógica virtual =======
bool podeMover(int passo) {
  int nx = x, ny = y;

  switch (direcao) {
    case NORTE: ny -= passo; break;
    case SUL:   ny += passo; break;
    case LESTE: nx += passo; break;
    case OESTE: nx -= passo; break;
  }

  return (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT);
}

void atualizarPosicao(int passo) {
  switch (direcao) {
    case NORTE: y -= passo; break;
    case SUL:   y += passo; break;
    case LESTE: x += passo; break;
    case OESTE: x -= passo; break;
  }
}

void imprimirDirecao() {
  switch (direcao) {
    case NORTE: Serial.print("NORTE"); break;
    case SUL:   Serial.print("SUL"); break;
    case LESTE: Serial.print("LESTE"); break;
    case OESTE: Serial.print("OESTE"); break;
  }
}

// ======= Controle dos motores =======
void moverMotoresFrente() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moverMotoresTras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moverMotoresDireita() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moverMotoresEsquerda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
