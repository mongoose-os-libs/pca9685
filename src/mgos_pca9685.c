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
  dev->osc_freq = 25e6;
  if (!mgos_pca9685_reset(dev)) {
    free(dev);
    return NULL;
  }
  // 7:RESTART 6:EXTCLK 5:AI 4:SLEEP 3:SUB1 2:SUB2 1:SUB3 0:ALLCALL
  mgos_i2c_write_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_MODE1, 0x20);

  // 765: RESV 4:INVRT 3:OCH 2:OUTDRV 10:OUTNE
  mgos_i2c_write_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_MODE2, 0x04);

  return dev;
}

bool mgos_pca9685_destroy(struct mgos_pca9685 **dev) {
  if (!*dev) return false;
  free(*dev);
  *dev = NULL;
  return true;
}

bool mgos_pca9685_chan_write(struct mgos_pca9685 *dev, uint8_t chan, uint16_t on, uint16_t off) {
  uint8_t val[4];
  if (!dev || chan > 15) return false;
  val[0] = on & 0xFF;
  val[1] = on >> 8;
  val[2] = off & 0xFF;
  val[3] = off >> 8;
  if (!mgos_i2c_write_reg_n(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_LED0_ON_L + 4 * chan, 4, val)) return false;
  return true;
}

bool mgos_pca9685_chan_read(struct mgos_pca9685 *dev, uint8_t chan, uint16_t *on, uint16_t *off) {
  uint8_t val[4];
  if (!dev || !on || !off || chan > 15) return false;
  if (!mgos_i2c_read_reg_n(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_LED0_ON_L + 4 * chan, 4, val)) return false;
  *on = val[0];
  *on |= (val[1] << 8);
  *off = val[2];
  *off |= (val[3] << 8);
  return true;
}

bool mgos_pca9685_chan_set(struct mgos_pca9685 *dev, uint8_t chan, bool state) {
  return mgos_pca9685_chan_write(dev, chan, state == true ? 4096 : 0, state == false ? 4096 : 0);
}

bool mgos_pca9685_chan_get(struct mgos_pca9685 *dev, uint8_t chan, bool *state) {
  uint16_t on, off;
  if (!mgos_pca9685_chan_read(dev, chan, &on, &off)) return false;
  LOG(LL_DEBUG, ("chan[%u]: on=%u off=%u", chan, on, off));
  if (off > 4095) {
    *state = true;
    return true;
  }
  if (on > 4095) {
    *state = false;
    return true;
  }
  return false;
}

bool mgos_pca9685_mode_set_invert(struct mgos_pca9685 *dev, bool invert) {
  if (!dev) return false;
  mgos_i2c_setbits_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_MODE2, 4, 1, invert ? 1 : 0);
  return true;
}

bool mgos_pca9685_mode_get_invert(struct mgos_pca9685 *dev, bool *invert) {
  uint8_t val;
  if (!dev || !invert) return false;
  mgos_i2c_getbits_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_MODE2, 4, 1, &val);
  *invert = val ? true : false;
  return true;
}

bool mgos_pca9685_mode_set_output_drive(struct mgos_pca9685 *dev, bool totem) {
  if (!dev) return false;
  mgos_i2c_setbits_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_MODE2, 2, 1, totem ? 1 : 0);
  return true;
}

bool mgos_pca9685_mode_get_output_drive(struct mgos_pca9685 *dev, bool *totem) {
  uint8_t val;
  if (!dev || !totem) return false;
  mgos_i2c_getbits_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_MODE2, 2, 1, &val);
  *totem = val ? true : false;
  return true;
}

bool mgos_pca9685_mode_set_och(struct mgos_pca9685 *dev, bool och) {
  if (!dev) return false;
  mgos_i2c_setbits_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_MODE2, 3, 1, och ? 1 : 0);
  return true;
}

bool mgos_pca9685_mode_get_och(struct mgos_pca9685 *dev, bool *och) {
  uint8_t val;
  if (!dev || !och) return false;
  mgos_i2c_getbits_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_MODE2, 2, 1, &val);
  *och = val ? true : false;
  return true;
}

bool mgos_pca9685_reset(struct mgos_pca9685 *dev) {
  if (!dev) return false;
  if (!mgos_i2c_write_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_MODE1, 0x80)) return false;
  usleep(5);
  return true;
}

bool mgos_pca9685_pwm_frequency_set(struct mgos_pca9685 *dev, uint16_t freq) {
  float prescale;
  uint8_t val;
  if (!dev) return false;
  if (freq < 24 || freq > 1526) return false;

  prescale = (dev->osc_freq / (freq * 4096.0)) - 1;
  val = (uint8_t) prescale;
  if (val < 3) val = 3;
  LOG(LL_DEBUG, ("Setting PWM frequency to %uHz (prescaler=%u)", freq, val));

  mgos_i2c_setbits_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_MODE1, 4, 1, 1);  // Sleep
  mgos_i2c_write_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_PRE_SCALE, val);
  mgos_i2c_setbits_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_MODE1, 4, 1, 0);  // Sleep
  return true;
}

bool mgos_pca9685_pwm_frequency_get(struct mgos_pca9685 *dev, uint16_t *freq) {
  int val;
  if (!dev || !freq) return false;
  val = mgos_i2c_read_reg_b(dev->config.i2c, dev->config.i2c_addr, MGOS_PCA9685_REG_PRE_SCALE);
  if (val < 0 || val > 255) return false;
  *freq = (uint16_t)(dev->osc_freq / ((val + 1) * 4096.0));
  return true;
}

bool mgos_pca9685_init(void) {
  return true;
}
