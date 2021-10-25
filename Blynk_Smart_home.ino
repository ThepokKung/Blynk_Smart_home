#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BLYNK_PRINT Serial
#define DHTPIN 4
#define DHTTYPE DHT11
#define SCREEN_WIDTH 128 // pixel ความกว้าง
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 //ขา reset เป็น -1 ถ้าใช้ร่วมกับขา Arduino reset
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define Relay1 12
#define Relay2 14

DHT dht(DHTPIN, DHTTYPE);

char* auth = "jYw3GZwUf-noYlu1qNlOeRxGL2ntSHED";
char* ssid = "RSRC2"; //Wifi Name
char* pass = "043234794"; //Wifi Password

void setup() {
  Serial.begin(115200);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
}

BLYNK_WRITE(V2)
{
  if (param.asInt() == 1) {
    digitalWrite(Relay1, HIGH);

  } else {
    digitalWrite(Relay1, LOW);

  }
}
BLYNK_WRITE(V3) 
{
 if (param.asInt()==1) {
 digitalWrite(Relay2, HIGH);

 }else{
 digitalWrite(Relay2, LOW);

 }
 }
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("% Temperature: "));
  Serial.print(t);
  Serial.print(F(" C "));
  Serial.print(f);
  Serial.print(F(" F Heat index: "));
  Serial.print(hic);
  Serial.print(F(" C "));
  Serial.print(hif);
  Serial.println(F(" F"));

  OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด

  OLED.setTextColor(WHITE, BLACK);  //กำหนดข้อความสีขาว ฉากหลังสีดำ
  OLED.setTextSize(1);
  OLED.setCursor(0, 2); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
  OLED.print("humidity :");
  OLED.setCursor(80 , 2);
  OLED.print(h);
  OLED.setCursor(0, 20);
  OLED.print("Temperature :");
  OLED.setCursor(80, 20);
  OLED.print(t);

  OLED.display(); // สั่งให้จอแสดงผล
  delay(2000);

  Blynk.run();
  delay(100);
  Blynk.virtualWrite(V0, h);
  Blynk.virtualWrite(V1, t);
}
