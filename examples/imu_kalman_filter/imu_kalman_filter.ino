#include <Wire.h>

// MPU6050 I2C Address
const uint8_t MPU6050_ADDRESS = 0x68;

// Calibration constants
const int CALIBRATION_SAMPLES = 2000;
float RateCalibrationPitch, RateCalibrationYaw;

// Sensor data variables
float RatePitch, RateYaw;
float AccX, AccY, AccZ;
float AnglePitch;

// Kalman filter variables
float KalmanAnglePitch = 0, KalmanUncertaintyAnglePitch = 2 * 2;
float Kalman1DOutput[2] = {0, 0};

// Timer variables
uint32_t LoopTimer;

// Function prototypes
void initializeMPU6050();
void calibrateGyro();
void readSensorData();
void applyKalmanFilter();
void printAngles();

void setup()
{
    Serial.begin(57600);
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    Wire.setClock(400000);
    Wire.begin();
    delay(250);
    initializeMPU6050();
    calibrateGyro();
    LoopTimer = micros();
}

void loop()
{
    readSensorData();
    applyKalmanFilter();
    printAngles();
    while (micros() - LoopTimer < 4000)
        ;
    LoopTimer = micros();
}

void initializeMPU6050()
{
    // Power on MPU6050
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission();
}

void calibrateGyro()
{
    for (int RateCalibrationNumber = 0; RateCalibrationNumber < CALIBRATION_SAMPLES; RateCalibrationNumber++)
    {
        readSensorData();
        RateCalibrationPitch += RatePitch;
        RateCalibrationYaw += RateYaw;
        delay(1);
    }
    RateCalibrationPitch /= CALIBRATION_SAMPLES;
    RateCalibrationYaw /= CALIBRATION_SAMPLES;
}

void readSensorData()
{
    // Read accelerometer and gyroscope data
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x3B);
    Wire.endTransmission();
    Wire.requestFrom(MPU6050_ADDRESS, 14);
    int16_t AccXLSB = Wire.read() << 8 | Wire.read();
    int16_t AccYLSB = Wire.read() << 8 | Wire.read();
    int16_t AccZLSB = Wire.read() << 8 | Wire.read();
    int16_t GyroX = Wire.read() << 8 | Wire.read();
    int16_t GyroY = Wire.read() << 8 | Wire.read();
    int16_t GyroZ = Wire.read() << 8 | Wire.read();

    // Convert raw data to meaningful units
    RatePitch = (float)GyroY / 65.5;
    RateYaw = (float)GyroZ / 65.5;
    AccX = (float)AccXLSB / 4096;
    AccY = (float)AccYLSB / 4096;
    AccZ = (float)AccZLSB / 4096;
    AnglePitch = atan2(AccZ, AccX) * 1 / (3.142 / 180);
}

void applyKalmanFilter()
{
    kalman_1d(KalmanAnglePitch, KalmanUncertaintyAnglePitch, RatePitch, AnglePitch);
    KalmanAnglePitch = Kalman1DOutput[0];
    KalmanUncertaintyAnglePitch = Kalman1DOutput[1];
}

void kalman_1d(float KalmanState, float KalmanUncertainty, float KalmanInput, float KalmanMeasurement)
{
    KalmanState = KalmanState + 0.004 * KalmanInput;
    KalmanUncertainty = KalmanUncertainty + 0.004 * 0.004 * 4 * 4;
    float KalmanGain = KalmanUncertainty * 1 / (1 * KalmanUncertainty + 3 * 3);
    KalmanState = KalmanState + KalmanGain * (KalmanMeasurement - KalmanState);
    KalmanUncertainty = (1 - KalmanGain) * KalmanUncertainty;
    Kalman1DOutput[0] = KalmanState;
    Kalman1DOutput[1] = KalmanUncertainty;
}

void printAngles()
{
    Serial.print("Pitch Angle [°] ");
    Serial.println(KalmanAnglePitch);
}
