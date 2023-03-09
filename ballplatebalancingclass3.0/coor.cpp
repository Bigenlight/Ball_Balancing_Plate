#include <Arduino.h>
#include "coor.h"

//속도 계산 함수
void coor::calcul(double now, double past) {
  V =(now - past) / dt;
  // 잡음 방지
  if (V == 0) {
    V = V_past;
  }
}
//Low Pass Filter
double coor ::lowpassfilter(double filter, double data, double constant) {
  filter = filter * (1 - constant) + data * constant;
  return filter;
}
//PID
double coor ::computePID(double r, double data, double dt, double u, double Kp, double Ki, double Kd, double bangbang_control_range) {
  double error = r - data; 
  double P = Kp * error;
  double I = Ki * error * dt;
  double D = Kd * (-data + data_past) / dt;
  I = constrain(I, I_min, I_max);
  data_past = data;
  I_past = I;

  u = P + I + D;
  
  //bangbang control
  if (abs(error) <= bangbang_control_range) {
    u = 0;
  }

  u = constrain(u, -500, 500);

  return u;
}

void coor ::cascade() {
  r_f = lowpassfilter(r_f, r, lowpass_constant); //위치 필터
  calcul(r, r_past); //속도 계산
  V_f = lowpassfilter(V_f, V, lowpass_constant); //속도 필터

  //Cascade
  u_v = computePID(c, r_f, dt, u_v, 0.042, 0, 0.91, bangrange); //1차(위치)
  u = computePID(-u_v, V_f, dt, u, u_kp, 0, 0.09, 1); //2차(속도)

  r_past = r;
  V_past = V;
}
