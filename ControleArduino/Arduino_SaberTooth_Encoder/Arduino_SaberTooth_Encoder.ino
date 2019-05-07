#include <SabertoothSimplified.h>
SabertoothSimplified ST;

// We'll name the Sabertooth object ST.
// For how to configure the Sabertooth, see the DIP Switch Wizard for
//   http://www.dimensionengineering.com/datasheets/SabertoothDIPWizard/start.htm
// Be sure to select Simplified Serial Mode for use with this library.
// This sample uses a baud rate of 9600.
//
// Connections to make (See Gunther_Assembly_Manual):
//   Arduino TX->1  ->  Sabertooth S1
//   Arduino GND    ->  Sabertooth 0V
//   Arduino VIN    ->  Sabertooth 5V (OPTIONAL, if you want the Sabertooth to power the Arduino)
//   Arduino PIN 3  ->  Encoder white cable LA 
//   Arduino PIN 4  ->  Encoder white cable LB
//   Arduino PIN 5  ->  Encoder white cable RA
//   Arduino PIN 6  ->  Encoder white cable RB
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
int Converted_Gain = -40; // Start with this value for the wheel to start moving

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
  ST.motor(1, Converted_Gain); //Gain of the PI control
  ST.motor(2, -Converted_Gain);
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

//Front controlled speed with PI
void encoder_Forward() {
  n = digitalRead(encoder0PinA_Left);
  half_forward();
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB_Left) == LOW) {
      encoder0Pos++;
      // Time between encoder signals
      Delta_t = (millis() - PreviusMillis) * 0.001;
      PreviusMillis = millis();
      Sum_t = Sum_t + Delta_t;
      //Linear speed with respect to 10 degrees (encoder sensitivity) of wheel displacement of 7.5 cm radius.
      vel = ((10 * PI / 180) / Delta_t) * 0.0775;
      Sum_vel = Sum_vel + vel;

      //      Serial.print("Front wheel speed right ");
      //      Serial.print(encoder0Pos);
      //      Serial.print(": ");
      //      Serial.print(vel);
      //      Serial.println(" m/s");
    }
  }

  encoder0PinALast = n;
  //Average speed of a wheel
  Media_vel = Sum_vel / encoder0Pos;
  //Setting the linear velocity input
  float ref_velo = 0.5;
  double erro = ref_velo - Media_vel;
  //Proportional gain
  double kp = 0.003;
  //Integrative Gain
  double ki = 0.0003;


  if (abs(erro) > 0.02) {
    //PID control
    double u = erro * kp + (erro + epx) * ki;
    //Integrator Cumulative Error
    epx = epx + erro;
    //Speed saturation conversion
    Converted_Gain = round(-127 * u / 0.6);
    Serial.print("Velocity: ");
    Serial.print(vel);
    Serial.print(" Erro: ");
    Serial.print(erro);
    Serial.print(" Controlled Gain: ");
    Serial.print(u);
    Serial.print(" Gain Converted: ");
    Serial.println(Converted_Gain);
  } else {
    Serial.print("Controlled! ");
    Serial.print("Media Velocity: ");
    Serial.println(Media_vel);
  }

  //Setting the distance to be traveled (100 cm -> 1 meter)
  if (Media_vel * Sum_t >= 100) {
    Stop();
    delay(3000);
    Sum_vel = 0;
    encoder0Pos = 0;
    Sum_t = 0;
    PreviusMillis = millis();
    //Change the flag to enter the rotation routine
    flag = true;
  }
}

//Control of the right rotation distance without PI
void encoder_Right() {
  n = digitalRead(encoder0PinA_Left);
  half_right();
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB_Left) == LOW) {
      encoder0Pos++;
      Delta_t = (millis() - PreviusMillis) * 0.001;
      // Time between encoder signals
      PreviusMillis = millis();
      Sum_t = Sum_t + Delta_t;
      // Velocity with respect to 10 degrees (encoder sensitivity) of wheel displacement of 7.5 cm radius.
      vel = ((10 * PI / 180) / Delta_t) * 0.0775; 
      Sum_vel = Sum_vel + vel;

      //      Serial.print("Velocity Right Rotation Left Wheel ");
      //      Serial.print(encoder0Pos);
      //      Serial.print(": ");
      //      Serial.print(vel);
      //      Serial.println(" m/s");
    }
  }
  encoder0PinALast = n;
  Media_vel = Sum_vel / encoder0Pos;
  //Setting the distance to be traveled (perimeter -> 0.66 = 180 degress)
  if (Media_vel * Sum_t >= 0.66) {
    Stop();
    delay(3000);
    Sum_vel = 0;
    encoder0Pos = 0;
    Sum_t = 0;
    // Time between encoder signals
    PreviusMillis = millis();
    //Change the flag to enter the frontal routine
    flag = false;
  }
}

void loop()
{
  // Routine to rotate 360 degrees a wheel
  //  if (encoder0Pos == 36) {
  //    Stop();
  //    delay(3000);
  //    PreviusMillis = millis();
  //    encoder0Pos = 0;
  //  }


  // Routine go forward turn 180 degrees and go back
  //  if (flag == false) {
  encoder_Forward();
  //  }
  //  if (flag == true) {
  //    encoder_Right();
  //  }

}
