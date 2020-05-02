# Mongoose OS driver for PCA9685

This is a driver for [PCA9685](docs/PCA9685.pdf).

## Device details


The PCA9685 is an I2C-bus controlled 16-channel LED controller optimized for
Red/Green/Blue/Amber (RGBA) color backlighting applications. Each LED output has its
own 12-bit resolution (4096 steps) fixed frequency individual PWM controller that operates
at a programmable frequency from a typical of 24 Hz to 1526 Hz with a duty cycle that is
adjustable from 0 % to 100 % to allow the LED to be set to a specific brightness value.
All outputs are set to the same PWM frequency.

## Example application

```
#include "mgos.h"
#include "mgos_pca9685.h"

enum mgos_app_init_result mgos_app_init(void) {
  struct mgos_pca9685 *s_pwm;
  uint16_t freq;

  if (!(s_pwm = mgos_pca9685_create(NULL))) {
    LOG(LL_ERROR, ("Could not create PCA9685"));
    return false;
  }
  for (int i=0; i<4; i++)
    mgos_pca9685_chan_write(s_pwm, i, 1024*i, 1024*(i+1)-1);
  mgos_pca9685_chan_write(s_pwm, 4, 3072, 1023);
  mgos_pca9685_chan_write(s_pwm, 5, 4090, 10);
  mgos_pca9685_chan_write(s_pwm, 6, 0, 1);
  mgos_pca9685_chan_set(s_pwm, 7, true);

  return MGOS_APP_INIT_SUCCESS;
}
```
