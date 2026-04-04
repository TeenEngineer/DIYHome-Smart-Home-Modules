/*
  Скетч к проекту "Адресная матрица"
  Гайд по постройке матрицы: https://alexgyver.ru/matrix_guide/
  Страница проекта (схемы, описания): https://alexgyver.ru/GyverMatrixBT/
  Подробное описание прошивки: https://alexgyver.ru/gyvermatrixos-guide/
  Исходники на GitHub: https://github.com/AlexGyver/GyverMatrixBT/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver Technologies, 2018
  https://AlexGyver.ru/
*/

// GyverMatrixOS
// Версия прошивки 1.10, совместима с приложением GyverMatrixBT версии 1.12 и выше

// ************************ МАТРИЦА *************************
// если прошивка не лезет в Arduino NANO - отключай режимы! Строка 60 и ниже
#if (MCU_TYPE == 1)
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0
#include <ESP8266WiFi.h>
#endif
#include "FastLED.h"

int music = 0;
int musicFlag = 2;
bool isOnce = false;
int volume = 15;

unsigned long endtime;

bool fastl;

byte prevY = 0;
byte prevX = 0;
boolean runningFlag;
boolean gameFlag;
boolean effectsFlag;
byte game = 6;
byte effect;
uint32_t prevColor;
boolean recievedFlag;
byte lastMode = 0;
boolean parseStarted;

unsigned long lastMsg = 0;
int value = 0;

// ANIMATION HEADERS
#include "gas.h"
#include "fire.h"
#include "motion.h"
#include "vibration.h"
#include "water.h"
#include "rain.h"
#include "door.h"
#include "sound.h"
#include "welcome.h"

#define BRIGHTNESS 40       // стандартная маскимальная яркость (0-255)
#define CURRENT_LIMIT 1000  // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH 13    // ширина матрицы
#define HEIGHT 6    // высота матрицы
#define SEGMENTS 1  // диодов в одном "пикселе" (для создания матрицы из кусков ленты)

#define COLOR_ORDER GRB  // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE 1       // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE 2  // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION 2   // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
// при неправильной настрйоке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
// шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/

#define MCU_TYPE 1  // микроконтроллер:
//                            0 - AVR (Arduino NANO/MEGA/UNO)
//                            1 - ESP8266 (NodeMCU, Wemos D1)
//                            2 - STM32 (Blue Pill)

// ******************** ЭФФЕКТЫ И РЕЖИМЫ ********************
#define D_TEXT_SPEED 50    // скорость бегущего текста по умолчанию (мс)
#define D_EFFECT_SPEED 80   // скорость эффектов по умолчанию (мс)
#define D_GAME_SPEED 250    // скорость игр по умолчанию (мс)
#define D_GIF_SPEED 80      // скорость гифок (мс)
#define DEMO_GAME_SPEED 60  // скорость игр в демо режиме (мс)

boolean AUTOPLAY = 0;      // 0 выкл / 1 вкл автоматическую смену режимов (откл. можно со смартфона)
int AUTOPLAY_PERIOD = 10;  // время между авто сменой режимов (секунды)
#define IDLE_TIME 99999    // время бездействия кнопок или Bluetooth (в секундах) после которого запускается автосмена режимов и демо в играх

// о поддерживаемых цветах читай тут https://alexgyver.ru/gyvermatrixos-guide/
#define GLOBAL_COLOR_1 CRGB::Green   // основной цвет №1 для игр
#define GLOBAL_COLOR_2 CRGB::Orange  // основной цвет №2 для игр

#define SCORE_SIZE 0  // размер символов счёта в игре. 0 - маленький для 8х8 (шрифт 3х5), 1 - большой (шрифт 5х7)
#define FONT_TYPE 1   // (0 / 1) два вида маленького шрифта в выводе игрового счёта

// ************** ОТКЛЮЧЕНИЕ КОМПОНЕНТОВ СИСТЕМЫ (для экономии памяти) *************
// внимание! отключение модуля НЕ УБИРАЕТ его эффекты из списка воспроизведения!
// Это нужно сделать вручную во вкладке custom, удалив ненужные функции

