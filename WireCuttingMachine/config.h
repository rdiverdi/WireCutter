#define ENCpin1 2
#define ENCpin2 3
#define ENCreleasePin 10

#define M1dirPin 4
#define M1pwmPin 5

#define M2dirPin 7
#define M2pwmPin 6

#define BRAKE_PIN 8
#define BRAKE_TIME 50 // time to hold solenoid open (ms)

#define KICKER_SERVO_PIN 9
#define KICKER_POS_CLOSED 1500
#define KICKER_POS_OPEN 1800
#define KICKER_SERVO_TIME 300 // time for servo to reach position (ms)

#define CUT_MOTOR_PIN 11
#define CUT_DONE_PIN 12

#define TICKS_THRESHOLD 2

#define M1_P 0.02
#define M1_D 0.0005
#define M1_deadzone 65
#define M1_MAX_ACCEL 100

#define M2_P 0.3
#define M2_D 0
#define M2_deadzone 0
#define M2_MAX_ACCEL 100

void pin_setup(){
  pinMode(ENCpin1, INPUT);
  pinMode(ENCpin2, INPUT);
  pinMode(ENCreleasePin, INPUT_PULLUP);
  pinMode(M1dirPin, OUTPUT);
  pinMode(M1pwmPin, OUTPUT);
  pinMode(M2dirPin, OUTPUT);
  pinMode(M2pwmPin, OUTPUT);
  pinMode(BRAKE_PIN, OUTPUT);
  pinMode(KICKER_SERVO_PIN, OUTPUT);
  pinMode(CUT_MOTOR_PIN, OUTPUT);
  pinMode(CUT_DONE_PIN, INPUT_PULLUP);
}
