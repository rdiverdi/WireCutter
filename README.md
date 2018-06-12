# Wire Cutting Machine

The two folders contain code for each of the Arduinos in the wire cutting machine. The acting Arduino runs all of the motors and has a fast response time to get wire lengths as accurate as possible. It takes commands and sends responses over the USB interface (or digital pins 0 and 1). The control Arduino interprets all of the buttons and controls the display. It communicates with the first arduino through its USB/pins 0 and 1.

## Libraries

The code uses the built in `Encoder` and `Wire` libraries along with Adafruit's `Adafruit_LiquidCrystal` library. This can be installed through the arduino IDE by going to `Sketch > Include Library > Manage Libraries` and searching for it.

## Wiring

For the most part, the wiring is just defined in the `config.h` for each of the Arduinos. Pins 0 and 1 for both arduinos are used for communication. Pins 2 and 3 are the only pins with hardware interrupts, so they should be used for encoders. Pins A4 and A5 are I2C pins, so they cannot be used for anything else on the control arduino. A number of pins have PWM capabilities (check arduino documentation). These pins should be used for the speed control pins for motors or for LEDs. In general, analog pins can be used as digital pins.

## Communication

All of the communication between Arduinos is sent as strings over USB/serial. The strings are formatted as `<type>,<value>` or just `<type>`. The full set of commands is coded in `communication.h`, but here's the list at the time of writing:

- Commands to active Arduino:
  - `START` - start cutting wire
  - `STOP` - stop everything and return to manual control
  - `ZERO_ENC` - zero the encoder at current position
  - `ENC_VAL` - get current encoder value
  - `TOGGLE_KICK` - manually toggle kicker motor
  - `SET_LEN,<length in ticks>` - set desired wire length in encoder ticks
  - `SET_KICK_SPEED,<speed 0:100>` - set the kicker motor to a speed
  - `SET_SPOOL_SPEED,<speed 0:100>` - set the spool motor to a speed
  - `USE_SPOOL,<0 or 1>` - whether or not to spool wire (1 is yes, 0 is no)
  - `MOVE_TO,<length in ticks>` - move to a a length
  - `MOVE_BY,<length in ticks>` - change target by a number of ticks
  - `SET_NUM_WIRES,<nuber>` - set how many wires to cut (default 1)


## `config.h`

### active arduino

Most of the settings are pin numbers, which I'm going to skip.

- `BRAKE_TIME` is the time (in milliseconds) that the solenoid is held open to release the break.
- `KICKER_POS_...` are the PWM values for the servo to engage or disengage the kicker motor.
- `KICKER_SERVO_TIME` is the time (ms) to wait for the servo to reach its position.
- `TICKS_THRESHOLD` is the acceptable margin of error for the wire length in encoder ticks.

Motor Parameters (M1 is kicker motor, M2 is spool motor):

- `M*_P` is the proportional term for the control loop
- `M*_D` is the derivative term for the control loop
- `M*_deadzone` is the deadzone of the motor (scale 0 to 255)
- `M*_MAX_ACCEL` is the maximum accelration of the motor in % per second.

### control arduino

- `TICKS_PER_REV` - encoder ticks per revolution
- `ENC_WHEEL_R` - radius of encoder wheel
- `SPOOL_CUTOFF_LEN` - under this length, default to not using the spool
