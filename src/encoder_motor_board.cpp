#include "encoder_motor_board.h"

#include <Arduino.h>
#include <Wire.h>

namespace emakefun {

enum Command : uint8_t {
  kGetVersion = 0,
  kReset = 1,
  kStop = 2,
  kRunPwm = 3,
  kRunSpeed = 4,
  kMovePositionTo = 5,
  kMovePosition = 6,
  kSetSpeedPid = 7,
  kSetPositionPid = 8,
  kSetRatio = 9,
  kSetPpr = 10,
  kSetCurrentPosition = 11,
  kGetCurrentSpeed = 12,
  kGetCurrentPosition = 13,
  kQueryIsTargetPositionReached = 14,
  kGetSpeedPid = 15,
  kGetPositionPid = 16,
  kGetReductionRatio = 17,
  kGetPpr = 18,
  kGetCurrentPwm = 19,
  kSetPositiveBLevel = 20,
  kGetPositiveBLevel = 21,
};

EncoderMotorBoard::EncoderMotor::EncoderMotor(const uint8_t i2c_address, const uint8_t index)
    : i2c_address_(i2c_address), index_(index) {}

bool EncoderMotorBoard::EncoderMotor::Initialize(TwoWire *const wire) {
  wire_ = wire;
  Reset();
  return true;
}

void EncoderMotorBoard::EncoderMotor::Reset() {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kReset);
  wire_->write(index_);
  wire_->endTransmission();
}

void EncoderMotorBoard::EncoderMotor::Stop() {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kStop);
  wire_->write(index_);
  wire_->endTransmission();
}

void EncoderMotorBoard::EncoderMotor::RunSpeed(const int16_t speed) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kRunSpeed);
  wire_->write(index_);
  wire_->write(reinterpret_cast<const uint8_t *>(&speed), sizeof(speed));
  wire_->endTransmission();
}

void EncoderMotorBoard::EncoderMotor::RunPwm(const int16_t pwm) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kRunPwm);
  wire_->write(index_);
  wire_->write(reinterpret_cast<const uint8_t *>(&pwm), sizeof(pwm));
  wire_->endTransmission();
}

void EncoderMotorBoard::EncoderMotor::MovePosition(const int32_t position, const uint16_t speed) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kMovePosition);
  wire_->write(index_);
  wire_->write(reinterpret_cast<const uint8_t *>(&position), sizeof(position));
  wire_->write(reinterpret_cast<const uint8_t *>(&speed), sizeof(speed));
  wire_->endTransmission();
}

void EncoderMotorBoard::EncoderMotor::MovePositionTo(const int32_t position, const uint16_t speed) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kMovePositionTo);
  wire_->write(index_);
  wire_->write(reinterpret_cast<const uint8_t *>(&position), sizeof(position));
  wire_->write(reinterpret_cast<const uint8_t *>(&speed), sizeof(speed));
  wire_->endTransmission();
}

void EncoderMotorBoard::EncoderMotor::SpeedPID(const float p, const float i, const float d) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kSetSpeedPid);
  wire_->write(index_);
  uint16_t value = round(p * 100);
  wire_->write(reinterpret_cast<const uint8_t *>(&value), sizeof(value));
  value = round(i * 100);
  wire_->write(reinterpret_cast<const uint8_t *>(&value), sizeof(value));
  value = round(d * 100);
  wire_->write(reinterpret_cast<const uint8_t *>(&value), sizeof(value));
  wire_->endTransmission();
}

void EncoderMotorBoard::EncoderMotor::ReductionRatio(const uint8_t value) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kSetRatio);
  wire_->write(index_);
  wire_->write(reinterpret_cast<const uint8_t *>(&value), sizeof(value));
  wire_->endTransmission();
}

void EncoderMotorBoard::EncoderMotor::ABPpr(const uint8_t value) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kSetPpr);
  wire_->write(index_);
  wire_->write(reinterpret_cast<const uint8_t *>(&value), sizeof(value));
  wire_->endTransmission();
}

void EncoderMotorBoard::EncoderMotor::CurrentPosition(const int32_t value) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kSetCurrentPosition);
  wire_->write(index_);
  wire_->write(reinterpret_cast<const uint8_t *>(&value), sizeof(value));
  wire_->endTransmission();
}