#define USE_BUTTONS 1        // использовать физические кнопки управления играми (0 нет, 1 да)
#define BT_MODE 1            // использовать блютус (0 нет, 1 да)
#define USE_NOISE_EFFECTS 1  // крутые полноэкранные эффекты (0 нет, 1 да) СИЛЬНО ЖРУТ ПАМЯТЬ!!!11
#define USE_FONTS 1          // использовать буквы (бегущая строка) (0 нет, 1 да)
#define USE_CLOCK 1          // использовать часы (0 нет, 1 да)

// игры
#define USE_SNAKE 1   // игра змейка (0 нет, 1 да)
#define USE_TETRIS 1  // игра тетрис (0 нет, 1 да)
#define USE_MAZE 0    // игра лабиринт (0 нет, 1 да)
#define USE_RUNNER 0  // игра бегалка-прыгалка (0 нет, 1 да)
#define USE_FLAPPY 0  // игра flappy bird
#define USE_ARKAN 0   // игра арканоид

// ****************** ПИНЫ ПОДКЛЮЧЕНИЯ *******************
// Arduino (Nano, Mega)
#if (MCU_TYPE == 0)
#define LED_PIN 6     // пин ленты
#define BUTT_UP 3     // кнопка вверх
#define BUTT_DOWN 5   // кнопка вниз
#define BUTT_LEFT 2   // кнопка влево
#define BUTT_RIGHT 4  // кнопка вправо
#define BUTT_SET 7    // кнопка выбор/игра

// пины подписаны согласно pinout платы, а не надписям на пинах!
// esp8266 - плату выбирал Wemos D1 R1
#elif (MCU_TYPE == 1)
#define LED_PIN D2     // пин ленты
#define BUTT_UP 14     // кнопка вверх
#define BUTT_DOWN 13   // кнопка вниз
#define BUTT_LEFT 0    // кнопка влево
#define BUTT_RIGHT 12  // кнопка вправо
#define BUTT_SET 15    // кнопка выбор/игра

// STM32 (BluePill) - плату выбирал STM32F103C
#elif (MCU_TYPE == 2)
#define LED_PIN PB12    // пин ленты
#define BUTT_UP PA1     // кнопка вверх
#define BUTT_DOWN PA3   // кнопка вниз
#define BUTT_LEFT PA0   // кнопка влево
#define BUTT_RIGHT PA2  // кнопка вправо
#define BUTT_SET PA4    // кнопка выбор/игра
#endif

// ******************************** ДЛЯ РАЗРАБОТЧИКОВ ********************************
#define DEBUG 0
#define NUM_LEDS WIDTH* HEIGHT* SEGMENTS

#define RUNNING_STRING 0
#define CLOCK_MODE 1
#define GAME_MODE 2
#define MADNESS_NOISE 3
#define CLOUD_NOISE 4
#define LAVA_NOISE 5
#define PLASMA_NOISE 6
#define RAINBOW_NOISE 7
#define RAINBOWSTRIPE_NOISE 8
#define ZEBRA_NOISE 9
#define FOREST_NOISE 10
#define OCEAN_NOISE 11
#define SNOW_ROUTINE 12
#define SPARKLES_ROUTINE 13
#define MATRIX_ROUTINE 14
#define STARFALL_ROUTINE 15
#define BALL_ROUTINE 16
#define BALLS_ROUTINE 17
#define RAINBOW_ROUTINE 18
#define RAINBOWDIAGONAL_ROUTINE 19
#define FIRE_ROUTINE 20
#define IMAGE_MODE 21


#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
const uint8_t EEPROM_MAGIC = 0x42;
const char* mqtt_server = "YOUR_MQTT_SERVER";  // no "mqtts://"
const int mqtt_port = 8883;
const char* mqtt_user = "USERNAME";  // "" if none
const char* mqtt_pass = "PASSWORD";
#define MQTT_MAX_PACKET_SIZE 512

// ================= EEPROM STRUCT =================
struct config_t {
  char ssid[33];
  char pass[65];
  char email[64];
  uint8_t lang;     // 0 = UZ, 1 = RU
  uint8_t magic;
} cfg;


WiFiClientSecure espClient;
PubSubClient client(espClient);
ESP8266WebServer server(80);

