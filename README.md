<div align="center">
  <img src="./assets/m5stack-arduino-cli-icon.svg" alt="M5Stack Arduino CLI icon" width="140" height="140">
  <h1>M5Stack Arduino CLI Skill</h1>
  <p><strong>A focused Codex skill for setting up, diagnosing, and using M5Stack boards with Arduino CLI on Windows.</strong></p>
  <p>
    <img src="https://img.shields.io/badge/Platform-Windows_10%2B-0A7E8C?style=flat-square" alt="Windows badge">
    <img src="https://img.shields.io/badge/Tool-Arduino_CLI-1B5E20?style=flat-square" alt="Arduino CLI badge">
    <img src="https://img.shields.io/badge/Target-M5Stack_%2F_ESP32-37474F?style=flat-square" alt="M5Stack ESP32 badge">
    <img src="https://img.shields.io/badge/License-MIT-E65100?style=flat-square" alt="MIT license badge">
  </p>
  <p><a href="./README.md">English</a> | <a href="./README.ja.md">日本語</a></p>
</div>

## Overview

This repository packages a reusable Codex skill for one stubborn but common workflow:
getting an M5Stack board working from `arduino-cli` on Windows when the board shows up
as `Unknown`, the serial bridge is generic, or the right ESP32 FQBN is not attached yet.

The skill is optimized for practical troubleshooting. It prioritizes proving device health,
finding the correct COM port, attaching the right board profile, and getting back to a
successful compile or upload without unnecessary driver churn.

## Why This Skill

- Explains why `arduino-cli board list` can show a valid COM port but still report `Unknown`
- Covers Windows-specific diagnosis for common USB-serial bridges such as `CH9102` and `CP210x`
- Guides Codex to use `arduino-cli`, `esptool`, and Windows device information in the right order
- Includes board defaults for M5Core2 and common supporting libraries such as `M5GFX` and `M5Unified`

## Quick Start

Ask Codex to use the skill explicitly:

```text
Use $m5stack-arduino-cli to diagnose why my M5Core2 shows Unknown in arduino-cli board list on Windows and attach the correct FQBN.
```

The skill will steer Codex through this workflow:

1. Confirm Windows sees the device as a serial port.
2. Find `arduino-cli`, including the Arduino IDE bundled binary if needed.
3. Ensure `esp32:esp32` board support is installed.
4. Identify the correct COM port with Windows tools and `arduino-cli board list`.
5. Treat `Unknown` as an identification gap unless Windows or `esptool` also fails.
6. Attach the right FQBN and port to the sketch before compiling or uploading.

## Included Files

| Path | Purpose |
| --- | --- |
| [`SKILL.md`](./SKILL.md) | Main skill instructions, rules, and default workflow |
| [`agents/openai.yaml`](./agents/openai.yaml) | Agent-facing metadata such as display name and default prompt |
| [`references/windows-setup-and-diagnosis.md`](./references/windows-setup-and-diagnosis.md) | Windows commands, setup flow, and `Unknown` troubleshooting |
| [`references/m5-board-notes.md`](./references/m5-board-notes.md) | M5-specific board notes, bridge-chip context, and FQBN defaults |

## Workflow Highlights

### Treat `Unknown` correctly

The skill distinguishes between:

- port detection
- board identification

That difference matters because many healthy M5Stack boards expose only a generic USB-serial
bridge to Windows. In that situation, a COM port can be completely valid while Arduino CLI
still cannot infer the exact board model automatically.

### Prefer proof over guesswork

The guidance intentionally avoids random driver reinstalls. It asks Codex to confirm:

- Windows device status
- serial port visibility
- ESP32 package installation
- `esptool` reachability when needed
- the intended board profile and port attachment

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
`-- references/
    |-- m5-board-notes.md
    `-- windows-setup-and-diagnosis.md
```

## When To Reach For It

Use this repository when you want a repeatable, repo-local skill that helps Codex:

- set up an M5Stack board on Windows
- explain why `arduino-cli board list` says `Unknown`
- attach `esp32:esp32:m5stack_core2` or another intended FQBN
- compile and upload sketches with less trial and error

## License

This repository is available under the [MIT License](./LICENSE).