float EncoderMotorBoard::EncoderMotor::SpeedP() const {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kGetSpeedPid);
  wire_->write(index_);
  wire_->endTransmission();

  uint16_t pid[3] = {0};
  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(sizeof(pid)));
  wire_->readBytes(reinterpret_cast<uint8_t *>(&pid), sizeof(pid));
  return static_cast<float>(pid[0]) / 100.0;
}

float EncoderMotorBoard::EncoderMotor::SpeedI() const {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kGetSpeedPid);
  wire_->write(index_);
  wire_->endTransmission();

  uint16_t pid[3] = {0};
  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(sizeof(pid)));
  wire_->readBytes(reinterpret_cast<uint8_t *>(&pid), sizeof(pid));
  return static_cast<float>(pid[1]) / 100.0;
}

float EncoderMotorBoard::EncoderMotor::SpeedD() const {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kGetSpeedPid);
  wire_->write(index_);
  wire_->endTransmission();

  uint16_t pid[3] = {0};
  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(sizeof(pid)));
  wire_->readBytes(reinterpret_cast<uint8_t *>(&pid), sizeof(pid));
  return static_cast<float>(pid[2]) / 100.0;
}

uint8_t EncoderMotorBoard::EncoderMotor::ReductionRatio() const {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kGetReductionRatio);
  wire_->write(index_);
  wire_->endTransmission();
  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(1));
  return wire_->read();
}
uint8_t EncoderMotorBoard::EncoderMotor::ABPpr() const {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kGetPpr);
  wire_->write(index_);
  wire_->endTransmission();
  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(1));
  return wire_->read();
}

void EncoderMotorBoard::EncoderMotor::PositiveBLevel(const uint8_t level) {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kSetPositiveBLevel);
  wire_->write(index_);
  wire_->write(level);
  wire_->endTransmission();
}

uint8_t EncoderMotorBoard::EncoderMotor::PositiveBLevel() const {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kGetPositiveBLevel);
  wire_->write(index_);
  wire_->endTransmission();
  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(1));
  return wire_->read();
}

int16_t EncoderMotorBoard::EncoderMotor::CurrentPwm() const {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kGetCurrentPwm);
  wire_->write(index_);
  wire_->endTransmission();

  int16_t pwm = 0;
  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(sizeof(pwm)));
  wire_->readBytes(reinterpret_cast<uint8_t *>(&pwm), sizeof(pwm));
  return pwm;
}

float EncoderMotorBoard::EncoderMotor::CurrentSpeed() const {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kGetCurrentSpeed);
  wire_->write(index_);
  wire_->endTransmission();

  int32_t speed = 0;
  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(sizeof(speed)));
  wire_->readBytes(reinterpret_cast<uint8_t *>(&speed), sizeof(speed));
  return static_cast<float>(speed) / 100.0;
}

int32_t EncoderMotorBoard::EncoderMotor::CurrentPosition() const {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kGetCurrentPosition);
  wire_->write(index_);
  wire_->endTransmission();

  int32_t position = 0;
  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(sizeof(position)));
  wire_->readBytes(reinterpret_cast<uint8_t *>(&position), sizeof(position));
  return position;
}

bool EncoderMotorBoard::EncoderMotor::IsTargetPositionReached() const {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kQueryIsTargetPositionReached);
  wire_->write(index_);
  wire_->endTransmission();
  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(1));
  return wire_->read();
}

EncoderMotorBoard::EncoderMotorBoard(const uint8_t i2c_address)
    : i2c_address_(i2c_address),
      encoder_motors_{EncoderMotor(i2c_address, 0), EncoderMotor(i2c_address, 1), EncoderMotor(i2c_address, 2),
                      EncoderMotor(i2c_address, 3)} {}

bool EncoderMotorBoard::Initialize(TwoWire *const wire) {
  wire_ = wire;
  wire_->begin();
  for (auto &encoder_motor : encoder_motors_) {
    encoder_motor.Initialize(wire);
  }
  return true;
}

uint8_t EncoderMotorBoard::Version() {
  wire_->beginTransmission(i2c_address_);
  wire_->write(kGetVersion);
  wire_->endTransmission();
  wire_->requestFrom(i2c_address_, static_cast<uint8_t>(1));
  return wire_->read();
}
}  // namespace emakefun