const char* sensorNames[] = {
  "welcome","gas","fire","motion","vibration","water","rain","door","sound"
};

String email;
String baseTopic;
bool useRU = false;
unsigned long mqttIgnoreUntil = 0;
#define SENSOR_COUNT 9
void subscribeSensors() {
  for (int i = 1; i < SENSOR_COUNT; i++) {
    String topic = String(cfg.email) + "/" + sensorNames[i];
    client.subscribe(topic.c_str());
    Serial.print("Subscribed to: ");
    Serial.println(topic);
  }
  client.subscribe((String(cfg.email)+"/hub/btn").c_str());
  client.subscribe((String(cfg.email)+"/hub/effect").c_str());
  client.subscribe((String(cfg.email)+"/hub/text").c_str());
  client.subscribe((String(cfg.email)+"/hub/color").c_str());
  client.subscribe((String(cfg.email)+"/hub/ignore").c_str());
  client.subscribe((String(cfg.email)+"/hub/music").c_str());
  client.subscribe((String(cfg.email)+"/hub/musicflag").c_str());
  client.subscribe((String(cfg.email)+"/hub/volume").c_str());
}

// Sensor topic names



// Queue system
#define MAX_QUEUE 10
uint8_t queueBuf[MAX_QUEUE];
uint8_t qHead=0,qTail=0,qSize=0;

bool enqueue(uint8_t s){
  if(qSize>=MAX_QUEUE) return false;
  queueBuf[qTail]=s;
  qTail=(qTail+1)%MAX_QUEUE;
  qSize++; return true;
}
bool dequeue(uint8_t &s){
  if(qSize==0) return false;
  s=queueBuf[qHead];
  qHead=(qHead+1)%MAX_QUEUE;
  qSize--; return true;
}

// ================= HTML =================
const char* page =
"<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'>"
"<style>body{font-family:Arial;background:#fafafa;padding:20px;text-align:center;color:#222}"
"input,select{width:90%;padding:12px;margin:8px;font-size:18px;border-radius:6px;border:1px solid #ccc}"
"button{width:95%;padding:14px;font-size:20px;background:#28a745;color:white;border:none;border-radius:6px;margin-top:12px}"
"h2{margin-bottom:10px}</style></head><body>"
"<h2>WiFi setup</h2>"
"<form action='/save' method='POST'>"
"<input name='ssid' placeholder='WiFi SSID' required>"
"<input name='pass' placeholder='WiFi password' required>"
"<input name='email' placeholder='Email' required>"
"<select name='lang'><option value='0'>O'zbekcha</option><option value='1'>Russian</option></select>"
"<button>Save</button>"
"</form></body></html>";

const char* rebootPage =
"<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'>"
"<style>body{font-family:Arial;text-align:center;margin-top:40px;color:#222}</style>"
"</head><body><h2>Device is restarting...</h2></body></html>";

// ================= EEPROM =================
void loadCfg() {
  EEPROM.begin(512);
  EEPROM.get(0, cfg);
  if (cfg.magic != EEPROM_MAGIC) memset(&cfg, 0, sizeof(cfg));
  EEPROM.end();
}
void saveCfg() {
  cfg.magic = EEPROM_MAGIC;
  EEPROM.begin(512);
  EEPROM.put(0, cfg);
  EEPROM.commit();
  EEPROM.end();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT connecting... ");
    String clientId = "ESP8266-" + String(ESP.getChipId());
    String statusTopic = String(cfg.email)+"/hub/status";
    yield();
    if (client.connect(
  clientId.c_str(),
  mqtt_user,
  mqtt_pass,
  statusTopic.c_str(),
  0,
  true,
  "offline",
  true   // ✅ cleanSession
)) {
      String topic = String(cfg.email)+"/hub/status";
      Serial.println("connected!");
      client.publish(topic.c_str(),"online",true);
      // Subscribe here:
      subscribeSensors();
      mqttIgnoreUntil = millis() + 3000;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry...");
      delay(2000);
    }
  }
}

