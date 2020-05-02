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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "mgos_i2c.h"

struct mgos_pca9685;

struct mgos_pca9685_config {
  struct mgos_i2c *i2c;
  uint8_t i2c_addr;
};

// Create an instance of the driver at the given config.
// The *cfg pointer can be NULL, in which case defaults are used.
// Returns a pointer to the object upon success, NULL otherwise.
struct mgos_pca9685 *mgos_pca9685_create(const struct mgos_pca9685_config *cfg);

// Set the given config structure to reasonable defaults.
void mgos_pca9685_default_cfg(struct mgos_pca9685_config *cfg);

// Write or read PWM on/off positions to/from the given channel.
// chan is a number from [0..16>
// on is the position in the [0..4096> cycle to turn the output ON
// off is the position in the [0..4096> cycle to turn the output OFF
//
// The helper function chan_set() will set the channel completely
// on/off (ie. 100% duty cycle).
//
// NOTE: chan_get() will return an error if the channel
// is in PWM mode (ie not fully on or off).
//
// Returns true on success or false if an error occured.
bool mgos_pca9685_chan_write(struct mgos_pca9685 *dev, uint8_t chan, uint16_t on, uint16_t off);
bool mgos_pca9685_chan_read(struct mgos_pca9685 *dev, uint8_t chan, uint16_t *on, uint16_t *off);
bool mgos_pca9685_chan_set(struct mgos_pca9685 *dev, uint8_t chan, bool state);
bool mgos_pca9685_chan_get(struct mgos_pca9685 *dev, uint8_t chan, bool *state);

// --- Low level API
// Get or set inversion for all channels.
// Setting 'invert' to true inverts the logic state on all output channels.
// Returns true on success or false if an error occured.
bool mgos_pca9685_mode_set_invert(struct mgos_pca9685 *dev, bool invert);
bool mgos_pca9685_mode_get_invert(struct mgos_pca9685 *dev, bool *invert);

// Get or set output drive mode for all channels.
// Setting 'totem' to true configures the output drivers with a totem pole
// structure, and false configures them as open-drain outputs.
// Returns true on success or false if an error occured.
bool mgos_pca9685_mode_set_output_drive(struct mgos_pca9685 *dev, bool totem);
bool mgos_pca9685_mode_get_output_drive(struct mgos_pca9685 *dev, bool *totem);

// Get or set PWM output change event.
// Setting 'och' to true changes outputs on the I2C STOP bit, and setting
// to false changes the outputs upon I2C ACK.
// Returns true on success or false if an error occured.
bool mgos_pca9685_mode_set_och(struct mgos_pca9685 *dev, bool och);
bool mgos_pca9685_mode_get_och(struct mgos_pca9685 *dev, bool *och);

// Reset the chip to power-on state.
// Returns true on success or false if an error occured.
bool mgos_pca9685_reset(struct mgos_pca9685 *dev);

// Get or set the PWM frequency (in Hertz)
// The input 'freq' has to be between [24,1526] Hertz inclusive and will
// clamp to the next available bracket (there are 253 possible values). See
// the datasheet or the code for an explanation.
// Returns true on success or false if an error occured.
bool mgos_pca9685_pwm_frequency_set(struct mgos_pca9685 *dev, uint16_t freq);
bool mgos_pca9685_pwm_frequency_get(struct mgos_pca9685 *dev, uint16_t *freq);

// Clean up the driver and return memory used for it.
bool mgos_pca9685_destroy(struct mgos_pca9685 **dev);

#ifdef __cplusplus
}
#endif
