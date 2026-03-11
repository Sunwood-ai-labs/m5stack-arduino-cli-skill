#include <M5Unified.h>

#include "generated_cat_animation.h"

namespace {

constexpr uint16_t kSkyColor = 0x0000;
constexpr uint16_t kFloorColor = 0x0000;
constexpr uint16_t kFloorShadow = 0x0000;
constexpr uint16_t kTextColor = 0xFFDF;
constexpr uint16_t kBubbleColor = 0xFFDF;
constexpr uint16_t kBubbleBorder = 0x6B4D;
constexpr uint16_t kOutlineColor = 0x0841;
constexpr uint16_t kHeartColor = 0xE8E7;
constexpr uint16_t kFishColor = 0xFEC0;

constexpr int kIconSize = 8;
constexpr int kPetBaseX = (320 - kCatFrameWidth) / 2;
constexpr int kPetBaseY = 94;

const char* const kHeartIcon[kIconSize] = {
    "..rr.rr.",
    ".rrrrrr.",
    "rrrrrrrr",
    "rrrrrrrr",
    ".rrrrrr.",
    "..rrrr..",
    "...rr...",
    "........",
};

const char* const kFishIcon[kIconSize] = {
    "...yy...",
    "..yyyy..",
    "yyyyyyyy",
    ".yyyyyyy",
    "yyyyyyyy",
    "..yyyy..",
    "...yy...",
    "........",
};

enum class Mood {
  Idle,
  Happy,
  Snack,
};

struct PetState {
  Mood mood = Mood::Idle;
  bool sleeping = false;
  uint32_t moodUntilMs = 0;
  uint32_t nextFrameMs = 0;
  uint32_t nextDrawMs = 0;
  int frameIndex = 0;
};

PetState pet;
M5Canvas frameBuffer(&M5.Display);

uint16_t paletteColor(char cell) {
  switch (cell) {
    case 'r':
      return kHeartColor;
    case 'y':
      return kFishColor;
    default:
      return 0;
  }
}

void setMood(Mood mood, uint32_t durationMs) {
  pet.mood = mood;
  pet.moodUntilMs = millis() + durationMs;
}

const char* moodLabel() {
  if (pet.sleeping) {
    return "Cat nap";
  }

  switch (pet.mood) {
    case Mood::Happy:
      return "Purr purr";
    case Mood::Snack:
      return "Treat time";
    case Mood::Idle:
    default:
      return "Tiny meow";
  }
}

const char* helperLabel() {
  if (pet.sleeping) {
    return "C to wake up";
  }

  switch (pet.mood) {
    case Mood::Happy:
      return "A for more pets";
    case Mood::Snack:
      return "B for more treats";
    case Mood::Idle:
    default:
      return "A pet  B feed  C sleep";
  }
}

void drawIcon(const char* const* sprite, int rows, int cols, int x, int y, int scale) {
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      const char cell = sprite[row][col];
      if (cell == '.') {
        continue;
      }

      frameBuffer.fillRect(
          x + (col * scale),
          y + (row * scale),
          scale,
          scale,
          paletteColor(cell));
    }
  }
}

void drawBackground() {
  frameBuffer.fillScreen(kSkyColor);
}

void drawStatusBubble(uint32_t now) {
  frameBuffer.fillRoundRect(14, 14, 188, 54, 12, kBubbleColor);
  frameBuffer.drawRoundRect(14, 14, 188, 54, 12, kBubbleBorder);
  frameBuffer.setTextColor(kOutlineColor, kBubbleColor);
  frameBuffer.setTextDatum(top_left);
  frameBuffer.setTextSize(2);
  frameBuffer.drawString(moodLabel(), 26, 24);

  if (pet.mood == Mood::Happy && !pet.sleeping) {
    const int rise = (now / 120) % 6;
    drawIcon(kHeartIcon, kIconSize, kIconSize, 166, 22 - rise, 3);
  } else if (pet.mood == Mood::Snack && !pet.sleeping) {
    const int sway = (now / 160) % 2;
    drawIcon(kFishIcon, kIconSize, kIconSize, 166 + sway, 22, 3);
  }
}

void drawHints() {
  frameBuffer.setTextDatum(top_left);
  frameBuffer.setTextColor(kTextColor, kSkyColor);
  frameBuffer.setTextSize(1);
  frameBuffer.drawString(helperLabel(), 16, 78);
  frameBuffer.drawString("A Pet   B Feed   C Sleep/Wake", 54, 220);
}

void drawScene(uint32_t now) {
  drawBackground();
  drawStatusBubble(now);
  drawHints();

  const int x = kPetBaseX;
  const int y = kPetBaseY;

  frameBuffer.pushImage(
      x,
      y,
      kCatFrameWidth,
      kCatFrameHeight,
      reinterpret_cast<const lgfx::v1::rgb565_t*>(kCatAnimationPixels[pet.frameIndex]),
      lgfx::v1::rgb565_t(kCatTransparentColor));

  frameBuffer.pushSprite(0, 0);
}

void handleButtons() {
  if (M5.BtnA.wasPressed()) {
    pet.sleeping = false;
    setMood(Mood::Happy, 3200);
  }

  if (M5.BtnB.wasPressed()) {
    pet.sleeping = false;
    setMood(Mood::Snack, 2800);
  }

  if (M5.BtnC.wasPressed()) {
    pet.sleeping = !pet.sleeping;
    if (!pet.sleeping) {
      pet.mood = Mood::Idle;
    }
  }
}

void updatePet(uint32_t now) {
  if (!pet.sleeping && pet.mood != Mood::Idle && now >= pet.moodUntilMs) {
    pet.mood = Mood::Idle;
  }

  if (pet.sleeping) {
    return;
  }

  if (now >= pet.nextFrameMs) {
    pet.frameIndex = (pet.frameIndex + 1) % kCatFrameCount;
    uint32_t delayMs = 120;
    if (pet.mood == Mood::Happy) {
      delayMs = 90;
    } else if (pet.mood == Mood::Snack) {
      delayMs = 140;
    }
    pet.nextFrameMs = now + delayMs;
  }
}

}  // namespace

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Serial.begin(115200);
  Serial.println("pixel_pet sprite sketch booted");

  frameBuffer.setColorDepth(16);
  frameBuffer.createSprite(M5.Display.width(), M5.Display.height());

  const uint32_t now = millis();
  pet.nextFrameMs = now + 120;
  drawScene(now);
}

void loop() {
  M5.update();
  handleButtons();

  const uint32_t now = millis();
  updatePet(now);

  if (now >= pet.nextDrawMs) {
    drawScene(now);
    pet.nextDrawMs = now + 33;
  }

  delay(10);
}
