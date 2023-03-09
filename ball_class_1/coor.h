// #include <Arduino.h>
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include <Servo.h>

#define I_max 300
#define I_min 0
#define X_SERVO 9
#define Y_SERVO 10
#define lowpass_constant 0.1

class coor
{
  public:

//수정: 클래스 안으로 추가
float millisTime_i;
float millisTime_f;
float dt = 0;
float I_past = 0;
float data_past = 0;

  double c;
  double r;
  double r_f;
  double V;
  double V_past = 0;
  double r_past = 0;
  double V_f = 0;
  double u;
  double u_v = 0;
  double bangrange, u_kp;
  
  void calcul(double, double);
  double lowpassfilter (double, double);
  double computePID(double, double, double, double, double, double, double, double);
  void filtering();
};