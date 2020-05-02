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

// Clean up the driver and return memory used for it.
bool mgos_pca9685_destroy(struct mgos_pca9685 **dev);

#ifdef __cplusplus
}
#endif
