#include "coor.h"
#include "matrix.h"


Servo x_servo;
Servo y_servo;
HUSKYLENS ball_tracker;

void Control_Servo(double ctrlx, double ctrly) {
  x_servo.writeMicroseconds(1350 + ctrlx);
  y_servo.writeMicroseconds(1520 + ctrly);
}


//객체 선언
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
  // x.u_kp = 0.7;
  // y.u_kp = 0.6;
    x.u_kp = 1;
  y.u_kp = 0.9;

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

    //오프셋 지정
    // x.c = 150;
    // y.c = 140;
    x.c = 180;
    y.c = 180;

    //행렬 업데이트
    // mat xyplot(x.r_f, y.r_f);
    // xyplot.update();

    //명칭변경
    x.cascade();
    y.cascade();

    Control_Servo(x.u, y.u);
  }

  x.millisTime_f = millis();
  y.millisTime_f = millis();
  x.dt = x.millisTime_f - x.millisTime_i;
  y.dt = y.millisTime_f - y.millisTime_i;

  Serial.print(" xV= ");
  Serial.print(x.V);
  Serial.print(" yV= ");
  Serial.print(y.V);
  Serial.print(" xV_f= ");
  Serial.print(x.V_f);
  Serial.print(" yV_f= ");
  Serial.print(y.V_f);
  Serial.print(" xr_f= ");
  Serial.print(x.r_f);
  Serial.print(" yr_f= ");
  Serial.print(y.r_f);
  Serial.print(" xerror= ");
  Serial.print(x.c - x.r_f);
  Serial.print(" yerror= ");
  Serial.print(y.c - y.r_f);
  Serial.print(" xu_v= ");
  Serial.print(x.u_v);
  Serial.print(" yu_v= ");
  Serial.print(y.u_v);
  Serial.print(", ");
  Serial.print(" xu= ");
  Serial.print(x.u);
  Serial.print(" yu= ");
  Serial.println(y.u);
}