#include <TinyGPS++.h>

// Serial Definitions
#define sim800l Serial2
#define gpsSerial Serial1
#define btSerial Serial3

// Pins
const int trigPinLeft = 2, echoPinLeft = 3;
const int trigPinCenter = 4, echoPinCenter = 5;
const int trigPinRight = 12, echoPinRight = 13;
const int leftBuzzer = 9, rightBuzzer = 10;
const int touchSensor = 8;

// System State
TinyGPSPlus gps;
bool obstacleDetectionEnabled = true;
bool findMyDeviceActive = false;
int mode = 0;
int volumeLevel = 1;
int volumeValues[3] = {10, 100, 255};

// Timing
unsigned long obstacleTimer = 0, lastBeepToggle = 0, touchPressTime = 0;
bool buzzState = false, touchHeld = false;
int buzzerMode = 0, beepInterval = 0;

// Safe Distances & Refresh
const unsigned long obstacleCheckInterval = 50;
const int safeDistanceCenter = 40;
const int safeDistanceIndoor = 100;
const int safeDistanceOutdoor = 200;

void setup() {
  Serial.begin(9600);
  sim800l.begin(9600);
  gpsSerial.begin(9600);
  btSerial.begin(9600);

  pinMode(trigPinLeft, OUTPUT); pinMode(echoPinLeft, INPUT);
  pinMode(trigPinCenter, OUTPUT); pinMode(echoPinCenter, INPUT);
  pinMode(trigPinRight, OUTPUT); pinMode(echoPinRight, INPUT);
  pinMode(leftBuzzer, OUTPUT); pinMode(rightBuzzer, OUTPUT);
  pinMode(touchSensor, INPUT);

  sim800l.println("AT+CMGF=1"); delay(500);

  Serial.println("System Ready");
}

void loop() {
  while (gpsSerial.available()) gps.encode(gpsSerial.read());
  handleTouch();
  checkBluetooth();
  updateObstacleDetection();
  handleBuzzer();
  checkIncomingSMS();
}

void handleTouch() {
  if (digitalRead(touchSensor) == HIGH) {
    if (!touchHeld) {
      touchHeld = true;
      touchPressTime = millis();
    } else if (millis() - touchPressTime >= 5000) {
      sendLocation(true);
      touchHeld = false;
    }
  } else if (touchHeld) {
    touchHeld = false;
    if (millis() - touchPressTime < 5000) {
      obstacleDetectionEnabled = !obstacleDetectionEnabled;
      Serial.println(obstacleDetectionEnabled ? "Obstacle Avoidance Enabled" : "Obstacle Avoidance Disabled");
    }
  }
}

void checkBluetooth() {
  if (btSerial.available()) {
    char c = btSerial.read();
    switch (c) {
      case '1': findMyDeviceActive = true; Serial.println("Find My Device initiated."); break;
      case '2': findMyDeviceActive = false; Serial.println("Find My Device stopped."); break;
      case 'S': sendLocation(true); break;
      case 'O': mode = 1; Serial.println("Outdoor mode activated."); break;
      case 'I': mode = 0; Serial.println("Indoor mode activated."); break;
      case '+':
        if (volumeLevel < 2) {
          volumeLevel++;
          Serial.println("Volume Increased.");
        } else {
          Serial.println("Volume is already at maximum!");
        }
        break;
      case '-':
        if (volumeLevel > 0) {
          volumeLevel--;
          Serial.println("Volume Decreased.");
        } else {
          Serial.println("Volume is already at minimum!");
        }
        break;
    }
  }
}

void updateObstacleDetection() {
  if (!obstacleDetectionEnabled || millis() - obstacleTimer < obstacleCheckInterval) return;
  obstacleTimer = millis();

  long dL = getDistance(trigPinLeft, echoPinLeft);
  long dC = getDistance(trigPinCenter, echoPinCenter);
  long dR = getDistance(trigPinRight, echoPinRight);

  auto mapInterval = [](long d, long maxDist) -> int {
    if (d > maxDist) return 0;
    return int(map(d, 0, maxDist, 800, 100));
  };

  buzzerMode = 0;

  if (dC < safeDistanceCenter) {
    beepInterval = 0;
    buzzerMode = 3;
  } else if (dL < (mode == 0 ? safeDistanceIndoor : safeDistanceOutdoor)) {
    beepInterval = mapInterval(dL, (mode == 0 ? safeDistanceIndoor : safeDistanceOutdoor));
    buzzerMode = 1;
  } else if (dR < (mode == 0 ? safeDistanceIndoor : safeDistanceOutdoor)) {
    beepInterval = mapInterval(dR, (mode == 0 ? safeDistanceIndoor : safeDistanceOutdoor));
    buzzerMode = 2;
  } else {
    beepInterval = 0;
    buzzerMode = 0;
  }

  // Debug print distances and intervals
  Serial.print("Left: " + String(dL) + " cm | Center: " + String(dC) + " cm | Right: " + String(dR) + " cm | ");
  Serial.print("Beep Interval: ");
  if (buzzerMode == 0) Serial.println("0 (silent)");
  else if (buzzerMode == 3) Serial.println("Continuous");
  else Serial.println(String(beepInterval) + " ms");
}

void handleBuzzer() {
  if (findMyDeviceActive) {
    analogWrite(leftBuzzer, volumeValues[volumeLevel]);
    analogWrite(rightBuzzer, volumeValues[volumeLevel]);
    return;
  }

  if (!obstacleDetectionEnabled || (beepInterval == 0 && buzzerMode != 3)) {
    analogWrite(leftBuzzer, 0);
    analogWrite(rightBuzzer, 0);
    return;
  }

  if (buzzerMode == 3) {
    analogWrite(leftBuzzer, volumeValues[volumeLevel]);
    analogWrite(rightBuzzer, volumeValues[volumeLevel]);
    return;
  }

  if (millis() - lastBeepToggle >= beepInterval) {
    lastBeepToggle = millis();
    buzzState = !buzzState;
    int vol = buzzState ? volumeValues[volumeLevel] : 0;

    switch (buzzerMode) {
      case 1:
        analogWrite(leftBuzzer, vol);
        analogWrite(rightBuzzer, 0);
        break;
      case 2:
        analogWrite(leftBuzzer, 0);
        analogWrite(rightBuzzer, vol);
        break;
    }
  }
}

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 20000);
  return (duration == 0) ? 400 : duration * 0.034 / 2;
}

void sendLocation(bool sosAlert) {
  if (gps.location.isValid()) {
    String lat = String(gps.location.lat(), 6);
    String lng = String(gps.location.lng(), 6);
    String msg = (sosAlert ? "SOS! Help needed.\n" : "");
    msg += "Location: http://maps.google.com/?q=" + lat + "," + lng;

    sim800l.println("AT+CMGS=\"+918070801434\"");
    delay(500);
    sim800l.println(msg);
    delay(500);
    sim800l.write(26);
    delay(1000);

    Serial.println("Message Sent:");
    Serial.println(msg);
  } else {
    Serial.println("No GPS fix! Message not sent.");
  }
}

void checkIncomingSMS() {
  sim800l.println("AT+CMGL=\"REC UNREAD\"");
  delay(1000);
  String res = "";
  while (sim800l.available()) res += char(sim800l.read());
  res.toUpperCase();

  if (res.indexOf("LOCATION") >= 0) {
    sendLocation(false);
    sim800l.println("AT+CMGD=1,4");
    delay(1000);
  }
}
