#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <FastLED.h>
// =================== Pins ==========================
#define RESET_PIN 1   // GPIO1 for hardware reset
#define SENSOR_PIN 0   // GPIO0 for sensor
#define LED_PIN 2     // GPIO2 for LED
#define NUM_PIXELS 16  // 4x4

#define NUM_LEDS 16
CRGB leds[NUM_LEDS];

// =================== Globals =======================
ESP8266WebServer server(80);
WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

bool turningLed=false;

struct settings {
  char ssid[30];
  char password[30];
  char email[50];   // optional, you can keep for identification
  char topic[50];   // NEW: store the full MQTT topic
  uint8_t magic;
} user_wifi = {};

const uint8_t EEPROM_MAGIC = 0x42;

// MQTT config
const char* mqtt_server = "YOUR_MQTT_SERVER";
const int mqtt_port = 8883;
const char* mqtt_user = "USERNAME";
const char* mqtt_pass = "PASSWORD"; // change this

unsigned long lastPublish = 0;
const unsigned long publishInterval = 2000; // 5 sec

bool lastSensorState = false;
bool firstRun = true;   // so we publish once after boot

bool ledTopicActive = false; // track if topic matched "LED"
bool soundOn = false;
String baseTopic;

void setupMatrix() {
  FastLED.addLeds<WS2812, SENSOR_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  
}

void handleLEDMessage(const char* topic, const char* payload) {
  String t = String(topic);
  String val = String(payload);
  if (t.endsWith("led")) {
    if (val == "true") {
      fill_solid(leds,NUM_LEDS,CRGB::White);
      FastLED.show();
    } else {
      FastLED.clear();
      FastLED.show();
    }
    FastLED.show();
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';
  String top = String(topic);
  if (!top.startsWith(baseTopic + "/")) {
    // wrong email – ignore silently
    return;
  }
  String incoming = "";
  for (int i = 0; i < length; i++) incoming += (char)payload[i];
  
  if (String(user_wifi.topic) == "led") {
  handleLEDMessage(topic, message);}
  else if (String(user_wifi.topic) == "sound") {
        if (top.endsWith("led"))
        {
          if(incoming=="true")
          {
            turningLed=true;
            digitalWrite(LED_PIN,1);
          }
          else
          {
            turningLed=false;
          }
        }
      }
}
// =================== Setup ==========================
void setup() {
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  EEPROM.begin(sizeof(user_wifi));
  EEPROM.get(0, user_wifi);
  baseTopic = String(user_wifi.email);
  
  if (user_wifi.magic != EEPROM_MAGIC) factoryReset();

  pinMode(RESET_PIN, INPUT_PULLUP);

  // Determine mode based on topic
  if (String(user_wifi.topic) == "led") {
    pinMode(SENSOR_PIN, OUTPUT);
    ledTopicActive = true;
    setupMatrix();
    FastLED.setBrightness(100);
  }

  // MQTT setup
  

  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  EEPROM.begin(sizeof(user_wifi));
  EEPROM.get(0, user_wifi);

  // Check EEPROM magic
  if (user_wifi.magic != EEPROM_MAGIC) {
    factoryReset();
  }

  // Try Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(user_wifi.ssid, user_wifi.password);

  byte tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (tries++ > 20) { // ~10 sec
      //Serial.println("\nCan't connect, starting AP...");
      startSetupAP();
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    //Serial.println("\nWi-Fi connected: " + WiFi.localIP().toString());
    mqttSetup();
  }

  // Web server handlers
  server.on("/", handlePortal);
  server.on("/reset", handleFactoryReset);
  server.begin();
  delay(250);
  FastLED.clear();
  FastLED.show();
}


// =================== Loop ==========================
void loop() {
  server.handleClient();

  // Hardware reset
  if (digitalRead(RESET_PIN) == LOW) {
    //Serial.println("GPIO1 pressed! Factory reset triggered.");
    delay(500); // debounce
    factoryReset();
  }

  server.handleClient();

  // Read sensor
  bool sensorState = digitalRead(SENSOR_PIN);
  if (String(user_wifi.topic) != "door") {
    sensorState = !sensorState;
  }

  // MQTT maintenance
  if (WiFi.status() == WL_CONNECTED) {
    if (!mqttClient.connected()) {
      reconnectMQTT();
    }
    mqttClient.loop();
  }
  // Publish ONLY if state changed (or first boot)
  if (!ledTopicActive && WiFi.status() == WL_CONNECTED && mqttClient.connected() && (sensorState != lastSensorState || firstRun)) {
    firstRun = false;
    lastSensorState = sensorState;

    if(String(user_wifi.topic)=="sound")
    {
      
      if(!soundOn && sensorState)
      {
        soundOn= true;
        digitalWrite(LED_PIN,1);
        String fullTopic = String(user_wifi.email) + "/" + String(user_wifi.topic); // use topic from setup page
    
    mqttClient.publish(fullTopic.c_str(), soundOn ? "true" : "false", true); // retained
    if(turningLed)
      {
        String fullTopic = String(user_wifi.email) + "/led"; // use topic from setup page
    
        mqttClient.publish(fullTopic.c_str(), soundOn ? "true" : "false", true); // retained
      }
      }
      else if(soundOn && sensorState)
      {
        soundOn= false;
        digitalWrite(LED_PIN,0);
        String fullTopic = String(user_wifi.email) + "/" + String(user_wifi.topic); // use topic from setup page
    
    mqttClient.publish(fullTopic.c_str(), soundOn ? "true" : "false", true); // retained
    if(turningLed)
      {
        String fullTopic = String(user_wifi.email) + "/led"; // use topic from setup page
    
        mqttClient.publish(fullTopic.c_str(), soundOn ? "true" : "false", true); // retained
      }
      }
      delay(100);
    }
    else
    {
      // LED reacts instantly
      digitalWrite(LED_PIN, sensorState ? HIGH : LOW);
      String fullTopic = String(user_wifi.email) + "/" + String(user_wifi.topic); // use topic from setup page

    mqttClient.publish(fullTopic.c_str(), sensorState ? "true" : "false", true); // retained
    }
    

    
  }
  

}

// =================== Functions ======================

void startSetupAP() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP01 Setup", "diyhome1");
}

