/**
 * @example run_speed.ino
 */

#include <Wire.h>

#include "encoder_motor_board.h"

// 创建EncoderMotorBoard对象，I2C地址为0x09
const emakefun::EncoderMotorBoard g_encoder_motor_board(0x09);

void setup() {
  Serial.begin(115200);
  Serial.println("setup");

  // 启动Wire
  Wire.begin();

  // 初始化，传入Wire对象，可自定义Wire参数，如设置SDA和SCL引脚后再传入Wire
  g_encoder_motor_board.Initialize(&Wire);

  // 对4路编码电机进行初始化
  for (const auto& encoder_motor : g_encoder_motor_board) {
    // 设置编码电机编码盘每转脉冲数
    encoder_motor.ABPpr(12);
    // 设置编码电机的减速比
    encoder_motor.ReductionRatio(45);
    // 设置在编码电机正转时,A相上升沿对应B相的电平逻辑
    encoder_motor.PositiveBLevel(1);
  }

  Serial.println("setup done");
}

void loop() {
  // 设置编码电机0以80RMP的速度正转
  g_encoder_motor_board[0].RunSpeed(80);
  // 设置编码电机1以90RMP的速度反转
  g_encoder_motor_board[1].RunSpeed(-90);
  // 设置编码电机2以100RMP的速度反转
  g_encoder_motor_board[2].RunSpeed(-100);
  // 设置编码电机3以110RMP的速度正传
  g_encoder_motor_board[3].RunSpeed(110);

  // 打印每路编码电机的各项信息
  for (const auto& encoder_motor : g_encoder_motor_board) {
    Serial.print("index: ");
    Serial.print(encoder_motor.Index());
    Serial.print(", speed: ");
    Serial.print(encoder_motor.CurrentSpeed());
    Serial.print(", pwm: ");
    Serial.print(encoder_motor.CurrentPwm());
    Serial.print(", position: ");
    Serial.println(encoder_motor.CurrentPosition());
  }

  delay(100);
}