#define MATRIX_WIDTH 13
#define MATRIX_HEIGHT 6
#define NUM_PIXELS (MATRIX_WIDTH * MATRIX_HEIGHT)
#define LED_BRIGHTNESS 60
#define FRAME_DELAY 80  // ms between animation frames
#define MAX_QUEUE_SIZE 10
#define ALERT_TIMEOUT 15000  // ms
#define SCROLL_DELAY 100     // ms between text scrolls
#define CHAR_WIDTH 4         // 3 pixels char + 1 space


CRGB leds[NUM_LEDS];
String runningText = "";

static const byte maxDim = max(WIDTH, HEIGHT);
byte buttons = 4;  // 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата
int globalBrightness = BRIGHTNESS;
byte globalSpeed = 100;
uint32_t globalColor = 0x00ff00;  // цвет при запуске зелёный
byte breathBrightness;
boolean loadingFlag = true;
byte frameNum;
int gameSpeed = DEMO_GAME_SPEED;
boolean gameDemo = false;
boolean idleState = true;   // флаг холостого режима работы
boolean BTcontrol = false;  // флаг контроля с блютус. Если false - управление с кнопок
int8_t thisMode = 0;
boolean controlFlag = false;
boolean gamemodeFlag = false;
boolean mazeMode = false;
int effects_speed = D_EFFECT_SPEED;
int8_t hrs = 10, mins = 25, secs;
boolean dotFlag;
byte modeCode;  // 0 бегущая, 1 часы, 2 игры, 3 нойс маднесс и далее, 21 гифка или картинка,
boolean fullTextFlag = false;
boolean clockSet = false;

#if (USE_FONTS == 1)
#include "fonts.h"
#endif

uint32_t autoplayTime = ((long)AUTOPLAY_PERIOD * 1000);
uint32_t autoplayTimer;

#include "timerMinim.h"
timerMinim effectTimer(D_EFFECT_SPEED);
timerMinim gameTimer(DEMO_GAME_SPEED);
timerMinim scrollTimer(D_TEXT_SPEED);
timerMinim idleTimer((long)IDLE_TIME * 1000);
timerMinim changeTimer(70);
timerMinim halfsecTimer(500);

#if (USE_CLOCK == 1 && (MCU_TYPE == 0 || MCU_TYPE == 1))
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
// RTC_DS1307 rtc;
#endif

// LIBRARIES AND TIME
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define BUSY_PIN D8
#define DF_RX D6
#define DF_TX D7
bool systemInitialized = false;

unsigned long delayStart = 0;
bool waiting = false;

#define RESET_PIN D5  // for hardware reset
bool useClock = true;
unsigned long musicStartMs = 0;
bool dfPlayerReady = false;

struct {
  bool isPlaying;
  uint8_t currentTrack;
  unsigned long alertEndTime;
} alertState = { false, 0, 0 };

struct {
  bool isActive;
  const uint16_t* const* animation;
  uint8_t frameCount;
  uint8_t currentFrame;
  unsigned long lastFrameTime;
  uint8_t track;
  bool isAlert;
} animationState = { false, nullptr, 0, 0, 0, 0, false };

const uint8_t font3x5s[][5] PROGMEM = {
  { B111, B101, B101, B101, B111 },  // 0
  { B010, B110, B010, B010, B010 },  // 1
  { B111, B001, B111, B100, B111 },  // 2
  { B111, B001, B111, B001, B111 },  // 3
  { B101, B101, B111, B001, B001 },  // 4
  { B111, B100, B111, B001, B111 },  // 5
  { B111, B100, B111, B101, B111 },  // 6
  { B111, B001, B010, B010, B010 },  // 7
  { B111, B101, B111, B101, B111 },  // 8
  { B111, B101, B111, B001, B111 },  // 9
  { B000, B000, B000, B000, B000 },  // space (10)
  { B000, B010, B000, B010, B000 },  // : (11)
};

// Clock State
bool colonState = false;
unsigned long lastColonToggle = 0;

bool ignoreSensors=false;

struct settings {
  char ssid[30];
  char password[30];
  char email[50];
  uint8_t magic;
} user_wifi = {};

