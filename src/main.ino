// Pins Mapping
//
// LEFT STEPPER:
//      Direction -> 0
//      Step -> 1
// RIGHT STEPPER:
//      Direction -> 2
//      Step -> 3
// LED:
//       Green -> 17
//       Red -> 18
//
// VOLTAGE INPUT FOR LED -> A0

#define LMotorDir  0
#define LMotorStep 1

#define RMotorDir  2
#define RMotorStep 3

#define GLed       17
#define RLed       18

void setup() {
  pinMode(LMotorDir, OUTPUT);
  pinMode(LMotorStep, OUTPUT);
  pinMode(RMotorDir, OUTPUT);
  pinMode(RMotorStep, OUTPUT);
  pinMode(GLed, OUTPUT);
  pinMode(RLed, OUTPUT);
}

void checkVoltage(){
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (18.26 / 1023.0) - 0.03;
  Serial.println(voltage);
  if(voltage < 16 && voltage > 13.8){
    digitalWrite(17, LOW);
    digitalWrite(18, HIGH);
  } else {
    digitalWrite(17, HIGH);
    digitalWrite(18, LOW);
  }
}

void loop() {
  checkVoltage();
}
