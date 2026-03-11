#include <M5Unified.h>

bool showAlternate = false;

void drawScreen() {
  M5.Display.clear();
  M5.Display.setTextDatum(middle_center);
  M5.Display.setTextSize(2);
  M5.Display.drawString("M5Core2 + Arduino CLI", 160, 70);
  M5.Display.setTextSize(1);
  M5.Display.drawString(showAlternate ? "Button A pressed" : "Press Button A", 160, 130);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Serial.begin(115200);
  Serial.println("M5Core2 hello sketch booted");
  drawScreen();
}

void loop() {
  M5.update();
  if (M5.BtnA.wasPressed()) {
    showAlternate = !showAlternate;
    Serial.println(showAlternate ? "Button A: ON" : "Button A: OFF");
    drawScreen();
  }
  delay(20);
}
