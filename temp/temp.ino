#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_AHTX0.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// =================== WEMOS OLED + AHT11 ===================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1

#define I2C_SDA D1
#define I2C_SCL D2
TwoWire I2Cone;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cone, OLED_RESET);
Adafruit_AHTX0 aht;

// =================== PINS ===================
#define RESET_PIN D3

// =================== MQTT CONFIG ===================
const char* mqtt_server = "YOUR_MQTT_SERVER";
const int mqtt_port = 8883;
const char* mqtt_user = "USERNAME";
const char* mqtt_pass = "PASSWORD";

// =================== EEPROM ===================
struct settings {
  char ssid[32];
  char password[32];
  char email[50];
  uint8_t magic;
} user_wifi;

const uint8_t EEPROM_MAGIC = 0x42;

// =================== GLOBALS ===================
ESP8266WebServer server(80);
WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 2000;
unsigned long lastSwitch = 0;
const unsigned long switchInterval = 5000; // 5s display rotation
bool showTemp = true;

// =================== FUNCTIONS ===================
void factoryReset() {
  memset(&user_wifi, 0, sizeof(user_wifi));
  user_wifi.magic = EEPROM_MAGIC;
  EEPROM.put(0, user_wifi);
  EEPROM.commit();
  delay(500);
  ESP.restart();
}

void startSetupAP() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("TEMP Setup", "diyhome1");
}

void handlePortal() {
  if (server.method() == HTTP_POST) {
    strncpy(user_wifi.ssid, server.arg("ssid").c_str(), sizeof(user_wifi.ssid)-1);
    strncpy(user_wifi.password, server.arg("password").c_str(), sizeof(user_wifi.password)-1);
    strncpy(user_wifi.email, server.arg("email").c_str(), sizeof(user_wifi.email)-1);

    user_wifi.ssid[sizeof(user_wifi.ssid)-1] = 0;
    user_wifi.password[sizeof(user_wifi.password)-1] = 0;
    user_wifi.email[sizeof(user_wifi.email)-1] = 0;
    user_wifi.magic = EEPROM_MAGIC;

    EEPROM.put(0, user_wifi);
    EEPROM.commit();

    server.send(200, "text/html", R"rawliteral(
      <!doctype html>
      <html>
      <head><meta charset='utf-8'><title>Saved!</title></head>
      <body><h2>Settings saved! Rebooting...</h2></body>
      </html>
    )rawliteral");

    delay(2000);
    ESP.restart();
  } else {
    String page = R"rawliteral(
      <!doctype html>
      <html>
      <head>
        <meta charset='utf-8'>
        <title>Wemos WiFi Setup</title>
        <style>
          body {font-family: sans-serif; margin:20px;}
          input {width:100%; padding:8px; margin:5px 0;}
          button {padding:10px; width:100%; background:#007bff; color:white; border:none;}
        </style>
      </head>
      <body>
        <h2>WiFi Setup</h2>
        <form method='post' action='/'>
          <label>SSID:</label>
          <input type='text' name='ssid')rawliteral" + String(user_wifi.ssid) + R"rawliteral(' required>
          <label>Password:</label>
          <input type='password' name='password')rawliteral" + String(user_wifi.password) + R"rawliteral(' required>
          <label>Email:</label>
          <input type='email' name='email')rawliteral" + String(user_wifi.email) + R"rawliteral(' required>
          <button type='submit'>Save</button>
        </form>
      </body>
      </html>
    )rawliteral";
    server.send(200, "text/html", page);
  }
}

// =================== MQTT ===================
void mqttSetup() {
  mqttClient.setClient(espClient);
  mqttClient.setServer(mqtt_server, mqtt_port);
  espClient.setInsecure(); // skip LetsEncrypt verification
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting MQTT...");
    if (mqttClient.connect("WemosClient", mqtt_user, mqtt_pass)) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}

// =================== SETUP ===================
void setup() {
  Serial.begin(115200);
  pinMode(RESET_PIN, INPUT_PULLUP);
  I2Cone.begin(I2C_SDA, I2C_SCL); // SDA, SCL

  EEPROM.begin(sizeof(user_wifi));
  EEPROM.get(0, user_wifi);
  if (user_wifi.magic != EEPROM_MAGIC) {
    startSetupAP();
  }

  if(!aht.begin()) {
    Serial.println("AHTxx not found!");
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, &I2Cone)) {
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }
  display.clearDisplay();
  display.display();

  WiFi.mode(WIFI_STA);
  WiFi.begin(user_wifi.ssid, user_wifi.password);

  byte tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (tries++ > 20) {
      startSetupAP();
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    mqttSetup();
  }

  server.on("/", handlePortal);
  server.begin();
  display.dim(false);
}

// =================== LOOP ===================
void loop() {
  server.handleClient();

  // Safe reset logic
  if (digitalRead(RESET_PIN) == LOW) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Factory reset triggered!");
    display.display();
    delay(500); // debounce
    factoryReset();
  }

  if (WiFi.status() == WL_CONNECTED && !mqttClient.connected()) reconnectMQTT();
  mqttClient.loop();

  // Update every 2 seconds
  if (millis() - lastUpdate > updateInterval) {
    lastUpdate = millis();
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);

    // Rotate display every switchInterval
    if (millis() - lastSwitch > switchInterval) {
      lastSwitch = millis();
      showTemp = !showTemp; // toggle
    }

    display.clearDisplay();
    display.setTextSize(3); // big text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);

    if (showTemp) {
      display.print(temp.temperature, 1);
      display.print(" C");
    } else {
      display.print(humidity.relative_humidity, 1);
      display.print(" %");
    }

    display.display();

    // Publish to MQTT: email + topic
    if (mqttClient.connected()) {
      String topicPrefix = String(user_wifi.email) + "/temp";

      String payloadTemp = String(temp.temperature,1);
      mqttClient.publish((topicPrefix + "/temp").c_str(), payloadTemp.c_str(), true);

      String payloadHum = String(humidity.relative_humidity,1);
      mqttClient.publish((topicPrefix + "/humidity").c_str(), payloadHum.c_str(), true);
    }
  }
}
