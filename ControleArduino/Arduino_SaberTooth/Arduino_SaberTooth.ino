#include <SabertoothSimplified.h>
SabertoothSimplified ST;

// We'll name the Sabertooth object ST.
// For how to configure the Sabertooth, see the DIP Switch Wizard for
//   http://www.dimensionengineering.com/datasheets/SabertoothDIPWizard/start.htm
// Be sure to select Simplified Serial Mode for use with this library.
// This sample uses a baud rate of 9600.
//
// Connections to make:
//   Arduino TX->1  ->  Sabertooth S1
//   Arduino GND    ->  Sabertooth 0V
//   Arduino VIN    ->  Sabertooth 5V (OPTIONAL, if you want the Sabertooth to power the Arduino)
//
// If you want to use a pin other than TX->1, see the SoftwareSerial example.

int val;
int encoder0PinA_Left = 3;
int encoder0PinB_Left = 4;
int encoder0PinA_Right = 5;
int encoder0PinB_Right = 6;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;
float  vel = 0;
float  Delta_t = 0;
float Sum_t = 0;
float Sum_vel = 0;
float Media_vel = 0;
float  PreviusMillis = 0;
bool flag = false;

//variaveis controlador
double epx = 0;
int Ganho_Convertido = -40;

void setup()
{
  SabertoothTXPinSerial.begin(9600); // This is the baud rate you chose with the DIP switches.
  delay(1000);
  pinMode (encoder0PinA_Right, INPUT);
  pinMode (encoder0PinB_Right, INPUT);
  pinMode (encoder0PinA_Left, INPUT);
  pinMode (encoder0PinB_Left, INPUT);

}

void full_backward() {
  ST.motor(1, 127);
  ST.motor(2, -127);
}

void full_forward() {
  ST.motor(1, -127);
  ST.motor(2, 127);
}

void half_forward() {
  ST.motor(1, Ganho_Convertido);
  ST.motor(2, -Ganho_Convertido);
}

void Stop() {
  ST.motor(1, 0);
  ST.motor(2, 0);
}

void full_right() {
  ST.motor(1, 127);
  ST.motor(2, 127);
}

void half_right() {
  ST.motor(1, 43);
  ST.motor(2, 40);
}

void full_left() {
  ST.motor(1, -127);
  ST.motor(2, -127);
}

void encoder_Forward() {
  n = digitalRead(encoder0PinA_Left);
  half_forward();
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB_Left) == LOW) {
      encoder0Pos++;
      Delta_t = (millis() - PreviusMillis) * 0.001;
      PreviusMillis = millis();
      Sum_t = Sum_t + Delta_t;
      vel = ((10 * PI / 180) / Delta_t) * 0.0775; //Velocidade com relação a 10 graus (sensibilidade do encoder) de deslocamento da roda de 7,5 cm de raio.
      Sum_vel = Sum_vel + vel;

//      Serial.print("Velocidade Frontal Roda Direita ");
//      Serial.print(encoder0Pos);
//      Serial.print(": ");
//      Serial.print(vel);
//      Serial.println(" m/s");
    }
  }
  encoder0PinALast = n;
  Media_vel = Sum_vel / encoder0Pos;
  float ref_velo = 0.5;
  double erro = ref_velo - Media_vel;
  double kp = 0.003;
  double ki = 0.0003;
  
  
  if(abs(erro) > 0.02){
    double u = erro*kp + (erro+epx)*ki;
    epx = epx + erro;
    Ganho_Convertido = round(-127*u/0.6);
    Serial.print("Velocidade: ");
    Serial.print(vel);
    Serial.print(" Erro: ");
    Serial.print(erro);
    Serial.print(" Ganho controlador: ");
    Serial.print(u);
    Serial.print(" Ganho Convertido: ");
    Serial.println(Ganho_Convertido);
  }else{
    Serial.print("Controlado! ");
    Serial.print("Media da Velocidade: ");
    Serial.println(Media_vel);
  }

  
  if (Media_vel * Sum_t >= 100) {
    Stop();
    delay(3000);
    Sum_vel=0;
    encoder0Pos = 0;
    Sum_t = 0;
    PreviusMillis = millis();
    flag = true;
  }
}

void encoder_Right() {
  n = digitalRead(encoder0PinA_Left);
  half_right();
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB_Left) == LOW) {
      encoder0Pos++;
      Delta_t = (millis() - PreviusMillis) * 0.001;
      PreviusMillis = millis();
      Sum_t = Sum_t + Delta_t;
      vel = ((10 * PI / 180) / Delta_t) * 0.0775; //Velocidade com relação a 10 graus (sensibilidade do encoder) de deslocamento da roda de 7,5 cm de raio.
      Sum_vel = Sum_vel + vel;

//      Serial.print("Velocidade Rotação Direita Roda Esquerda ");
//      Serial.print(encoder0Pos);
//      Serial.print(": ");
//      Serial.print(vel);
//      Serial.println(" m/s");
    }
  }
  encoder0PinALast = n;
  Media_vel = Sum_vel / encoder0Pos;
  if (Media_vel * Sum_t >= 0.66) {
    Stop();
    delay(3000);
    Sum_vel=0;
    encoder0Pos = 0;
    Sum_t = 0;
    PreviusMillis = millis();
    flag = false;
  }
}

void loop()
{

  //  if (encoder0Pos == 36) {
  //    Stop();
  //    delay(3000);
  //    PreviusMillis = millis();
  //    encoder0Pos = 0;
  //  }

//  if (flag == false) {
    encoder_Forward();
//  }
//  if (flag == true) {
//    encoder_Right();
//  }

}
