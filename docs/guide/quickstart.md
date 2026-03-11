# Quick Start

## What This Skill Solves

Use this skill when an M5Stack board is connected to Windows but `arduino-cli board list` still reports the board as `Unknown`, or when you want Codex to set up, attach, and flash the correct board profile before development starts.

## Recommended Prompt

```text
Use $m5stack-arduino-cli to set up my M5Core2 on Windows, attach the correct FQBN, and upload a sample sketch from Arduino CLI.
```

## Default Workflow

1. Confirm that Windows sees the board as a serial device.
2. Locate `arduino-cli`, including the Arduino IDE bundled binary if it is not on `PATH`.
3. Ensure the ESP32 core is configured and installed.
4. Identify the current COM port from Windows and `arduino-cli board list`.
5. Treat `Unknown` as an auto-identification limit unless transport checks fail too.
6. Install `M5GFX` and `M5Unified` when the sketch uses M5 libraries.
7. Attach the intended FQBN and port to the sketch.
8. Compile and upload with the attached configuration.

## High-Value Commands

```powershell
where.exe arduino-cli
Get-CimInstance Win32_SerialPort | Select-Object DeviceID,Name,Description,PNPDeviceID
Get-PnpDevice -PresentOnly | Where-Object { $_.Class -in @('Ports','USB') } |
  Select-Object Class,FriendlyName,Status,InstanceId
arduino-cli board list
arduino-cli core update-index
arduino-cli core install esp32:esp32
arduino-cli lib install M5GFX
arduino-cli lib install M5Unified
arduino-cli board attach -p COM11 -b esp32:esp32:m5stack_core2 .\examples\m5core2\hello
arduino-cli compile .\examples\m5core2\hello
arduino-cli upload -p COM11 .\examples\m5core2\hello
```

## Bundled Helpers

```powershell
.\scripts\setup-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
```

## Direct Compile And Upload

```powershell
$cli = "C:\Users\<User>\AppData\Local\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

& $cli board attach -p COM11 -b esp32:esp32:m5stack_core2 .\examples\m5core2\hello
& $cli compile .\examples\m5core2\hello
& $cli upload -p COM11 .\examples\m5core2\hello
```

## Good Defaults

- M5Core2 FQBN: `esp32:esp32:m5stack_core2`
- Common bridge names: `USB-Enhanced-SERIAL CH9102`, `Silicon Labs CP210x USB to UART Bridge`
- Common libraries: `M5Unified`, `M5GFX`
- Default example sketch: `examples/m5core2/hello/hello.ino`

## Next Step

Move to [Diagnosis Playbook](/guide/diagnosis) when you need a more detailed decision tree, or to [Development Support](/guide/development) when you want sample sketches and repeatable helper scripts.
