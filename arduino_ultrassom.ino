// Definir os pinos do sensor ultrassônico
const int pinoTrig = 9;  // Pino do Trig
const int pinoEcho = 10; // Pino do Echo

// Definir constantes para divisores de conversão
const float divisorCm = 27.6233;  // Fator de conversão para cm
const float raio = 3.5;         // Raio da base do cilindro em cm (exemplo)
const float pi = 3.14;      // Definir valor de PI

// Definir limites do volume de água
const int volumeMinimo = 200;    // Limite mínimo em ml
const int volumeMedio = 500;     // Limite médio em ml
const int volumeMaximo = 650;    // Limite máximo em ml

int tam_da_g = 0;  // Variável para armazenar o valor fornecido pelo usuário

void setup() {
  // Inicializa o monitor serial
  Serial.begin(9600);
  
  // Configurar os pinos como entrada e saída
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);

  // Pedir o valor ao usuário
  Serial.println("Digite um número inteiro para 'tam_da_g' e pressione Enter:");
  
  // Aguarda até que o usuário insira um número válido
  while (Serial.available() == 0) {
    // Espera até que haja algo no buffer serial
  }
  
  // Após a inserção do número, lê o valor
  tam_da_g = Serial.parseInt();
  Serial.print("Você digitou: ");
  Serial.println(tam_da_g);
}

void loop() {
  // Calcula a duração do tempo de viagem do som em microssegundos
  long duracao = medirDistancia();

  // Converte a duração em distância (cm)
  float distanciaCm = converterParaCm(duracao);

  // Calcula o volume do cilindro
  float volume = calcularVolumeCilindro(raio, distanciaCm) - tam_da_g;  // Subtrai tam_da_g

  // Garante que o volume seja positivo
  volume = abs(volume);

  // Exibir a distância e o volume no monitor serial
  Serial.print("Distância: ");
  Serial.print(distanciaCm);
  Serial.print(" cm, ");

  Serial.print("Volume: ");
  Serial.print(volume);
  Serial.println(" ml");

  // Verificar os níveis de água
  if (volume < volumeMinimo) {
    Serial.println("Nível de água baixo! Ligando a água...");
    // Aqui você poderia acionar um relé para ligar a água
  } 
  else if (volume >= volumeMinimo && volume < volumeMedio) {
    Serial.println("Água em bom nível, mas pode encher mais se quiser.");
  } 
  else if (volume >= volumeMedio && volume <= volumeMaximo) {
    Serial.println("Água está boa, mas pode encher um pouco mais.");
  } 
  else if (volume > volumeMaximo) {
    Serial.println("Nível de água alto! Pare de encher.");
    // Aqui você poderia acionar um relé para desligar a água
  }

  // Aguardar 1 segundo antes da próxima medição
  delay(1000);
}

// Função para medir a duração do pulso de eco
long medirDistancia() {
  // Enviar pulso baixo e depois alto no pino de trig
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);

  // Medir a duração do pulso de eco em microssegundos
  long duracao = pulseIn(pinoEcho, HIGH);

  return duracao;
}

// Função para converter a duração em centímetros
float converterParaCm(long microssegundos) {
  return microssegundos / divisorCm / 2.0;  // Dividir por 2 para ida e volta do sinal
}

// Função para calcular o volume de um cilindro
float calcularVolumeCilindro(float raio, float altura) {
  return pi * sq(raio) * altura;  // V = π * r^2 * h
}
