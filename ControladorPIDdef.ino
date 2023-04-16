#include <Servo.h> 
#include <NewPing.h>
#include "ultrasonico3pin.h"
#include "flow.h"

//Mapeamento de hardware
#define Servo_pin 12
#define trig       9
#define echo       8

//Variáveis independentes
int max_dist_cm = 25;
double limite_inferior = 0;
double limite_superior = 90;
double old_tilt_deg = limite_inferior; 
double cumulative_error = 0; 
double previous_error = 0; 

//valores de Kp, Ki, Kd
double Kp = 1.5;
double Ki = 0.05;
double Kd = 0.1;

Servo Servo1; // objeto que controla o servo motor
NewPing sonar(trig, echo, max_dist_cm); // objeto que controla o sensor ultrasonico de 4 pinos

void setup(){
  Servo1.attach(Servo_pin);
  Serial.begin(9600);
  utrasonicSetup();
  flowSetup();
  delay(50);
  Servo1.write(limite_inferior);
}

void loop(){
  double distancia = sonar.ping_cm();
  Serial.print("distancia: ");
  Serial.print(distancia);
  Serial.print(",");  
  double tempo_de_delay = 5; // em milisegundos
  // condição para ignorar os valores negativos do ultrasonico
  if(distancia > 0){
    double novo_angulo = pid(distancia);
    Serial.print("angulo: ");
    Serial.print(novo_angulo);
    Serial.print(",");    
    atualiza_servo(novo_angulo, old_tilt_deg, tempo_de_delay);
    old_tilt_deg = novo_angulo;       
  }    
}

void atualiza_servo(double new_tilt_deg, double old_tilt_deg, double delay_time_ms) { 
  // as duas condicionais mudam do valor antigo para o novo valor um por um
  if (new_tilt_deg > old_tilt_deg) {
    for (double pos = old_tilt_deg; pos <= new_tilt_deg; pos += 1) {
      // incrementa de grau em grau
      Servo1.write(pos);             
      delay(delay_time_ms);                       
    }
  }

  if (new_tilt_deg < old_tilt_deg) {
    for (double pos = old_tilt_deg; pos >= new_tilt_deg; pos -= 1) {
      // incrementa de grau em grau
      Servo1.write(pos);             
      delay(delay_time_ms);                       
    }
  }
}


double pid(double distance_cm) {
  
  double setpoint_cm = 11.75;
  Serial.print("setpoint_cm:");
  Serial.print(setpoint_cm);
  Serial.print(","); 

  double error = setpoint_cm - distance_cm;
  Serial.print("error_cm:");
  Serial.print(error);
  Serial.print(","); 
  
  double p_value = error * Kp;

  double i_value = cumulative_error * Ki;

  double d_value = (error - previous_error) * Kd;

  
  double pid_value = p_value + i_value + d_value;
  
  Serial.print("Valor do controlador:");
  Serial.println(pid_value);

  cumulative_error += error; 
  double servo_range = limite_superior - limite_inferior;
  double new_servo_angle = map(pid_value, servo_range, -1*servo_range, limite_inferior, limite_superior);

  /
  if (new_servo_angle > limite_superior) {
    new_servo_angle = limite_superior;
  }
  
  if (new_servo_angle < limite_inferior) {
    new_servo_angle = limite_inferior;
  }
  
  return new_servo_angle;
}