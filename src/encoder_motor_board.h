#pragma once

#include <Wire.h>
#include <inttypes.h>

namespace emakefun {

/**
 * @brief 编码电机驱动板类
 *
 */

class EncoderMotorBoard {
 public:
  /**
   * @brief 最大支持编码电机数量
   *
   */
  static constexpr uint8_t kEncoderMotorNum = 4;

  /**
   * @brief 默认I2C地址
   *
   */
  static constexpr uint8_t kDefaultI2cAddress = 0x09;

  /**
   * @brief 编码电机类，每个对象对应一路编码电机
   *
   */
  class EncoderMotor {
   public:
    /**
     * @brief 返回对应的编译电机位置
     *
     * @return uint8_t 范围为 0 ~ 3
     */
    inline uint8_t Index() const { return index_; }
    /**
     * @brief 初始化函数
     *
     * @param wire TwoWire*对象
     * @return true 代表成功
     * @return false 代表失败
     */
    bool Initialize(TwoWire* const wire);

    /**
     * @brief 重置编码电机的所有参数，包括减速比、速度、PWM、位置信息、PID信息
     *
     */
    void Reset();
    /**
     * @brief 停止转动
     *
     */
    void Stop();
    /**
     * @brief 以指定速度转动
     *
     * @param speed 单位为RPM，大于0代表正向转动，小于0代表反向转动
     */
    void RunSpeed(const int16_t speed);
    /**
     * @brief 以指定的PWM值转动
     *
     * @param pwm 大于0代表正向转动，小于0代表反向转动，范围为 -255 ~ 255
     */
    void RunPwm(const int16_t pwm);
    /**
     * @brief 以指定速度转动到相对位置
     *
     * @param position 角度位置，编码电机正转一圈角度位置会增加360，反转一圈角度位置会减少360
     * @param speed 单位为RPM，必须大于0
     */
    void MovePosition(const int32_t position, const uint16_t speed);
    /**
     * @brief 以指定速度转动到绝对位置
     *
     * @param position 角度位置，编码电机正转一圈角度位置会增加360，反转一圈角度位置会减少360
     * @param speed 单位为RPM，必须大于0
     */
    void MovePositionTo(const int32_t position, const uint16_t speed);

    /**
     * @brief 设置速度PID，如不设置会采用默认值
     *
     * @param p p值，最小为0.01，默认为0.8
     * @param i i值，最小为0.01，默认为0.4
     * @param d d值, 最小为0，默认为0
     */
    void SpeedPID(const float p, const float i, const float d);
    /**
     * @brief 设置编码电机的减速比，默认为90
     *
     * @param value 减速比
     */
    void ReductionRatio(const uint8_t value);
    /**
     * @brief 设置编码电机编码盘每转脉冲数(PPR)，默认为12
     *
     * @param value 每转脉冲数(PPR)
     */
    void ABPpr(const uint8_t value);

    /**
     * @brief 设置当前编码电机的角度位置，编码电机正转一圈角度位置会增加360，反转一圈角度位置会减少360
     *
     * @param value 角度位置
     */
    void CurrentPosition(const int32_t value);

    /**
     * @brief 获取速度PID中的P值
     *
     * @return float
     */
    float SpeedP() const;
    /**
     * @brief 获取速度PID中的I值
     *
     * @return float
     */
    float SpeedI() const;
    /**
     * @brief 获取速度PID中的D值
     *
     * @return float
     */
    float SpeedD() const;

    /**
     * @brief 获取当前编码电机的减速比
     *
     * @return 减速比
     */
    uint8_t ReductionRatio() const;

    /**
     * @brief 获取当前编码电机的编码盘每转脉冲数(PPR)
     *
     * @return 每转脉冲数(PPR)
     */
    uint8_t ABPpr() const;

    /**
     * @brief 获取当前PWM值
     *
     * @return PWM值
     */
    int16_t CurrentPwm() const;