SoftwareSerial dfSerial(DF_TX, DF_RX);
DFRobotDFPlayerMini dfplayer;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600 * 5, 60000);
Adafruit_NeoPixel pixels(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

String bases[] = {
  "gas", "fire", "motion", "vibration",
  "water", "rain", "door", "sound"
};
String hubVariables[] = {
  "btn", "volume", "music", "musicflag", "effect", "text", "color"
};

void clearRetained() {
  for(int i=0;i<8;i++){
    String topic = String(cfg.email) + "/"+bases[i];
    client.publish(topic.c_str(), "", true); // wipe retain
    Serial.println("Clearing topic:");
Serial.println(topic);

  }
}

void startAP(){
  WiFi.mode(WIFI_AP);
  WiFi.softAP("HUB", "diyhome1");
  server.on("/", [](){ server.send(200,"text/html",page); });
  server.on("/save", [](){
    strncpy(cfg.ssid, server.arg("ssid").c_str(),32);
    cfg.ssid[32] = '\0';
    strncpy(cfg.pass, server.arg("pass").c_str(),64);
    cfg.pass[64] = '\0';
    strncpy(cfg.email, server.arg("email").c_str(),63);
    cfg.email[63] = '\0';
    cfg.lang = server.arg("lang").toInt();
    saveCfg();
    server.send(200,"text/html",rebootPage);
    delay(1500);
    ESP.restart();
  }); 
  server.begin();
  unsigned long lastShow = 0;

  // Крутимся пока нет валидного user_wifi.magic
  while (true) {
    server.handleClient();   // даём шанс странице принять POST


    yield();   // чтобы не зависло
  }
}

bool tryConnectWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  WiFi.disconnect();
  delay(100);
  Serial.print("Trying WiFi SSID: ");
  Serial.println(cfg.ssid);
  Serial.print("SSID length: ");
  Serial.println(strlen(cfg.ssid));
  Serial.print("Password length: ");
  Serial.println(strlen(cfg.pass));
  WiFi.begin(cfg.ssid,cfg.pass);
  unsigned long ms=millis();
  while(millis()-ms<20000){
    wl_status_t status = WiFi.status();
    if(status==WL_CONNECTED) {
      Serial.println();
      Serial.print("WiFi connected, IP: ");
      Serial.println(WiFi.localIP());
      return true;
    }
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.print("WiFi connection failed, status=");
  Serial.println(WiFi.status());
  return false;
}

void callback(char* topic, byte* payload, unsigned int length) {
  char msg[length + 1];
  memcpy(msg, payload, length);
  msg[length] = '\0';  // important!
  Serial.print("Incoming: ");
  for (unsigned int i = 0; i < length; i++) Serial.print((char)payload[i]);
  Serial.print("Topic: ");
  Serial.print(topic);
  Serial.println();

  // get the payload text
  String incoming = "";
  for (int i = 0; i < length; i++) incoming += (char)payload[i];

  // CURRENT state topic
  if (String(topic) == String(cfg.email)+"/hub/btn") {
    handleButton(incoming);
  }
  String top = String(topic);
  if (!top.startsWith(baseTopic + "/")) {
    // wrong email – ignore silently
    return;
  }

  if (millis() < mqttIgnoreUntil) {
    Serial.print("Ignoring startup MQTT on topic: ");
    Serial.println(topic);
    return;
  }

  String cleanedIncoming = "";
  for (unsigned int i = 0; i < length; i++) {
    char c = (char)payload[i];
    if (c >= 32 && c != 127) cleanedIncoming += c;
  }
  cleanedIncoming.trim();

  // now slice after "email/" and detect which sensor
  if(top.endsWith("speed"))
  {
    globalSpeed=cleanedIncoming.toInt();
  }
  if(!ignoreSensors){
  if(cleanedIncoming=="true"){
  for (int i = 0; i < SENSOR_COUNT; i++) {
    if (top.endsWith(sensorNames[i])) {
      enqueue(i);
      return;
    }
  }}}
  if(top.endsWith("ignore"))
  {
    if(cleanedIncoming=="true"){
    ignoreSensors=true;
    }
    else
    {
      ignoreSensors=false;
    }
  }
  else if(top.endsWith("effect"))
  {
    if (cleanedIncoming.length() == 0) return;
    fastl=true;
    useClock=false;
    FastLED.clear();
    gameFlag=false;
    runningFlag=false;
    effectsFlag=true;
    effect=cleanedIncoming.toInt();
  }
  else if(top.endsWith("text"))
  {
    if (cleanedIncoming.length() == 0) return;
    fastl=true;
    FastLED.clear();
    useClock=false;
    gameFlag=false;
    runningFlag=true;
    effectsFlag=false;
    runningText=cleanedIncoming;

  }
  else if(top.endsWith("color"))
  {
    if (cleanedIncoming.length() == 0) return;
    globalColor=strtoul(cleanedIncoming.substring(1).c_str(),nullptr,16);
  }
  else if(top.endsWith("music"))
  {
    if (cleanedIncoming.length() == 0) return;
    music = cleanedIncoming.toInt()+16;
    isOnce=true;
  }
  else if(top.endsWith("musicflag"))
  {
    if (cleanedIncoming.length() == 0) return;
    musicFlag=cleanedIncoming.toInt();
    if (musicFlag==1) isOnce=true;
    
  }
  else if(top.endsWith("volume"))
  {
    if (cleanedIncoming.length() == 0) return;
    volume = cleanedIncoming.toInt();
    if (dfPlayerReady) dfplayer.volume(volume);
  }
}

// FUNCTIONS
int getPixelIndex(int x, int y) {
  return y * WIDTH + (WIDTH - 1 - x);
}

uint8_t getCharIndex(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c == ' ') return 10;
  if (c == ':') return 11;
  if (c >= 'A' && c <= 'Z') return 12 + (c - 'A');
  if (c >= 'a' && c <= 'z') return 12 + (c - 'a');
  return 10;  // Default to space for unknown characters
}

