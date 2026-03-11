---
name: m5stack-arduino-cli
description: Set up, diagnose, and use M5Stack boards with Arduino CLI on Windows. Use when Codex needs to install ESP32 board support, detect the correct COM port, explain why `arduino-cli board list` shows `Unknown`, attach a board/FQBN to a sketch, compile or upload to M5Stack devices such as M5Core2, or troubleshoot CH9102/CP210x USB-serial behavior.
---

# M5Stack Arduino CLI

Use this skill to work with M5Stack boards from Arduino CLI on Windows, especially when the board appears as `Unknown` in `arduino-cli board list`.

## Follow this workflow

1. Confirm that Windows sees the device as a serial port.
2. Confirm that `arduino-cli` exists. If it is not on `PATH`, look for the Arduino IDE bundled binary.
3. Ensure the ESP32 package index is configured and `esp32:esp32` is installed.
4. Identify the correct COM port with `arduino-cli board list`, Windows device info, and, if needed, `esptool`.
5. Treat `Unknown` as a board auto-detection limitation unless Windows or `esptool` also fails.
6. Attach the intended FQBN and port to the sketch with `arduino-cli board attach`.
7. Compile and upload using the attached settings or explicit `--fqbn` and `-p`.
8. If the sketch uses M5Stack display or device helpers, ensure `M5GFX` and `M5Unified` are installed.

## Key rules

- Do not assume `Unknown` means a missing driver.
- Prefer proving device health with Windows device status and `esptool` before changing drivers.
- For M5Core2 on Windows, expect the device to appear as a generic USB-serial bridge such as `CH9102` or `CP210x`.
- Explain the difference between port detection and board identification: Arduino CLI may know the COM port while still not knowing the exact board model.
- If upload already works, do not recommend random driver reinstalls.
- If a sketch folder does not exist, create a minimal one and attach the board there.

## M5Core2 defaults

- FQBN: `esp32:esp32:m5stack_core2`
- Common USB bridge names:
  - `USB-Enhanced-SERIAL CH9102`
  - `Silicon Labs CP210x USB to UART Bridge`
- Common supporting libraries:
  - `M5GFX`
  - `M5Unified`

## Read references as needed

- Read [references/windows-setup-and-diagnosis.md](./references/windows-setup-and-diagnosis.md) for the Windows and Arduino CLI workflow, command checklist, and explanation of `Unknown`.
- Read [references/m5-board-notes.md](./references/m5-board-notes.md) when you need board mappings, M5-specific package/library guidance, or a concise explanation of why M5 boards often stay `Unknown` in `board list`.
