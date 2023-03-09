#include <Arduino.h>
#include "coor.h"

//필터 값으로 과거 대입
void coor::calcul(double now, double past) {
  //dt가 완전히 일정치 않아 dt로 나눔
  V = (now - past) / dt;
  if (V == 0) {
    V = V_past;
  }
  if (V==1 || V ==-1){
    V = 0;
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
  
  //bangbang
  // if (abs(error) <= bangbang_control_range) {
  //   u = 0;
  // }

  u = constrain(u, -500, 500);


  return u;
}

void coor ::filtering() {
  //수정  
  r_f = lowpassfilter(r_f, r, 0.1);
  calcul(r, r_past);
  V_f = lowpassfilter(V_f, V, 0.1);


//대폭 수정
//V Pid 먼저 // 가속도 추가!
  u_v = computePID(0, V_f + 5*(V_f - V_f_past), dt, u_v, 75, 0, 0.5, bangrange);

//error 값은 음수가 나와 순서 바꿈
  u = computePID(u_v, r_f - c, dt, u, u_kp, 0, 0, 1);
  // u_l = lowpassfilter(u_l, u, 0.5);

//y 축의 u_v 값이 불명의 이유로 엄청 커짐 -> 해결한듯

// 맨 아래로 이동
  r_past = r;
  //필터 값 대입 - 취소 오히려 잘 안됨
  V_past = V;
  V_f_past = V_f;

  millisTime_f = millis();
  dt = millisTime_f - millisTime_i;
}
