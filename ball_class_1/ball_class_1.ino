#include "coor.h"

Servo x_servo;
Servo y_servo;
HUSKYLENS ball_tracker;

void Control_Servo(double ctrlx, double ctrly) {
  x_servo.writeMicroseconds(1350 + ctrlx);
  y_servo.writeMicroseconds(1520 + ctrly);
}

coor x;
coor y;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  ball_tracker.begin(Wire);
  x_servo.attach(X_SERVO);
  y_servo.attach(Y_SERVO);
  x.bangrange = 5.5;
  y.bangrange = 4.5;
  x.u_kp = 10;
  y.u_kp = 10.5;
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
  
    x.c = 150;
    y.c = 140;

    x.filtering();
    y.filtering();
//수
    Control_Servo(x.u, y.u);
  }

  x.millisTime_f = millis();
  y.millisTime_f = millis();
  x.dt = x.millisTime_f - x.millisTime_i;
  y.dt = y.millisTime_f - y.millisTime_i;
}