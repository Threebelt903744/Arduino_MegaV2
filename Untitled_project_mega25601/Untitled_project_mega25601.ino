// pins for the 3 encoders
const int enc1_pinA = 2; 
const int enc1_pinB = 3; 

const int enc2_pinA = 18;
const int enc2_pinB = 19;

const int enc3_pinA = 20;
const int enc3_pinB = 21;

// variables to store position
// using volatile because they update in the background interrupts
volatile long enc1_pos = 0;
volatile long enc2_pos = 0;
volatile long enc3_pos = 0;

void setup() {
  Serial.begin(115200);
  
  // set pins as input with internal pullup
  pinMode(enc1_pinA, INPUT_PULLUP);
  pinMode(enc1_pinB, INPUT_PULLUP);
  
  pinMode(enc2_pinA, INPUT_PULLUP);
  pinMode(enc2_pinB, INPUT_PULLUP);
  
  pinMode(enc3_pinA, INPUT_PULLUP);
  pinMode(enc3_pinB, INPUT_PULLUP);

  // attach interrupts to read pins when they change
  attachInterrupt(digitalPinToInterrupt(enc1_pinA), isr_enc1A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc1_pinB), isr_enc1B, CHANGE);
  
  attachInterrupt(digitalPinToInterrupt(enc2_pinA), isr_enc2A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc2_pinB), isr_enc2B, CHANGE);

  attachInterrupt(digitalPinToInterrupt(enc3_pinA), isr_enc3A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc3_pinB), isr_enc3B, CHANGE);

  Serial.println("System started");
}

void loop() {
  // pause interrupts for a moment to read data safely
  noInterrupts();
  long pos1 = enc1_pos;
  long pos2 = enc2_pos;
  long pos3 = enc3_pos;
  interrupts();

  // print values to the screen
  Serial.print("E1: "); Serial.print(pos1);
  Serial.print(" | E2: "); Serial.print(pos2);
  Serial.print(" | E3: "); Serial.println(pos3);
  
  delay(100); 
}

// background functions for encoder 1
// they check the direction by comparing pin A and pin B
void isr_enc1A() {
  if (digitalRead(enc1_pinA) == digitalRead(enc1_pinB)) {
    enc1_pos++;
  } else {
    enc1_pos--;
  }
}
void isr_enc1B() {
  if (digitalRead(enc1_pinA) != digitalRead(enc1_pinB)) {
    enc1_pos++;
  } else {
    enc1_pos--;
  }
}

// background functions for encoder 2
void isr_enc2A() {
  if (digitalRead(enc2_pinA) == digitalRead(enc2_pinB)) {
    enc2_pos++;
  } else {
    enc2_pos--;
  }
}
void isr_enc2B() {
  if (digitalRead(enc2_pinA) != digitalRead(enc2_pinB)) {
    enc2_pos++;
  } else {
    enc2_pos--;
  }
}

// background functions for encoder 3
void isr_enc3A() {
  if (digitalRead(enc3_pinA) == digitalRead(enc3_pinB)) {
    enc3_pos++;
  } else {
    enc3_pos--;
  }
}
void isr_enc3B() {
  if (digitalRead(enc3_pinA) != digitalRead(enc3_pinB)) {
    enc3_pos++;
  } else {
    enc3_pos--;
  }
}