void drawChar(char c, int offsetX, uint32_t color) {
  uint8_t index = getCharIndex(c);
  for (int row = 0; row < 5; row++) {
    uint8_t rowData = pgm_read_byte(&font3x5s[index][row]);
    for (int col = 0; col < 3; col++) {
      if (rowData & (1 << (2 - col))) {
        int xPos = offsetX + col;
        int yPos = row;  // Start from first row
        if (xPos >= 0 && xPos < MATRIX_WIDTH && yPos >= 0 && yPos < MATRIX_HEIGHT) {
          pixels.setPixelColor(getPixelIndex(xPos, yPos), color);
        }
      }
    }
  }
}

void showClock() {
  timeClient.update();

  pixels.clear();
  // Get hours and minutes as separate integers
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();


  // First digit (tens of hour) - columns 0-2
  drawChar((hours / 10) + '0', 0, pixels.Color(64, 13, 0));  // Orange

  // Second digit (units of hour) - columns 3-5
  drawChar((hours % 10) + '0', 3, pixels.Color(0, 38, 64));  // Blue

  // Colon - column 6 (center of 13 columns)
  if (colonState) {
    pixels.setPixelColor(getPixelIndex(6, 1), pixels.Color(64, 64, 64));  // Gray
    pixels.setPixelColor(getPixelIndex(6, 3), pixels.Color(64, 64, 64));  // Gray
  }

  // First digit of minutes (tens) - columns 7-9
  drawChar((minutes / 10) + '0', 7, pixels.Color(64, 13, 0));  // Orange

  // Second digit of minutes (units) - columns 10-12
  drawChar((minutes % 10) + '0', 10, pixels.Color(0, 38, 64));  // Blue
  pixels.show();
}

// ==================== ANIMATION FUNCTIONS ====================
void showAnimationFrame() {
  if (!animationState.animation) return;

  const uint16_t* frame = animationState.animation[animationState.currentFrame];
  for (int y = 0; y < MATRIX_HEIGHT; y++) {
    for (int x = 0; x < MATRIX_WIDTH; x++) {
      int index = getPixelIndex(x, y);
      uint16_t color565 = pgm_read_word(&frame[index]);
      uint8_t r = ((color565 >> 11) & 0x1F) << 3;
      uint8_t g = ((color565 >> 5) & 0x3F) << 2;
      uint8_t b = (color565 & 0x1F) << 3;
      pixels.setPixelColor(index, pixels.Color(r, g, b));
    }
  }
  pixels.show();
}

