//Cascade 클래스
#include "coor.h"
//Matrix 클래스
#include "matrix.h"

Servo x_servo;
Servo y_servo;
HUSKYLENS ball_tracker;

//서보모터 Microseconds 제어
void Control_Servo(double ctrlx, double ctrly) {
  x_servo.writeMicroseconds(1350 + ctrlx);
  y_servo.writeMicroseconds(1520 + ctrly);
}

//행렬 객채선언
coor x;
coor y;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  ball_tracker.begin(Wire);
  x_servo.attach(X_SERVO);
  y_servo.attach(Y_SERVO);
  x.bangrange = 5.5;
  y.bangrange = 2.5;

  x.u_kp = 0.7;
  y.u_kp = 0.6;

  x.c = 150;
  y.c = 140;
}

void loop() {

  x.millisTime_i = millis();
  y.millisTime_i = millis();

  ball_tracker.request();
  ball_tracker.isLearned();

  while (ball_tracker.available()) {
    HUSKYLENSResult result = ball_tracker.read();
    x.r = result.xCenter;
    y.r = result.yCenter;

    //행렬 업데이트
    mat xyplot(x.r_f, y.r_f);
    xyplot.update();

    x.filtering();
    y.filtering();

    Control_Servo(x.u, y.u);
  }

  // x.millisTime_f = millis();
  // y.millisTime_f = millis();
  // x.dt = x.millisTime_f - x.millisTime_i;
  // y.dt = y.millisTime_f - y.millisTime_i;

}
