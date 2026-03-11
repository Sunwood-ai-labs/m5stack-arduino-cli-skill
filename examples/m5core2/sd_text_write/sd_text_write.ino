#include <M5Unified.h>
#include <SD.h>
#include <SPI.h>

namespace {

static constexpr gpio_num_t kSdCardCsPin = GPIO_NUM_4;
static constexpr uint32_t kSdClockHz = 25000000;
static constexpr const char* kLogFilePath = "/codex_log.txt";

String gStatusMessage = "Booting...";
String gFilePreview = "";
String gStorageMessage = "";
uint32_t gAppendCount = 0;

String formatMiB(uint64_t bytes) {
  return String(static_cast<uint32_t>(bytes / (1024ULL * 1024ULL))) + " MB";
}

void updateStorageMessage() {
  uint64_t totalBytes = SD.totalBytes();
  uint64_t usedBytes = SD.usedBytes();
  uint64_t fallbackCardBytes = SD.cardSize();

  if (totalBytes == 0 && fallbackCardBytes > 0) {
    totalBytes = fallbackCardBytes;
  }
  if (usedBytes > totalBytes) {
    usedBytes = totalBytes;
  }

  uint64_t freeBytes = totalBytes - usedBytes;
  gStorageMessage =
      "Free: " + formatMiB(freeBytes) +
      " / Total: " + formatMiB(totalBytes);
}

void drawScreen() {
  M5.Display.startWrite();
  M5.Display.clear(TFT_BLACK);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.setTextDatum(top_left);
  M5.Display.setCursor(12, 10);
  M5.Display.setTextSize(2);
  M5.Display.println("SD Text Writer");
  M5.Display.setTextSize(1);
  M5.Display.println();
  M5.Display.println("A: append text");
  M5.Display.println("B: reload file");
  M5.Display.println();
  M5.Display.printf("Status: %s\n", gStatusMessage.c_str());
  M5.Display.printf("Storage: %s\n", gStorageMessage.c_str());
  M5.Display.println();
  M5.Display.println("Preview:");
  M5.Display.println(gFilePreview);
  M5.Display.endWrite();
}

bool appendLine(const String& line) {
  File file = SD.open(kLogFilePath, FILE_APPEND);
  if (!file) {
    gStatusMessage = "Open for append failed";
    return false;
  }

  if (!file.println(line)) {
    gStatusMessage = "Write failed";
    file.close();
    return false;
  }

  file.close();
  return true;
}

String readFilePreview() {
  File file = SD.open(kLogFilePath, FILE_READ);
  if (!file) {
    return "Could not open file.";
  }

  String preview;
  while (file.available()) {
    preview += static_cast<char>(file.read());
    if (preview.length() >= 320) {
      preview += "\n...";
      break;
    }
  }

  file.close();
  return preview;
}

bool initializeLogFile() {
  if (SD.exists(kLogFilePath)) {
    SD.remove(kLogFilePath);
  }

  if (!appendLine("M5Core2 SD card text write sample")) {
    return false;
  }
  if (!appendLine("------------------------------")) {
    return false;
  }
  if (!appendLine("Boot millis: " + String(millis()))) {
    return false;
  }
  if (!appendLine("Press Button A to append more lines.")) {
    return false;
  }

  return true;
}

bool mountSdCard() {
  if (!SD.begin(kSdCardCsPin, SPI, kSdClockHz)) {
    gStatusMessage = "SD.begin failed";
    gStorageMessage = "Free: n/a / Total: n/a";
    gFilePreview = "Insert a microSD card and reset.";
    return false;
  }

  updateStorageMessage();
  gStatusMessage = "SD mounted";
  return true;
}

void refreshPreview(const String& statusMessage) {
  gStatusMessage = statusMessage;
  updateStorageMessage();
  gFilePreview = readFilePreview();
  drawScreen();
  Serial.println("---- " + statusMessage + " ----");
  Serial.println(gStorageMessage);
  Serial.println(gFilePreview);
}

}  // namespace

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Serial.begin(115200);
  delay(200);

  if (!mountSdCard()) {
    drawScreen();
    Serial.println(gStatusMessage);
    Serial.println(gFilePreview);
    return;
  }

  if (!initializeLogFile()) {
    gFilePreview = "Could not create the sample file.";
    drawScreen();
    Serial.println(gStatusMessage);
    return;
  }

  refreshPreview("Created " + String(kLogFilePath));
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    ++gAppendCount;
    String line = "Append #" + String(gAppendCount) + " at " + String(millis()) + " ms";
    if (appendLine(line)) {
      refreshPreview("Appended one line");
    } else {
      gFilePreview = "Append failed.";
      drawScreen();
      Serial.println(gStatusMessage);
    }
  }

  if (M5.BtnB.wasPressed()) {
    refreshPreview("Reloaded from SD");
  }

  delay(20);
}
