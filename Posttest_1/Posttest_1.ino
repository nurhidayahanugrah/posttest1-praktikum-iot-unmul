#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Universitas Mulawarman";
const char* password = "";

ESP8266WebServer server(80);

// Define LED
const int LED_PIN = D4;
const int LED_RESET = D7;
const int LED_MAX = D5;
const int LED_MIN = D6;

// Define Buzzer
const int BUZZER_PIN = D8;
const int BUZZER_RESET = D3;
const int BUZZER_MAX = D2;
const int BUZZER_MIN = D1;

// Define variables
int enable = 0;
int enableBuzzer = 0;
int maxled;
int minled;
int maxbuzzer;
int minbuzzer;
int kecerahan = 10;
int kecerahan2 = 10;
int freq = 0;
int freq2 = 100;
int maxKecerahan = 250;
int minKecerahan = 0;
int kelipatan = 5;
int maxBuzzer = 2500;
int minBuzzer = 100;

void setup() {
  Serial.begin(9600);
  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_RESET, INPUT_PULLUP);
  pinMode(LED_MAX, INPUT_PULLUP);
  pinMode(LED_MIN, INPUT_PULLUP);
  pinMode(BUZZER_RESET, INPUT_PULLUP);
  pinMode(BUZZER_MAX, INPUT_PULLUP);
  pinMode(BUZZER_MIN, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Sedang Menghubungkan Ke WiFi...");
  }
  Serial.println("Terhubung Ke WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", mainMenu);
  server.begin();
}

void loop() {
  server.handleClient();
  delay(150);

  // Check tombol
  maxled = digitalRead(LED_MAX);
  minled = digitalRead(LED_MIN);
  maxbuzzer = digitalRead(BUZZER_MAX);
  minbuzzer = digitalRead(BUZZER_MIN);

  //LED RESET
  if (digitalRead(LED_RESET) == LOW && enable == 0) {
    enable = 1;
    kecerahan = kecerahan2;
    Serial.println(enable);
  } else if (digitalRead(LED_RESET) == LOW && enable == 1) {
    enable = 0;
    kecerahan2 = kecerahan;
    kecerahan = 0;
    Serial.println(enable);
  }

  //Buzzer RESET
  if (digitalRead(BUZZER_RESET) == LOW && enableBuzzer == 0) {
    enableBuzzer = 1;
    freq = freq2;
    Serial.println(enableBuzzer);
  } else if (digitalRead(BUZZER_RESET) == LOW && enableBuzzer == 1) {
    enableBuzzer = 0;
    freq2 = freq;
    freq = 0;
    Serial.println(enableBuzzer);
  }

  // LED Kontrol
  if (maxled == LOW && kecerahan < maxKecerahan && enable == 1) {
    kecerahan += kelipatan;
    Serial.println(kecerahan);
  } else if (maxled == LOW && kecerahan >= maxKecerahan && enable == 1) {
    kecerahan = 250;
    Serial.println(kecerahan);
  } else if (minled == LOW && kecerahan > minKecerahan && enable == 1) {
    kecerahan -= kelipatan;
    Serial.println(kecerahan);
  } else if (minled == LOW && kecerahan <= minKecerahan && enable == 1) {
    kecerahan = 0;
    Serial.println(kecerahan);
  }

  //BUZZER Kontrol
  if (maxbuzzer == LOW && freq < maxBuzzer && enableBuzzer == 1) {
    freq += kelipatan;
    Serial.println(freq);
  } else if (maxbuzzer == LOW && freq >= maxBuzzer && enableBuzzer == 1) {
    freq = 2500;
    Serial.println(freq);
  } else if (minbuzzer == LOW && freq > minBuzzer && enableBuzzer == 1) {
    freq -= kelipatan;
    Serial.println(freq);
  } else if (minbuzzer == LOW && freq <= minBuzzer && enableBuzzer == 1) {
    freq = 100;
    Serial.println(freq);
  }

  delay(100);
  analogWrite(LED_PIN, kecerahan);
  analogWrite(BUZZER_PIN, freq);
}

void mainMenu() {
  server.send(150, "text/html", "<html><body><h1>Monitoring Kecerahan LED dan Frekuensi Buzzer </h1></p></body><h1>Kecerahan Led</h1><p>" + String(kecerahan) + " </p><h1>Frekuensi Buzzer</h1><p>" + String(freq) + " Hz</p></html>");  // Menampilkan halaman utama
}

