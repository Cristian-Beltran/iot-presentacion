#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int TRIG_PIN = 5;
const int ECHO_PIN = 18;

unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 250;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED no encontrado");
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Iniciando...");
  display.display();
  delay(1000);
}

void loop() {
  if (millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();

    float distance = measureDistance();
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Sensor Ultrasonico");
    display.println("--------------------");

    display.setTextSize(3);
    display.setCursor(10, 25);
    if (distance > 0 && distance < 400) {
      display.printf("%.1f", distance);
    } else {
      display.print("---");
    }

    display.setTextSize(1);
    display.setCursor(100, 30);
    display.print("cm");

    drawBar(distance);
    display.display();

    Serial.printf("Distancia: %.1f cm\n", distance);
  }
}

float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.0343 / 2.0;
}

void drawBar(float distance) {
  int y = 56;
  int maxBarWidth = 120;
  int barWidth = 0;

  if (distance > 0 && distance < 400) {
    barWidth = max(0, maxBarWidth - (int)(distance / 400.0 * maxBarWidth));
  }

  display.drawRect(4, y - 2, maxBarWidth + 2, 6, SSD1306_WHITE);
  if (barWidth > 0) {
    display.fillRect(5, y - 1, barWidth, 4, SSD1306_WHITE);
  }
}
