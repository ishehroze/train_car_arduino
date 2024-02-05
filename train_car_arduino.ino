// C++ code
//
#include <Servo.h>

const int CAR_TRIGGER_PIN = 13;
const int CAR_ECHO_PIN = 12;
const int RACK_3_PIN = 9;

const int TRAIN_TRIGGER_PIN = 7;
const int TRAIN_ECHO_PIN = 8;
const int RAIL_BAR_1_PIN = 11;
const int RAIL_BAR_2_PIN = 10;

const int RAIL_BAR_RAISE_ANGLE = 90;
const int RAIL_BAR_LOWER_ANGLE = 0;

const int RACKER_RAISE_ANGLE = 75;
const int RACKER_LOWER_ANGLE = 0;

long delay_ratio = 1/3;

const int WITHIN_RANGE = 1;
const int OUT_OF_RANGE = 0;

int train_status = OUT_OF_RANGE;
int transit_start = 0;
int transit_end = 0;

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
  long distance = 0.01723 * readUltrasonicDistance(TRAIN_TRIGGER_PIN, TRAIN_ECHO_PIN);
  Serial.print("Train: ");
  Serial.print(distance);
  Serial.print("; ");
  return distance;
}

long getCarDistance()
{
  long distance = 0.01723 * readUltrasonicDistance(CAR_TRIGGER_PIN, CAR_ECHO_PIN);
  Serial.print("Car: ");
  Serial.print(distance);
  Serial.print("; ");
  return distance;
}

void raise_rail_bars(Servo bar1, Servo bar2) {
  bar1.write(RAIL_BAR_RAISE_ANGLE);
  bar2.write(RAIL_BAR_RAISE_ANGLE);

  Serial.print("Rail bars raised; ");
}

void lower_rail_bars(Servo bar1, Servo bar2) {
  bar1.write(RAIL_BAR_LOWER_ANGLE);
  bar2.write(RAIL_BAR_LOWER_ANGLE);
}

void raise_racker(Servo racker) {
  racker.write(RACKER_RAISE_ANGLE);

  Serial.print("Racker raised; ");
}

void lower_racker(Servo racker) {
  racker.write(RACKER_LOWER_ANGLE);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("serial test 0021");

  rail_bar_1.attach(RAIL_BAR_1_PIN, 500, 2500);
  rail_bar_2.attach(RAIL_BAR_2_PIN, 500, 2500);
  rack_3.attach(RACK_3_PIN, 500, 2500);
}

void loop()
{
  TrainDistance = getTrainDistance();

  if (TrainDistance < 5) {
    switch (train_status) {
      case OUT_OF_RANGE:
        train_status = WITHIN_RANGE;
        transit_start = millis();
        raise_rail_bars(rail_bar_1, rail_bar_2);
        break;
      case WITHIN_RANGE:
        break;
    }
  } else {
    switch (train_status) {
      case OUT_OF_RANGE:
        if ((millis() - transit_end) > (transit_end - transit_start) * delay_ratio) {
          lower_rail_bars(rail_bar_1, rail_bar_2);
        }
        break;
      case WITHIN_RANGE:
        transit_end = millis() - transit_start;
        train_status = OUT_OF_RANGE;
        break;
    }
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
