# Development Support

## Goal

Use this guide when you want the skill to help beyond first setup: starter sketches, repeatable helper scripts, library installation, and an Arduino CLI workflow that stays useful during day-to-day development.

## Bundled Sample Sketch

The default starter lives here:

```text
examples/m5core2/hello/hello.ino
```

It demonstrates:

- `M5Unified` initialization
- text output on the M5Core2 display
- serial boot logging
- button handling for a tiny interaction loop

The SD card validation sample lives here:

```text
examples/m5core2/sd_text_write/sd_text_write.ino
```

It demonstrates:

- mounting the microSD card on M5Core2
- creating and appending a text file on the card
- reading the file back on screen and over serial
- showing free and total SD card capacity for a quick storage check

The animated cat sample lives here:

```text
examples/m5core2/pixel_pet/pixel_pet.ino
```

It demonstrates:

- converting a transparent animated WebP into RGB565 frames for M5Core2
- fitting each imported frame into a square canvas without distorting aspect ratio
- preserving transparency while replaying the imported animation on the device
- inserting blended loop-transition frames so the loop wraps more smoothly
- using Button A, Button B, and Button C for pet, feed, and sleep or wake reactions
- keeping extra motion out of the sketch when the source animation already contains motion

Preview strip from the published example:

![Animated cat preview strip](/examples/pixel_pet/generated_cat_animation_preview.png)

Sprite sheet from the published example:

![Animated cat sprite sheet](/examples/pixel_pet/generated_cat_animation_sheet.png)

## Helper Scripts

### Setup helper

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
```

This helper:

- finds `arduino-cli`
- ensures the ESP32 package URL is present
- installs the ESP32 core
- installs `M5GFX` and `M5Unified`
- attaches the board and port to the sketch

### Upload helper

```powershell
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
```

This helper compiles and uploads the sketch with attached settings or explicit arguments.

To run the SD card validation sample instead:

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\sd_text_write -Port COM11
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\sd_text_write -Port COM11
```

To run the animated cat example instead:

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\pixel_pet -Port COM11
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\pixel_pet -Port COM11
```

To regenerate the animation header and preview artifacts with `uv`:

```powershell
uv run .\scripts\generate_sprite_animation.py --input 'D:\path\to\cat.webp' --output .\examples\m5core2\pixel_pet\generated_cat_animation.h --preview .\docs\public\examples\pixel_pet\generated_cat_animation_preview.png --sheet .\docs\public\examples\pixel_pet\generated_cat_animation_sheet.png --size 112 --frame-step 4 --sheet-columns 8 --loop-blend-frames 3
```

That converter keeps the full frame, fits it into a square canvas, adds blended transition frames at the loop boundary, and can emit both a quick preview strip and a sprite sheet for review.

## Direct CLI Commands For Development

```powershell
$cli = "C:\Users\<User>\AppData\Local\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

& $cli compile .\examples\m5core2\hello
& $cli upload -p COM11 .\examples\m5core2\hello
```

For the SD card sample:

```powershell
& $cli compile .\examples\m5core2\sd_text_write
& $cli upload -p COM11 .\examples\m5core2\sd_text_write
```

For the animated cat sample:

```powershell
& $cli compile .\examples\m5core2\pixel_pet
& $cli upload -p COM11 .\examples\m5core2\pixel_pet
```

Use explicit board selection when the sketch is not attached yet:

```powershell
& $cli compile --fqbn esp32:esp32:m5stack_core2 .\examples\m5core2\hello
& $cli upload -p COM11 --fqbn esp32:esp32:m5stack_core2 .\examples\m5core2\hello
```

## Suggested Development Loop

1. Run the setup helper once for a new sketch.
2. Edit the sketch.
3. Compile early and often.
4. Upload when the build is clean.
5. Keep `sketch.yaml` with the sketch so future CLI commands stay short.

## What To Leave Behind For Users

When Codex supports development, leave behind:

- a working sketch path
- the attached board metadata
- the exact CLI commands used
- any library install commands that were needed

That keeps later sessions reproducible without reopening the IDE.

## Scaling Out

Use [Repository Structure](/guide/structure) when you want to add more board families, helper scripts, or starter sketches without turning the repo into a flat list of unrelated files.
