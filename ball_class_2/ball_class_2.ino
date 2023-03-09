#include "coor.h"
#include "matrix.h"

//c보정용, 보정 후 지울 것
int state = 0;
unsigned long prevmillis;
//

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
  y.bangrange = 2.5;

  x.u_kp = 0.7;
  y.u_kp = 0.6;
  // x.u_kp = 0.8;
  // y.u_kp = 0.7;

  x.c = 150;
  y.c = 140;
  // x.c = 180;
  // y.c = 180;
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

  x.millisTime_f = millis();
  y.millisTime_f = millis();
  x.dt = x.millisTime_f - x.millisTime_i;
  y.dt = y.millisTime_f - y.millisTime_i;

  Serial.print(" cx= ");
  Serial.print(x.c);
  Serial.print(" cy= ");
  Serial.print(y.c);
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
  Serial.print(y.u);
  Serial.print(" t= ");
  Serial.println(x.dt);

  //현장 c값 보정용
  if (Serial.available() > 0 && state == 0 && millis() - prevmillis > 3000) {
    x.c= Serial.parseInt();
    state = 1;
    prevmillis = millis();
  }
  if (Serial.available() > 0 && state == 1 && millis() - prevmillis > 3000) {
    y.c= Serial.parseInt();
    state = 0;
    prevmillis = millis();
  }
}
