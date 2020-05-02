/*
 * Copyright 2020 Pim van Pelt <pim@ipng.nl>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "mgos.h"
#include "mgos_pca9685.h"

#define MGOS_PCA9685_I2C_ADDR 0x40

// Registers
//
#define MGOS_PCA9685_REG_MODE1 0x00
#define MGOS_PCA9685_REG_MODE2 0x01
#define MGOS_PCA9685_REG_SUBADR1 0x02
#define MGOS_PCA9685_REG_SUBADR2 0x03
#define MGOS_PCA9685_REG_SUBADR3 0x04
#define MGOS_PCA9685_REG_ALLCALLADR 0x05
#define MGOS_PCA9685_REG_LED0_ON_L 0x06
#define MGOS_PCA9685_REG_LED0_ON_H 0x07
#define MGOS_PCA9685_REG_LED0_OFF_L 0x08
#define MGOS_PCA9685_REG_LED0_OFF_H 0x09
#define MGOS_PCA9685_REG_ALL_LED_ON_L 0xFA
#define MGOS_PCA9685_REG_ALL_LED_ON_H 0xFB
#define MGOS_PCA9685_REG_ALL_LED_OFF_L 0xFC
#define MGOS_PCA9685_REG_ALL_LED_OFF_H 0xFD
#define MGOS_PCA9685_REG_PRE_SCALE 0xFE
#define MGOS_PCA9685_REG_TESTMODE 0xFF

struct mgos_pca9685 {
  struct mgos_pca9685_config config;

  uint32_t osc_freq;
};
