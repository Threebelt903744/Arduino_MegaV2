// pins for the 3 encoders (roll, pitch, yaw)
const int rollPinA = 2;
const int rollPinB = 3;

const int pitchPinA = 18;
const int pitchPinB = 19;

const int yawPinA = 20;
const int yawPinB = 21;

// 0 = roll, 1 = pitch, 2 = yaw
// volatile because they change inside interrupts
volatile long count[3] = {0, 0, 0};
volatile int prev[3] = {0, 0, 0};

void setup() {
  Serial.begin(115200);

  // set pins as input with pullup
  pinMode(rollPinA, INPUT_PULLUP);
  pinMode(rollPinB, INPUT_PULLUP);
  pinMode(pitchPinA, INPUT_PULLUP);
  pinMode(pitchPinB, INPUT_PULLUP);
  pinMode(yawPinA, INPUT_PULLUP);
  pinMode(yawPinB, INPUT_PULLUP);

  // read starting values of the pins
  prev[0] = (digitalRead(rollPinA) << 1) | digitalRead(rollPinB);
  prev[1] = (digitalRead(pitchPinA) << 1) | digitalRead(pitchPinB);
  prev[2] = (digitalRead(yawPinA) << 1) | digitalRead(yawPinB);

  // attach interrupts on all 6 pins
  attachInterrupt(digitalPinToInterrupt(rollPinA), isrRoll, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rollPinB), isrRoll, CHANGE);

  attachInterrupt(digitalPinToInterrupt(pitchPinA), isrPitch, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pitchPinB), isrPitch, CHANGE);

  attachInterrupt(digitalPinToInterrupt(yawPinA), isrYaw, CHANGE);
  attachInterrupt(digitalPinToInterrupt(yawPinB), isrYaw, CHANGE);
}

void loop() {
  long liveCount[3];

  // stop interrupts for a bit so readings dont get messed up
  noInterrupts();
  liveCount[0] = count[0];
  liveCount[1] = count[1];
  liveCount[2] = count[2];
  interrupts();

  // convert count to angle
  float rollAngle  = liveCount[0] * 0.15;
  float pitchAngle = liveCount[1] * 0.15;
  float yawAngle   = liveCount[2] * 0.15;

  // print angles
  Serial.print("Roll: ");  Serial.print(rollAngle);
  Serial.print(" | Pitch: "); Serial.print(pitchAngle);
  Serial.print(" | Yaw: ");   Serial.println(yawAngle);

  delay(50);
}

// check direction using prev and live states
void updateEncoder(int index, int pinA, int pinB) {
  int live = (digitalRead(pinA) << 1) | digitalRead(pinB);
  int p = prev[index];

  // clockwise (00 -> 01 -> 11 -> 10)
  if (p == 0b00 && live == 0b01)      { count[index]++; }
  else if (p == 0b01 && live == 0b11) { count[index]++; }
  else if (p == 0b11 && live == 0b10) { count[index]++; }
  else if (p == 0b10 && live == 0b00) { count[index]++; }

  // anticlockwise (10 -> 11 -> 01 -> 00)
  else if (p == 0b00 && live == 0b10) { count[index]--; }
  else if (p == 0b10 && live == 0b11) { count[index]--; }
  else if (p == 0b11 && live == 0b01) { count[index]--; }
  else if (p == 0b01 && live == 0b00) { count[index]--; }

  // same state or noise jump, keep count same
  else {
    count[index] = count[index];
  }

  // save current state as prev for next time
  prev[index] = live;
}

// interrupt functions
void isrRoll()  { updateEncoder(0, rollPinA, rollPinB); }
void isrPitch() { updateEncoder(1, pitchPinA, pitchPinB); }
void isrYaw()   { updateEncoder(2, yawPinA, yawPinB); }