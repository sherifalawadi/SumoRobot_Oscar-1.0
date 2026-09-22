// ==============================================================
// Sumo Robot Firmware - L298N + 3 Sonars + 3 Rear Line Sensors
// ==============================================================

// ------------------------
// Motor Pins (L298N)
// ------------------------
#define ENA 3
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7
#define ENB 11

// ------------------------
// Ultrasonic Pins (HC-SR04)
// ------------------------
#define TRIG_FRONT 2
#define ECHO_FRONT 8
#define TRIG_RIGHT A5
#define ECHO_RIGHT 12
#define TRIG_LEFT  A0
#define ECHO_LEFT  A1

// ------------------------
// Rear Digital Line Sensors
// ------------------------
#define BACK_LINE        A2
#define BACK_LEFT_LINE   A3
#define BACK_RIGHT_LINE  A4

// TEST THIS FIRST: place bot on ring, power on, watch what happens
// at the white border. If it drives OFF the ring instead of away from
// it, flip this from LOW to HIGH.
#define LINE_DETECTED LOW

// ------------------------
// Detection thresholds
// ------------------------
const int kAttackDistanceCm = 35;        // start charging within this range
const int kSearchDistanceCm = 100;       // can "see" opponent out to here while scanning
const unsigned long US_TIMEOUT = 6000;   // ~100cm max range (was 2200 = only 37cm, too blind)

// ------------------------
// Timing
// ------------------------
unsigned long lastDecisionMs = 0;
const unsigned long kDecisionPeriodMs = 20;

// ------------------------
// Motor Control
// ------------------------
void moveForward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveBackward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ------------------------
// Sensor Helpers
// ------------------------
int readDistanceCm(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, US_TIMEOUT);
  if (duration == 0) {
    return 999; // no echo = nothing in range
  }

  int centimeters = (int)(duration * 0.034 / 2);
  if (centimeters <= 0 || centimeters > kSearchDistanceCm) {
    return 999;
  }
  return centimeters;
}

// Rear edge avoidance: drives forward away from the border
void edgeAvoidManeuver(bool sawCenter, bool sawLeft, bool sawRight) {
  if (sawLeft && !sawRight) {
    turnRight();
  } else if (sawRight && !sawLeft) {
    turnLeft();
  } else {
    moveForward();
  }
  delay(250);
}

// ------------------------
// Setup
// ------------------------
void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);
  pinMode(TRIG_LEFT,  OUTPUT);
  pinMode(ECHO_LEFT,  INPUT);

  pinMode(BACK_LINE, INPUT);
  pinMode(BACK_LEFT_LINE, INPUT);
  pinMode(BACK_RIGHT_LINE, INPUT);

  stopMotors();
  delay(5000); // mandatory match start delay
}

// ------------------------
// Main Loop
// ------------------------
void loop() {
  unsigned long now = millis();
  if (now - lastDecisionMs < kDecisionPeriodMs) {
    return;
  }
  lastDecisionMs = now;

  // 1) Rear line safety check — highest priority
  bool backC = (digitalRead(BACK_LINE)       == LINE_DETECTED);
  bool backL = (digitalRead(BACK_LEFT_LINE)  == LINE_DETECTED);
  bool backR = (digitalRead(BACK_RIGHT_LINE) == LINE_DETECTED);

  if (backC || backL || backR) {
    edgeAvoidManeuver(backC, backL, backR);
    return;
  }

  // 2) Scan for opponent
  int distFront = readDistanceCm(TRIG_FRONT, ECHO_FRONT);
  delayMicroseconds(200);
  int distLeft  = readDistanceCm(TRIG_LEFT, ECHO_LEFT);
  delayMicroseconds(200);
  int distRight = readDistanceCm(TRIG_RIGHT, ECHO_RIGHT);

  // 3) Attack if in close range
  if (distFront > 0 && distFront <= kAttackDistanceCm) {
    moveForward();
  }
  else if (distLeft > 0 && distLeft <= kAttackDistanceCm) {
    turnLeft();
    delay(60);
  }
  else if (distRight > 0 && distRight <= kAttackDistanceCm) {
    turnRight();
    delay(60);
  }
  // 4) Not close, but visible somewhere -> turn toward it before it's in range
  else if (distFront < kSearchDistanceCm) {
    moveForward();
  }
  else if (distLeft < kSearchDistanceCm && distLeft <= distRight) {
    turnLeft();
  }
  else if (distRight < kSearchDistanceCm && distRight < distLeft) {
    turnRight();
  }
  else {
    // nothing visible anywhere -> spin to search
    turnRight();
  }
}
