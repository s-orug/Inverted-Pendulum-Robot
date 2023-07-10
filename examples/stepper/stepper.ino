// Pins Mapping
//
// LEFT STEPPER:
//      Direction -> 0
//      Step -> 1
// RIGHT STEPPER:
//      Direction -> 2
//      Step -> 3

#define LMotorDir 0
#define LMotorStep 1

#define RMotorDir 2
#define RMotorStep 3

#define MIN_VELOCITY_THRESHOLD 1e-6
#define MAX_STEP_DELAY 1e6
#define STEP_PULSE_WIDTH 1e-6
#define MAX_RPM 60

class Stepper
{
private:
    int _dir_pin, _step_pin, _ppr;
    int position = 0;
    int dx = 1;
    int _direction = 0;
    float step_delay = 100;
    float last_step_ts = 0;
    float _rps = 0, _rpm = 0;

public:
    int CW = 1;
    int CCW = 0;
    Stepper(int dir_pin, int step_pin, int ppr)
    {
        pinMode(dir_pin, OUTPUT);
        pinMode(step_pin, OUTPUT);
        _dir_pin = dir_pin;
        _step_pin = step_pin;
        _ppr = ppr;
    }

    void set_direction(int direction)
    {
        _direction = direction;
        digitalWrite(_dir_pin, _direction);
        if (_direction == CW)
        {
            dx = CW;
        }
        else
        {
            dx = CCW;
        }
    }

    void step()
    {
        digitalWrite(_step_pin, HIGH);
        delay(STEP_PULSE_WIDTH);
        digitalWrite(_step_pin, LOW);
        position += dx;
    }

    void set_velocity(double velocity)
    {
        if (std::abs(velocity) < MIN_VELOCITY_THRESHOLD)
        {
            step_delay = MAX_STEP_DELAY;
        }
        else
        {
            step_delay = (((2.0 * M_PI) / _ppr) / abs(velocity)) - (1e-6);
            set_direction(velocity > 0 ? CW : CCW);
        }
    }

    void set_velocity_rpm(float rpm)
    {
        if (std::abs(rpm) > MAX_RPM)
        {
            step_delay = step_delay;
        }
        else if (rpm < 0 && _direction != CCW)
        {
            _direction = CCW;
            digitalWrite(_dir_pin, _direction);
        }
        else
        {
            _direction = CW;
            digitalWrite(_dir_pin, _direction);
        }
        _rps = abs(rpm) / 60;
        step_delay = ((1 / _rps) / 360) * 1.8 / 4;
    }

    /*  void set_acceleration(float acceleration){
        if (acceleration = 0.0){
    return;
        if (acceleration < 0.0){
    acceleration = - acceleration;
    }
        if (_acceleration != acceleration){
    _n = _n * (_acceleration / acceleration);
    _c0 = 0.676 * sqrt(2.0 / acceleration) * 1000000.0;
    _acceleration = acceleration;
    computeNewSpeed();
      }
      }*/

    void loop()
    {
        float now = micros() / 1e6;
        if (now - last_step_ts >= step_delay)
        {
            step();
            last_step_ts = now;
        }
    }
};

void setup()
{
    pinMode(LMotorDir, OUTPUT);
    pinMode(LMotorStep, OUTPUT);
    pinMode(RMotorDir, OUTPUT);
    pinMode(RMotorStep, OUTPUT);
}

void loop()
{

    Stepper motor(LMotorDir, LMotorStep, 200);

    float rpm = 10;
    motor.set_direction(1);
    motor.set_velocity_rpm(rpm);

    while (true)
    {
        motor.loop();
        Serial.println(rpm);
    }
}