void playAnimation(uint8_t animID, bool isAlert = false) {
  const uint16_t* const* newAnimation = nullptr;
  uint8_t frameCount = 0;

  switch (animID) {
    case 1:
      newAnimation = gas_array;
      frameCount = sizeof(gas_array) / sizeof(gas_array[0]);
      break;
    case 2:
      newAnimation = fire_array;
      frameCount = sizeof(fire_array) / sizeof(fire_array[0]);
      break;
    case 3:
      newAnimation = motion_array;
      frameCount = sizeof(motion_array) / sizeof(motion_array[0]);
      break;
    case 4:
      newAnimation = vibration_array;
      frameCount = sizeof(vibration_array) / sizeof(vibration_array[0]);
      break;
    case 5:
      newAnimation = water_array;
      frameCount = sizeof(water_array) / sizeof(water_array[0]);
      break;
    case 6:
      newAnimation = rain_array;
      frameCount = sizeof(rain_array) / sizeof(rain_array[0]);
      break;
    case 7:
      newAnimation = door_array;
      frameCount = sizeof(door_array) / sizeof(door_array[0]);
      break;
    case 8:
      newAnimation = sound_array;
      frameCount = sizeof(sound_array) / sizeof(sound_array[0]);
      break;
    case 9:
      newAnimation = welcome_array;
      frameCount = sizeof(welcome_array) / sizeof(welcome_array[0]);
      break;
  }

  if (newAnimation) {
    animationState.animation = newAnimation;
    animationState.frameCount = frameCount;
    animationState.currentFrame = 0;
    animationState.lastFrameTime = millis();
    animationState.isActive = true;
    animationState.track = animID;
    animationState.isAlert = isAlert;
    showAnimationFrame();
  }
}

void stopAnimation() {
  animationState.isActive = false;
  pixels.clear();
  pixels.show();
}

bool initDFPlayer() {
  // Let the module finish its own power-up before the ESP starts sending commands.
  delay(1500);

  if (dfplayer.begin(dfSerial, false, true)) {
    delay(200);
    dfplayer.volume(15);
    delay(200);
    dfplayer.stop();
    Serial.print("DFPlayer ready");
    dfplayer.volume(15);
    return true;
  }
}

void factoryReset() {
  Serial.println("FACTORY RESET!");
  memset(&cfg, 0, sizeof(cfg));   // wipe struct
  cfg.magic = 0;                  // ensure load sees empty config

  EEPROM.begin(512);
  EEPROM.put(0, cfg);
  EEPROM.commit();
  EEPROM.end();

  delay(500);
  ESP.restart();
}

void setup() {
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(BUSY_PIN,INPUT);
  Serial.begin(9600);
  loadCfg();

  email = cfg.email;
  baseTopic = email;
  useRU = cfg.lang==1;

  if(strlen(cfg.ssid)==0){
    Serial.println("WiFi SSID is empty, starting AP mode");
    startAP();
    return;
  }

  Serial.println("Connecting to WiFi before audio/animation");
  if(!tryConnectWiFi()){
    Serial.println("Stored WiFi failed, starting AP mode");
    startAP();
    return;
  }

  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  dfSerial.begin(9600);
  pixels.begin();
  pixels.clear();
  pixels.setBrightness(LED_BRIGHTNESS);
  pixels.show();

  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // DFPLAYER
  dfPlayerReady = initDFPlayer();
  if (dfPlayerReady) {
    delay(300);
  } else {
    Serial.println("DFPlayer init failed, continuing without audio");
  }

  // 2. Start welcome sequence
  playAnimation(9);
  unsigned long animationStart = millis();
  yield();

  while (millis() - animationStart < 5000) {
    // Handle animation frames
    if (millis() - animationState.lastFrameTime >= FRAME_DELAY) {
      animationState.currentFrame = (animationState.currentFrame + 1) % animationState.frameCount;
      showAnimationFrame();
      animationState.lastFrameTime = millis();
    }

    if ((millis() - animationStart > 5000)) {
      break;
    }

    delay(10);  // Prevent watchdog issues
  }
  stopAnimation();
  reconnect();
  
  clearRetained();

  timeClient.begin();
  timeClient.update();
  systemInitialized = true;

  // настройки ленты
  FastLED.addLeds<WS2812, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.clear();
  FastLED.show();
  client.setKeepAlive(60);
  useClock = true;
  colonState = true;
  lastColonToggle = millis();
  showClock();
}



