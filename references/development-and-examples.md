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

## Sample upload command

```powershell
.\scripts\upload-m5core2.ps1 -SketchPath .\examples\m5core2\hello -Port COM11
```

## What the sample sketch demonstrates

- `M5Unified` initialization
- `M5GFX` backed display output through `M5.Display`
- a serial boot message for quick monitor checks
- a simple button-triggered state change

## Development support expectations

When using this skill for development support:

- keep using Arduino CLI commands instead of mixing IDE-only state
- attach the board to the sketch early
- prefer adding small example sketches for new hardware checks
- explain whether a failure is about transport, board identity, libraries, or sketch logic
- leave behind runnable commands the user can repeat on the same machine
