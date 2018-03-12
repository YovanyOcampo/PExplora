#define RS485ControlIO A7
#define RS485Transmit HIGH
#define RS485Receive LOW
#define RS485Serial Serial2
#define interruptPin 21
#define triggerTransmitter A3

volatile unsigned long timeFlaks = 0;
volatile unsigned long timeFlanksDelay = 0;
volatile unsigned long timeTrigger = 0;
volatile unsigned long timeInterruptControl = 0;

int byteIO = 0x0000;

bool detectedFlag = false;

volatile byte triggerState = LOW;

void setup() {
  attachInterrupt(digitalPinToInterrupt(interruptPin), interruption, FALLING);
  // initialize both serial ports:
  Serial.begin(9600);
  Serial.write("Communication ready");
  Serial.write("Detection ready");
  RS485Serial.begin(9600);
  pinMode(RS485ControlIO, OUTPUT);
  digitalWrite(RS485ControlIO, RS485Receive);
  timeTrigger = millis();
}

void loop() {
  if (Serial.available()) {
    byteIO = Serial.read();
    RS485Write(byteIO);
  }
  if (RS485Serial.available()) {
    byteIO = RS485Serial.read();
    Serial.write(byteIO);
  }

  if (detectedFlag == true) {
    detectedFlag = false;
    if (millis() > ( timeInterruptControl+ 1000)) {
      timeInterruptControl = millis();
      Serial.println("DETECTADO!");
    }
  }

  if ((millis() - timeTrigger) >= 50) {
    triggerState = !triggerState;
    digitalWrite(triggerTransmitter, triggerState);
    timeTrigger = millis();
  }
}

void interruption() {
  timeFlanksDelay = millis() - timeFlaks;
  if (timeFlanksDelay > 120 || timeFlanksDelay < 40) {
    detectedFlag = true;
  }
  timeFlaks = millis();
}

void RS485Write (int data) {
  digitalWrite(RS485ControlIO, RS485Transmit);
  delay(10);
  RS485Serial.write(data);
  digitalWrite(RS485ControlIO, RS485Receive);
}