void handlePortal() {
  if (server.method() == HTTP_POST) {
    // Save Wi-Fi + MQTT settings
    strncpy(user_wifi.ssid, server.arg("ssid").c_str(), sizeof(user_wifi.ssid) - 1);
    strncpy(user_wifi.password, server.arg("password").c_str(), sizeof(user_wifi.password) - 1);
    strncpy(user_wifi.email, server.arg("email").c_str(), sizeof(user_wifi.email) - 1);
    strncpy(user_wifi.topic, server.arg("topic").c_str(), sizeof(user_wifi.topic) - 1); // from dropdown
    user_wifi.ssid[sizeof(user_wifi.ssid) - 1] = 0;
    user_wifi.password[sizeof(user_wifi.password) - 1] = 0;
    user_wifi.email[sizeof(user_wifi.email) - 1] = 0;
    user_wifi.topic[sizeof(user_wifi.topic) - 1] = 0;
    user_wifi.magic = EEPROM_MAGIC;

    EEPROM.put(0, user_wifi);
    EEPROM.commit();

    server.send(200, "text/html", R"rawliteral(
<!doctype html>
<html lang='en'>

<head>
    <meta charset='utf-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <title>Настройка WIFI DIYHome</title>
    <style>
        *,
        ::after,
        ::before {
            box-sizing: border-box;
        }

        body {
            margin: 0;
            font-family: 'Segoe UI', Roboto, 'Helvetica Neue', Arial, 'Noto Sans', 'Liberation Sans';
            font-size: 1rem;
            font-weight: 400;
            line-height: 1.5;
            color: #212529;
            background-color: #f5f5f5;
        }

        .form-control {
            display: block;
            width: 100%;
            height: calc(1.5em + .75rem + 2px);
            border: 1px solid #ced4da;
        }

        button {
            border: 1px solid transparent;
            color: #fff;
            background-color: #007bff;
            border-color: #007bff;
            padding: .5rem 1rem;
            font-size: 1.25rem;
            line-height: 1.5;
            border-radius: .3rem;
            width: 100%
        }

        .form-signin {
            width: 100%;
            max-width: 400px;
            padding: 15px;
            margin: auto;
        }

        h1,
        p {
            text-align: center
        }
    </style>
</head>

<body>
    <main class='form-signin'>
        <h1>Настройка WIFI</h1> <br />
        <p>Настройки были успешно сохранены!<br />Устройство перезагрузится.</p>
    </main>
</body>

</html>
)rawliteral");
    delay(2000);
    ESP.restart();
  } else {
    // Serve form with dropdown instead of text field for topic
    String page = R"rawliteral(
<!doctype html>
<html lang='en'>
<head>
  <meta charset='utf-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <title>Настройка WIFI DIYHome</title>
  <style>
        *,
        ::after,
        ::before {
            box-sizing: border-box;
        }

        body {
            margin: 0;
            font-family: 'Segoe UI', Roboto, 'Helvetica Neue', Arial, 'Noto Sans', 'Liberation Sans';
            font-size: 1rem;
            font-weight: 400;
            line-height: 1.5;
            color: #212529;
            background-color: #f5f5f5;
        }

        .form-control {
            display: block;
            width: 100%;
            height: calc(1.5em + .75rem + 2px);
            border: 1px solid #ced4da;
        }

        button {
            cursor: pointer;
            border: 1px solid transparent;
            color: #fff;
            background-color: #007bff;
            border-color: #007bff;
            padding: .5rem 1rem;
            font-size: 1.25rem;
            line-height: 1.5;
            border-radius: .3rem;
            width: 100%
        }

        .form-signin {
            width: 100%;
            max-width: 400px;
            padding: 15px;
            margin: auto;
        }

        h1 {
            text-align: center
        }
        select.form-control {
    height: calc(2em + .75rem + 2px); /* a bit taller for options */
    padding: .375rem .75rem;
    border-radius: .3rem;
    background-color: #fff;
    border: 1px solid #ced4da;
    appearance: none; /* remove default arrow style */
    cursor: pointer;
}

select.form-control:focus {
    border-color: #80bdff;
    outline: 0;
    box-shadow: 0 0 0 .2rem rgba(0,123,255,.25);
}
    </style>
</head>
<body>
  <main>
    <form action='/' method='post'>
      <h1>Настройка WIFI</h1>
      <label>Название WIFI сети:</label>
      <input type='text' name='ssid' value=')rawliteral" + String(user_wifi.ssid) + R"rawliteral('><br><br>
      
      <label>Пароль:</label>
      <input type='password' name='password' value=')rawliteral" + String(user_wifi.password) + R"rawliteral('><br><br>

      <label>Email:</label>
      <input type='email' name='email' value=')rawliteral" + String(user_wifi.email) + R"rawliteral('><br><br>

      <label>Выберите датчик:</label>
      <select name='topic'>
        <option value='gas' )rawliteral" + (String(user_wifi.topic) == "gas" ? "selected" : "") + R"rawliteral(>Gas</option>
        <option value='fire' )rawliteral" + (String(user_wifi.topic) == "fire" ? "selected" : "") + R"rawliteral(>Fire</option>
        <option value='door' )rawliteral" + (String(user_wifi.topic) == "door" ? "selected" : "") + R"rawliteral(>Door</option>
        <option value='motion' )rawliteral" + (String(user_wifi.topic) == "motion" ? "selected" : "") + R"rawliteral(>Motion</option>
        <option value='rain' )rawliteral" + (String(user_wifi.topic) == "rain" ? "selected" : "") + R"rawliteral(>Rain</option>
        <option value='water' )rawliteral" + (String(user_wifi.topic) == "water" ? "selected" : "") + R"rawliteral(>Water</option>
        <option value='led' )rawliteral" + (String(user_wifi.topic) == "led" ? "selected" : "") + R"rawliteral(>LED</option>
        <option value='vibration' )rawliteral" + (String(user_wifi.topic) == "vibration" ? "selected" : "") + R"rawliteral(>Vibration</option>
        <option value='sound' )rawliteral" + (String(user_wifi.topic) == "sound" ? "selected" : "") + R"rawliteral(>Sound</option>
      </select><br><br>

      <button type='submit'>Сохранить</button>
    </form>
  </main>
</body>
</html>
)rawliteral";
    server.send(200, "text/html", page);
  }
}

