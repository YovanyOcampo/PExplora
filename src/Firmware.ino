#define RS485ControlIO A7
#define RS485Transmit HIGH
#define RS485Receive LOW
#define RS485Serial Serial2

#define interruptPin 20

volatile unsigned long tiempo = 0;
volatile unsigned long retardo = 0;

int byteIO;

void setup() {
  attachInterrupt(digitalPinToInterrupt(interruptPin), interruption, FALLING);
  // initialize both serial ports:
  Serial.begin(9600);
  Serial.write("Communication ready");
  Serial.write("Detection ready");
  RS485Serial.begin(9600);
  pinMode(RS485ControlIO, OUTPUT);
  digitalWrite(RS485ControlIO, RS485Receive);
}

void loop() {
  if(Serial.available()) {
    byteIO = Serial.read();
    digitalWrite(RS485ControlIO, RS485Transmit);
    RS485Serial.write(byteIO);
    delay(10);
    digitalWrite(RS485ControlIO, RS485Receive);
  }
  if(RS485Serial.available()) {
    byteIO = RS485Serial.read();
    Serial.write(byteIO);
    delay(10);
  }
}

void interruption() {
  retardo = millis() - tiempo;
  if (retardo > 120 || retardo < 40) {
    Serial.write("Detected");
    digitalWrite(RS485ControlIO, RS485Transmit);
    RS485Serial.write("D");
    delay(10);
    digitalWrite(RS485ControlIO, RS485Receive);
  }
  tiempo = millis();
}
