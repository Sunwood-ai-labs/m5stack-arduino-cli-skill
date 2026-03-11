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

## Direct CLI Commands For Development

```powershell
$cli = "C:\Users\<User>\AppData\Local\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

& $cli compile .\examples\m5core2\hello
& $cli upload -p COM11 .\examples\m5core2\hello
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