void handleFactoryReset() {
  //Serial.println("Factory reset triggered via web!");
  factoryReset();
}

void factoryReset() {
  memset(&user_wifi, 0, sizeof(user_wifi));
  user_wifi.magic = EEPROM_MAGIC;
  EEPROM.put(0, user_wifi);
  EEPROM.commit();
  delay(500);
  ESP.restart();
}

// =================== MQTT ===========================

void mqttSetup() {
  mqttClient.setClient(espClient);
  mqttClient.setServer(mqtt_server, mqtt_port);
  espClient.setInsecure(); // skip certificate verification for LetsEncrypt
mqttClient.setCallback(callback);}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");
    if (mqttClient.connect("ESP01Client", mqtt_user, mqtt_pass)) {
      if (String(user_wifi.topic) == "led") {
        String fullTopic = String(user_wifi.email) + "/led";
        mqttClient.subscribe(fullTopic.c_str());
        Serial.println("Subscribed to: " + fullTopic);
      }
      else if (String(user_wifi.topic) == "sound") {
        String fullTopic = String(user_wifi.email) + "/sound/led";
        mqttClient.subscribe(fullTopic.c_str());
        Serial.println("Subscribed to: " + fullTopic);
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      //Serial.println(" try again in 2 sec");
      delay(2000);
    }
  }
}
