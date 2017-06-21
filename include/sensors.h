/*
 *
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, James Jackson and Daniel Koch, BYU MAGICC Lab, Provo UT
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ROSFLIGHT_FIRMWARE_SENSORS_H
#define ROSFLIGHT_FIRMWARE_SENSORS_H

#include <stdint.h>
#include <stdbool.h>
#include <turbovec.h>

namespace rosflight_firmware
{

class ROSflight;

class Sensors
{

  struct Data
  {
    vector_t _accel = {0, 0, 0};
    vector_t _gyro = {0, 0, 0};
    float _imu_temperature = 0;
    uint64_t _imu_time = 0;

    float _diff_pressure_velocity = 0;
    float _diff_pressure = 0;
    float _diff_pressure_temp = 0;

    float _baro_altitude = 0;
    float _baro_pressure = 0;
    float _baro_temperature = 0;

    float _sonar_range = 0;

    vector_t _mag = {0, 0, 0};
  };

public:
  Sensors(ROSflight& rosflight);

  inline const Data& data() const { return data_; }

  // function declarations
  void init();
  bool update_sensors();
  void IMU_ISR();

  // Calibration Functions
  bool start_imu_calibration(void);
  bool start_gyro_calibration(void);
  void start_baro_calibration(void);
  void start_airspeed_calibration(void);
  bool gyro_calibration_complete(void);


  inline bool should_send_imu_data(void)
  {
    if (imu_data_sent_)
      return false;
    else
      imu_data_sent_ = true;
    return true;
  }

//  // Sensor present functions
//  bool magnetometer_present(void);
//  bool differential_pressure_present(void);
//  bool barometer_present(void);
//  bool sonar_present(void);

private:
  ROSflight& rf_;

  Data data_;

  float accel[3] = {0, 0, 0};
  float gyro[3] = {0, 0, 0};

  bool calibrating_acc_flag = false;
  bool calibrating_gyro_flag = false;
  void calibrate_accel(void);
  void calibrate_gyro(void);
  void correct_imu(void);
  void correct_mag(void);
  void imu_ISR(void);
  bool update_imu(void);
  uint32_t last_time_look_for_disarmed_sensors = 0;
  uint32_t last_imu_update_ms = 0;

  bool new_imu_data_;
  bool imu_data_sent_;

  // IMU calibration
  uint16_t gyro_calibration_count = 0;
  vector_t gyro_sum = {0, 0, 0};
  uint16_t accel_calibration_count = 0;
  vector_t acc_sum = {0, 0, 0};
  const vector_t gravity = {0.0f, 0.0f, 9.80665f};
  float acc_temp_sum = 0.0f;
  vector_t max = {-1000.0f, -1000.0f, -1000.0f};
  vector_t min = {1000.0f, 1000.0f, 1000.0f};

};

} // namespace rosflight_firmware

#endif // ROSFLIGHT_FIRMWARE_SENSORS_H