    /**
     * @brief 设置在编码电机正转时,A相上升沿对应B相的电平逻辑
     * @details 编码电机的A相和B相之间存在90度的电气相位差。不同型号的编码电机,其A相上升沿时刻B相的电平可能是高或低
     *          所以需要设置这个标志位来匹配具体电机的A、B相布线与正转方向的关系
     *
     * @param level 当该标志位为1时,表示A相上升沿检测到B相高电平判定为正转
     *              当该标志位为0时,表示A相上升沿检测到B相低电平判定为正转
     *              默认为0
     */
    void PositiveBLevel(const uint8_t level);
    /**
     * @brief 获取编码电机正转时,A相上升沿对应B相的电平逻辑
     *
     * @return 编码电机正转时,A相上升沿对应B相的电平逻辑
     */
    uint8_t PositiveBLevel() const;

    /**
     * @brief 获取当前转动速度
     *
     * @return 当前转动速度，单位为RPM，正值代表正转，负值代表反转
     */
    float CurrentSpeed() const;

    /**
     * @brief 获取当前的角度位置
     *
     * @return 角度位置
     */
    int32_t CurrentPosition() const;

    /**
     * @brief 查询是否转动到指定位置
     *
     * @return true 代表转动到指定位置
     * @return false 未转动到指定位置
     */
    bool IsTargetPositionReached() const;

   private:
    friend EncoderMotorBoard;
    EncoderMotor(const uint8_t i2c_address, const uint8_t index);
    EncoderMotor operator=(const EncoderMotor&) = delete;
    EncoderMotor(const EncoderMotor&) = default;

    const uint8_t i2c_address_ = 0x00;
    const uint8_t index_ = 0;
    TwoWire* wire_ = nullptr;
  };

  class Iterator {
   public:
    Iterator(EncoderMotor* const encoder_motor) : encoder_motor_(encoder_motor) {}

    EncoderMotor& operator*() const { return *encoder_motor_; }

    Iterator& operator++() {
      encoder_motor_++;
      return *this;
    }
    bool operator!=(const Iterator& other) const { return encoder_motor_ != other.encoder_motor_; }

   private:
    EncoderMotor* encoder_motor_;
  };

  EncoderMotorBoard(const uint8_t i2c_address = kDefaultI2cAddress);

  /**
   * @brief 初始化
   *
   * @param[in] wire Wire对象的地址，用于I2C通讯，可选，默认使用Arduino标准的Wire对象进行I2C通讯
   * @return true 初始化成功
   * @return false 初始化失败
   */
  bool Initialize(TwoWire* const wire = &Wire);

  /**
   * @brief 获取驱动板固件版本号
   *
   * @return 驱动板固件版本号
   */
  uint8_t Version();

  /**
   * @brief 重载[]操作符，可以像数组一样操作对每路编码电机进行操作
   * 
   * 示例:
   * @code
   * const emakefun::EncoderMotorBoard encoder_motor_board(0x09);
   * encoder_motor_board[1].RunPwm(255);
   * @endcode
   *
   * @param index 0 ~ 3
   * @return const EncoderMotor& 编码电机对象引用
   *
   */
  inline const EncoderMotor& operator[](const uint8_t index) { return encoder_motors_[index]; }
  inline const EncoderMotor (&EncoderMotors())[kEncoderMotorNum] { return encoder_motors_; }
  Iterator begin() { return Iterator(encoder_motors_); }
  Iterator end() { return Iterator(encoder_motors_ + kEncoderMotorNum); }

 private:
  EncoderMotorBoard operator=(const EncoderMotorBoard&) = delete;
  EncoderMotorBoard(const EncoderMotorBoard&) = delete;

  const uint8_t i2c_address_ = 0x00;
  const EncoderMotor encoder_motors_[kEncoderMotorNum];
  TwoWire* wire_ = nullptr;
};

}  // namespace emakefun