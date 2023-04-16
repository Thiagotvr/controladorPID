#define flowPin 2
double flowRate;
volatile int count;

void Flow();

void flowSetup(){
  pinMode(flowPin, INPUT);
  attachInterrupt(0, Flow, RISING);
}

float vazao_atual(){
  count = 0;      // Reseta o contador para iniciarmos a contagem em 0 novamente
  interrupts();   //Habilita o interrupção no Arduino
  delay (1000);   //Espera 1 segundo
  noInterrupts(); //Desabilita o interrupção no Arduino
   
  //Cálculos matemáticos
  flowRate = (count * 2.25);        //Conta os pulsos no último segundo e multiplica por 2,25mL, que é a vazão de cada pulso
  flowRate = flowRate * 60;         //Converte segundos em minutos, tornando a unidade de medida mL/min
  flowRate = flowRate / 1000;       //Converte mL em litros, tornando a unidade de medida L/min
 
  return (flowRate); 
}

void Flow()
{
   count++; //Quando essa função é chamada, soma-se 1 a variável "count" 
}
