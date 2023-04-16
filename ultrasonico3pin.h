#define sig 10 // Pino de comunicaçao ligado ao SIG
float distancia; // Variavel de leitura da distancia

void leitura(); // Funçao de leitura da distancia

void utrasonicSetup(){
  pinMode(sig, OUTPUT);
  digitalWrite(sig, LOW); // Inicia com o pino em nível baixo
}

void ultrasonico(){
  leitura();

  distancia = distancia/2; // Divide por 2 pois esse e o tempo da onda ir e voltar
  distancia = distancia*0.034029; // Multiplicamos por 0,034 por causa da velocidade do som (d=v*t)

  Serial.print("Altura do tanque 2 em cm: ");
  Serial.println(distancia);

  delay(500); // Esse delay pode ser menor
}

void leitura(){
  pinMode(sig, OUTPUT); // Define o pino como saida
  digitalWrite(sig, HIGH); // Manda um sinal de nível alto por 5 microssegundos
  delayMicroseconds(5);
  digitalWrite(sig, LOW);

  pinMode(sig, INPUT); // Define o pino como entrada para fazer a leitura
  distancia = pulseIn(sig, HIGH); // Le o comprimento de onda em alta
}