/**
 * @example move_position_to.ino
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
  // 设置编码电机0以100RPM的速度转动到360度
  g_encoder_motor_board[0].MovePositionTo(360, 100);
  // 设置编码电机1以100RPM的速度转动到-360度
  g_encoder_motor_board[1].MovePositionTo(-360, 100);
  // 设置编码电机2以100RPM的速度转动到360度
  g_encoder_motor_board[2].MovePositionTo(360, 100);
  // 设置编码电机3以100RPM的速度转动到-360度
  g_encoder_motor_board[3].MovePositionTo(-360, 100);

  delay(2000);

  // 设置每路编码电机以100RPM的速度转动到0度
  for (const auto& encoder_motor : g_encoder_motor_board) {
    encoder_motor.MovePositionTo(0, 100);
  }

  delay(2000);
}