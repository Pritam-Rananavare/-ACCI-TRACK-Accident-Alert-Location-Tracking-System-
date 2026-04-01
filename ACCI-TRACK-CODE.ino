#include <Wire.h>
#include <MPU6050.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

// MPU6050
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

// GPS
TinyGPSPlus gps;
HardwareSerial gpsSerial(1); // GPS on UART1 (GPIO16 RX, GPIO17 TX)

// SIM800L
HardwareSerial sim800(2); // SIM800L on UART2 (GPIO26 RX, GPIO27 TX)

// Thresholds
#define ACCIDENT_THRESHOLD 24000 

// Flags
bool accidentDetected = false;
unsigned long lastAlertTime = 0;
const unsigned long alertCooldown = 30000; 

void setup() {
  Serial.begin(115200);

  // GPS setup
  gpsSerial.begin(9600, SERIAL_8N1, 26, 27);

  // SIM800L setup
  sim800.begin(9600, SERIAL_8N1, 26, 27);
  delay(1000);
  sendSIM800Command("AT");
  sendSIM800Command("AT+CMGF=1"); 
  sendSIM800Command("AT+CNMI=1,2,0,0,0"); 

  // MPU6050 init
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  } else {
    Serial.println("MPU6050 connected");
  }
}

void loop() {
  // Read acceleration values
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  int totalAcc = abs(ax) + abs(ay) + abs(az);

  
  Serial.print("AX: "); Serial.print(ax);
  Serial.print(" | AY: "); Serial.print(ay);
  Serial.print(" | AZ: "); Serial.print(az);
  Serial.print(" | TotalAcc: "); Serial.println(totalAcc);

  delay(500); 

  if (totalAcc > ACCIDENT_THRESHOLD && millis() - lastAlertTime > alertCooldown) {
    Serial.println("⚠ Accident detected!");
    accidentDetected = true;
    lastAlertTime = millis();

    float latitude = 0.0, longitude = 0.0;

    // Wait for GPS fix
    unsigned long start = millis();
    while ((millis() - start) < 10000) { 
      while (gpsSerial.available()) {
        gps.encode(gpsSerial.read());
      }
      if (gps.location.isUpdated()) {
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        break;
      }
    }

    if (latitude != 0.0 && longitude != 0.0) {
      String locationLink = "http://maps.google.com/maps?q=" + String(latitude, 6) + "," + String(longitude, 6);
      sendSMS("+917499858295", "🚨 Accident Detected!\nLocation: " + locationLink);
    } else {
      sendSMS("+917499858295", "🚨 Accident Detected! But GPS fix not found.");
    }
  }
}

void sendSIM800Command(String cmd) {
  sim800.println(cmd);
  delay(500);
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
}

void sendSMS(String number, String message) {
  sim800.print("AT+CMGS=\"");
  sim800.print(number);
  sim800.println("\"");
  delay(1000);
  sim800.print(message);
  delay(500);
  sim800.write(26); // Ctrl+Z to send
  delay(5000);
  Serial.println("✅ SMS Sent.");
}