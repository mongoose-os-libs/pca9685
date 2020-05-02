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

#include "mgos_pca9685_internal.h"

void mgos_pca9685_default_cfg(struct mgos_pca9685_config *cfg) {
  if (!cfg) return;

  memset(cfg, 0, sizeof(*cfg));

  cfg->i2c = mgos_i2c_get_global();
  cfg->i2c_addr = MGOS_PCA9685_I2C_ADDR;
}

struct mgos_pca9685 *mgos_pca9685_create(const struct mgos_pca9685_config *cfg) {
  struct mgos_pca9685 *dev;

  if (!(dev = calloc(1, sizeof(*dev)))) return NULL;
  if (cfg)
    memcpy((void *) &dev->config, cfg, sizeof(*cfg));
  else
    mgos_pca9685_default_cfg(&dev->config);

  return dev;
}

bool mgos_pca9685_destroy(struct mgos_pca9685 **dev) {
  if (!*dev) return false;
  free(*dev);
  *dev = NULL;
  return true;
}

bool mgos_pca9685_init(void) {
  return true;
}
