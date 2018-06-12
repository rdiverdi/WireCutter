#include <Arduino.h>

class DCmotor{

  public:

  DCmotor(int pwm_pin, int dir_pin){
    _pwm_pin = pwm_pin;
    _dir_pin = dir_pin;
    pinMode(_pwm_pin, OUTPUT);
    pinMode(_dir_pin, OUTPUT);
    _last_time = micros();
  }

  void setSpeed(float speed){
    _req_speed = lim(speed, -100, 100);
  }
  void update(){
    // input floats from -100 to 100
    // sets speed of motor
    _dt = micros() - _last_time;
    _last_time = micros();
    float max_dv = max_accel*_dt/1000000.;
    _speed = lim(_req_speed, _speed-max_dv, _speed+max_dv);
    bool dir = HIGH;
    if (_speed>0){
      dir = LOW;
    }
    digitalWrite(_dir_pin, dir);
    int pwm = 0;
    if (abs(_speed)>0.01){
      pwm = map(abs(_speed), 0, 100, deadzone, 255);
    }
    //if (dir) analogWrite(_pwm_pin, 255-pwm);
    //else analogWrite(_pwm_pin, pwm);
    analogWrite(_pwm_pin, pwm);
  }

  float P=0;
  float I=0;
  float D=0;

  int deadzone=0;
  int max_accel=1000;

  long int accum=0;
  long int last_err=0;

  long int MotorControl(long int target, long int current){
    long int err = target-current;
    if (I!=0){
      accum += err;
    }
    float deriv = 0;
    _now = micros();
    _control_dt = _now-_control_last_time;
    if (_control_last_time > 0 && _control_dt<50000){
      deriv = (last_err - err)*1000000./_control_dt;
    }
    _control_last_time = _now;
    float speed = (P*err + I*accum - D*deriv);
    delayMicroseconds(500);
    last_err = err;
    setSpeed(speed);
    return err;
  }

  float Speed(){
    return _speed;
  }

  void MotorControlReset(){
    accum=0;
    last_err=32767;
  }

  float lim(float n, float min, float max){
    if (n<min) return min;
    if (n>max) return max;
    return n;
  }

  private:

  int _pwm_pin;
  int _dir_pin;
  long _last_time;
  long _now;
  long _control_last_time;
  long _control_dt;
  long _dt;
  float _speed = 0;
  float _req_speed = 0;

};
