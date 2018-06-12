# Wire Cutting Machine

The two folders contain code for each of the Arduinos in the wire cutting machine. The acting Arduino runs all of the motors and has a fast response time to get wire lengths as accurate as possible. It takes commands and sends responses over the USB interface (or digital pins 0 and 1). The control Arduino interprets all of the buttons and controls the display. It communicates with the first arduino through its USB/pins 0 and 1.

## Using the machine

Generally, when using the machine, you need to set the length of wire you want to cut, set the initial zero point, attach the wire to the spool, and start the machine.

### Set Wire Length

To set the wire length, hit the small menu button until the set length screen appears, then press enter to edit the value. Type the value and press enter again to return to the original screen. The length value should be updated.

### Set Zero and Start

Position the end of the wire in the cutter jaws and press the `set zero` button to set the length (ensure that there is no slack between the cutter and the first set of rollers). If you are using the spool, now connect the wire to the spool and press start to automatically spool and cut the wire.

### The "Kicker" motor

The orange wheels can clamp onto the wire and push it through the machine. These are used for short wire lengths which will not be spooled, as well as for holding the wire after it is cut and positioning the wire for setting the initial zero point. These wheels engage or and disengage either with the `toggle` button or with the foot pedal. The fine positioning is controlled with the small black knob (note, pressing the knob runs the motor continuously, turning it moves it a certain distance). When using the spool, these wheels will release while the wire is wound up, then clamp down to hold the wire in place when it is cut. To attach the wire to the spool again, just press the foot pedal to release the wheels and pull the wire through to attach it.

### Other Buttons

- **STOP** - the stop button stops the machine wherever it is, but it does not lose its position, so you can stop in the middle of a length of wire, then press start again and the wire will be cut to the correct length.
- **CUT** - the cut button cuts the wire at the current location and sets the wire length to zero (you can use this to get a more accurate initial zero if you don't mind wasting a piece of wire).

### Other Settings

- **units** The default units are inches (shown as `(in)` next to the length). If you press the menu button until you find `Set Units`, then press enter, you can select from inches, feet, millimeters, centimeters, or meters by hitting the corresponding number. (menu is the only way out of this screen)

- **Use Spool** By default, the machine will use a spool for any wire longer than two feet, but you can change the current setting by pressing menu until you find `Use Spool` and pressing enter. (menu is the only way out of this screen)

### General Notes

- Every time it cuts, it resets the wire length, so you can just keep pressing start to cut a bunch of the same length wire.
- When you pull the lever to change the wire, it hits a switch to tell the machine that you will need to reset the zero position.

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
