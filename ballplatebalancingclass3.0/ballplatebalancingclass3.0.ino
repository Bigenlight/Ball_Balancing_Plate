//Cascade 클래스
#include "coor.h"
//Matrix 클래스
#include "matrix.h"

Servo x_servo;
Servo y_servo;
HUSKYLENS ball_tracker;

//cascade 행렬 객채선언
coor x; // x축 계산
coor y; // y축 계산
//matrix 선언
mat xyplot;

void setup() {
  Serial.begin(115200);
  //Huskylens setup
  Wire.begin();
  ball_tracker.begin(Wire);
  //servo attach <coor.h>
  x_servo.attach(X_SERVO);
  y_servo.attach(Y_SERVO);
  //뱅뱅컨트롤 값
  x.bangrange = 5.5;
  y.bangrange = 2.5;
  //튜닝용 Kp 값
  x.u_kp = 0.7;
  y.u_kp = 0.6;
  //중심좌표(판 기준)
  x.c = 150;
  y.c = 140;
}

void loop() {
  //시간 측정 시작
  x.millisTime_i = millis();
  y.millisTime_i = millis();
  //huskylens 설정
  ball_tracker.request();
  ball_tracker.isLearned();
  //물체 감지하는 동안 발동
  while (ball_tracker.available()) {
    //huskylens 값 읽기
    HUSKYLENSResult result = ball_tracker.read();
    //Matrix update
    xyplot.update(result.xCenter, result.yCenter);
    x.r = xyplot.getx();
    y.r = xyplot.gety();
    //Cascade
    x.cascade();
    y.cascade();
    //서보암 조종
    Control_Servo(x.u, y.u);
    //시간 끝
    x.millisTime_f = millis();
    y.millisTime_f = millis();
    x.dt = x.millisTime_f - x.millisTime_i;
    y.dt = y.millisTime_f - y.millisTime_i;
  }
}

//서보모터 Microseconds 제어 함수
void Control_Servo(double ctrlx, double ctrly) {
  //Microseconds을 이용한 정밀 제어
  x_servo.writeMicroseconds(1350 + ctrlx);
  y_servo.writeMicroseconds(1520 + ctrly);
}
