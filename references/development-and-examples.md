# Development And Examples

## Purpose

Use this reference when you need to go beyond a one-off diagnosis and support a repeatable development workflow for M5Stack boards on Windows.

## Default development flow

1. Start from the sample sketch in `examples/`.
2. Run the setup helper to ensure toolchain, libraries, and board attachment are ready.
3. Edit the sketch.
4. Compile often.
5. Upload only after the board and port are explicit.
6. Keep the sketch path stable so `sketch.yaml` remains useful.

## Sample setup command

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
```

For SD card validation with file write and remaining-capacity checks:

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\sd_text_write -Port COM11
```

For a button-reactive cat animation that replays imported frames from an animated WebP:

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\pixel_pet -Port COM11
```

## Sample upload command

```powershell
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
```

For the SD card sample:

```powershell
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\sd_text_write -Port COM11
```

For the pixel pet sample:

```powershell
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\pixel_pet -Port COM11
```

## What the sample sketches demonstrate

`examples/m5core2/hello/hello.ino` demonstrates:

- `M5Unified` initialization
- `M5GFX` backed display output through `M5.Display`
- a serial boot message for quick monitor checks
- a simple button-triggered state change

`examples/m5core2/sd_text_write/sd_text_write.ino` demonstrates:

- mounting the microSD card on M5Core2 with the expected SPI chip select pin
- creating a text file on the SD card and appending lines with Button A
- reading the file back to the display and serial monitor with Button B
- showing free and total SD card capacity so the board can double as a storage check

`examples/m5core2/pixel_pet/pixel_pet.ino` demonstrates:

- converting a transparent animated WebP into an RGB565 animation header for M5Core2
- fitting full frames into a square canvas without distorting aspect ratio
- preserving transparency while replaying the imported animation on screen
- generating preview-strip and sprite-sheet PNGs during asset conversion
- changing the pet reaction text and timing with Button A, Button B, and Button C while reusing the imported art

To regenerate the transparent cat animation asset with `uv` and Pillow:

```powershell
uv run .\scripts\generate_sprite_animation.py --input 'D:\path\to\cat.webp' --output .\examples\m5core2\pixel_pet\generated_cat_animation.h --preview .\tmp\generated_cat_animation_preview.png --sheet .\tmp\generated_cat_animation_sheet.png --size 112 --frame-step 4 --sheet-columns 8
```

That conversion flow leaves behind:

- `examples/m5core2/pixel_pet/generated_cat_animation.h` for the sketch
- `tmp/generated_cat_animation_preview.png` for quick visual checks
- `tmp/generated_cat_animation_sheet.png` for reviewing sampled frames as a sprite sheet

## Development support expectations

When using this skill for development support:

- keep using Arduino CLI commands instead of mixing IDE-only state
- attach the board to the sketch early
- prefer adding small example sketches for new hardware checks
- explain whether a failure is about transport, board identity, libraries, or sketch logic
- leave behind runnable commands the user can repeat on the same machine
