#include <Arduino.h>
#include "coor.h"

void coor::calcul(double now, double past) {
  V = now - past;
  if (V == 0) {
    V = V_past;
  }
}

double coor ::lowpassfilter(double filter, double data, double constant) {
  filter = filter * (1 - constant) + data * constant;
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

  //뱅뱅 일단 삭제
  // if (abs(error) <= bangbang_control_range) {
  //   u = 0;
  // }

  u = constrain(u, -500, 500);


  return u;
}

void coor ::cascade() {
  //대폭 수정
  r_f = lowpassfilter(r_f, r, 0.1);
  //필터한 값을 속도로 지정
  // calcul(r, r_past);
  calcul(r_f, r_f_past);
  //필터값
  // V_f = lowpassfilter(V_f, V, 0.1);

  // r_past = r;
  r_f_past = r_f;
  V_past = V;

  //대폭 수정
  //V Pid 먼저
  u_v = computePID(0, V + (V*V/2), dt, u_v, 60, 0, 0.5, bangrange);

  //error 값은 음수가 나와 순서 바꿈
  //R
  u = computePID(u_v, r_f - c, dt, u, u_kp, 0, 0, 1);
  // u_l = lowpassfilter(u_l, u, 0.5);

  //y 축의 u_v 값이 불명의 이유로 엄청 커짐 -> 해결한듯

  // millisTime_f = millis();
  // dt = millisTime_f - millisTime_i;
}
