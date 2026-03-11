# M5 Board Notes

## Why M5 boards often show `Unknown`

Many M5Stack boards expose USB through a generic USB-serial bridge rather than a board-specific USB identity. On Windows, that often looks like:

- `USB-Enhanced-SERIAL CH9102`
- `Silicon Labs CP210x USB to UART Bridge`

Arduino CLI can usually discover the port, but it may not label the exact board model because the visible USB identity belongs to the bridge chip, not the M5 board family itself.

## Practical implication

This is the normal workflow:

1. Discover the COM port.
2. Identify the board model from user context or project intent.
3. Set the FQBN manually.
4. Attach the board to the sketch.
5. Upload.

## M5Core2

- FQBN: `esp32:esp32:m5stack_core2`
- Typical libraries:
  - `M5Unified`
  - `M5GFX`
- Useful attach command:

```powershell
& $cli board attach -p COM11 -b esp32:esp32:m5stack_core2 D:\Prj\M5\VerifyCore2
```

## Recommended explanation to users

When a user is worried about `Unknown`, explain:

- the COM port itself is detected
- Windows has a working USB-serial driver
- the board name is missing because the bridge chip is generic
- manual FQBN selection is the normal fix

## High-confidence signs the setup is already healthy

- `Get-PnpDevice` shows the port as `OK`
- `arduino-cli upload` completes
- `esptool` identifies an ESP32 on the port
- a test sketch compiles and uploads
