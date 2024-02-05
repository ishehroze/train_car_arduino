// C++ code
//
#include <Servo.h>

int CarTriggerPin = 13;
int CarEchoPin = 12;
int rack_3_pin = 9;

int TrainTriggerPin = 7;
int TrainEchoPin = 8;
int rail_bar_1_pin = 11;
int rail_bar_2_pin = 10;

int rail_bar_raise_angle = 90;
int rail_bar_lower_angle = 0;

int racker_raise_angle = 75;
int racker_lower_angle = 0;

const int BAR_LOWERED = 0;
const int BAR_TRANSIT = 1;
const int BAR_RAISED = 2;

int CarDistance;
int TrainDistance;

Servo rail_bar_1;
Servo rail_bar_2;
Servo rack_3;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

long getTrainDistance()
{
  long distance = 0.01723 * readUltrasonicDistance(TrainTriggerPin, TrainEchoPin);
  Serial.print("Train: ");
  Serial.print(distance);
  Serial.print("; ");
  return distance;
}

long getCarDistance()
{
  long distance = 0.01723 * readUltrasonicDistance(CarTriggerPin, CarEchoPin);
  Serial.print("Car: ");
  Serial.print(distance);
  Serial.print("; ");
  return distance;
}

void raise_rail_bars(Servo bar1, Servo bar2) {
  bar1.write(rail_bar_raise_angle);
  bar2.write(rail_bar_raise_angle);

  Serial.print("Rail bars raised; ");
}

void lower_rail_bars(Servo bar1, Servo bar2) {
  bar1.write(rail_bar_lower_angle);
  bar2.write(rail_bar_lower_angle);
}

void raise_racker(Servo racker) {
  racker.write(racker_raise_angle);

  Serial.print("Racker raised; ");
}

void lower_racker(Servo racker) {
  racker.write(racker_lower_angle);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("serial test 0021");

  rail_bar_1.attach(rail_bar_1_pin, 500, 2500);
  rail_bar_2.attach(rail_bar_2_pin, 500, 2500);
  rack_3.attach(rack_3_pin, 500, 2500);
}

void loop()
{
  TrainDistance = getTrainDistance();

  if (TrainDistance < 5) {
    raise_rail_bars(rail_bar_1, rail_bar_2);
  } else {
    lower_rail_bars(rail_bar_1, rail_bar_2);
  }

  CarDistance = getCarDistance();

  if (CarDistance < 5) {
    raise_racker(rack_3);
  } else {
    lower_racker(rack_3);
  }

  Serial.println();

  delay(50); // Delay a little bit to improve simulation performance
}
