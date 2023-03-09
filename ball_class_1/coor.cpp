#include <Arduino.h>
#include "coor.h"

void coor::calcul(double now, double past) {
  V = now - past;
  if (V == 0) {
    V = V_past;
  }
}

double coor ::lowpassfilter(double filter, double data) {
  filter = filter * (1 - lowpass_constant) + data * lowpass_constant;
  return filter;
}

double coor ::computePID(double r, double data, double dt, double u, double Kp, double Ki, double Kd, double bangbang_control_range) {
  double error = r - data;
  double P = Kp * error;
  double I = Ki * error * dt;
  double D = Kd * (-data + data_past) / dt;
  I = constrain(I, I_min, I_max);
  data_past = data;
  I_past = I;

  u = P + I + D;

  if (abs(error) <= bangbang_control_range) {
    u = 0;
  }
  
  u = constrain(u, -500, 500);


  return u;
}

void coor ::filtering() {
  //수정  
  r_f = lowpassfilter(r_f, r);
  calcul(r, r_past);
  V_f = lowpassfilter(V_f, V);

  r_past = r;
  V_past = V;

  u_v = computePID(c, r_f, dt, u_v, 0.0412, 0, 0.91, bangrange);

  u = computePID(-u_v, V_f, dt, u, u_kp, 0, 0.09, 1);

//y 축의 u_v 값이 불명의 이유로 엄청 커짐
  Serial.print("bang ");
  Serial.print(bangrange);
  Serial.print(" V_f= ");
  Serial.print(V_f);
  Serial.print(" r_f= ");
  Serial.print(r_f);
  Serial.print(" error= ");
  Serial.print(c - r_f);
  Serial.print(" u_v= ");
  Serial.print(u_v);
  Serial.print(", ");
  Serial.print("u= ");
  Serial.println(u);


}
