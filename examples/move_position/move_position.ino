/**
 * @example move_position.ino
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
  // 以100RPM的速度正转720度
  g_encoder_motor_board[0].MovePosition(720, 100);
  g_encoder_motor_board[1].MovePosition(720, 100);
  g_encoder_motor_board[2].MovePosition(720, 100);
  g_encoder_motor_board[3].MovePosition(720, 100);

  delay(3000);

  // 以100RPM的速度反转720度
  g_encoder_motor_board[0].MovePosition(-720, 100);
  g_encoder_motor_board[1].MovePosition(-720, 100);
  g_encoder_motor_board[2].MovePosition(-720, 100);
  g_encoder_motor_board[3].MovePosition(-720, 100);

  delay(3000);
}