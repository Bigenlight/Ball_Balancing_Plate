#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include <Servo.h>
//I constrain
#define I_max 300
#define I_min 0
//서보 핀
#define X_SERVO 9
#define Y_SERVO 10
//lowpass 상수
#define lowpass_constant 0.1

//x축과 y축 coordinate 클래스
class coor {
public:
  //변수 선언
  float millisTime_i; // 시간
  float millisTime_f;
  float dt = 0; // 루프 시간
  float I_past = 0; 
  float data_past = 0;
  double c; // 목표 좌표 (중심좌표)
  double r; // 현재 좌표
  double r_past = 0;
  double r_f; // 필터링 된 좌표
  double V = 0; // 속도
  double V_past = 0;
  double V_f = 0;
  double u;
  double u_v = 0;
  double bangrange, u_kp;
  
  void cascade(); // cascade 메인 함수
  void calcul(double, double); //속도 계산 함수
  double lowpassfilter(double, double, double); //Low Pass Filter
  double computePID(double, double, double, double, double, double, double, double); //PiD
};