void handleButton(String name) {
  if (name == "snake") {
    fastl=true;
    effectsFlag=false;
    runningFlag=false;
    useClock = false;
    FastLED.clear();
    gameSpeed = 100 * 4;
    gameTimer.setInterval(gameSpeed);
    gameDemo = false;
    game = 0;
  } else if (name == "tetris") {
    fastl=true;
    effectsFlag=false;
    runningFlag=false;
    useClock = false;
    FastLED.clear();
    gameSpeed = 100 * 4;
    gameTimer.setInterval(gameSpeed);
    gameDemo = false;
    game = 1;
  } else if (name == "clock") {
    FastLED.clear();
    fastl=false;
    effectsFlag = false;
    runningFlag = false;
    gameFlag = false;
    useClock = true;
    showClock();
  } else if (name == "start") {
    gameFlag = !gameFlag;
  } else if (name == "up") {
    buttons = 0;
    controlFlag = true;
  } else if (name == "right") {
    buttons = 1;
    controlFlag = true;
  } else if (name == "down") {
    buttons = 2;
    controlFlag = true;
  } else if (name == "left") {
    buttons = 3;
    controlFlag = true;
  }
}

bool isDonce = true;


void loop() {
  if (digitalRead(RESET_PIN) == LOW) {
    delay(50);  // debounce
    if (digitalRead(RESET_PIN) == LOW) {
      factoryReset();
    }
  }
  if (!client.connected()) reconnect();
  yield();
  yield();
  uint32_t now = millis();
  unsigned long currentMillis = millis();

  client.loop();
  yield();  // let WiFi breathe

  if (useClock && !animationState.isActive) {
    if (lastColonToggle == 0 || currentMillis - lastColonToggle >= 1000) {
      colonState = !colonState;
      lastColonToggle = currentMillis;
      showClock();
    }
  }
  if (!client.connected()) reconnect();


  // Handle animation frame updates
  if (animationState.isActive && currentMillis - animationState.lastFrameTime >= FRAME_DELAY) {
    animationState.currentFrame = (animationState.currentFrame + 1) % animationState.frameCount;
    showAnimationFrame();
    animationState.lastFrameTime = currentMillis;
  }
  // ===========================
  if (effectsFlag && fastl) effects();
  if (gameFlag && fastl) games();
  if (runningFlag && fastl) customRoutine();
  if (musicFlag == 1 && music>16){
    
    if(dfPlayerReady && isOnce){
      dfplayer.play(music);
      musicStartMs = millis();
      isOnce=false;
    }
    if(dfPlayerReady && millis() - musicStartMs > 300 && digitalRead(BUSY_PIN)==true) musicFlag=2;
    
  }
  else if (musicFlag == 0)
  {
    if (dfPlayerReady) dfplayer.stop();
    musicFlag = 2;
  }
  if (!client.connected()) reconnect();
  static unsigned long alertTimer=0;
  static bool busy=false;
  static uint8_t current=255;

  if(!busy && qSize>0){
    runningFlag=false;
    effectsFlag=false;
    gameFlag=false;
    useClock=true;
    if (dfPlayerReady) dfplayer.stop();
    musicFlag=0;
    musicFlag=2;
    dequeue(current);
    busy=true;
    alertTimer=millis();
    

    // TODO: CALL anim + dfplayer here
    playAnimation(current);
    endtime = millis()+7000;
    if(dfPlayerReady && !useRU){
      dfplayer.play(current);
    }
    else if (dfPlayerReady)
    {
      dfplayer.play(current+8);
    }
  }

  if(busy && millis() >=endtime){
    busy=false;
    current=255;
    stopAnimation();
    if (dfPlayerReady) dfplayer.stop();
    useClock=true;
  }
  if (!client.connected()) reconnect();
  client.loop();

  if (!useClock) {
    FastLED.show();
  }
  }
