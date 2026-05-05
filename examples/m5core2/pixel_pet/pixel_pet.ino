#include <M5Unified.h>

#include "generated_cat_animation.h"
#include "generated_happy_animation.h"
#include "generated_running_animation.h"

namespace {

constexpr uint16_t kSkyColor = 0x0000;
constexpr uint16_t kFloorColor = 0x0000;
constexpr uint16_t kFloorShadow = 0x0000;
constexpr uint16_t kTextColor = 0xFFDF;
constexpr uint16_t kBubbleColor = 0xFFDF;
constexpr uint16_t kBubbleBorder = 0x6B4D;
constexpr uint16_t kOutlineColor = 0x0841;
constexpr uint16_t kHeartColor = 0xE8E7;
constexpr uint16_t kDashColor = 0x9D7C;

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

const char* const kDashIcon[kIconSize] = {
    "........",
    "..gggg..",
    ".gggggg.",
    "........",
    "..gggg..",
    ".gggggg.",
    "........",
    "........",
};

enum class AnimationMode {
  Normal,
  Happy,
  Running,
};

struct PetState {
  AnimationMode animation = AnimationMode::Normal;
  bool sleeping = false;
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
    case 'g':
      return kDashColor;
    default:
      return 0;
  }
}

int activeFrameCount() {
  switch (pet.animation) {
    case AnimationMode::Happy:
      return kHappyFrameCount;
    case AnimationMode::Running:
      return kRunningFrameCount;
    case AnimationMode::Normal:
    default:
      return kCatFrameCount;
  }
}

void selectAnimation(AnimationMode animation) {
  pet.animation = animation;
  pet.frameIndex = 0;
}

void stepAnimation(int direction) {
  const int modeCount = 3;
  int next = static_cast<int>(pet.animation) + direction;
  if (next < 0) {
    next += modeCount;
  } else if (next >= modeCount) {
    next -= modeCount;
  }
  selectAnimation(static_cast<AnimationMode>(next));
}

const char* animationLabel() {
  if (pet.sleeping) {
    return "Cat nap";
  }

  switch (pet.animation) {
    case AnimationMode::Happy:
      return "Happy dance";
    case AnimationMode::Running:
      return "Zoomies";
    case AnimationMode::Normal:
    default:
      return "Tiny meow";
  }
}

const char* helperLabel() {
  if (pet.sleeping) {
    return "C to wake up";
  }

  switch (pet.animation) {
    case AnimationMode::Happy:
      return "A previous  B next";
    case AnimationMode::Running:
      return "A previous  B next";
    case AnimationMode::Normal:
    default:
      return "A previous  B next";
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
  frameBuffer.drawString(animationLabel(), 26, 24);

  if (pet.animation == AnimationMode::Happy && !pet.sleeping) {
    const int rise = (now / 120) % 6;
    drawIcon(kHeartIcon, kIconSize, kIconSize, 166, 22 - rise, 3);
  } else if (pet.animation == AnimationMode::Running && !pet.sleeping) {
    const int sway = (now / 80) % 2;
    drawIcon(kDashIcon, kIconSize, kIconSize, 164 + sway, 24, 3);
  }
}

void drawHints() {
  frameBuffer.setTextDatum(top_left);
  frameBuffer.setTextColor(kTextColor, kSkyColor);
  frameBuffer.setTextSize(1);
  frameBuffer.drawString(helperLabel(), 16, 78);
  frameBuffer.drawString("A Prev   B Next   C Sleep/Wake", 34, 220);
}

void drawScene(uint32_t now) {
  drawBackground();
  drawStatusBubble(now);
  drawHints();

  if (pet.animation == AnimationMode::Happy) {
    frameBuffer.pushImage(
        kPetBaseX,
        kPetBaseY,
        kHappyFrameWidth,
        kHappyFrameHeight,
        reinterpret_cast<const lgfx::v1::rgb565_t*>(kHappyAnimationPixels[pet.frameIndex]),
        lgfx::v1::rgb565_t(kHappyTransparentColor));
  } else if (pet.animation == AnimationMode::Running) {
    frameBuffer.pushImage(
        kPetBaseX,
        kPetBaseY,
        kRunningFrameWidth,
        kRunningFrameHeight,
        reinterpret_cast<const lgfx::v1::rgb565_t*>(kRunningAnimationPixels[pet.frameIndex]),
        lgfx::v1::rgb565_t(kRunningTransparentColor));
  } else {
    frameBuffer.pushImage(
        kPetBaseX,
        kPetBaseY,
        kCatFrameWidth,
        kCatFrameHeight,
        reinterpret_cast<const lgfx::v1::rgb565_t*>(kCatAnimationPixels[pet.frameIndex]),
        lgfx::v1::rgb565_t(kCatTransparentColor));
  }

  frameBuffer.pushSprite(0, 0);
}

void handleButtons() {
  if (M5.BtnA.wasPressed()) {
    pet.sleeping = false;
    stepAnimation(-1);
  }

  if (M5.BtnB.wasPressed()) {
    pet.sleeping = false;
    stepAnimation(1);
  }

  if (M5.BtnC.wasPressed()) {
    pet.sleeping = !pet.sleeping;
    if (!pet.sleeping) {
      pet.frameIndex = 0;
    }
  }
}

void updatePet(uint32_t now) {
  if (pet.sleeping) {
    return;
  }

  if (now >= pet.nextFrameMs) {
    pet.frameIndex = (pet.frameIndex + 1) % activeFrameCount();

    uint32_t delayMs = 120;
    if (pet.animation == AnimationMode::Happy) {
      delayMs = 90;
    } else if (pet.animation == AnimationMode::Running) {
      delayMs = 70;
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
