<div align="center">
  <img src="./assets/m5stack-arduino-cli-icon.svg" alt="M5Stack Arduino CLI icon" width="140" height="140">
  <h1>M5Stack Arduino CLI Skill</h1>
  <p><strong>A focused Codex skill for setting up, flashing, diagnosing, and supporting M5Stack development with Arduino CLI on Windows.</strong></p>
  <p>
    <img src="https://img.shields.io/badge/Platform-Windows_10%2B-0A7E8C?style=flat-square" alt="Windows badge">
    <img src="https://img.shields.io/badge/Tool-Arduino_CLI-1B5E20?style=flat-square" alt="Arduino CLI badge">
    <img src="https://img.shields.io/badge/Target-M5Stack_%2F_ESP32-37474F?style=flat-square" alt="M5Stack ESP32 badge">
    <img src="https://img.shields.io/badge/License-MIT-E65100?style=flat-square" alt="MIT license badge">
  </p>
  <p><a href="./README.md">English</a> | <a href="./README.ja.md">日本語</a></p>
</div>

## Docs

- Browse the docs site: [Sunwood-ai-labs.github.io/m5stack-arduino-cli-skill](https://sunwood-ai-labs.github.io/m5stack-arduino-cli-skill/)
- English quick start: [`docs/guide/quickstart.md`](./docs/guide/quickstart.md)
- Japanese quick start: [`docs/ja/guide/quickstart.md`](./docs/ja/guide/quickstart.md)

## Overview

This repository packages a reusable Codex skill for a stubborn but common workflow: getting an M5Stack board working from `arduino-cli` on Windows when the board shows up as `Unknown`, the serial bridge is generic, or the right ESP32 FQBN is not attached yet.

The skill is optimized for practical troubleshooting and day-to-day development. It prioritizes proving device health, finding the correct COM port, attaching the right board profile, installing common libraries, and getting back to a successful compile or upload without unnecessary driver churn.

## Why This Skill

- Explains why `arduino-cli board list` can show a valid COM port but still report `Unknown`
- Covers Windows-specific diagnosis for common USB-serial bridges such as `CH9102` and `CP210x`
- Guides Codex to use `arduino-cli`, `esptool`, and Windows device information in the right order
- Includes board defaults for M5Core2 and common supporting libraries such as `M5GFX` and `M5Unified`
- Includes sample PowerShell helpers and a starter M5Core2 sketch for setup, upload, and development support

## Quick Start

Ask Codex to use the skill explicitly:

```text
Use $m5stack-arduino-cli to set up my M5Core2 on Windows, attach the correct FQBN, and upload a sample sketch from Arduino CLI.
```

The skill will steer Codex through this workflow:

1. Confirm Windows sees the device as a serial port.
2. Find `arduino-cli`, including the Arduino IDE bundled binary if needed.
3. Ensure `esp32:esp32` board support is installed.
4. Identify the correct COM port with Windows tools and `arduino-cli board list`.
5. Treat `Unknown` as an identification gap unless Windows or `esptool` also fails.
6. Install `M5GFX` and `M5Unified` when the sketch needs M5 device helpers.
7. Attach the right FQBN and port to the sketch before compiling or uploading.
8. Reuse the bundled scripts and examples for repeatable setup and development.

## Sample Scripts And Example Sketch

Use the included PowerShell helpers when you want repeatable setup or upload commands:

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
```

Use the bundled starter sketch when you need a first flash check or a clean development base:

```text
examples/m5core2/hello/hello.ino
```

Use the SD card validation sample when you want to confirm that a microSD card can be mounted, written, read back, and checked for remaining capacity:

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\sd_text_write -Port COM11
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\sd_text_write -Port COM11
```

```text
examples/m5core2/sd_text_write/sd_text_write.ino
```

That sample demonstrates:

- SD card mount with `SD.begin(GPIO_NUM_4, SPI, 25000000)`
- creating and appending a text log file on the card
- reading the file back to the display and serial monitor
- showing free and total SD card capacity on screen

Use the pixel pet sample when you want a playful display demo that stays inside one Arduino sketch:

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\pixel_pet -Port COM11
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\pixel_pet -Port COM11
```

```text
examples/m5core2/pixel_pet/pixel_pet.ino
```

That sample demonstrates:

- converting a transparent animated WebP into an RGB565 animation header for M5Core2
- preserving the full-frame aspect ratio while fitting each frame into a square canvas
- generating optional preview-strip and sprite-sheet PNGs during conversion
- replaying the imported animation directly instead of adding extra bobbing motion in code
- using Buttons A, B, and C to pet, feed, or put the character to sleep

Preview strip generated from the sampled frames:

![Animated cat preview strip](./docs/public/examples/pixel_pet/generated_cat_animation_preview.png)

Sprite sheet generated from the full sampled animation:

![Animated cat sprite sheet](./docs/public/examples/pixel_pet/generated_cat_animation_sheet.png)

When you want to regenerate the animation assets from an animated WebP with `uv`, run:

```powershell
uv run .\scripts\generate_sprite_animation.py --input 'D:\path\to\cat.webp' --output .\examples\m5core2\pixel_pet\generated_cat_animation.h --preview .\docs\public\examples\pixel_pet\generated_cat_animation_preview.png --sheet .\docs\public\examples\pixel_pet\generated_cat_animation_sheet.png --size 112 --frame-step 4 --sheet-columns 8
```

That command leaves behind:

- `examples/m5core2/pixel_pet/generated_cat_animation.h` for the Arduino sketch
- `docs/public/examples/pixel_pet/generated_cat_animation_preview.png` for a quick frame sanity check
- `docs/public/examples/pixel_pet/generated_cat_animation_sheet.png` for reviewing the sampled frames as a sprite sheet

## Direct Arduino CLI Commands

Use these commands when you want to show the exact `arduino-cli` flow instead of the helper scripts:

```powershell
$cli = "C:\Users\<User>\AppData\Local\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

& $cli board attach -p COM11 -b esp32:esp32:m5stack_core2 .\examples\m5core2\hello
& $cli compile .\examples\m5core2\hello
& $cli upload -p COM11 .\examples\m5core2\hello
```

If you want each command to be explicit and not rely on `sketch.yaml`, use:

```powershell
& $cli compile --fqbn esp32:esp32:m5stack_core2 .\examples\m5core2\hello
& $cli upload -p COM11 --fqbn esp32:esp32:m5stack_core2 .\examples\m5core2\hello
```

## Included Files

| Path | Purpose |
| --- | --- |
| [`SKILL.md`](./SKILL.md) | Main skill instructions, rules, and default workflow |
| [`agents/openai.yaml`](./agents/openai.yaml) | Agent-facing metadata such as display name and default prompt |
| [`scripts/setup-m5core2.ps1`](./scripts/setup-m5core2.ps1) | PowerShell helper for CLI discovery, ESP32 setup, library install, and board attach |
| [`scripts/upload-m5core2.ps1`](./scripts/upload-m5core2.ps1) | PowerShell helper for compile and upload |
| [`scripts/generate_sprite_animation.py`](./scripts/generate_sprite_animation.py) | `uv`-driven converter from transparent animated WebP to RGB565 animation frames plus optional preview artifacts |
| [`examples/m5core2/hello/hello.ino`](./examples/m5core2/hello/hello.ino) | Sample M5Core2 sketch for first flash and development |
| [`examples/m5core2/sd_text_write/sd_text_write.ino`](./examples/m5core2/sd_text_write/sd_text_write.ino) | Sample M5Core2 sketch for SD card text write, readback, and free-space checks |
| [`examples/m5core2/pixel_pet/pixel_pet.ino`](./examples/m5core2/pixel_pet/pixel_pet.ino) | Sample M5Core2 sketch for transparent WebP-driven cat animation and button-driven reactions |
| [`docs/`](./docs/) | Bilingual VitePress docs for browsing the workflow as a site |
| [`references/windows-setup-and-diagnosis.md`](./references/windows-setup-and-diagnosis.md) | Windows commands, setup flow, and `Unknown` troubleshooting |
| [`references/m5-board-notes.md`](./references/m5-board-notes.md) | M5-specific board notes, bridge-chip context, and FQBN defaults |
| [`references/development-and-examples.md`](./references/development-and-examples.md) | Development workflow, sample commands, and example usage |

## Workflow Highlights

### Treat `Unknown` correctly

The skill distinguishes between:

- port detection
- board identification

That difference matters because many healthy M5Stack boards expose only a generic USB-serial bridge to Windows. In that situation, a COM port can be completely valid while Arduino CLI still cannot infer the exact board model automatically.

### Prefer proof over guesswork

The guidance intentionally avoids random driver reinstalls. It asks Codex to confirm:

- Windows device status
- serial port visibility
- ESP32 package installation
- `esptool` reachability when needed
- the intended board profile and port attachment

### Support real development work

The repository is not limited to one-time setup. It also helps Codex:

- bootstrap a new M5Core2 sketch
- install the common M5 libraries
- keep board and port attachment reproducible
- compile and upload during iterative development
- leave behind runnable CLI commands instead of IDE-only instructions

### Scale to more samples cleanly

The repository now uses a growth-friendly structure:

- board samples live under `examples/<board>/<sample>/`
- shared PowerShell logic lives under `scripts/common/`
- board setup flows live under `scripts/setup/`
- generic upload flows live under `scripts/upload/`
- user-facing wrapper commands stay short at the top level

## Repository Layout

```text
.
|-- SKILL.md
|-- README.md
|-- README.ja.md
|-- agents/
|   `-- openai.yaml
|-- assets/
|   `-- m5stack-arduino-cli-icon.svg
|-- docs/
|   |-- .vitepress/
|   |-- guide/
|   `-- ja/
|-- examples/
|   `-- m5core2/
|       |-- hello/
|       |-- pixel_pet/
|       `-- sd_text_write/
|-- scripts/
|   |-- common/
|   |-- setup/
|   |-- upload/
|   |-- setup-m5core2.ps1
|   `-- upload-m5core2.ps1
`-- references/
    |-- development-and-examples.md
    |-- m5-board-notes.md
    `-- windows-setup-and-diagnosis.md
```

## When To Reach For It

Use this repository when you want a repeatable, repo-local skill that helps Codex:

- set up an M5Stack board on Windows
- explain why `arduino-cli board list` says `Unknown`
- attach `esp32:esp32:m5stack_core2` or another intended FQBN
- install the libraries an M5 sketch usually needs
- compile and upload sketches with less trial and error
- seed or support an M5Core2 development workflow from CLI only

## License

This repository is available under the [MIT License](./LICENSE).
